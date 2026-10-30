#include "common.h"

#include <stdlib.h>
#include <string.h>

typedef enum { FILTER_ALL, FILTER_DONE, FILTER_PENDING } Filter;

static size_t build_view(App *app, Filter f, size_t *view) {
    size_t n = 0;
    for (size_t i = 0; i < app->info.len; i++) {
        bool done = app->info.items[i].done;
        if (f == FILTER_ALL || (f == FILTER_DONE && done) || (f == FILTER_PENDING && !done)) view[n++] = i;
    }
    return n;
}

static int name_width(App *app) {
    int w = 4;
    for (size_t i = 0; i < app->info.len; i++) {
        int n = (int)strlen(app->info.items[i].name);
        if (n > w) w = n;
    }
    return w;
}

bool list_ui(App *app) {
    size_t *view = xmalloc(app->info.len * sizeof *view);
    Filter filter = FILTER_ALL;
    size_t n = build_view(app, filter, view);
    size_t sel = app->current, top = 0;
    size_t start_current = app->current;
    int nw = name_width(app);
    char *msg = NULL;

    term_alt_screen(true);
    for (;;) {
        int rows, cols;
        term_size(&rows, &cols);
        size_t page = rows > 6 ? (size_t)rows - 5 : 1;
        if (n == 0) sel = 0;
        else if (sel >= n) sel = n - 1;
        if (sel < top) top = sel;
        if (sel >= top + page) top = sel - page + 1;

        fputs("\x1b[H\x1b[2J", stdout);
        printf("%s  Current  State    %-*s  Path%s\n", C_BOLD, nw, "Name", C_RESET);
        for (size_t r = 0; r < page; r++) {
            size_t vi = top + r;
            if (vi >= n) {
                putchar('\n');
                continue;
            }
            Exercise *ex = &app->info.items[view[vi]];
            bool is_cur = view[vi] == app->current;
            if (vi == sel) printf("%s", C_REV);
            printf("  %s%-7s%s  %s%-7s%s  %-*s  %s",
                   C_YELLOW, is_cur ? ">>>>>>>" : "", vi == sel ? "" : C_RESET,
                   ex->done ? C_GREEN : C_YELLOW, ex->done ? "DONE" : "PENDING", vi == sel ? "" : C_RESET,
                   nw, ex->name, ex->path);
            printf("%s\n", C_RESET);
        }
        size_t done = app_done_count(app);
        const char *fname = filter == FILTER_DONE ? " (showing done)" : filter == FILTER_PENDING ? " (showing pending)" : "";
        printf("%s%zu/%zu done%s%s\n", C_DIM, done, app->info.len, fname, C_RESET);
        printf("%s↓/j ↑/k home/g end/G%s | %sc%s:continue at | %sx%s:reset | filter %sd%s:done/%sp%s:pending | %sq%s:quit list\n",
               C_BOLD, C_RESET, C_BOLD, C_RESET, C_BOLD, C_RESET, C_BOLD, C_RESET, C_BOLD, C_RESET, C_BOLD, C_RESET);
        printf("%s", msg ? msg : "");
        fflush(stdout);

        int k = term_read_key(-1);
        free(msg);
        msg = NULL;
        switch (k) {
        case 'j': case KEY_DOWN: if (sel + 1 < n) sel++; break;
        case 'k': case KEY_UP: if (sel > 0) sel--; break;
        case 'g': case KEY_HOME: sel = 0; break;
        case 'G': case KEY_END: sel = n ? n - 1 : 0; break;
        case KEY_PGDN: sel += page; break;
        case KEY_PGUP: sel = sel > page ? sel - page : 0; break;
        case 'd': case 'p': {
            Filter want = k == 'd' ? FILTER_DONE : FILTER_PENDING;
            size_t keep = n ? view[sel] : 0;
            filter = filter == want ? FILTER_ALL : want;
            n = build_view(app, filter, view);
            sel = 0;
            for (size_t i = 0; i < n; i++)
                if (view[i] == keep) sel = i;
            break;
        }
        case 'c': case KEY_ENTER:
            if (n) {
                app_set_current(app, view[sel]);
                goto out;
            }
            break;
        case 'x':
            if (n) {
                Exercise *ex = &app->info.items[view[sel]];
                char err[256];
                if (app_reset_exercise(app, ex, err, sizeof err))
                    msg = xasprintf("%sReset %s%s", C_YELLOW, ex->path, C_RESET);
                else
                    msg = xasprintf("%s%s%s", C_RED, err, C_RESET);
                n = build_view(app, filter, view);
            }
            break;
        case 'q': case KEY_ESC: case 3:
            goto out;
        default:
            break;
        }
    }
out:
    free(msg);
    free(view);
    term_alt_screen(false);
    return app->current != start_current;
}
