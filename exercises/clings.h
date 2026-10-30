/*
 * clings.h - the tiny test harness used by the c-lings exercises.
 *
 * Including this file gives your program a `main` that runs every TEST(...)
 * in the file. You don't need to understand how it works to do the
 * exercises, but it's a fun read once you've done the pointers, memory and
 * preprocessor sections!
 *
 *   TEST(name) { ... }            define a test
 *   ASSERT(cond)                  cond must be true
 *   ASSERT_FALSE(cond)            cond must be false
 *   ASSERT_EQ(a, b)               integers a == b
 *   ASSERT_NE(a, b)               integers a != b
 *   ASSERT_STR_EQ(a, b)           strings have the same contents
 *   ASSERT_NEAR(a, b, eps)        |a - b| <= eps for floating point numbers
 *   ASSERT_NULL(p) / ASSERT_NOT_NULL(p)
 *   ASSERT_PTR_EQ(a, b)           two pointers are the same address
 *
 * It also watches malloc/calloc/realloc/free: a test fails if it leaks
 * memory or writes past the end of a heap block.
 */
#ifndef CLINGS_H
#define CLINGS_H

#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ---- test registry ---------------------------------------------------- */

typedef void (*clings_test_fn)(void);

static struct {
    const char *name;
    clings_test_fn fn;
} clings_tests[256];
static int clings_test_count;
static bool clings_test_failed;

static inline void clings_register(const char *name, clings_test_fn fn) {
    if (clings_test_count < 256) {
        clings_tests[clings_test_count].name = name;
        clings_tests[clings_test_count].fn = fn;
        clings_test_count++;
    }
}

static inline void clings_fail(const char *file, int line, const char *what) {
    if (!clings_test_failed) puts("FAILED");
    clings_test_failed = true;
    if (file) printf("    %s:%d: %s\n", file, line, what);
    else printf("    %s\n", what);
}

#define TEST(name)                                                          \
    static void clings_test_##name(void);                                   \
    __attribute__((constructor)) static void clings_register_##name(void) { \
        clings_register(#name, clings_test_##name);                         \
    }                                                                       \
    static void clings_test_##name(void)

/* ---- assertions ------------------------------------------------------- */

