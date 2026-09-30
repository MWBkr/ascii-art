#include "ascii_art.hpp"

#include <array>

namespace {

bool isAsciiWhitespace(unsigned char character) {
  return character == ' ' || character == '\t' || character == '\n' ||
         character == '\r' || character == '\f' || character == '\v';
}

char toLowerAscii(unsigned char character) {
  if (character >= 'A' && character <= 'Z') {
    return static_cast<char>(character - 'A' + 'a');
  }
  return static_cast<char>(character);
}

using ArtEntry = std::pair<std::string_view, std::vector<std::string>>;

const std::array<ArtEntry, 5> kCatalog = {
    ArtEntry{"cat", {R"( /\_/\)", R"(( o.o ))", R"( > ^ <)"}},
    ArtEntry{"dog", {R"( / \__)", R"((    @\___)", R"( /         O)",
                     R"(/   (_____/)", R"(/_____/   U)"}},
    ArtEntry{"tree", {R"(  *)", R"( ***)", R"(*****)", R"(  |)", R"( / \)"}},
    ArtEntry{"house", {R"(  /\)", R"( /  \)", R"(/____\)", R"(| [] |)",
                       R"(|____|)"}},
    ArtEntry{"rocket", {R"( /\)", R"( ||)", R"(/==\)", R"(|  |)", R"(/\ /\)"}},
};

}  // namespace

std::string normalizeDescription(std::string_view description) {
  std::size_t first = 0;
  while (first < description.size() &&
         isAsciiWhitespace(static_cast<unsigned char>(description[first]))) {
    ++first;
  }

  std::size_t last = description.size();
  while (last > first &&
         isAsciiWhitespace(static_cast<unsigned char>(description[last - 1]))) {
    --last;
  }

  std::string normalized;
  normalized.reserve(last - first);
  for (std::size_t index = first; index < last; ++index) {
    normalized.push_back(
        toLowerAscii(static_cast<unsigned char>(description[index])));
  }
  return normalized;
}

const std::vector<std::string>* findArt(std::string_view normalizedDescription) {
  for (const ArtEntry& entry : kCatalog) {
    if (entry.first == normalizedDescription) {
      return &entry.second;
    }
  }
  return nullptr;
}

std::vector<std::string> scaleRows(
    const std::vector<std::string>& source, std::size_t targetRows) {
  std::vector<std::string> result;
  if (source.empty() || targetRows == 0) {
    return result;
  }

  result.reserve(targetRows);
  for (std::size_t index = 0; index < targetRows; ++index) {
    result.push_back(source[index * source.size() / targetRows]);
  }
  return result;
}
