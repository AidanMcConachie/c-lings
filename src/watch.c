#include "common.h"

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

typedef struct {
    App *app;
    Buf out;
    RunResult result;
    bool show_hint;
    char *msg; /* one-off message shown under the status */
    long long mtime;
} Watch;

void print_progress_bar(size_t done, size_t total, int cols) {
    int width = cols - 24;
    if (width > 60) width = 60;
    if (width < 10) width = 10;
    int filled = total ? (int)((done * (size_t)width) / total) : 0;
    printf("%sProgress:%s [", C_BOLD, C_RESET);
    printf("%s", C_GREEN);
    for (int i = 0; i < filled; i++) putchar('#');
    if (filled < width) putchar('>');
    printf("%s", C_RED);
    for (int i = filled + 1; i < width; i++) putchar('-');
    printf("%s] %3zu/%zu\n", C_RESET, done, total);
}

static void set_msg(Watch *w, char *msg) {
    free(w->msg);
    w->msg = msg;
}

static void render(Watch *w) {
    App *app = w->app;
    Exercise *ex = app_current(app);
    int rows, cols;
    term_size(&rows, &cols);
    term_clear();

    if (w->out.len) {
        fwrite(w->out.data, 1, w->out.len, stdout);
        if (w->out.data[w->out.len - 1] != '\n') putchar('\n');
        putchar('\n');
    }

    switch (w->result) {
    case RES_PASS:
        printf("%s%s✓ Exercise done%s\n", C_GREEN, C_BOLD, C_RESET);
        if (file_exists(ex->sol_path))
            printf("Solution for comparison: %s%s%s\n", C_CYAN, ex->sol_path, C_RESET);
        printf("When done experimenting, enter `n` to move on to the next exercise\n");
        break;
    case RES_COMPILE_ERROR:
        printf("%s✗ Compiling %s failed! Please try again.%s\n", C_RED, ex->path, C_RESET);
        break;
    case RES_FAIL:
        printf("%s✗ Ran %s with errors%s\n", C_RED, ex->path, C_RESET);
        break;
    case RES_CRASH:
        printf("%s✗ %s crashed%s\n", C_RED, ex->path, C_RESET);
        break;
    case RES_TIMEOUT:
        printf("%s✗ %s timed out%s\n", C_RED, ex->path, C_RESET);
        break;
    case RES_INTERNAL:
        printf("%s✗ clings could not run the compiler. Is a C compiler installed? (set $CC to choose one)%s\n",
               C_RED, C_RESET);
        break;
    }

    if (w->show_hint) printf("\n%s%sHint:%s\n%s\n", C_CYAN, C_BOLD, C_RESET, ex->hint);
    if (w->msg) printf("\n%s\n", w->msg);

    putchar('\n');
    print_progress_bar(app_done_count(app), app->info.len, cols);
    printf("Current exercise: %s%s%s\n\n", C_BOLD, ex->path, C_RESET);
    if (w->result == RES_PASS) printf("%sn%s:next / ", C_BOLD, C_RESET);
    printf("%sh%s:%s / %sl%s:list / %sc%s:check all / %sx%s:reset / %sq%s:quit ? ",
           C_BOLD, C_RESET, w->show_hint ? "hide hint" : "hint", C_BOLD, C_RESET, C_BOLD, C_RESET,
           C_BOLD, C_RESET, C_BOLD, C_RESET);
    fflush(stdout);
}

static void run_current(Watch *w) {
    App *app = w->app;
    Exercise *ex = app_current(app);
    term_clear();
    printf("Checking %s...\n", ex->path);
    fflush(stdout);
    w->mtime = file_mtime_ns(ex->path);
    if (w->mtime < 0) {
        buf_clear(&w->out);
        buf_printf(&w->out, "%s does not exist. Press `x` to restore it.\n", ex->path);
        w->result = RES_COMPILE_ERROR;
    } else {
        w->result = run_source(ex, ex->path, ex->name, &w->out);
        if (w->result == RES_PASS) app_mark_done(app, ex);
    }
    render(w);
}

