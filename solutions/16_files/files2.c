#include <stdio.h>

#include "clings.h"

#define PATH ".clings/build/files2.txt"

long sum_scores(const char *path) {
    FILE *f = fopen(path, "r");
    if (f == NULL) {
        return -1;
    }
    long total = 0;
    char line[256];
    while (fgets(line, sizeof line, f) != NULL) {
        char name[64];
        int score;
        // %63[^,] reads up to 63 characters that aren't commas. sscanf
        // returns how many conversions succeeded.
        if (sscanf(line, "%63[^,],%d", name, &score) == 2) {
            total += score;
        }
    }
    fclose(f);
    return total;
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
