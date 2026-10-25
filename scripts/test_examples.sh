#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT_DIR"

tmpdir="$(mktemp -d)"
trap 'rm -rf "$tmpdir"' EXIT

compile_example() {
  local source="$1"
  local output="$2"
  gcc -std=c11 -D_POSIX_C_SOURCE=200809L -Wall -Wextra -Wpedantic -Werror -pthread "$source" -lm -o "$output"
}

assert_contains() {
  local output_file="$1"
  local expected="$2"
  if ! grep -Fq "$expected" "$output_file"; then
    echo "FAIL: expected output not found: $expected" >&2
    echo "--- program output ---" >&2
    cat "$output_file" >&2
    exit 1
  fi
}

compile_example "algorithms/strings/aho_corasick.c" "$tmpdir/aho_array"
"$tmpdir/aho_array" > "$tmpdir/aho_array.out"
assert_contains "$tmpdir/aho_array.out" 'Found pattern "she" ending at index 3'
assert_contains "$tmpdir/aho_array.out" 'Found pattern "he" ending at index 3'
assert_contains "$tmpdir/aho_array.out" 'Found pattern "hers" ending at index 5'
echo "PASS Aho-Corasick fixed-array example"

compile_example "algorithms/strings/aho_corasick_trie.c" "$tmpdir/aho_trie"
"$tmpdir/aho_trie" > "$tmpdir/aho_trie.out"
assert_contains "$tmpdir/aho_trie.out" 'Pattern [ID 2: "his"] at text index 1'
assert_contains "$tmpdir/aho_trie.out" 'Pattern [ID 1: "she"] at text index 3'
assert_contains "$tmpdir/aho_trie.out" 'Pattern [ID 0: "he"] at text index 4'
assert_contains "$tmpdir/aho_trie.out" 'Pattern [ID 3: "hers"] at text index 4'
echo "PASS Aho-Corasick pointer-trie example"

compile_example "data-structures/trees/bst.c" "$tmpdir/bst"
"$tmpdir/bst" > "$tmpdir/bst.out"
assert_contains "$tmpdir/bst.out" 'Sorted Inorder Traversal: 20 30 40 50 60 70 80'
assert_contains "$tmpdir/bst.out" 'Search 40: FOUND'
assert_contains "$tmpdir/bst.out" 'Search 99: NOT FOUND'
echo "PASS BST insertion/search example"

compile_example "data-structures/trees/bst_delete.c" "$tmpdir/bst_delete"
"$tmpdir/bst_delete" > "$tmpdir/bst_delete.out"
assert_contains "$tmpdir/bst_delete.out" 'After deleting 20 (leaf): 30 40 50 60 70 80'
assert_contains "$tmpdir/bst_delete.out" 'After deleting 50 (root with 2 kids): 30 40 60 70 80'
echo "PASS BST deletion example"

compile_example "algorithms/graphs/dinic_max_flow.c" "$tmpdir/dinic"
"$tmpdir/dinic" > "$tmpdir/dinic.out"
assert_contains "$tmpdir/dinic.out" 'Calculated Maximum Flow: 20 (Expected: 20)'
echo "PASS Dinic maximum-flow example"

compile_example "algorithms/strings/kmp_search.c" "$tmpdir/kmp"
"$tmpdir/kmp" > "$tmpdir/kmp.out"
assert_contains "$tmpdir/kmp.out" 'Occurrences found at text indices: [ 10 ] (Total: 1)'
assert_contains "$tmpdir/kmp.out" 'Occurrences found at text indices: [ 0 2 5 8 10 12 ] (Total: 6)'
echo "PASS KMP string search"

compile_example "tests/test_kmp_search.c" "$tmpdir/test_kmp"
"$tmpdir/test_kmp" > "$tmpdir/test_kmp.out"
assert_contains "$tmpdir/test_kmp.out" 'Occurrences found at text indices: [ 0 1 2 ] (Total: 3)'
assert_contains "$tmpdir/test_kmp.out" 'PASS KMP invalid-input and overlapping-match tests'
echo "PASS KMP edge tests"

compile_example "algorithms/graphs/prim_mst_heap.c" "$tmpdir/prim_list"
"$tmpdir/prim_list" > "$tmpdir/prim_list.out"
assert_contains "$tmpdir/prim_list.out" 'Total MST Weight: 19'
echo "PASS Prim MST adjacency-list example"

compile_example "algorithms/graphs/prim_mst_matrix.c" "$tmpdir/prim_matrix"
"$tmpdir/prim_matrix" > "$tmpdir/prim_matrix.out"
assert_contains "$tmpdir/prim_matrix.out" 'Total MST Weight: 17'
echo "PASS Prim MST adjacency-matrix example"

compile_example "algorithms/dynamic-programming/knapsack_01.c" "$tmpdir/knapsack"
"$tmpdir/knapsack" > "$tmpdir/knapsack.out"
assert_contains "$tmpdir/knapsack.out" 'Maximum Attainable Value: 220'
echo "PASS 0/1 knapsack example"

compile_example "data-structures/range-queries/fenwick_tree.c" "$tmpdir/fenwick"
"$tmpdir/fenwick" > "$tmpdir/fenwick.out"
assert_contains "$tmpdir/fenwick.out" 'Prefix Sum up to index 5: 15'
assert_contains "$tmpdir/fenwick.out" 'Range Sum [3, 6]:         14'
assert_contains "$tmpdir/fenwick.out" 'New Range Sum [3, 6]:     20'
assert_contains "$tmpdir/fenwick.out" 'New Prefix Sum up to 5:   21'
echo "PASS Fenwick tree example"

compile_example "tests/test_fenwick_tree.c" "$tmpdir/test_fenwick"
"$tmpdir/test_fenwick" > "$tmpdir/test_fenwick.out"
assert_contains "$tmpdir/test_fenwick.out" 'PASS Fenwick tree boundary tests'
echo "PASS Fenwick tree edge tests"

compile_example "data-structures/tries/trie_autocomplete.c" "$tmpdir/trie"
"$tmpdir/trie" > "$tmpdir/trie.out"
assert_contains "$tmpdir/trie.out" 'Autocomplete suggestions for "cat":'
assert_contains "$tmpdir/trie.out" ' -> category'
assert_contains "$tmpdir/trie.out" 'No suggestions found for prefix "xyz"'
echo "PASS trie autocomplete example"

compile_example "tests/test_trie_autocomplete.c" "$tmpdir/test_trie"
"$tmpdir/test_trie" > "$tmpdir/test_trie.out"
assert_contains "$tmpdir/test_trie.out" ' -> cater'
assert_contains "$tmpdir/test_trie.out" 'PASS trie autocomplete input-validation tests'
echo "PASS trie autocomplete edge tests"

compile_example "tests/test_aho_corasick_array.c" "$tmpdir/test_aho_array"
"$tmpdir/test_aho_array" > "$tmpdir/test_aho_array.out"
assert_contains "$tmpdir/test_aho_array.out" 'PASS fixed-array Aho-Corasick edge cases'
echo "PASS fixed-array Aho-Corasick edge tests"

compile_example "tests/test_aho_corasick_trie.c" "$tmpdir/test_aho_trie"
"$tmpdir/test_aho_trie" > "$tmpdir/test_aho_trie.out"
assert_contains "$tmpdir/test_aho_trie.out" 'Pattern [ID 4: "he"] at text index 4'
assert_contains "$tmpdir/test_aho_trie.out" 'PASS pointer-trie Aho-Corasick edge cases'
echo "PASS pointer-trie Aho-Corasick edge tests"

echo "All targeted smoke tests passed."
