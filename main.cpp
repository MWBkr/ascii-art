#include "ascii_art.hpp"

#include <cstddef>
#include <iostream>
#include <string_view>
#include <vector>

namespace {

constexpr std::size_t kMinimumRows = 1;
constexpr std::size_t kMaximumRows = 20;

void printUsage(std::ostream& stream) {
  stream << "Usage: ascii_art \"<description>\" <rows>\n"
         << "       ascii_art --help\n"
         << "Rows must be an integer from 1 through 20.\n"
         << "Supported descriptions: cat, dog, tree, house, rocket\n";
}

bool parseRowCount(std::string_view input, std::size_t& rowCount) {
  if (input.empty()) {
    return false;
  }

  std::size_t value = 0;
  for (char character : input) {
    if (character < '0' || character > '9') {
      return false;
    }

    const std::size_t digit = static_cast<std::size_t>(character - '0');
    if (value > (kMaximumRows - digit) / 10) {
      return false;
    }
    value = value * 10 + digit;
  }

  if (value < kMinimumRows || value > kMaximumRows) {
    return false;
  }

  rowCount = value;
  return true;
}

}  // namespace

int main(int argc, char* argv[]) {
  if (argc == 2 && std::string_view(argv[1]) == "--help") {
    printUsage(std::cout);
    return 0;
  }

  if (argc != 3) {
    std::cerr << "invalid arguments\n";
    printUsage(std::cerr);
    return 2;
  }

  std::size_t rowCount = 0;
  if (!parseRowCount(argv[2], rowCount)) {
    std::cerr << "rows must be an integer from 1 through 20\n";
    printUsage(std::cerr);
    return 2;
  }

  const std::string description = normalizeDescription(argv[1]);
  if (description.empty()) {
    std::cerr << "description must not be empty\n";
    printUsage(std::cerr);
    return 2;
  }

  const std::vector<std::string>* art = findArt(description);
  if (art == nullptr) {
    std::cerr << "not recognized: " << description << '\n';
    return 3;
  }

  for (const std::string& row : scaleRows(*art, rowCount)) {
    std::cout << row << '\n';
  }
  return 0;
}
