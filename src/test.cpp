#include <gtest/gtest.h>

#include <string>
#include <string_view>

#include "basic_defs.h"

#include "toolbox.hpp"

#include "mnemonic.hpp"
#include "words.hpp"

TEST(ToolBox, ParseHexString)
{
    char const* sz_example = "abffdd0e110098000102";
    auto res = ParseHex(sz_example);
    EXPECT_EQ(res.size(), 10);
    EXPECT_EQ(res[0], 0xab);
    EXPECT_EQ(res[1], 0xff);
    EXPECT_EQ(res[9], 2);
}

TEST(ToolBox, ParseWordsString)
{
    char const* sz_example = "hello world this is an example";
    auto words = ParseWords(sz_example, " ");
    EXPECT_EQ(words.size(), 6);
    EXPECT_EQ(words[0], "hello");
    EXPECT_EQ(words[1], "world");
    EXPECT_EQ(words[2], "this");
    EXPECT_EQ(words[3], "is");
    EXPECT_EQ(words[4], "an");
    EXPECT_EQ(words[5], "example");
}

TEST(WordList, Initial)
{
    bip39::WordListLoader word_list("docs");
    for (auto const& lang : bip39::LANGUAGES) {
        EXPECT_EQ(word_list.Load(lang).size(), 2048);
    }
}

TEST(Bits, ShiftWithMultiplyElements)
{
    std::vector<uint8_t> before { 0b11011101, 0b01100011 };
    bip39::Bits bits(before, 16);
    bits.ShiftToLeft(3);

    uint8_t after[] = { 0b11101011, 0b00011000 };
    EXPECT_EQ(bits.GetData()[0], after[0]);
    EXPECT_EQ(bits.GetData()[1], after[1]);
}

TEST(Mnemonic, EntropyToWords)
{
    Json::Value root = ReadTestJsonFile("docs/tests.json");
    auto lang_names = root.getMemberNames();
    for (auto const& lang : lang_names) {
        Json::Value tests = root[lang];
        if (!tests.isArray()) {
            throw std::runtime_error("the tests object type is not an array");
        }
        bip39::WordListLoader loader("docs");
        for (Json::Value const& test_obj : tests) {
            std::string hash_str = test_obj[0].asString();
            std::vector<uint8_t> hash = ParseHex(hash_str);
            std::string words_str = test_obj[1].asString();
            std::vector<std::string> words = ParseWords(words_str, ((lang == "japanese") ? u8"\u3000" : u8"\u0020"));
            bip39::Mnemonic mnemonic(hash, loader);
            EXPECT_EQ(mnemonic.GetWordList(lang), words);
        }
    }
}
