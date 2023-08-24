#include <gtest/gtest.h>

#include "basic_defs.h"
#include "words.hpp"
#include "random.hpp"
#include "bit_opts.hpp"

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

TEST(Bits, FirstNBits)
{
    uint16_t hello{0b1011011101111000};
    bip39::Bits<uint16_t> bits(hello);
    auto val = bits.FrontBitsToInt(3);
    EXPECT_EQ(val, 0b101);
}

TEST(Bits, ShiftWith1Element)
{
    uint16_t before{0b1011011101111000};
    bip39::Bits<uint16_t> bits(before);

    uint16_t after{0b0110111011110000};
    bits.ShiftToLeft(1);
    EXPECT_EQ(bits.GetData()[0], after);
}

TEST(Bits, ThrowWhenRetrievingWrongNumFrontBits)
{
    uint8_t before[] = { 0b11011101, 0b01100011 };
    bip39::Bits<uint8_t> bits(before, 16);
    EXPECT_THROW({
        bits.FrontBitsToInt(9);
    }, std::exception);
}

TEST(Bits, ShiftWithMultiplyElements)
{
    uint8_t before[] = { 0b11011101, 0b01100011 };
    bip39::Bits<uint8_t> bits(before, 16);
    bits.ShiftToLeft(3);

    uint8_t after[] = { 0b11101011, 0b00011000 };
    EXPECT_EQ(bits.GetData()[0], after[0]);
    EXPECT_EQ(bits.GetData()[1], after[1]);
}
