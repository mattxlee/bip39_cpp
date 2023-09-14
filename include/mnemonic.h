#ifndef MNEMONIC_HPP
#define MNEMONIC_HPP

#include <cstdint>

#include <vector>

#include <string>
#include <string_view>

#include <iostream>

#include "langs.h"
#include "toolbox.h"

#include "sha256.h"
#include "bit_opts.h"

namespace bip39 {

class Mnemonic {
public:
    Mnemonic(std::vector<uint8_t> entropy, std::string lang);

    Mnemonic(WordList const& word_list, std::string lang);

    WordList const& GetWordList() const;

    std::vector<uint8_t> const& GetEntropyData() const;

    std::vector<uint8_t> CreateSeed(std::string_view passphrase);

private:
    std::vector<uint8_t> entropy_;
    std::string lang_;
    int num_bits_;
    WordList word_list_;
};

} // namespace bip39

#endif
