#include "common.h"

#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define COMPILE_TIMEOUT_MS 30000
#define RUN_TIMEOUT_MS 10000
#define MAX_OUTPUT (128 * 1024)

static volatile pid_t child_pid;
static int sanitize = -1; /* -1 = not probed yet */

void runner_kill_child(void) {
    if (child_pid > 0) kill(-child_pid, SIGKILL);
}

static long long now_ms(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (long long)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

typedef struct {
    int status;
    bool timed_out;
    bool spawn_failed;
} Proc;

/* Runs argv with stdout+stderr captured into `out` and stdin from /dev/null. */
static Proc spawn_capture(char *const argv[], Buf *out, int timeout_ms) {
    Proc pr = {0};
    int fds[2];
    if (pipe(fds) != 0) {
        pr.spawn_failed = true;
        return pr;
    }
    fflush(stdout);
    fflush(stderr);
    pid_t pid = fork();
    if (pid < 0) {
        close(fds[0]);
        close(fds[1]);
        pr.spawn_failed = true;
        return pr;
    }
    if (pid == 0) {
        setpgid(0, 0);
        signal(SIGINT, SIG_DFL);
        signal(SIGTERM, SIG_DFL);
        signal(SIGHUP, SIG_DFL);
        int devnull = open("/dev/null", O_RDONLY);
        if (devnull >= 0) dup2(devnull, STDIN_FILENO);
        dup2(fds[1], STDOUT_FILENO);
        dup2(fds[1], STDERR_FILENO);
        close(fds[0]);
        close(fds[1]);
        execvp(argv[0], argv);
        fprintf(stderr, "failed to execute '%s': %s\n", argv[0], strerror(errno));
        _exit(127);
    }
    setpgid(pid, pid);
    child_pid = pid;
    close(fds[1]);

    long long deadline = now_ms() + timeout_ms;
    bool truncated = false;
    char chunk[4096];
    for (;;) {
        long long left = deadline - now_ms();
        if (left <= 0) {
            pr.timed_out = true;
            break;
        }
        struct pollfd pfd = {.fd = fds[0], .events = POLLIN};
        int r = poll(&pfd, 1, (int)left);
        if (r < 0 && errno == EINTR) continue;
        if (r == 0) {
            pr.timed_out = true;
            break;
        }
        ssize_t n = read(fds[0], chunk, sizeof chunk);
        if (n < 0 && errno == EINTR) continue;
        if (n <= 0) break;
        if (out->len < MAX_OUTPUT) {
            buf_append(out, chunk, (size_t)n);
        } else if (!truncated) {
            truncated = true;
            buf_puts(out, "\n... (output truncated) ...\n");
        }
    }
    if (pr.timed_out) kill(-pid, SIGKILL);
    close(fds[0]);
    while (waitpid(pid, &pr.status, 0) < 0 && errno == EINTR) {}
    /* clean up anything the program left running in its process group */
    kill(-pid, SIGKILL);
    child_pid = 0;
    return pr;
}

static const char *compiler(void) {
    const char *cc = getenv("CC");
    return cc && *cc ? cc : "cc";
}

/* Builds the compiler argv. Returned array and strings are leaked on purpose
 * (they're tiny and the process is short-lived). */
static char **build_compile_argv(const char *src, const char *bin, bool with_sanitizers, bool color) {
    size_t cap = 32, n = 0;
    char **argv = xmalloc(cap * sizeof *argv);
    char *cc = xstrdup(compiler());
    for (char *tok = strtok(cc, " "); tok; tok = strtok(NULL, " ")) {
        if (n + 1 >= cap) argv = xrealloc(argv, (cap *= 2) * sizeof *argv);
        argv[n++] = tok;
    }
    const char *flags[] = {
        "-std=c17", "-Wall", "-Wextra", "-Wno-unused-parameter", "-Werror", "-g", "-Iexercises",
    };
    for (size_t i = 0; i < sizeof flags / sizeof *flags; i++) argv[n++] = (char *)flags[i];
    if (color) argv[n++] = "-fdiagnostics-color=always";
    if (with_sanitizers) {
        argv[n++] = "-fsanitize=address,undefined";
        argv[n++] = "-fno-omit-frame-pointer";
    }
    argv[n++] = "-o";
    argv[n++] = (char *)bin;
    argv[n++] = (char *)src;
    argv[n++] = "-lm";
    argv[n] = NULL;
    return argv;
}

void runner_prepare(void) {
    if (!mkdir_p(BUILD_DIR)) die("could not create %s", BUILD_DIR);
    if (sanitize != -1) return;
    const char *env = getenv("CLINGS_SANITIZE");
    if (env && *env) {
        sanitize = strcmp(env, "0") != 0;
        return;
    }
    /* Probe whether the toolchain can build and run sanitized binaries. */
    const char *probe_src = BUILD_DIR "/probe.c";
    const char *probe_bin = BUILD_DIR "/probe";
    const char *code = "#include <stdlib.h>\nint main(void){char *p=malloc(4);free(p);return 0;}\n";
    sanitize = 0;
    if (!write_file(probe_src, code, strlen(code))) return;
    Buf out = {0};
    char **argv = build_compile_argv(probe_src, probe_bin, true, false);
    Proc pr = spawn_capture(argv, &out, COMPILE_TIMEOUT_MS);
    if (!pr.spawn_failed && !pr.timed_out && WIFEXITED(pr.status) && WEXITSTATUS(pr.status) == 0) {
        char *run_argv[] = {(char *)probe_bin, NULL};
        buf_clear(&out);
        pr = spawn_capture(run_argv, &out, RUN_TIMEOUT_MS);
        sanitize = !pr.timed_out && WIFEXITED(pr.status) && WEXITSTATUS(pr.status) == 0;
    }
    buf_free(&out);
}

RunResult run_source(const Exercise *ex, const char *src_path, const char *bin_name, Buf *out) {
    (void)ex;
    runner_prepare();
    buf_clear(out);
    buf_append(out, "", 0);

    char *bin = xasprintf("%s/%s", BUILD_DIR, bin_name);
    char **argv = build_compile_argv(src_path, bin, sanitize == 1, color_enabled);
    Proc pr = spawn_capture(argv, out, COMPILE_TIMEOUT_MS);
    free(argv);
    if (pr.spawn_failed) {
        buf_printf(out, "failed to start the compiler '%s'\n", compiler());
        free(bin);
        return RES_INTERNAL;
    }
    if (pr.timed_out) {
        buf_puts(out, "\nThe compiler took too long and was stopped.\n");
        free(bin);
        return RES_COMPILE_ERROR;
    }
    if (!WIFEXITED(pr.status) || WEXITSTATUS(pr.status) != 0) {
        free(bin);
        return WIFEXITED(pr.status) && WEXITSTATUS(pr.status) == 127 ? RES_INTERNAL : RES_COMPILE_ERROR;
    }

    /* Compiler output (if any) is noise once it succeeded; show the program's. */
    buf_clear(out);
    setenv("UBSAN_OPTIONS", "halt_on_error=1:print_stacktrace=1", 0);
    char *run_argv[] = {bin, NULL};
    pr = spawn_capture(run_argv, out, RUN_TIMEOUT_MS);
    free(bin);
    if (pr.spawn_failed) return RES_INTERNAL;
    if (pr.timed_out) {
        buf_printf(out, "\n%sThe program ran for more than %d seconds and was stopped. "
                        "Is there an infinite loop?%s\n",
                   C_RED, RUN_TIMEOUT_MS / 1000, C_RESET);
        return RES_TIMEOUT;
    }
    if (WIFSIGNALED(pr.status)) {
        int sig = WTERMSIG(pr.status);
        const char *what = sig == SIGSEGV ? " (invalid memory access: NULL or dangling pointer, or out of bounds?)"
                         : sig == SIGABRT ? " (abort: failed assert, double free or corrupted heap?)"
                         : sig == SIGFPE  ? " (arithmetic error: division by zero?)"
                                          : "";
        buf_printf(out, "\n%sThe program crashed: %s%s%s\n", C_RED, strsignal(sig), what, C_RESET);
        return RES_CRASH;
    }
    return WEXITSTATUS(pr.status) == 0 ? RES_PASS : RES_FAIL;
}

size_t check_all(App *app, bool show_progress) {
    runner_prepare();
    Info *info = &app->info;
    long ncpu = sysconf(_SC_NPROCESSORS_ONLN);
    size_t jobs = ncpu > 0 ? (size_t)ncpu : 4;
    pid_t *pids = xmalloc(info->len * sizeof *pids);
    size_t next = 0, running = 0, finished = 0, passed = 0;

    while (finished < info->len) {
        while (running < jobs && next < info->len) {
            fflush(stdout);
            pid_t pid = fork();
            if (pid == 0) {
                Buf out = {0};
                Exercise *ex = &info->items[next];
                RunResult r = run_source(ex, ex->path, ex->name, &out);
                _exit(r == RES_PASS ? 0 : 1);
            }
            if (pid < 0) die("fork failed");
            pids[next++] = pid;
            running++;
        }
        int status;
        pid_t pid = wait(&status);
        if (pid < 0) {
            if (errno == EINTR) continue;
            break;
        }
        for (size_t i = 0; i < next; i++) {
            if (pids[i] != pid) continue;
            bool ok = WIFEXITED(status) && WEXITSTATUS(status) == 0;
            info->items[i].done = ok;
            passed += ok;
            break;
        }
        running--;
        finished++;
        if (show_progress) {
            printf("\rChecking all exercises... %zu/%zu", finished, info->len);
            fflush(stdout);
        }
    }
    if (show_progress) printf("\r\x1b[K");
    free(pids);
    for (size_t i = 0; i < info->len; i++)
        if (info->items[i].done) app_mark_done(app, &info->items[i]);
    state_save(app);
    return passed;
}
