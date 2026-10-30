// Parse a file of scores. Each line looks like `name,score`, for example:
//
//   alice,90
//   bob,75
//
// `sum_scores` returns the sum of all scores in the file. Lines that don't
// match the format are skipped. Returns -1 if the file can't be opened.
//
// fgets reads a line into a buffer, and sscanf can pick it apart.

#include <stdio.h>

#include "clings.h"

#define PATH ".clings/build/files2.txt"

long sum_scores(const char *path) {
    // TODO
    return -1;
}

static void write_file(const char *contents) {
    FILE *f = fopen(PATH, "w");
    if (f != NULL) {
        fputs(contents, f);
        fclose(f);
    }
}

TEST(sums_scores) {
    write_file("alice,90\nbob,75\ncarol,100\n");
    ASSERT_EQ(sum_scores(PATH), 265);
}

TEST(skips_bad_lines) {
    write_file("alice,90\n\nthis line is junk\nbob,x\ndave,10\n");
    ASSERT_EQ(sum_scores(PATH), 100);
}

TEST(no_trailing_newline) {
    write_file("eve,5\nfrank,6");
    ASSERT_EQ(sum_scores(PATH), 11);
}

TEST(missing_file) {
    ASSERT_EQ(sum_scores(".clings/build/no-such-file.txt"), -1);
}
