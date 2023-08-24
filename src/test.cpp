#include <gtest/gtest.h>

#include "basic_defs.h"
#include "words.hpp"
#include "random.hpp"

TEST(WordList, Initial)
{
    bip39::WordListLoader word_list("docs");
    for (auto const& lang : bip39::LANGUAGES) {
        EXPECT_EQ(word_list.Load(lang).size(), 2048);
    }
}

TEST(Random, Random)
{
    bip39::RandomBytes bytes(128);
    auto res = bytes.Random();
    EXPECT_EQ(res.size(), 128);

    auto res2 = bytes.Random();
    EXPECT_NE(res, res2);
}
