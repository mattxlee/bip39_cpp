#ifndef BASE_DEFS_H
#define BASE_DEFS_H

#include <string>

namespace bip39 {

std::string const LANGUAGES[] = {"chinese_simplified", "chinese_traditional", "czech", "english", "french", "italian", "japanese", "korean", "portuguese", "spanish"};

// the number of bits of check-sum
int const CS_MIN = 4;
int const CS_MAX = 8;

} // namespace bip39

#endif
