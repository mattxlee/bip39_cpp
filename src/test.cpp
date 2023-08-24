#include <gtest/gtest.h>

#include "basic_defs.h"
#include "words.hpp"

TEST(WordList, Initial)
{
    bip39::WordListLoader word_list("docs");
    for (auto const& lang : bip39::LANGUAGES) {
        EXPECT_EQ(word_list.Load(lang).size(), 2048);
    }
}
