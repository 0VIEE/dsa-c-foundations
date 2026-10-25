#include <assert.h>
#include <stdio.h>

#define FENWICK_TREE_NO_MAIN
#include "../data-structures/range-queries/fenwick_tree.c"

int main(void) {
    assert(create_bit(0) == NULL);
    assert(create_bit(-1) == NULL);

    struct FenwickTree *bit = create_bit(4);
    assert(bit != NULL);
    update(bit, 1, 1);
    update(bit, 2, 2);
    update(bit, 3, 3);
    update(bit, 4, 4);

    assert(query(bit, 0) == 0);
    assert(query(bit, 3) == 6);
    assert(query(bit, 100) == 10);
    assert(range_query(bit, 1, 3) == 6);
    assert(range_query(bit, 0, 2) == 0);
    assert(range_query(bit, 4, 3) == 0);
    assert(range_query(bit, 1, 5) == 0);

    update(bit, 0, 100); /* Must not enter an infinite loop. */
    update(bit, 5, 100);
    assert(query(bit, 4) == 10);

    free_bit(bit);
    free_bit(NULL);
    puts("PASS Fenwick tree boundary tests");
    return 0;
}
