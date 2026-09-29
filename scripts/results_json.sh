#!/bin/sh
# Prints the results of the last run as JSON, read back from the KLEE logs
# the run left next to each test. Used by "make json_*", which runs the
# tests first; see the top-level Makefile.
#
# Each argument is a suite ("open") or a single test in it ("open_12").
#
# A test fails when KLEE reports "completed paths = 0", the same rule the
# Makefiles apply, so the JSON always agrees with the usual output.

cd "$(dirname "$0")/.." || exit 1

nl='
'

# The number KLEE printed on its "KLEE: done: <what> = N" line, or null.
# Anchored at the start of the line, because "partially completed paths"
# also contains "completed paths".
done_count() {
    n=$(sed -n "s/^KLEE: done: $1 = \([0-9][0-9]*\).*/\1/p" "$2" | tail -n 1)
    echo "${n:-null}"
}

total=0 passed=0 failed=0 suites_json=''

for arg in "$@"; do
    suite=${arg%%_*}
    dir=individual-tests/$suite
    # A whole suite is every test_*.c in it, which is what its Makefile's
    # TESTS lists. Keep the two in step, or a test drops out of the JSON.
    case $arg in
        *_*) tests=test_${arg#*_} ;;
        *)   tests=$(for c in "$dir"/test_*.c; do basename "$c" .c; done) ;;
    esac

    s_total=0 s_passed=0 s_failed=0 tests_json=''
    for t in $tests; do
        outdir=$dir/klee-out-$t
        if [ ! -f "$outdir.log" ]; then
            echo "results_json.sh: no log for $suite/$t, left out" >&2
            continue
        fi
        completed=$(done_count 'completed paths' "$outdir.log")
        partial=$(done_count 'partially completed paths' "$outdir.log")
        generated=$(done_count 'generated tests' "$outdir.log")
        if [ "$completed" = 0 ]; then
            result=fail; s_failed=$((s_failed + 1))
        else
            result=pass; s_passed=$((s_passed + 1))
        fi
        s_total=$((s_total + 1))
        tests_json="$tests_json${tests_json:+,$nl}        {\"suite\": \"$suite\", \"test\": \"$t\", \"result\": \"$result\", \"completed_paths\": $completed, \"partially_completed_paths\": $partial, \"generated_tests\": $generated, \"output_dir\": \"$outdir\"}"
    done

    total=$((total + s_total))
    passed=$((passed + s_passed))
    failed=$((failed + s_failed))
    suites_json="$suites_json${suites_json:+,$nl}    {
      \"suite\": \"$suite\",
      \"total\": $s_total,
      \"passed\": $s_passed,
      \"failed\": $s_failed,
      \"tests\": [
$tests_json
      ]
    }"
done

printf '%s\n' "{
  \"total\": $total,
  \"passed\": $passed,
  \"failed\": $failed,
  \"suites\": [
$suites_json
  ]
}"
