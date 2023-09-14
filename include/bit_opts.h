#ifndef BIT_OPTS_HPP
#define BIT_OPTS_HPP

#include <cstdint>
#include <cstring>

#include <vector>
#include <bitset>

namespace bip39 {

class Bits {
public:
    using ElementType = uint8_t;
    using IntType = uint64_t;
    using Container = std::vector<ElementType>;

    static constexpr int NumElementBits = sizeof(ElementType) * 8;
    static constexpr int NumIntBits = sizeof(IntType) * 8;

    Bits();

    Bits(std::vector<uint8_t> const& from, uint32_t num_bits);

    static ElementType MakeMask(uint8_t num_bits);

    static uint16_t MakeInt16FromTwoBytes(uint8_t b1, uint8_t b2, int total_bits);

    template <typename T>
    static T FirstNBits(int num_bits, T val)
    {
        T mask;
        memset(&mask, 0xff, sizeof(mask));
        int num_padding_bits = (sizeof(T) * 8 - num_bits);
        mask <<= num_padding_bits;
        return (val & mask) >> num_padding_bits;
    }

    uint16_t FrontBitsToUInt16(int num_front_bits) const;

    void ShiftToLeft(int num_bits_to_shift);

    void AddBits(int num_bits, uint16_t val);

    int GetNumBits() const;

    Container const& GetData() const;

private:
    Container container_;
    int num_bits_;
};

template <typename StreamType>
StreamType& operator<<(StreamType& s, Bits const& bits)
{
    for (uint8_t ch : bits.GetData()) {
        std::bitset<8> byte(ch);
        s << byte << " ";
    }
    return s;
}

} // namespace bip39

#endif