#define ASSERT(cond)                                                        \
    do {                                                                    \
        if (!(cond)) {                                                      \
            clings_fail(__FILE__, __LINE__, "ASSERT(" #cond ") failed");    \
            return;                                                         \
        }                                                                   \
    } while (0)

#define ASSERT_TRUE(cond) ASSERT(cond)

#define ASSERT_FALSE(cond)                                                    \
    do {                                                                      \
        if (cond) {                                                           \
            clings_fail(__FILE__, __LINE__, "ASSERT_FALSE(" #cond ") failed"); \
            return;                                                           \
        }                                                                     \
    } while (0)

#define CLINGS_CMP_INT_(a, b, op, what)                                          \
    do {                                                                         \
        long long clings_a_ = (long long)(a), clings_b_ = (long long)(b);        \
        if (!(clings_a_ op clings_b_)) {                                         \
            clings_fail(__FILE__, __LINE__, what " failed");                     \
            printf("        left:  %lld\n        right: %lld\n", clings_a_, clings_b_); \
            return;                                                              \
        }                                                                        \
    } while (0)

/* Stringified here, before the arguments get macro-expanded. */
#define ASSERT_EQ(a, b) CLINGS_CMP_INT_(a, b, ==, "ASSERT_EQ(" #a ", " #b ")")
#define ASSERT_NE(a, b) CLINGS_CMP_INT_(a, b, !=, "ASSERT_NE(" #a ", " #b ")")

#define ASSERT_STR_EQ(a, b)                                                          \
    do {                                                                             \
        const char *clings_a_ = (a), *clings_b_ = (b);                               \
        if (!clings_a_ || !clings_b_ || strcmp(clings_a_, clings_b_) != 0) {         \
            clings_fail(__FILE__, __LINE__, "ASSERT_STR_EQ(" #a ", " #b ") failed"); \
            printf(clings_a_ ? "        left:  \"%s\"\n" : "        left:  %s\n",    \
                   clings_a_ ? clings_a_ : "NULL");                                  \
            printf(clings_b_ ? "        right: \"%s\"\n" : "        right: %s\n",    \
                   clings_b_ ? clings_b_ : "NULL");                                  \
            return;                                                                  \
        }                                                                            \
    } while (0)

#define ASSERT_NEAR(a, b, eps)                                                             \
    do {                                                                                   \
        double clings_a_ = (double)(a), clings_b_ = (double)(b);                           \
        if (!(fabs(clings_a_ - clings_b_) <= (eps))) {                                     \
            clings_fail(__FILE__, __LINE__, "ASSERT_NEAR(" #a ", " #b ", " #eps ") failed"); \
            printf("        left:  %.10g\n        right: %.10g\n", clings_a_, clings_b_);  \
            return;                                                                        \
        }                                                                                  \
    } while (0)

#define ASSERT_PTR_EQ(a, b)                                                          \
    do {                                                                             \
        const void *clings_a_ = (a), *clings_b_ = (b);                               \
        if (clings_a_ != clings_b_) {                                                \
            clings_fail(__FILE__, __LINE__, "ASSERT_PTR_EQ(" #a ", " #b ") failed"); \
            printf("        left:  %p\n        right: %p\n", clings_a_, clings_b_);  \
            return;                                                                  \
        }                                                                            \
    } while (0)

#define ASSERT_NULL(p)                                                       \
    do {                                                                     \
        if ((p) != NULL) {                                                   \
            clings_fail(__FILE__, __LINE__, "ASSERT_NULL(" #p ") failed");   \
            return;                                                          \
        }                                                                    \
    } while (0)

#define ASSERT_NOT_NULL(p)                                                     \
    do {                                                                       \
        if ((p) == NULL) {                                                     \
            clings_fail(__FILE__, __LINE__, "ASSERT_NOT_NULL(" #p ") failed"); \
            return;                                                            \
        }                                                                      \
    } while (0)

/* ---- heap tracking ------------------------------------------------------ */

#define CLINGS_CANARY_SIZE 16
#define CLINGS_CANARY_BYTE 0xCA

typedef struct {
    void *ptr;
    size_t size;
    const char *file;
    int line;
} clings_alloc_t;

static clings_alloc_t *clings_allocs;
static size_t clings_allocs_len, clings_allocs_cap;

static inline void clings_track(void *p, size_t size, const char *file, int line) {
    if (!p) return;
    memset((char *)p + size, CLINGS_CANARY_BYTE, CLINGS_CANARY_SIZE);
    if (clings_allocs_len == clings_allocs_cap) {
        clings_allocs_cap = clings_allocs_cap ? clings_allocs_cap * 2 : 64;
        clings_allocs = realloc(clings_allocs, clings_allocs_cap * sizeof *clings_allocs);
        if (!clings_allocs) abort();
    }
    clings_allocs[clings_allocs_len++] = (clings_alloc_t){p, size, file, line};
}

static inline void clings_check_canary(const clings_alloc_t *a) {
    const unsigned char *end = (const unsigned char *)a->ptr + a->size;
    for (int i = 0; i < CLINGS_CANARY_SIZE; i++) {
        if (end[i] != CLINGS_CANARY_BYTE) {
            char msg[256];
            snprintf(msg, sizeof msg, "heap buffer overflow: something wrote past the end of the %zu-byte block allocated here",
                     a->size);
            clings_fail(a->file, a->line, msg);
            return;
        }
    }
}

/* Removes p from the table. Returns false if p wasn't allocated by us. */
static inline bool clings_untrack(void *p) {
    for (size_t i = clings_allocs_len; i-- > 0;) {
        if (clings_allocs[i].ptr == p) {
            clings_check_canary(&clings_allocs[i]);
            clings_allocs[i] = clings_allocs[--clings_allocs_len];
            return true;
        }
    }
    return false;
}

static inline void *clings_malloc(size_t n, const char *file, int line) {
    void *p = malloc(n + CLINGS_CANARY_SIZE);
    clings_track(p, n, file, line);
    return p;
}

static inline void *clings_calloc(size_t count, size_t n, const char *file, int line) {
    if (n && count > (SIZE_MAX - CLINGS_CANARY_SIZE) / n) return NULL;
    void *p = calloc(1, count * n + CLINGS_CANARY_SIZE);
    clings_track(p, count * n, file, line);
    return p;
}

static inline void *clings_realloc(void *old, size_t n, const char *file, int line) {
    if (old) clings_untrack(old);
    void *p = realloc(old, n + CLINGS_CANARY_SIZE);
    if (p) {
        clings_track(p, n, file, line);
    } else if (old) {
        /* realloc failed, so the old block is still alive: track it again */
        clings_allocs[clings_allocs_len++] = (clings_alloc_t){old, 0, file, line};
    }
    return p;
}

static inline void clings_free(void *p) {
    if (p) clings_untrack(p);
    free(p);
}

static inline void clings_check_leaks(void) {
    for (size_t i = 0; i < clings_allocs_len; i++) clings_check_canary(&clings_allocs[i]);
    if (clings_allocs_len) {
        char msg[256];
        snprintf(msg, sizeof msg, "memory leak: %zu block(s) were never freed. They were allocated at:",
                 clings_allocs_len);
        clings_fail(NULL, 0, msg);
        for (size_t i = 0; i < clings_allocs_len && i < 10; i++)
            printf("        %s:%d (%zu bytes)\n", clings_allocs[i].file, clings_allocs[i].line,
                   clings_allocs[i].size);
    }
    clings_allocs_len = 0; /* each test starts with a clean slate */
}

/* ---- main --------------------------------------------------------------- */

int main(void) {
    setvbuf(stdout, NULL, _IONBF, 0); /* keep output even if a test crashes */
    int failed = 0;
    printf("running %d test%s\n", clings_test_count, clings_test_count == 1 ? "" : "s");
    for (int i = 0; i < clings_test_count; i++) {
        printf("test %s ... ", clings_tests[i].name);
        clings_test_failed = false;
        clings_tests[i].fn();
        clings_check_leaks();
        if (clings_test_failed) failed++;
        else puts("ok");
    }
    printf("\ntest result: %s. %d passed; %d failed\n", failed ? "FAILED" : "ok", clings_test_count - failed,
           failed);
    return failed ? 1 : 0;
}

/* From here on, the exercise's malloc & co. go through the tracker. */
#define malloc(n) clings_malloc((n), __FILE__, __LINE__)
#define calloc(count, n) clings_calloc((count), (n), __FILE__, __LINE__)
#define realloc(p, n) clings_realloc((p), (n), __FILE__, __LINE__)
#define free(p) clings_free(p)

#endif
