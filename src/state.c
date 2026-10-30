/*
 * Progress is stored in .clings-state.txt:
 *
 *   DON'T EDIT THIS FILE!
 *
 *   <current exercise>
 *
 *   <done exercise>
 *   <done exercise>
 *   ...
 */
#include "common.h"

#include <stdlib.h>
#include <string.h>

static const char STATE_HEADER[] = "DON'T EDIT THIS FILE!";

void state_load(App *app) {
    Info *info = &app->info;
    Buf b = {0};
    app->current = 0;
    if (!read_file(STATE_FILE, &b)) {
        app->fresh = true;
        return;
    }
    app->fresh = false;

    const char *current = NULL;
    int lineno = 0;
    for (char *line = strtok(b.data, "\n"); line; line = strtok(NULL, "\n")) {
        size_t n = strlen(line);
        if (n && line[n - 1] == '\r') line[n - 1] = '\0';
        if (!*line) continue;
        if (lineno++ == 0) {
            if (strcmp(line, STATE_HEADER) != 0) break; /* unknown format: start over */
            continue;
        }
        if (!current) {
            current = line;
            continue;
        }
        Exercise *ex = info_find(info, line);
        if (ex) ex->done = true;
    }

    Exercise *cur = current ? info_find(info, current) : NULL;
    if (cur) {
        app->current = (size_t)(cur - info->items);
    } else {
        size_t next = app_next_pending(app, 0);
        app->current = next < info->len ? next : 0;
    }
    buf_free(&b);
}

bool state_save(const App *app) {
    const Info *info = &app->info;
    Buf b = {0};
    buf_printf(&b, "%s\n\n", STATE_HEADER);
    if (info->len) buf_printf(&b, "%s\n\n", info->items[app->current].name);
    for (size_t i = 0; i < info->len; i++)
        if (info->items[i].done) buf_printf(&b, "%s\n", info->items[i].name);
    bool ok = write_file(STATE_FILE, b.data, b.len);
    buf_free(&b);
    return ok;
}

size_t app_done_count(const App *app) {
    size_t n = 0;
    for (size_t i = 0; i < app->info.len; i++) n += app->info.items[i].done;
    return n;
}

size_t app_next_pending(const App *app, size_t from) {
    size_t n = app->info.len;
    for (size_t k = 0; k < n; k++) {
        size_t i = (from + k) % n;
        if (!app->info.items[i].done) return i;
    }
    return n;
}

void app_set_current(App *app, size_t idx) {
    if (idx >= app->info.len) return;
    app->current = idx;
    state_save(app);
}

Exercise *app_current(App *app) { return &app->info.items[app->current]; }

/* Marks an exercise as done and, like rustlings, drops the official solution
 * into solutions/ so the learner can compare. Returns true if newly done. */
bool app_mark_done(App *app, Exercise *ex) {
    bool newly = !ex->done;
    ex->done = true;
    if (!file_exists(ex->sol_path)) {
        const EmbeddedFile *sol = embedded_find(ex->sol_path);
        if (sol) write_file(ex->sol_path, (const char *)sol->data, sol->len);
    }
    state_save(app);
    return newly;
}

bool app_reset_exercise(App *app, Exercise *ex, char *err, size_t errlen) {
    const EmbeddedFile *orig = embedded_find(ex->path);
    if (!orig) {
        snprintf(err, errlen, "no original copy of %s is bundled with clings", ex->path);
        return false;
    }
    if (!write_file(ex->path, (const char *)orig->data, orig->len)) {
        snprintf(err, errlen, "failed to write %s", ex->path);
        return false;
    }
    ex->done = false;
    state_save(app);
    return true;
}
