#!/bin/sh
# Judges a test from its KLEE log: exits 0 if it passed, or prints why and
# exits 1.
#
#   sh scripts/verdict.sh individual-tests/open/klee-out-test_01.log
#
# A test fails if any path hits a KLEE error (a failed __sra_assert, a memory
# error, ...), or if no path completes. A provably false __assume is not an
# error: it just excludes that path.

log=$1

if [ ! -f "$log" ]; then
    echo "no log: $log"
    exit 1
fi

errors=$(grep '^KLEE: ERROR: ' "$log" | grep -v 'invalid klee_assume call (provably false)')
if [ -n "$errors" ]; then
    echo "$errors" | head -n 1 | sed 's/^KLEE: ERROR: [^ ]*: //'
    exit 1
fi

if grep -q '^KLEE: done: completed paths = 0' "$log"; then
    echo "no path completes: the __assume preconditions never hold"
    exit 1
fi

exit 0
