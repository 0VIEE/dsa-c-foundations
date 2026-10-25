#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT_DIR"

tmpdir="$(mktemp -d)"
trap 'rm -rf "$tmpdir"' EXIT

compile_example() {
  local source="$1"
  local output="$2"
  gcc -std=c11 -D_POSIX_C_SOURCE=200809L -Wall -Wextra -Wpedantic -pthread "$source" -lm -o "$output"
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

echo "All targeted smoke tests passed."
