# Offline ASCII Art CLI

A small C++17 command-line program that renders one of five built-in ASCII
designs at a requested height. It has no external dependencies and never uses
the network.

## Build

\`\`\`bash
g++ -std=c++17 -Wall -Wextra -Werror main.cpp ascii_art.cpp -o ascii_art
\`\`\`

## Use

\`\`\`bash
./ascii_art "<description>" <rows>
./ascii_art --help
\`\`\`

Descriptions are matched case-insensitively after surrounding ASCII whitespace
is removed. The supported descriptions are:

- `cat`
- `dog`
- `tree`
- `house`
- `rocket`

Rows must be an ASCII decimal integer from 1 through 20. The tool vertically
scales art by repeating or sampling its source rows, so successful output
always contains exactly the requested number of lines. It does not add blank
padding or scale horizontally.

Example:

\`\`\`bash
./ascii_art "cat" 5
\`\`\`

Output:

\`\`\`text
 /\\_/\\
 /\\_/\\
( o.o )
( o.o )
 > ^ <
\`\`\`

## Errors

- `--help` prints usage and exits with status 0.
- Invalid or missing arguments, including a height outside 1–20, print an
  error and usage to standard error, then exit with status 2.
- Unsupported descriptions print `not recognized: <description>` to standard
  error and exit with status 3.

## Test

\`\`\`bash
./test_runner.sh
\`\`\`

The test runner builds the application and tests with warnings treated as
errors, then checks catalog data, scaling, input validation, help, output
streams, and exit codes.

## Design

The detailed specification and implementation plan are in
[specs/ascii-art-cli-plan.md](specs/ascii-art-cli-plan.md).
