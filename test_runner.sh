#!/usr/bin/env bash

set -euo pipefail

mkdir -p build
g++ -std=c++17 -Wall -Wextra -Werror main.cpp ascii_art.cpp -o build/ascii_art
g++ -std=c++17 -Wall -Wextra -Werror -I. tests/test_ascii_art.cpp ascii_art.cpp \
  -o build/test_ascii_art
./build/test_ascii_art
bash tests/test_cli.sh build/ascii_art
