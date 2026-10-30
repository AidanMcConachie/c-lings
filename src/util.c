#include "common.h"

#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

void *xmalloc(size_t n) {
    void *p = malloc(n ? n : 1);
    if (!p) die("out of memory");
    return p;
}

void *xrealloc(void *p, size_t n) {
    p = realloc(p, n ? n : 1);
    if (!p) die("out of memory");
    return p;
}

char *xstrdup(const char *s) {
    size_t n = strlen(s) + 1;
    char *d = xmalloc(n);
    memcpy(d, s, n);
    return d;
}

char *xasprintf(const char *fmt, ...) {
    va_list ap, ap2;
    va_start(ap, fmt);
    va_copy(ap2, ap);
    int n = vsnprintf(NULL, 0, fmt, ap);
    va_end(ap);
    char *s = xmalloc((size_t)n + 1);
    vsnprintf(s, (size_t)n + 1, fmt, ap2);
    va_end(ap2);
    return s;
}

void die(const char *fmt, ...) {
    term_raw_disable();
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, "%sclings: error:%s ", C_RED, C_RESET);
    vfprintf(stderr, fmt, ap);
    fputc('\n', stderr);
    va_end(ap);
    exit(1);
}

void buf_append(Buf *b, const char *s, size_t n) {
    if (b->len + n + 1 > b->cap) {
        size_t cap = b->cap ? b->cap : 256;
        while (cap < b->len + n + 1) cap *= 2;
        b->data = xrealloc(b->data, cap);
        b->cap = cap;
    }
    memcpy(b->data + b->len, s, n);
    b->len += n;
    b->data[b->len] = '\0';
}

void buf_puts(Buf *b, const char *s) { buf_append(b, s, strlen(s)); }

void buf_printf(Buf *b, const char *fmt, ...) {
    va_list ap, ap2;
    va_start(ap, fmt);
    va_copy(ap2, ap);
    int n = vsnprintf(NULL, 0, fmt, ap);
    va_end(ap);
    if (n > 0) {
        buf_append(b, "", 0); /* ensure allocated */
        if (b->len + (size_t)n + 1 > b->cap) {
            size_t cap = b->cap;
            while (cap < b->len + (size_t)n + 1) cap *= 2;
            b->data = xrealloc(b->data, cap);
            b->cap = cap;
        }
        vsnprintf(b->data + b->len, (size_t)n + 1, fmt, ap2);
        b->len += (size_t)n;
    }
    va_end(ap2);
}

void buf_clear(Buf *b) {
    b->len = 0;
    if (b->data) b->data[0] = '\0';
}

void buf_free(Buf *b) {
    free(b->data);
    b->data = NULL;
    b->len = b->cap = 0;
}

bool read_file(const char *path, Buf *out) {
    FILE *f = fopen(path, "rb");
    if (!f) return false;
    char chunk[8192];
    size_t n;
    buf_clear(out);
    buf_append(out, "", 0);
    while ((n = fread(chunk, 1, sizeof chunk, f)) > 0) buf_append(out, chunk, n);
    bool ok = !ferror(f);
    fclose(f);
    return ok;
}

bool mkdir_p(const char *path) {
    char *tmp = xstrdup(path);
    for (char *p = tmp + 1; *p; p++) {
        if (*p != '/') continue;
        *p = '\0';
        if (mkdir(tmp, 0755) != 0 && errno != EEXIST) {
            free(tmp);
            return false;
        }
        *p = '/';
    }
    bool ok = mkdir(tmp, 0755) == 0 || errno == EEXIST;
    free(tmp);
    return ok;
}

bool write_file(const char *path, const char *data, size_t len) {
    const char *slash = strrchr(path, '/');
    if (slash) {
        char *dir = xstrdup(path);
        dir[slash - path] = '\0';
        bool ok = mkdir_p(dir);
        free(dir);
        if (!ok) return false;
    }
    FILE *f = fopen(path, "wb");
    if (!f) return false;
    bool ok = fwrite(data, 1, len, f) == len;
    if (fclose(f) != 0) ok = false;
    return ok;
}

bool file_exists(const char *path) {
    struct stat st;
    return stat(path, &st) == 0 && S_ISREG(st.st_mode);
}

bool dir_exists(const char *path) {
    struct stat st;
    return stat(path, &st) == 0 && S_ISDIR(st.st_mode);
}

long long file_mtime_ns(const char *path) {
    struct stat st;
    if (stat(path, &st) != 0) return -1;
#if defined(__APPLE__)
    return (long long)st.st_mtimespec.tv_sec * 1000000000LL + st.st_mtimespec.tv_nsec + st.st_size;
#else
    return (long long)st.st_mtim.tv_sec * 1000000000LL + st.st_mtim.tv_nsec + st.st_size;
#endif
}
