#ifndef TOOLBOX_HPP
#define TOOLBOX_HPP

#include <cstdint>

#include <stdexcept>

#include <string>
#include <string_view>

#include <vector>

namespace bip39 {

uint8_t CharToInt(char ch);

std::vector<uint8_t> ParseHex(std::string_view hex);

std::vector<std::string> ParseWords(std::string_view words, std::string_view delimiter = u8"\u0020");

std::string GenerateWords(std::vector<std::string> const& word_list, std::string_view delimiter = u8"\u0020");

std::string GetDelimiterByLang(std::string_view lang);

} // namespace bip39

#endif
