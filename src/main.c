#include "common.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void usage(FILE *f) {
    fprintf(f,
        "clings %s - small exercises to get you used to reading and writing C code\n"
        "\n"
        "Usage: clings [COMMAND]\n"
        "\n"
        "Commands:\n"
        "  (none), watch   Interactive watch mode: re-checks the current exercise whenever you save it\n"
        "  run [NAME]      Compile and run an exercise (the current one by default)\n"
        "  hint [NAME]     Show the hint for an exercise (the current one by default)\n"
        "  list            List all exercises and your progress\n"
        "  reset NAME      Restore an exercise to its original state\n"
        "  check           Check every exercise and update your progress\n"
        "  init [DIR]      Create a new c-lings directory (default: ./c-lings)\n"
        "  dev check       Maintainers: verify exercises fail and solutions pass\n"
        "  help            Show this help\n"
        "  version         Show the version\n"
        "\n"
        "Environment:\n"
        "  CC                 C compiler to use (default: cc)\n"
        "  CLINGS_SANITIZE    1/0 to force AddressSanitizer/UBSan on or off (default: auto-detect)\n"
        "  NO_COLOR           Disable colored output\n",
        CLINGS_VERSION);
}

const EmbeddedFile *embedded_find(const char *path) {
    for (size_t i = 0; i < embedded_files_len; i++)
        if (strcmp(embedded_files[i].path, path) == 0) return &embedded_files[i];
    return NULL;
}

static void load_info(Info *info) {
    const EmbeddedFile *f = embedded_find("info.toml");
    if (!f) die("info.toml is missing from this build");
    char err[512];
    if (!info_parse((const char *)f->data, f->len, info, err, sizeof err)) die("%s", err);
}

static void require_workdir(void) {
    if (!dir_exists("exercises"))
        die("this command must be run inside a c-lings directory (one with an `exercises/` folder).\n"
            "Run `clings init` to create one.");
}

static Exercise *pick(App *app, int argc, char **argv) {
    if (argc < 1) return app_current(app);
    Exercise *ex = info_find(&app->info, argv[0]);
    if (!ex) die("no exercise named '%s' (see `clings list`)", argv[0]);
    return ex;
}

static const char *result_label(RunResult r) {
    switch (r) {
    case RES_PASS: return "passed";
    case RES_COMPILE_ERROR: return "failed to compile";
    case RES_FAIL: return "failed";
    case RES_CRASH: return "crashed";
    case RES_TIMEOUT: return "timed out";
    case RES_INTERNAL: return "could not be run";
    }
    return "?";
}

static int cmd_run(App *app, int argc, char **argv) {
    Exercise *ex = pick(app, argc, argv);
    Buf out = {0};
    RunResult r = run_source(ex, ex->path, ex->name, &out);
    if (out.len) {
        fwrite(out.data, 1, out.len, stdout);
        if (out.data[out.len - 1] != '\n') putchar('\n');
        putchar('\n');
    }
    if (r != RES_PASS) {
        printf("%s✗ %s %s%s\n", C_RED, ex->path, result_label(r), C_RESET);
        printf("Run `clings hint %s` for a hint.\n", ex->name);
        return 1;
    }
    printf("%s%s✓ Successfully ran %s%s\n", C_GREEN, C_BOLD, ex->path, C_RESET);
    bool was_current = ex == app_current(app);
    app_mark_done(app, ex);
    if (file_exists(ex->sol_path)) printf("Solution for comparison: %s%s%s\n", C_CYAN, ex->sol_path, C_RESET);
    if (was_current) {
        size_t next = app_next_pending(app, app->current);
        if (next < app->info.len) {
            app_set_current(app, next);
            printf("Next exercise: %s\n", app_current(app)->path);
        } else {
            printf("\n%s\n", app->info.final_msg);
        }
    }
    return 0;
}

static int cmd_hint(App *app, int argc, char **argv) {
    Exercise *ex = pick(app, argc, argv);
    printf("%s%sHint for %s:%s\n%s\n", C_CYAN, C_BOLD, ex->name, C_RESET, ex->hint);
    return 0;
}

static int cmd_list(App *app) {
    int w = 4;
    for (size_t i = 0; i < app->info.len; i++) {
        int n = (int)strlen(app->info.items[i].name);
        if (n > w) w = n;
    }
    printf("%s  %-8s %-*s  Path%s\n", C_BOLD, "State", w, "Name", C_RESET);
    for (size_t i = 0; i < app->info.len; i++) {
        Exercise *ex = &app->info.items[i];
        printf("%s %s%-8s%s %-*s  %s\n", i == app->current ? ">" : " ", ex->done ? C_GREEN : C_YELLOW,
               ex->done ? "DONE" : "PENDING", C_RESET, w, ex->name, ex->path);
    }
    int rows, cols;
    term_size(&rows, &cols);
    putchar('\n');
    print_progress_bar(app_done_count(app), app->info.len, cols);
    return 0;
}

static int cmd_reset(App *app, int argc, char **argv) {
    if (argc < 1) die("usage: clings reset NAME");
    Exercise *ex = pick(app, argc, argv);
    char err[256];
    if (!app_reset_exercise(app, ex, err, sizeof err)) die("%s", err);
    printf("The exercise %s has been reset\n", ex->path);
    return 0;
}

