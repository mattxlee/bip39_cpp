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

    explicit Mnemonic(std::vector<uint8_t> entropy);

    Mnemonic(WordList const& word_list, std::string const& lang);

    WordList GetWordList(std::string const& lang) const;

    std::vector<uint8_t> const& GetEntropyData() const;

    std::vector<uint8_t> CreateSeed(std::string_view passphrase, std::string_view lang = "english") const;

private:
    std::vector<uint8_t> entropy_;
};

} // namespace bip39

#endif
