#ifndef ASCII_ART_HPP
#define ASCII_ART_HPP

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

std::string normalizeDescription(std::string_view description);
const std::vector<std::string>* findArt(std::string_view normalizedDescription);

std::vector<std::string> scaleRows(
    const std::vector<std::string>& source, std::size_t targetRows);

#endif