static int cmd_check(App *app) {
    size_t passed = check_all(app, isatty(STDOUT_FILENO));
    for (size_t i = 0; i < app->info.len; i++) {
        Exercise *ex = &app->info.items[i];
        if (!ex->done) printf("%s✗ %s%s\n", C_RED, ex->path, C_RESET);
    }
    printf("%s%zu/%zu exercises pass%s\n", passed == app->info.len ? C_GREEN : C_YELLOW, passed,
           app->info.len, C_RESET);
    if (passed == app->info.len) {
        printf("\n%s\n", app->info.final_msg);
        return 0;
    }
    app_set_current(app, app_next_pending(app, 0));
    printf("Current exercise: %s\n", app_current(app)->path);
    return 1;
}

static int cmd_init(int argc, char **argv) {
    const char *dir = argc > 0 ? argv[0] : "c-lings";
    if (dir_exists("exercises") && file_exists("exercises/clings.h"))
        die("the current directory already looks like a c-lings directory");
    if (dir_exists(dir) || file_exists(dir)) die("'%s' already exists", dir);
    if (!mkdir_p(dir)) die("could not create '%s'", dir);

    size_t written = 0;
    for (size_t i = 0; i < embedded_files_len; i++) {
        const EmbeddedFile *f = &embedded_files[i];
        bool want = strncmp(f->path, "exercises/", 10) == 0 || strcmp(f->path, "solutions/README.md") == 0 ||
                    strcmp(f->path, "compile_flags.txt") == 0;
        if (!want) continue;
        char *path = xasprintf("%s/%s", dir, f->path);
        if (!write_file(path, (const char *)f->data, f->len)) die("could not write '%s'", path);
        free(path);
        written++;
    }
    char *gi = xasprintf("%s/.gitignore", dir);
    const char *gitignore = ".clings/\n";
    write_file(gi, gitignore, strlen(gitignore));
    free(gi);

    printf("%sInitialized c-lings in ./%s%s (%zu files)\n\n", C_GREEN, dir, C_RESET, written);
    printf("Get started with:\n\n    cd %s\n    clings\n\n", dir);
    return 0;
}

/* For maintainers: every exercise must fail as shipped and its solution must pass. */
static int cmd_dev_check(App *app) {
    runner_prepare();
    size_t problems = 0;
    Buf out = {0};
    for (size_t i = 0; i < app->info.len; i++) {
        Exercise *ex = &app->info.items[i];
        printf("[%2zu/%zu] %-24s ", i + 1, app->info.len, ex->name);
        fflush(stdout);
        if (!file_exists(ex->path)) {
            printf("%smissing %s%s\n", C_RED, ex->path, C_RESET);
            problems++;
            continue;
        }
        if (!file_exists(ex->sol_path)) {
            printf("%smissing %s%s\n", C_RED, ex->sol_path, C_RESET);
            problems++;
            continue;
        }
        bool ok = true;
        if (!ex->skip_check_unsolved) {
            RunResult r = run_source(ex, ex->path, ex->name, &out);
            if (r == RES_PASS) {
                printf("%sexercise passes without changes%s\n", C_RED, C_RESET);
                ok = false;
            } else if (r == RES_INTERNAL) {
                printf("%scould not run compiler%s\n", C_RED, C_RESET);
                ok = false;
            }
        }
        if (ok) {
            char *bin = xasprintf("%s-solution", ex->name);
            RunResult r = run_source(ex, ex->sol_path, bin, &out);
            free(bin);
            if (r != RES_PASS) {
                printf("%ssolution %s%s\n%s\n", C_RED, result_label(r), C_RESET, out.data);
                ok = false;
            }
        }
        if (ok) printf("%sok%s\n", C_GREEN, C_RESET);
        else problems++;
    }
    buf_free(&out);
    if (problems) {
        printf("\n%s%zu problem(s) found%s\n", C_RED, problems, C_RESET);
        return 1;
    }
    printf("\n%sAll %zu exercises look good%s\n", C_GREEN, app->info.len, C_RESET);
    return 0;
}

int main(int argc, char **argv) {
    term_init();
    const char *cmd = argc > 1 ? argv[1] : "watch";
    int rest_argc = argc > 2 ? argc - 2 : 0;
    char **rest = argv + 2;

    if (!strcmp(cmd, "help") || !strcmp(cmd, "-h") || !strcmp(cmd, "--help")) {
        usage(stdout);
        return 0;
    }
    if (!strcmp(cmd, "version") || !strcmp(cmd, "-V") || !strcmp(cmd, "--version")) {
        printf("clings %s\n", CLINGS_VERSION);
        return 0;
    }
    if (!strcmp(cmd, "init")) return cmd_init(rest_argc, rest);

    App app = {0};
    load_info(&app.info);
    require_workdir();
    state_load(&app);

    if (!strcmp(cmd, "watch")) return cmd_watch(&app);
    if (!strcmp(cmd, "run")) return cmd_run(&app, rest_argc, rest);
    if (!strcmp(cmd, "hint")) return cmd_hint(&app, rest_argc, rest);
    if (!strcmp(cmd, "list")) return cmd_list(&app);
    if (!strcmp(cmd, "reset")) return cmd_reset(&app, rest_argc, rest);
    if (!strcmp(cmd, "check") || !strcmp(cmd, "verify")) return cmd_check(&app);
    if (!strcmp(cmd, "dev")) {
        if (rest_argc >= 1 && !strcmp(rest[0], "check")) return cmd_dev_check(&app);
        die("usage: clings dev check");
    }
    fprintf(stderr, "clings: unknown command '%s'\n\n", cmd);
    usage(stderr);
    return 2;
}
