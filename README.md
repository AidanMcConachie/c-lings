# c-lings

Small exercises to get you used to reading and writing C code, in the
spirit of [rustlings](https://github.com/rust-lang/rustlings). This includes
reading and responding to compiler warnings and errors!

c-lings itself is written in C, with no dependencies beyond a C compiler and
a POSIX system (Linux, macOS, WSL).

## Getting started

```sh
make                 # builds ./clings
make install         # optional: installs to ~/.local/bin (set PREFIX to change)

clings init          # creates a fresh ./c-lings directory with all exercises
cd c-lings
clings               # starts watch mode
```

You can also just run `./clings` from this repository's root, but then the
official solutions are already sitting in `solutions/`. No peeking!

Open the exercise shown under the progress bar in your editor, fix it and
save. clings recompiles and re-checks it every time the file changes. Once it
passes, press `n` to move on.

### Watch mode keys

| Key | Action                                                       |
| --- | ------------------------------------------------------------ |
| `n` | Next exercise (once the current one passes)                  |
| `h` | Show / hide the hint                                         |
| `l` | Interactive list: browse, jump to (`c`) or reset (`x`) any exercise |
| `c` | Re-check all exercises                                       |
| `x` | Reset the current exercise to its original state            |
| `r` | Re-run the current exercise                                  |
| `q` | Quit                                                         |

### Other commands

```
clings run [NAME]    compile and run an exercise (the current one by default)
clings hint [NAME]   show a hint
clings list          list all exercises and your progress
clings reset NAME    restore an exercise
clings check         check every exercise
clings help          all commands and environment variables
```

## How exercises are checked

Each exercise is compiled with `cc -std=c17 -Wall -Wextra -Werror -g` (set
`$CC` to use another compiler). An exercise passes when it compiles and the
program exits with status 0. Programs that crash, or run for over 10
seconds, fail.

Most exercises include [`exercises/clings.h`](exercises/clings.h), a tiny test
harness that supplies `main()`, runs every `TEST(...)` and provides `ASSERT_*`
macros. It also tracks `malloc`/`free` so tests catch memory leaks and heap
buffer overflows, even without sanitizers. When the toolchain supports them,
clings also builds with `-fsanitize=address,undefined` (override with
`CLINGS_SANITIZE=0/1`).

## Topics

| Section              | Exercises |
| -------------------- | --------- |
| Intro                | 2 |
| Variables            | 4 |
| Functions            | 5 |
| If / switch          | 3 |
| Types                | 4 |
| Loops                | 3 |
| Arrays               | 4 |
| Strings              | 4 |
| Pointers             | 5 |
| Structs              | 4 |
| Enums & unions       | 3 |
| Memory (heap)        | 5 |
| Preprocessor         | 3 |
| Function pointers    | 3 |
| Bitwise operations   | 3 |
| Error handling       | 3 |
| Files                | 2 |
| Linked lists         | 3 |
| Quizzes              | 4 |

## Project layout

```
src/             the clings runner (C)
  main.c         command line
  watch.c        watch mode
  list.c         interactive exercise list
  runner.c       compiling and running exercises (with timeouts)
  info.c         parser for info.toml
  state.c        progress tracking (.clings-state.txt)
  term.c         terminal handling
  embed.c        build tool: bundles exercises/solutions into the binary
info.toml        exercise order, metadata and hints
exercises/       the exercises, one directory per topic
solutions/       official solutions
```

The exercises and solutions are embedded into the `clings` binary at build
time, which is what makes `clings reset` and `clings init` work anywhere.

## Adding an exercise

1. Write `exercises/<dir>/<name>.c` and `solutions/<dir>/<name>.c`.
2. Add an `[[exercises]]` entry to `info.toml` with `name`, `dir`, `hint`
   and, if the exercise has its own `main()` instead of tests, `test = false`.
3. Run `make dev-check`. It checks that every exercise fails as shipped and
   every solution passes.
