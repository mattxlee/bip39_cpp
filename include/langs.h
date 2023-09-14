#ifndef LANG_CONSTS_HPP
#define LANG_CONSTS_HPP

#include <string>

#include <array>
#include <vector>
#include <map>

namespace bip39 {

std::string const LANGUAGES[] = {"chinese_simplified", "chinese_traditional", "czech", "english", "french", "italian", "japanese", "korean", "portuguese", "spanish"};

// the number of bits of check-sum
int const CS_MIN = 4;
int const CS_MAX = 8;

extern std::array<std::string, 2048> chinese_simplified_words;
extern std::array<std::string, 2048> chinese_traditional_words;
extern std::array<std::string, 2048> czech_words;
extern std::array<std::string, 2048> english_words;
extern std::array<std::string, 2048> french_words;
extern std::array<std::string, 2048> italian_words;
extern std::array<std::string, 2048> japanese_words;
extern std::array<std::string, 2048> korean_words;
extern std::array<std::string, 2048> portuguese_words;
extern std::array<std::string, 2048> spanish_words;

extern std::map<std::string, std::array<std::string, 2048>> langs;

namespace utils {

bool LangExists(std::string_view lang);

std::vector<std::string> GetLangList();

int GetLangIndex(std::string_view lang, std::string_view word);

std::string GetLangWord(std::string_view lang, int index);

} // namespace utils

} // namespace bip39

#endif
