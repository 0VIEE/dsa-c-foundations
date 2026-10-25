#!/usr/bin/env bash
set -uo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT_DIR"

if ! command -v gcc >/dev/null 2>&1; then
  echo "ERROR: gcc is required to compile the examples." >&2
  exit 127
fi

tmpdir="$(mktemp -d)"
trap 'rm -rf "$tmpdir"' EXIT

total=0
failures=0
warning_files=0

while IFS= read -r -d '' source; do
  total=$((total + 1))
  relative_path="${source#./}"
  output_name="${relative_path//\//_}"
  output="$tmpdir/${output_name%.c}"
  log="$tmpdir/${output_name}.log"

  if gcc -std=c11 -D_POSIX_C_SOURCE=200809L -Wall -Wextra -Wpedantic -pthread "$source" -lm -o "$output" 2>"$log"; then
    if [[ -s "$log" ]]; then
      warning_files=$((warning_files + 1))
      echo "WARN $relative_path"
      sed 's/^/  /' "$log"
    else
      echo "PASS $relative_path"
    fi
  else
    failures=$((failures + 1))
    echo "FAIL $relative_path"
    sed 's/^/  /' "$log"
  fi
done < <(find . -type f -name '*.c' -not -path './.git/*' -print0 | sort -z)

echo
echo "Compile summary: $total source file(s), $failures failure(s), $warning_files file(s) compiled with warnings."

if [[ "$total" -eq 0 ]]; then
  echo "ERROR: no C source files were found." >&2
  exit 1
fi

if [[ "$failures" -ne 0 ]]; then
  exit 1
fi
