#include <assert.h>
#include <stdio.h>

#define KMP_SEARCH_NO_MAIN
#include "../algorithms/strings/kmp_search.c"

int main(void) {
    assert(!kmp_search(NULL, "a"));
    assert(!kmp_search("abc", NULL));
    assert(!kmp_search("abc", ""));
    assert(kmp_search("aaaa", "aa"));
    puts("PASS KMP invalid-input and overlapping-match tests");
    return 0;
}
