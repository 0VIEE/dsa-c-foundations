#include <assert.h>
#include <stdio.h>
#include <string.h>

#define AHO_CORASICK_NO_MAIN
#include "../algorithms/strings/aho_corasick.c"

int main(void) {
    struct ACAry *ac = create_ac();
    assert(ac != NULL);
    assert(!insert_pattern(NULL, "he"));
    assert(!insert_pattern(ac, NULL));
    assert(!insert_pattern(ac, ""));
    assert(!insert_pattern(ac, "He"));
    assert(!insert_pattern(ac, "he!"));

    char too_long[MAX_PATTERN_LENGTH + 2];
    memset(too_long, 'a', sizeof(too_long) - 1);
    too_long[sizeof(too_long) - 1] = '\0';
    assert(!insert_pattern(ac, too_long));

    assert(insert_pattern(ac, "he"));
    int nodes_after_first_insert = ac->sz;
    assert(insert_pattern(ac, "he")); /* Duplicate strings are harmless. */
    assert(ac->sz == nodes_after_first_insert);
    assert(insert_pattern(ac, "she"));
    assert(insert_pattern(ac, "hers"));
    assert(build_automaton(ac));
    assert(build_automaton(ac)); /* Building twice must be safe. */
    assert(!insert_pattern(ac, "his"));
    assert(!search_text(ac, "usHers"));
    assert(!search_text(ac, NULL));
    assert(search_text(ac, "ushers"));
    free_ac(ac);

    /* Four distinct 31-character patterns use 125 nodes including the root.
       The fifth must fail cleanly instead of writing past the fixed array. */
    struct ACAry *limited = create_ac();
    assert(limited != NULL);
    char pattern[MAX_PATTERN_LENGTH + 1];
    for (int i = 0; i < 4; i++) {
        pattern[0] = (char)('a' + i);
        memset(pattern + 1, 'z', MAX_PATTERN_LENGTH - 1);
        pattern[MAX_PATTERN_LENGTH] = '\0';
        assert(insert_pattern(limited, pattern));
    }
    pattern[0] = 'e';
    assert(!insert_pattern(limited, pattern));
    assert(limited->sz <= MAX_NODES);
    free_ac(limited);

    puts("PASS fixed-array Aho-Corasick edge cases");
    return 0;
}