static void final_screen(App *app) {
    term_raw_disable();
    term_clear();
    printf("%s%s\n\n", C_GREEN, C_BOLD);
    printf("  +----------------------------------------------+\n");
    printf("  |   You finished every c-lings exercise!       |\n");
    printf("  +----------------------------------------------+%s\n\n", C_RESET);
    printf("%s\n", app->info.final_msg);
}

static bool confirm(const char *question) {
    printf("\n%s [y/N] ", question);
    fflush(stdout);
    int k = term_read_key(-1);
    return k == 'y' || k == 'Y';
}

static void welcome(App *app) {
    term_clear();
    printf("%s\n\n%sPress ENTER to continue%s ", app->info.welcome, C_BOLD, C_RESET);
    fflush(stdout);
    term_raw_enable();
    for (;;) {
        int k = term_read_key(-1);
        if (k == KEY_ENTER) break;
        if (k == 'q') {
            term_raw_disable();
            putchar('\n');
            exit(0);
        }
    }
}

int cmd_watch(App *app) {
    if (!isatty(STDIN_FILENO)) die("watch mode needs an interactive terminal (try `clings run`)");
    if (app->fresh) welcome(app);
    state_save(app);
    term_raw_enable();

    Watch w = {.app = app};
    run_current(&w);

    for (;;) {
        int k = term_read_key(200);
        if (k == KEY_NONE) {
            long long m = file_mtime_ns(app_current(app)->path);
            if (m != w.mtime) {
                /* let the editor finish writing before compiling */
                struct timespec ts = {0, 50 * 1000000};
                nanosleep(&ts, NULL);
                set_msg(&w, NULL);
                run_current(&w);
            }
            continue;
        }
        switch (k) {
        case 'n': {
            if (w.result != RES_PASS) break;
            size_t next = app_next_pending(app, app->current);
            if (next >= app->info.len) {
                /* rustlings re-checks everything before declaring victory */
                term_clear();
                size_t passed = check_all(app, true);
                if (passed == app->info.len) {
                    final_screen(app);
                    return 0;
                }
                next = app_next_pending(app, 0);
                app_set_current(app, next);
                set_msg(&w, xasprintf("%sSome exercises you finished earlier fail now. Starting with this one.%s",
                                      C_YELLOW, C_RESET));
            } else {
                app_set_current(app, next);
                set_msg(&w, NULL);
            }
            w.show_hint = false;
            run_current(&w);
            break;
        }
        case 'h':
            w.show_hint = !w.show_hint;
            render(&w);
            break;
        case 'l':
            if (list_ui(app)) {
                w.show_hint = false;
                set_msg(&w, NULL);
            }
            run_current(&w);
            break;
        case 'c': {
            term_clear();
            size_t passed = check_all(app, true);
            if (passed == app->info.len) {
                final_screen(app);
                return 0;
            }
            app_set_current(app, app_next_pending(app, 0));
            w.show_hint = false;
            set_msg(&w, xasprintf("%sChecked all exercises: %zu/%zu pass. Continuing at the first pending one.%s",
                                  C_YELLOW, passed, app->info.len, C_RESET));
            run_current(&w);
            break;
        }
        case 'x': {
            Exercise *ex = app_current(app);
            if (confirm("Reset this exercise to its original state? Your changes will be lost.")) {
                char err[256];
                if (app_reset_exercise(app, ex, err, sizeof err))
                    set_msg(&w, xasprintf("%sReset %s%s", C_YELLOW, ex->path, C_RESET));
                else
                    set_msg(&w, xasprintf("%s%s%s", C_RED, err, C_RESET));
                run_current(&w);
            } else {
                render(&w);
            }
            break;
        }
        case 'r':
            run_current(&w);
            break;
        case 'q':
        case 3: /* ctrl-c, in case ISIG is off */
            term_raw_disable();
            printf("\n\nWe hope you're enjoying learning C! Run `clings` again to continue.\n");
            return 0;
        default:
            break;
        }
    }
}
