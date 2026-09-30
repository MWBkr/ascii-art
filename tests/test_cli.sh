#!/usr/bin/env bash

set -euo pipefail

app_path=$1
work_dir=$(mktemp -d)
trap 'rm -rf "$work_dir"' EXIT

expect_status() {
  local expected_status=$1
  shift

  set +e
  "$app_path" "$@" >"$work_dir/stdout" 2>"$work_dir/stderr"
  local actual_status=$?
  set -e

  if [[ $actual_status -ne $expected_status ]]; then
    echo "FAILED: expected exit $expected_status for: $*; got $actual_status" >&2
    exit 1
  fi
}

expect_status 0 cat 5
printf ' /\\_/\\\n /\\_/\\\n( o.o )\n( o.o )\n > ^ <\n' >"$work_dir/expected"
diff -u "$work_dir/expected" "$work_dir/stdout"
test ! -s "$work_dir/stderr"

expect_status 0 ' CAT ' 5
diff -u "$work_dir/expected" "$work_dir/stdout"
test ! -s "$work_dir/stderr"

expect_status 0 --help
grep -Fq 'cat, dog, tree, house, rocket' "$work_dir/stdout"
grep -Fq '1 through 20' "$work_dir/stdout"
test ! -s "$work_dir/stderr"

expect_status 2 cat 21
test ! -s "$work_dir/stdout"
grep -Fq 'Usage:' "$work_dir/stderr"

expect_status 3 unicorn 3
test ! -s "$work_dir/stdout"
grep -Fq 'not recognized: unicorn' "$work_dir/stderr"

for subject in cat dog tree house rocket; do
  for rows in {1..20}; do
    expect_status 0 "$subject" "$rows"
    test "$(wc -l <"$work_dir/stdout")" -eq "$rows"
    test ! -s "$work_dir/stderr"
  done
done

for invalid_rows in '+1' '1x' '1.0' ' 1' '0' '21' \
  '999999999999999999999999999999999999999999999999'; do
  expect_status 2 cat "$invalid_rows"
  test ! -s "$work_dir/stdout"
  grep -Fq 'Usage:' "$work_dir/stderr"
done
