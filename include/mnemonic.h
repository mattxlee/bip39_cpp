#ifndef MNEMONIC_HPP
#define MNEMONIC_HPP

#include <cstdint>

#include <vector>

#include <string>
#include <string_view>

namespace bip39 {

using WordList = std::vector<std::string>;

class Mnemonic {
public:
    static bool IsValidNumMnemonicSentences(int n);

    static int GetEntBitsByNumMnemonicSentences(int n);

    Mnemonic(std::vector<uint8_t> entropy, std::string lang);

    Mnemonic(WordList word_list, std::string lang);

    WordList const& GetWordList() const;

    std::vector<uint8_t> const& GetEntropyData() const;

    std::vector<uint8_t> CreateSeed(std::string_view passphrase) const;

private:
    std::vector<uint8_t> entropy_;
    std::string lang_;
    int num_bits_;
    WordList word_list_;
};

} // namespace bip39

#endif
