#!/bin/sh
# Prints the results of the last run as JSON, read from the KLEE log the run
# left next to each test, with totals per suite and overall. Used by
# "make json_*", which runs the tests first. Needs jq.
#
# Each argument is a suite ("open") or one of its tests ("open_12"). A test
# passes or fails by scripts/verdict.sh, as in the Makefiles.

cd "$(dirname "$0")/.." || exit 1

# The N on KLEE's "KLEE: done: <what> = N" line, or null. Anchored at the
# start of the line, because "partially completed paths" also contains
# "completed paths".
done_count() {
    sed -n "s/^KLEE: done: $1 = \([0-9]*\).*/\1/p" "$2" | tail -n 1 | grep . || echo null
}

# The tests an argument names: one test, or every test_*.c in a suite (what
# its Makefile's TESTS lists)
tests_of() {
    case $1 in
        *_*) echo "test_${1#*_}" ;;
        *)   for c in "individual-tests/$1"/test_*.c; do basename "$c" .c; done ;;
    esac
}

# One JSON object per test
test_results() {
    for arg in "$@"; do
        suite=${arg%%_*}
        for t in $(tests_of "$arg"); do
            log=individual-tests/$suite/klee-out-$t.log
            if [ ! -f "$log" ]; then
                echo "json-results.sh: no log for $suite/$t, left out" >&2
                continue
            fi

            if sh scripts/verdict.sh "$log" >/dev/null; then
                result=pass
            else
                result=fail
            fi

            jq -n \
                --arg suite "$suite" \
                --arg test "$t" \
                --arg result "$result" \
                --argjson completed "$(done_count 'completed paths' "$log")" \
                --argjson partial "$(done_count 'partially completed paths' "$log")" \
                --argjson generated "$(done_count 'generated tests' "$log")" \
                --arg output_dir "${log%.log}" \
                '{suite: $suite, test: $test, result: $result,
                  completed_paths: $completed,
                  partially_completed_paths: $partial,
                  generated_tests: $generated,
                  output_dir: $output_dir}'
        done
    done
}

# The suites the arguments name, in order, each once
suites=$(for arg in "$@"; do echo "${arg%%_*}"; done | awk '!seen[$0]++')

# The tests grouped by suite, with the counts of each suite and of all of
# them. A suite without logs is still listed, with no tests.
test_results "$@" | jq -s --args '
    def counts: {
        total:  length,
        passed: map(select(.result == "pass")) | length,
        failed: map(select(.result == "fail")) | length
    };

    . as $tests
    | counts + {
        suites: [
            $ARGS.positional[] as $s
            | $tests | map(select(.suite == $s))
            | {suite: $s} + counts + {tests: .}
        ]
    }' $suites
