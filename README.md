# A tool-independent file-system test suite for symbolic execution engines

133 tests covering eight POSIX system calls, written against a
tool-independent file-system API rather than against any one engine's
internals.

| System call | Tests | |
|---|---|---|
| `open` | 57 | |
| `close` | 10 | |
| `read` | 12 | |
| `write` | 13 | |
| `lseek` | 15 | |
| `chmod` | 13 | |
| `dup` / `dup2` | 13 | |
| **Total** | **133** | 59 pass, 74 fail |

Run natively on Linux every test passes, so each failure on an engine is
that engine's defect. The defects found by the earlier version of the suite
are in `issues/klee_posix_findings.xlsx`, and the spreadsheet in each
`individual-tests/` folder explains its tests.

## Requires the KLEE fork

> **This suite does not run on stock KLEE.**

The tests are written against a shared file-system API, the `__` functions
(`__file_create`, `__file_exists`, `__assume`, `__sra_assert`, `__file_offset`,
`__is_sat`, `__is_certain` and others) and the `_EQ_`-style constraints, so
the same tests run on other engines too. Each engine provides the API as
`sra.h`. Stock KLEE does not provide it; the fork does:

* **[github.com/dino-fan777/klee](https://github.com/dino-fan777/klee)**, branch **`shared-testsuite`**

## Quickest way to run them

The image is KLEE built with the fork; the suite is mounted into it, so
nothing else needs to be installed:

```bash
git clone https://github.com/dino-fan777/klee-tests.git
cd klee-tests
docker build -t klee-tests \
  --build-context fork=https://github.com/dino-fan777/klee.git#shared-testsuite .
docker run -it --rm -v "$PWD":/home/klee/klee-tests klee-tests
```

It opens in the suite and prints the usage notes on entry. Then:

```bash
make run_all            # all 133 tests; expect 59 passed / 74 failed
make run_open           # one system call
make run_open_12        # one test
```

`workflow.txt`, also printed when the container starts, covers how pass and
fail are decided, how to read a failure, and how to rebuild the engine after
editing the fork.

### Results as JSON

Every `run_*` target has a `json_*` counterpart that runs the same tests,
then prints the results as JSON on stdout while the usual output goes to
stderr. `scripts/json-results.sh` writes the JSON with `jq`, which the image
installs:

```bash
make json_all > results.json
```

From outside the container, leave out `-t`, which would merge the two
streams into the file:

```bash
docker run --rm -v "$PWD":/home/klee/klee-tests klee-tests make json_all > results.json
```

Each test gives its `result`, KLEE's `completed_paths`,
`partially_completed_paths` and `generated_tests`, and its `output_dir`.
Totals are given per suite and overall.

### Which fork is built

The image is built from whatever the `fork` build context points at: a
branch, tag or commit of the fork on GitHub, after the `#`, or a local
checkout:

```bash
docker build -t klee-tests --build-context fork=https://github.com/dino-fan777/klee.git#<branch, tag or commit> .
docker build -t klee-tests --build-context fork=../klee .
```

Naming a commit makes the build reproducible, which is worth doing when
reporting results. After editing a local checkout, build again: Docker's
cache keeps it to recompiling what changed.

## Running without Docker

You need the fork built and its `klee` binary on your `PATH`, plus `clang`
matching the LLVM that KLEE was built against. Then:

```bash
make run_all
```

Each suite compiles with `-I../../include -I../../include/klee`, so
`include/test_helper.h` and KLEE's `include/klee/sra.h` are found
automatically from `individual-tests/<syscall>/`.

## Layout

```
include/            test_helper.h, the assertions and symbolic-input helpers
include/klee/       sra.h, the shared API for KLEE, from the fork's klee/file_api.h
individual-tests/   one folder per system call, each with its results spreadsheet
issues/             klee_posix_findings.xlsx, the defects the suite found
scripts/            verdict.sh, which judges a test from its log, and
                    json-results.sh, which turns a run's logs into JSON
Dockerfile          builds the fork and this suite into a ready-to-run image
workflow.txt        how to run the tests, and how to read the results
```

## How a test is judged

`__sra_assert(c)` is a symbolic assertion: it holds only if the path
condition implies `c`, so it must hold on every path. A test **fails** if:

- any path fails an `__sra_assert` (KLEE's `ASSERTION FAIL`), or hits any
  other KLEE error, such as a memory error; or
- no path completes: the test's `__assume()` preconditions never hold.

A path ended by an `__assume` that cannot hold there (`invalid klee_assume
call (provably false)`) is not a failure: the `__assume` excludes that path's
inputs, as it is meant to. `scripts/verdict.sh` applies this rule, for the
Makefiles and `make json_*` alike.

This is the rule summbv applies too. On KLEE, a file created by
`__file_create` has a symbolic `stat`, so its permissions are symbolic: on
paths where they refuse the operation a test performs, its `__sra_assert`
fails.

## What a test looks like

```c
#include "test_helper.h"

int main(void) {
   char fname[FNAME_SIZE];
   int  flags;

   create_symbolic_file_name(fname);        // a symbolic name
   flags = O_RDONLY;

   create_test_file(fname);                 // the test creates its own file, under that name
   __assume(exists(fname));

   int fd = open(fname, flags);

   __sra_assert(open_succeeds(fd));
   __sra_assert(fd_is(fd, 3));

   __sra_assert(close_succeeds(close(fd)));
   return 0;
}
```
