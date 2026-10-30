#include <stdbool.h>
#include <stdio.h>

#include "clings.h"

#define PATH ".clings/build/files1.txt"

bool write_lines(const char *path, const char *lines[], size_t n) {
    FILE *f = fopen(path, "w");
    if (f == NULL) {
        return false;
    }
    for (size_t i = 0; i < n; i++) {
        fprintf(f, "%s\n", lines[i]);
    }
    // Without fclose the text stays in the stream's buffer and never
    // reaches the file (and the FILE leaks). fclose can fail too, e.g. when
    // the disk is full, so check it.
    return fclose(f) == 0;
}

int count_lines(const char *path) {
    FILE *f = fopen(path, "r");
    if (f == NULL) {
        return -1;
    }
    int count = 0;
    int c;
    while ((c = fgetc(f)) != EOF) {
        if (c == '\n') {
            count++;
        }
    }
    fclose(f);
    return count;
}

TEST(round_trip) {
    const char *lines[] = {"first", "second", "third"};
    ASSERT(write_lines(PATH, lines, 3));
    ASSERT_EQ(count_lines(PATH), 3);
}

TEST(missing_file) {
    ASSERT_EQ(count_lines(".clings/build/does-not-exist.txt"), -1);
}
