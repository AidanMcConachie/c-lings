#include "clings.h"

void str_reverse(char *s) {
    size_t len = strlen(s);
    if (len == 0) {
        return;
    }
    char *left = s;
    char *right = s + len - 1;
    while (left < right) {
        char tmp = *left;
        *left++ = *right;
        *right-- = tmp;
    }
}

size_t word_count(const char *s) {
    size_t count = 0;
    bool in_word = false;
    for (; *s; s++) {
        if (*s == ' ') {
            in_word = false;
        } else if (!in_word) {
            in_word = true;
            count++;
        }
    }
    return count;
}

void reverse_words(const char *s, char *out) {
    char *o = out;
    const char *end = s + strlen(s);
    // Walk backwards from the end, copying each word as we find it.
    while (end > s) {
        while (end > s && end[-1] == ' ') end--;
        const char *start = end;
        while (start > s && start[-1] != ' ') start--;
        if (start == end) break;
        if (o != out) *o++ = ' ';
        memcpy(o, start, (size_t)(end - start));
        o += end - start;
        end = start;
    }
    *o = '\0';
}

TEST(reverses_strings) {
    char a[] = "hello";
    str_reverse(a);
    ASSERT_STR_EQ(a, "olleh");

    char b[] = "ab";
    str_reverse(b);
    ASSERT_STR_EQ(b, "ba");

    char c[] = "";
    str_reverse(c);
    ASSERT_STR_EQ(c, "");
}

TEST(counts_words) {
    ASSERT_EQ(word_count("hello world"), 2);
    ASSERT_EQ(word_count("  lots   of   space  "), 3);
    ASSERT_EQ(word_count("single"), 1);
    ASSERT_EQ(word_count(""), 0);
    ASSERT_EQ(word_count("   "), 0);
}

TEST(reverses_words) {
    char out[64];
    reverse_words("the quick brown fox", out);
    ASSERT_STR_EQ(out, "fox brown quick the");

    reverse_words("  spaced   out ", out);
    ASSERT_STR_EQ(out, "out spaced");

    reverse_words("", out);
    ASSERT_STR_EQ(out, "");
}
