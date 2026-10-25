#include <assert.h>
#include <stdio.h>
#include <string.h>

#define AHO_CORASICK_NO_MAIN
#include "../algorithms/strings/aho_corasick_trie.c"

int main(void) {
    struct TrieNode *root = create_node();
    assert(root != NULL);
    assert(!insert_pattern(NULL, "he", 0));
    assert(!insert_pattern(root, NULL, 0));
    assert(!insert_pattern(root, "", 0));
    assert(!insert_pattern(root, "He", 0));
    assert(!insert_pattern(root, "he!", 0));
    assert(!insert_pattern(root, "he", -1));

    char too_long[MAX_PATTERN_LENGTH + 2];
    memset(too_long, 'a', sizeof(too_long) - 1);
    too_long[sizeof(too_long) - 1] = '\0';
    assert(!insert_pattern(root, too_long, 0));

    const char *patterns[] = {"he", "she", "his", "hers", "he"};
    assert(insert_pattern(root, patterns[0], 0));
    assert(insert_pattern(root, patterns[1], 1));
    assert(insert_pattern(root, patterns[2], 2));
    assert(insert_pattern(root, patterns[3], 3));
    assert(insert_pattern(root, patterns[4], 4)); /* Duplicate word, distinct ID. */
    assert(insert_pattern(root, patterns[4], 4)); /* Duplicate ID is ignored. */

    assert(build_failure_links(root));
    assert(build_failure_links(root)); /* Building twice must be safe. */
    assert(!insert_pattern(root, "new", 5));
    assert(!aho_corasick_search(root, "aHishers", patterns, 5));
    assert(!aho_corasick_search(root, NULL, patterns, 5));
    assert(!aho_corasick_search(root, "ahishers", patterns, 0));
    assert(aho_corasick_search(root, "ahishers", patterns, 5));

    free_trie(root);
    puts("PASS pointer-trie Aho-Corasick edge cases");
    return 0;
}
