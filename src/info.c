/*
 * A parser for the small subset of TOML used by info.toml:
 *
 *   key = "string" | """multi-line string""" | true | false | 123
 *   [[exercises]]
 *   # comments
 */
#include "common.h"

#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    const char *p, *end;
    int line;
    char *err;
    size_t errlen;
} Parser;

typedef enum { V_STR, V_BOOL, V_INT } ValueKind;

typedef struct {
    ValueKind kind;
    char *str;
    bool b;
} Value;

static bool fail(Parser *ps, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    int n = snprintf(ps->err, ps->errlen, "info.toml:%d: ", ps->line);
    if (n >= 0 && (size_t)n < ps->errlen) vsnprintf(ps->err + n, ps->errlen - (size_t)n, fmt, ap);
    va_end(ap);
    return false;
}

static bool at(Parser *ps, const char *s) {
    size_t n = strlen(s);
    return (size_t)(ps->end - ps->p) >= n && memcmp(ps->p, s, n) == 0;
}

static void skip_inline_ws(Parser *ps) {
    while (ps->p < ps->end && (*ps->p == ' ' || *ps->p == '\t')) ps->p++;
}

static void skip_to_eol(Parser *ps) {
    while (ps->p < ps->end && *ps->p != '\n') ps->p++;
}

static bool parse_escape(Parser *ps, Buf *out) {
    ps->p++; /* backslash */
    if (ps->p >= ps->end) return fail(ps, "unterminated escape");
    char c = *ps->p++;
    switch (c) {
    case 'n': buf_append(out, "\n", 1); break;
    case 't': buf_append(out, "\t", 1); break;
    case '"': buf_append(out, "\"", 1); break;
    case '\\': buf_append(out, "\\", 1); break;
    case '\n':
        /* line-ending backslash: trim the newline and leading whitespace */
        ps->line++;
        while (ps->p < ps->end && isspace((unsigned char)*ps->p)) {
            if (*ps->p == '\n') ps->line++;
            ps->p++;
        }
        break;
    default: return fail(ps, "unsupported escape '\\%c'", c);
    }
    return true;
}

static bool parse_value(Parser *ps, Value *v) {
    Buf b = {0};
    buf_append(&b, "", 0);
    if (at(ps, "\"\"\"")) {
        ps->p += 3;
        if (ps->p < ps->end && *ps->p == '\n') {
            ps->p++;
            ps->line++;
        }
        for (;;) {
            if (ps->p >= ps->end) return fail(ps, "unterminated multi-line string");
            if (at(ps, "\"\"\"")) {
                ps->p += 3;
                break;
            }
            if (*ps->p == '\\') {
                if (!parse_escape(ps, &b)) return false;
                continue;
            }
            if (*ps->p == '\n') ps->line++;
            buf_append(&b, ps->p++, 1);
        }
        v->kind = V_STR;
        v->str = b.data;
        return true;
    }
    if (*ps->p == '"') {
        ps->p++;
        for (;;) {
            if (ps->p >= ps->end || *ps->p == '\n') return fail(ps, "unterminated string");
            if (*ps->p == '"') {
                ps->p++;
                break;
            }
            if (*ps->p == '\\') {
                if (!parse_escape(ps, &b)) return false;
                continue;
            }
            buf_append(&b, ps->p++, 1);
        }
        v->kind = V_STR;
        v->str = b.data;
        return true;
    }
    buf_free(&b);
    if (at(ps, "true")) {
        ps->p += 4;
        v->kind = V_BOOL;
        v->b = true;
        return true;
    }
    if (at(ps, "false")) {
        ps->p += 5;
        v->kind = V_BOOL;
        v->b = false;
        return true;
    }
    if (ps->p < ps->end && isdigit((unsigned char)*ps->p)) {
        while (ps->p < ps->end && isdigit((unsigned char)*ps->p)) ps->p++;
        v->kind = V_INT;
        return true;
    }
    return fail(ps, "expected a value");
}

static char *trim_trailing(char *s) {
    size_t n = strlen(s);
    while (n > 0 && isspace((unsigned char)s[n - 1])) s[--n] = '\0';
    return s;
}

static bool expect_str(Parser *ps, const char *key, Value *v, char **dst) {
    if (v->kind != V_STR) return fail(ps, "'%s' must be a string", key);
    free(*dst);
    *dst = v->str;
    return true;
}

static bool expect_bool(Parser *ps, const char *key, Value *v, bool *dst) {
    if (v->kind != V_BOOL) return fail(ps, "'%s' must be true or false", key);
    *dst = v->b;
    return true;
}

