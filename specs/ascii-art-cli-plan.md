# Specification and Implementation Plan: Offline ASCII Art CLI

**Status:** Implemented and verified with `./test_runner.sh`.

## Objective

Build a dependency-free C++ command-line program that renders one of five
recognized descriptions as local ASCII art at an exact requested height.

**User story:** `./ascii_art "cat" 10` prints ten art rows. `--help` tells the
user which descriptions and heights are supported.

## CLI Contract

```text
ascii_art "<description>" <rows>
ascii_art --help
```

| Result | stdout | stderr | exit |
|---|---|---|---|
| Valid recognized input | Exactly `rows` art lines, each ending in `\n` | Empty | 0 |
| `--help` | Usage, range, and catalog | Empty | 0 |
| Bad/missing arguments or invalid height | Empty | Error and usage | 2 |
| Unsupported description | Empty | `not recognized: <description>` | 3 |

`rows` is ASCII digits only and must be an integer from 1 through 20. Signs,
whitespace, decimal points, trailing characters, zero, negatives, and values
above 20 are invalid. Check the bound before output/allocation.

Trim surrounding ASCII whitespace from descriptions and lowercase ASCII letters
only. Empty descriptions are invalid (exit 2). A non-ASCII or otherwise absent
description is unsupported (exit 3); do not use locale-dependent conversion.

## Catalog and Rendering Contract

The only v1 keys are `cat`, `dog`, `tree`, `house`, and `rocket`. Exact source
art rows for every key are fixed catalog data and must be pinned in tests (or
golden fixtures). No aliases or fuzzy matching are included.

The art consists solely of printable ASCII and spaces, with no empty source
rows or trailing spaces. For source height `sourceRows`, requested height
`rows`, and zero-based output index `i`, emit:

```text
source[floor(i * sourceRows / rows)]  for 0 <= i < rows
```

This repeats/scales source rows when expanding and samples them when shrinking.
It never pads blank rows or changes characters horizontally. Successful output
contains only the selected rows, each followed by one newline.

For five source rows, expected selected indices are:

```text
target 3:  0, 1, 3
target 5:  0, 1, 2, 3, 4
target 10: 0, 0, 1, 1, 2, 2, 3, 3, 4, 4
```

## Tech Stack

- C++17 compiler and standard library only.
- POSIX-compatible shell for `test_runner.sh`.
- No package manager, network service, or third-party dependency.

## Technical Design

- **C++17, standard library only.** No network, APIs, or third-party packages.
- `ascii_art.hpp` declares reusable normalization, lookup, and scaling APIs.
- `ascii_art.cpp` implements the catalog and rendering logic and has no `main`.
- `main.cpp` owns `argv` parsing, streams, help text, and exit statuses.
- `tests/test_ascii_art.cpp` is a separate test executable that links to
  `ascii_art.cpp`; this prevents duplicate `main` definitions.
- `test_runner.sh` starts with `set -euo pipefail`, compiles explicit source
  lists with warnings as errors, and keeps test/app output paths separate.

## Commands

```bash
g++ -std=c++17 -Wall -Wextra -Werror main.cpp ascii_art.cpp -o ascii_art
./ascii_art "cat" 10
./ascii_art --help
./test_runner.sh
```

## Project Structure

```text
ascii_art.hpp                    Testable application API
ascii_art.cpp                    Catalog, normalization, lookup, scaling
main.cpp                         CLI boundary
tests/test_ascii_art.cpp         Module and command-behavior tests
test_runner.sh                   Explicit warning-clean build/test runner
README.md                        Build, use, catalog, and error documentation
specs/ascii-art-cli-plan.md      This specification and plan
```

## Code Style

- Separate CLI orchestration from parsing, lookup, and scaling.
- Use lower camel case for functions and variables; fixed data is `const` or
  `constexpr`.
- Avoid locale-sensitive character conversion; operate on ASCII explicitly.

```cpp
std::vector<std::string> scaleRows(
    const std::vector<std::string>& source, std::size_t targetRows) {
  std::vector<std::string> result;
  for (std::size_t i = 0; i < targetRows; ++i) {
    result.push_back(source[i * source.size() / targetRows]);
  }
  return result;
}
```

## Testing Strategy

- Use standalone C++ assertions with descriptive failures and nonzero exit.
- Build tests and the program with `-std=c++17 -Wall -Wextra -Werror`.
- Pin exact art rows for all five subjects, plus all lookup results.
- Test ASCII trim/case handling, whitespace-only input, non-ASCII input, and
  unrecognized input.
- Test requested heights 1, native height, and 20, including the three index
  examples above.
- Invoke the executable in tests to capture exit code, stdout, and stderr.
  Assert exact newline count, empty success stderr, and the CLI-contract table.
- Reject `+1`, `1x`, `1.0`, ` 1`, `0`, `21`, and a very long digit sequence.

## Boundaries

- **Always:** validate inputs; keep the 1–20 limit; honor output stream and
  exit-code rules; run `./test_runner.sh` before handoff.
