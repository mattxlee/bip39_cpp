#include "mnemonic.h"

#include <cassert>

#include <iostream>

#include <utf8proc.h>

#include <openssl/evp.h>

#include "langs.h"
#include "toolbox.h"

#include "sha256.h"
#include "bit_opts.h"

namespace bip39 {

bool Mnemonic::IsValidNumMnemonicSentences(int n)
{
    return !(n % 3 != 0 || n < 12 || n > 24);
}

int Mnemonic::GetEntBitsByNumMnemonicSentences(int n)
{
    assert(IsValidNumMnemonicSentences(n));
    return n * 32 / 3;
}

Mnemonic::Mnemonic(std::vector<uint8_t> entropy)
    : entropy_(std::move(entropy))
{
}

Mnemonic::Mnemonic(WordList const& word_list, std::string const& lang)
{
    if (word_list.size() % 3 != 0 || word_list.size() < 12 || word_list.size() > 24) {
        throw std::runtime_error("invalid number of words to convert");
    }
    Bits bits;
    auto it = langs.find(lang);
    if (!utils::LangExists(lang)) {
        throw std::runtime_error("invalid lang name");
    }
    for (auto const& word : word_list) {
        int index = utils::GetLangIndex(lang, word);
        if (index == -1) {
            throw std::runtime_error("index of the word cannot be found");
        }
        bits.AddBits(11, index);
    }
    int num_ent_bits = word_list.size() * 32 / 3;
    int num_entropy_bytes = num_ent_bits / 8;
    entropy_.resize(num_entropy_bytes);
    std::copy(std::cbegin(bits.GetData()), std::cbegin(bits.GetData()) + num_entropy_bytes, std::begin(entropy_));
}

WordList Mnemonic::GetWordList(std::string const& lang) const
{
    auto data = entropy_;
    int num_bits = entropy_.size() * 8;
    SHA256 sha;
    sha.Update(data.data(), data.size());
    auto const& sha_res = sha.GetResult();
    int num_check_sum = num_bits / 32;
    uint8_t check_sum = sha_res[0] & (Bits::MakeMask(num_check_sum));
    data.push_back(check_sum);
    auto it = langs.find(lang);
    if (it == std::cend(langs)) {
        throw std::runtime_error("invalid lang name");
    }
    int total_bits = num_bits + num_check_sum;
    assert(total_bits % 11 == 0);
    int num_words = total_bits / 11;
    Bits bits(data, total_bits);
    WordList word_list;
    for (int i = 0; i < num_words; ++i) {
        int word_index = bits.FrontBitsToUInt16(11);
        word_list.push_back(langs[lang][word_index]);
        bits.ShiftToLeft(11);
    }
    return word_list;
}

std::vector<uint8_t> const& Mnemonic::GetEntropyData() const
{
    return entropy_;
}

std::string NormalizeString(std::string_view src)
{
    uint8_t* sz = utf8proc_NFKD(reinterpret_cast<uint8_t const*>(src.data()));
    std::string res(reinterpret_cast<char const*>(sz));
    free(sz);
    return res;
}

std::vector<uint8_t> Mnemonic::CreateSeed(std::string_view passphrase, std::string_view lang) const
{
    std::string salt = NormalizeString(std::string(u8"mnemonic") + std::string(passphrase));
    int const out_len{512 / 8};
    std::vector<uint8_t> out(out_len);
    std::string words = NormalizeString(GenerateWords(GetWordList(std::string(lang)), GetDelimiterByLang(lang)));
    int res = PKCS5_PBKDF2_HMAC(words.c_str(), words.size(), reinterpret_cast<uint8_t const*>(salt.c_str()), salt.size(), 2048, EVP_sha512(), out_len, out.data());
    if (1 != res) {
        throw std::runtime_error("failed to run algorithm: PBKDF2");
    }
    return out;
}

} // namespace bip39
