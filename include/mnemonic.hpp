#ifndef MNEMONIC_HPP
#define MNEMONIC_HPP

#include <cstdint>

#include <vector>

#include <string>
#include <string_view>

#include <iostream>

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
        , num_bits_(entropy_.size() * 8)
        , word_list_loader_(loader)
    {
    }

    WordList const& GetWordList(std::string const& lang) const
    {
        if (!word_list_.empty()) {
            return word_list_;
        }
        auto data = entropy_;
        SHA256 sha;
        sha.Update(data.data(), data.size());
        auto const& sha_res = sha.GetResult();
        int num_check_sum = num_bits_ / 32;
        uint8_t check_sum = sha_res[0] & (Bits::MakeMask(num_check_sum));
        data.push_back(check_sum);
        auto word_list = word_list_loader_.Load(lang);
        int total_bits = num_bits_ + num_check_sum;
        assert(total_bits % 11 == 0);
        int num_words = total_bits / 11;
        Bits bits(data, total_bits);
        for (int i = 0; i < num_words; ++i) {
            int word_index = bits.FrontBitsToUInt16(11);
            word_list_.push_back(word_list[word_index]);
            bits.ShiftToLeft(11);
        }
        return word_list_;
    }

private:
    std::vector<uint8_t> entropy_;
    int num_bits_;
    WordListLoader& word_list_loader_;
    mutable WordList word_list_;
};

} // namespace bip39

#endif
