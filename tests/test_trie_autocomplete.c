#include <assert.h>
#include <stdio.h>
#include <string.h>

#define TRIE_AUTOCOMPLETE_NO_MAIN
#include "../data-structures/tries/trie_autocomplete.c"

int main(void) {
    struct TrieNode *root = create_node();
    assert(root != NULL);

    assert(!insert(NULL, "cat"));
    assert(!insert(root, NULL));
    assert(!insert(root, ""));
    assert(!insert(root, "Cat"));
    assert(!insert(root, "cat!"));

    char too_long[MAX_WORD_LEN + 1];
    memset(too_long, 'a', sizeof(too_long) - 1);
    too_long[sizeof(too_long) - 1] = '\0';
    assert(!insert(root, too_long));

    assert(insert(root, "cat"));
    assert(insert(root, "cater"));
    assert(insert(root, "cat")); /* Duplicate insertion is harmless. */
    assert(print_autocomplete_suggestions(root, "cat"));
    assert(print_autocomplete_suggestions(root, "xyz"));
    assert(!print_autocomplete_suggestions(root, "Cat"));
    assert(!print_autocomplete_suggestions(root, too_long));

    free_trie(root);
    puts("PASS trie autocomplete input-validation tests");
    return 0;
}
