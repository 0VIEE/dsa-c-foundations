#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT_DIR"

if ! command -v gcc >/dev/null 2>&1; then
  echo "ERROR: gcc is required to run sanitizer tests." >&2
  exit 127
fi

tmpdir="$(mktemp -d)"
trap 'rm -rf "$tmpdir"' EXIT

for source in \
  tests/test_aho_corasick_array.c \
  tests/test_aho_corasick_trie.c \
  tests/test_kmp_search.c \
  tests/test_trie_autocomplete.c \
  tests/test_fenwick_tree.c
do
  name="${source##*/}"
  output="$tmpdir/${name%.c}"
  log="$tmpdir/${name}.log"

  if ! gcc -std=c11 -D_POSIX_C_SOURCE=200809L -Wall -Wextra -Wpedantic -Werror \
      -fsanitize=address,undefined -fno-omit-frame-pointer -g "$source" -o "$output" 2>"$log"; then
    echo "FAIL compile $source"
    cat "$log" >&2
    exit 1
  fi

  if ! ASAN_OPTIONS=detect_leaks=1:halt_on_error=1 UBSAN_OPTIONS=halt_on_error=1 \
      "$output" > "$tmpdir/${name}.out"; then
    echo "FAIL sanitizer run $source" >&2
    cat "$tmpdir/${name}.out" >&2
    exit 1
  fi

  echo "PASS sanitizer $source"
done

echo "All sanitizer checks passed."
