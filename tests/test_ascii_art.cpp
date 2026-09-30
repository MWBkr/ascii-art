#include "ascii_art.hpp"

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

namespace {

void expectEqual(const std::vector<std::string>& actual,
                 const std::vector<std::string>& expected,
                 const std::string& message) {
  if (actual != expected) {
    std::cerr << "FAILED: " << message << '\n';
    std::exit(1);
  }
}

void expectTrue(bool condition, const std::string& message) {
  if (!condition) {
    std::cerr << "FAILED: " << message << '\n';
    std::exit(1);
  }
}

}  // namespace

int main() {
  const std::vector<std::string> source = {"0", "1", "2", "3", "4"};
  const std::vector<std::string> expected = {
      "0", "0", "1", "1", "2", "2", "3", "3", "4", "4"};

  expectEqual(scaleRows(source, 10), expected,
              "scaling five rows to ten repeats each source row");

  expectTrue(normalizeDescription(" \tCaT\n") == "cat",
             "normalization trims ASCII whitespace and lowercases ASCII");
  expectTrue(normalizeDescription("   ").empty(),
             "normalization preserves an empty description after trimming");

  const std::vector<std::string>* cat = findArt("cat");
  expectTrue(cat != nullptr, "the cat catalog entry exists");
  expectEqual(*cat, {" /\\_/\\", "( o.o )", " > ^ <"},
              "the cat source art is pinned");

  const std::vector<std::string>* dog = findArt("dog");
  expectTrue(dog != nullptr, "the dog catalog entry exists");
  expectEqual(*dog, {R"( / \__)", R"((    @\___)", R"( /         O)",
                    R"(/   (_____/)", R"(/_____/   U)"},
              "the dog source art is pinned");

  const std::vector<std::string>* tree = findArt("tree");
  expectTrue(tree != nullptr, "the tree catalog entry exists");
  expectEqual(*tree, {R"(  *)", R"( ***)", R"(*****)", R"(  |)", R"( / \)"},
              "the tree source art is pinned");

  const std::vector<std::string>* house = findArt("house");
  expectTrue(house != nullptr, "the house catalog entry exists");
  expectEqual(*house, {R"(  /\)", R"( /  \)", R"(/____\)", R"(| [] |)",
                       R"(|____|)"},
              "the house source art is pinned");

  const std::vector<std::string>* rocket = findArt("rocket");
  expectTrue(rocket != nullptr, "the rocket catalog entry exists");
  expectEqual(*rocket, {R"( /\)", R"( ||)", R"(/==\)", R"(|  |)", R"(/\ /\)"},
              "the rocket source art is pinned");

  expectTrue(findArt("unicorn") == nullptr,
             "an unknown catalog entry is absent");
  return 0;
}