- **Ask first:** change syntax, subjects/art, matching, scaling, range, or add
  dependencies.
- **Never:** call a service; accept an image; fuzzy-match unknown input; put
  successful diagnostics on stdout; or weaken tests to pass.

## Success Criteria

- Each of the five subjects renders at every integer height from 1 to 20.
- Valid output contains exactly the requested newline-terminated rows and no
  stderr output.
- Scaling follows the stated formula; no blank or horizontal padding occurs.
- Help names all subjects and the permitted range.
- Errors obey the output/exit table; the complete runner passes warning-cleanly
  without downloads.

## Scope Boundaries

Included: finite local catalog, exact normalized matching, terminal output,
deterministic vertical scaling, help, validation, and tests.

Excluded: arbitrary prompt generation, fuzzy matching, custom art, image input,
web UI, network access, and horizontal scaling.

---

## Implementation Plan

### Dependency Flow

```text
reusable catalog + scaling module
          |                 |
          v                 v
     unit/golden tests    CLI contract
          |                 |
          +-------> command-level tests
                            |
                            v
                         README
```

### Task 1: Establish the testable module and build runner

**Description:** Create a reusable application module and replace wildcard
compilation with explicit, separate program/test builds.

**Acceptance criteria:**

- [ ] `ascii_art.hpp`/`.cpp` contain no entry point; `main.cpp` is the sole CLI
      entry point.
- [ ] `test_runner.sh` uses `set -euo pipefail`, explicit source lists,
      warnings-as-errors, and distinct program/test outputs.
- [ ] The standalone test executable builds and runs independently.

**Verification:** Run `./test_runner.sh` and the explicit application build.

**Dependencies:** None.

**Files likely touched:** `ascii_art.hpp`, `ascii_art.cpp`, `main.cpp`,
`tests/test_ascii_art.cpp`, `test_runner.sh`, `.gitignore`.

### Task 2: Add the fixed catalog and normalized lookup

**Description:** Implement the five pinned art designs and safe ASCII-only
normalization/lookup results.

**Acceptance criteria:**

- [ ] Exactly `cat`, `dog`, `tree`, `house`, and `rocket` resolve.
- [ ] Tests pin every subject’s source-art rows.
- [ ] Case/whitespace, empty, non-ASCII, and unknown descriptions follow the
      specification.

**Verification:** Run module tests for each lookup and normalization outcome.

**Dependencies:** Task 1.

**Files likely touched:** `ascii_art.hpp`, `ascii_art.cpp`,
`tests/test_ascii_art.cpp`.

### Task 3: Add exact-height scaling and CLI behavior

**Description:** Implement the index formula and expose it with strict parsing,
help output, and required streams/statuses.

**Acceptance criteria:**

- [ ] Every height 1–20 produces the exact scaled-row sequence and no blank
      padding.
- [ ] Strict decimal parsing rejects all documented malformed values.
- [ ] `--help`, success, invalid invocation, and unknown descriptions obey the
      CLI table exactly.

**Verification:** Add scaling tests and command-level captured-output tests;
run `./test_runner.sh`.

**Dependencies:** Tasks 1–2.

**Files likely touched:** `ascii_art.cpp`, `main.cpp`,
`tests/test_ascii_art.cpp`, `test_runner.sh`.

### Checkpoint: Behavioral Contract Complete

- [ ] Tests pin catalog art, lookup behavior, errors, and exact output bytes.
- [ ] All five subjects succeed for every height 1–20.
- [ ] Application and tests compile warning-cleanly.

### Task 4: Document the command-line experience

**Description:** Replace starter instructions with build, usage, catalog,
scaling, and error documentation.

**Acceptance criteria:**

- [ ] README includes copyable build, usage, help, and test commands.
- [ ] README lists the five subjects, `1..20` limit, and exact matching rule.
- [ ] README makes the offline/dependency-free boundary and error behavior
      clear.

**Verification:** Follow README commands from a clean build-artifact directory.

**Dependencies:** Tasks 1–3.

**Files likely touched:** `README.md`.

### Checkpoint: Ready for Implementation Review

- [ ] `./test_runner.sh` passes warning-cleanly.
- [ ] Each subject is spot-checked at heights 1, native, and 20.
- [ ] Help/error output matches documentation.

## Risks and Mitigations

| Risk | Impact | Mitigation |
|---|---|---|
| Users expect arbitrary prompts | High | Help lists the finite catalog; unknown input exits 3. |
| Duplicate test/app entry points | High | Reusable module and explicit compilation prevent it. |
| Scaling behavior regresses | Medium | Pin art and scaled rows in deterministic tests. |
| Malformed/huge row count uses resources | Medium | Strict parsing and maximum height 20. |
| Locale changes matching | Low | ASCII-only normalization with non-ASCII tests. |

## Open Questions

None. The initial scope, command contract, catalog, scaling rule, height
limit, error behavior, and test architecture have all been approved.
