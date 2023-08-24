#ifndef MNEMONIC_HPP
#define MNEMONIC_HPP

#include <cstdint>

#include <vector>

#include <string>
#include <string_view>

#include "random.hpp"
#include "words.hpp"
#include "sha256.hpp"
#include "bit_opts.hpp"

namespace bip39 {

class Mnemonic {
public:
    static Mnemonic GenerateMnemonicByBits(int num_bits, WordListLoader& loader)
    {
        if (num_bits % 8 != 0) {
            throw std::runtime_error("invalid number of bits");
        }
        RandomBytes rnd(num_bits / 8);
        return Mnemonic(rnd.Random(), loader);
    }

    static Mnemonic GenerateMnemonicByNumWords(int num_words, WordListLoader& loader)
    {
        if (num_words % 3 != 0) {
            throw std::runtime_error("invalid number of words");
        }
        return GenerateMnemonicByBits(num_words * 32 / 3, loader);
    }

    explicit Mnemonic(std::vector<uint8_t> entropy, WordListLoader& loader)
        : entropy_(std::move(entropy))
        , num_bits_(entropy.size() * 8)
        , word_list_loader_(loader)
    {
        SHA256 sha;
        sha.Update(entropy_.data(), entropy_.size());
        auto const& sha_res = sha.GetResult();
        entropy_.push_back(sha_res[0]);
    }

    WordList GetWordList(std::string const& lang) const
    {
        WordList res;
        auto word_list = word_list_loader_.Load(lang);

        int cs = num_bits_ / 32;
        int total_bits = num_bits_ + cs;
        assert(total_bits % 11 == 0);
        int num_words = total_bits / 11;
        Bits<uint64_t> bits(entropy_.data(), total_bits);
        for (int i = 0; i < num_words; ++i) {
            int word_index = bits.FrontBitsToInt(11);
            res[i] = word_list[word_index];
        }
        return res;
    }

private:
    std::vector<uint8_t> entropy_;
    int num_bits_;
    WordListLoader& word_list_loader_;
};

} // namespace bip39

#endif