bool info_parse(const char *src, size_t len, Info *info, char *err, size_t errlen) {
    Parser ps = {.p = src, .end = src + len, .line = 1, .err = err, .errlen = errlen};
    size_t cap = 0;
    Exercise *cur = NULL;
    memset(info, 0, sizeof *info);

    while (ps.p < ps.end) {
        skip_inline_ws(&ps);
        if (ps.p >= ps.end) break;
        char c = *ps.p;
        if (c == '\n') {
            ps.p++;
            ps.line++;
            continue;
        }
        if (c == '\r' || c == '#') {
            skip_to_eol(&ps);
            continue;
        }
        if (at(&ps, "[[exercises]]")) {
            ps.p += 13;
            if (info->len == cap) {
                cap = cap ? cap * 2 : 64;
                info->items = xrealloc(info->items, cap * sizeof *info->items);
            }
            cur = &info->items[info->len++];
            memset(cur, 0, sizeof *cur);
            cur->test = true;
        } else if (c == '[') {
            return fail(&ps, "unsupported table header");
        } else {
            const char *k0 = ps.p;
            while (ps.p < ps.end && (isalnum((unsigned char)*ps.p) || *ps.p == '_' || *ps.p == '-')) ps.p++;
            if (ps.p == k0) return fail(&ps, "expected a key");
            char key[64];
            size_t klen = (size_t)(ps.p - k0);
            if (klen >= sizeof key) return fail(&ps, "key too long");
            memcpy(key, k0, klen);
            key[klen] = '\0';

            skip_inline_ws(&ps);
            if (ps.p >= ps.end || *ps.p != '=') return fail(&ps, "expected '=' after '%s'", key);
            ps.p++;
            skip_inline_ws(&ps);
            Value v = {0};
            if (!parse_value(&ps, &v)) return false;

            bool ok = true;
            if (!cur) {
                if (strcmp(key, "welcome_message") == 0) ok = expect_str(&ps, key, &v, &info->welcome);
                else if (strcmp(key, "final_message") == 0) ok = expect_str(&ps, key, &v, &info->final_msg);
                else if (strcmp(key, "format_version") == 0) ok = true;
                else return fail(&ps, "unknown top-level key '%s'", key);
            } else {
                if (strcmp(key, "name") == 0) ok = expect_str(&ps, key, &v, &cur->name);
                else if (strcmp(key, "dir") == 0) ok = expect_str(&ps, key, &v, &cur->dir);
                else if (strcmp(key, "hint") == 0) ok = expect_str(&ps, key, &v, &cur->hint);
                else if (strcmp(key, "test") == 0) ok = expect_bool(&ps, key, &v, &cur->test);
                else if (strcmp(key, "skip_check_unsolved") == 0) ok = expect_bool(&ps, key, &v, &cur->skip_check_unsolved);
                else return fail(&ps, "unknown exercise key '%s'", key);
            }
            if (!ok) return false;
        }
        skip_inline_ws(&ps);
        if (ps.p < ps.end && *ps.p == '#') skip_to_eol(&ps);
        if (ps.p < ps.end && *ps.p == '\r') ps.p++;
        if (ps.p < ps.end && *ps.p != '\n') return fail(&ps, "unexpected trailing characters");
    }

    for (size_t i = 0; i < info->len; i++) {
        Exercise *ex = &info->items[i];
        if (!ex->name) {
            snprintf(err, errlen, "info.toml: exercise #%zu has no name", i + 1);
            return false;
        }
        if (!ex->dir) ex->dir = xstrdup("");
        for (size_t j = 0; j < i; j++) {
            if (strcmp(info->items[j].name, ex->name) == 0) {
                snprintf(err, errlen, "info.toml: duplicate exercise name '%s'", ex->name);
                return false;
            }
        }
        ex->hint = ex->hint ? trim_trailing(ex->hint) : xstrdup("No hint available for this exercise.");
        const char *sep = *ex->dir ? "/" : "";
        ex->path = xasprintf("exercises/%s%s%s.c", ex->dir, sep, ex->name);
        ex->sol_path = xasprintf("solutions/%s%s%s.c", ex->dir, sep, ex->name);
    }
    if (!info->welcome) info->welcome = xstrdup("");
    if (!info->final_msg) info->final_msg = xstrdup("");
    trim_trailing(info->welcome);
    trim_trailing(info->final_msg);
    return true;
}

Exercise *info_find(Info *info, const char *name) {
    for (size_t i = 0; i < info->len; i++)
        if (strcmp(info->items[i].name, name) == 0) return &info->items[i];
    return NULL;
}
