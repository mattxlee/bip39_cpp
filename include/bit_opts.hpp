#ifndef BIT_OPTS_HPP
#define BIT_OPTS_HPP

#include <cstdint>

#include <deque>
#include <bitset>

namespace bip39 {

class Bits {
public:
    using ElementType = uint8_t;
    using IntType = uint64_t;
    using Container = std::deque<ElementType>;

    static constexpr int NumElementBits = sizeof(ElementType) * 8;
    static constexpr int NumIntBits = sizeof(IntType) * 8;

    Bits()
        : num_bits_(0)
    {
    }

    Bits(std::vector<uint8_t> const& from, uint32_t num_bits)
        : container_(std::cbegin(from), std::cend(from))
        , num_bits_(num_bits)
    {
    }

    static ElementType MakeMask(uint8_t num_bits)
    {
        ElementType mask = 0xff;
        mask <<= (8 - num_bits);
        return mask;
    }

    static uint16_t MakeInt16FromTwoBytes(uint8_t b1, uint8_t b2, int total_bits)
    {
        if (total_bits > 16) {
            throw std::runtime_error("out of number of bits range");
        }
        int num_shift_bits = total_bits % 8;
        uint16_t res{b1};
        res <<= num_shift_bits;
        res += b2;
        return res;
    }

    uint16_t FrontBitsToUInt16(int num_front_bits) const
    {
        if (num_front_bits > 16) {
            throw std::runtime_error("number of bits out of integer range");
        }
        if (num_front_bits > container_.size() * 8) {
            throw std::runtime_error("reach the limits of current data");
        }
        int num_bytes = num_front_bits / NumElementBits;
        Container bytes(num_bytes);
        std::copy(std::begin(container_), std::begin(container_) + num_bytes, std::begin(bytes));
        int extra_bits = num_front_bits % NumElementBits;
        if (extra_bits > 0) {
            // also retrieve the final byte
            ElementType final_byte = container_[num_bytes];
            final_byte &= MakeMask(extra_bits);
            final_byte >>= (8 - extra_bits);
            bytes.push_back(final_byte);
        }
        return MakeInt16FromTwoBytes(bytes[0], bytes[1], num_front_bits);
    }

    void ShiftToLeft(int num_bits_to_shift)
    {
        int rm_n = (num_bits_to_shift / NumElementBits);
        if (rm_n > 0) {
            container_.erase(std::begin(container_), std::begin(container_) + rm_n);
        }
        int n = num_bits_to_shift % NumElementBits;
        int nn = NumElementBits - n;
        ElementType mask = MakeMask(n);
        for (int i = 0; i < container_.size(); ++i) {
            if (i > 0) {
                ElementType remaining_bits = (container_[i] & mask) >> nn;
                container_[i - 1] += remaining_bits;
            }
            container_[i] <<= n;
        }
        num_bits_ -= num_bits_to_shift;
        int bytes_after_shift = num_bits_ / 8 + ((num_bits_ % 8 > 0) ? 1 : 0);
        while (container_.size() > bytes_after_shift) {
            container_.erase(std::begin(container_) + (container_.size() - 1));
        }
    }

    void ShiftToRightByAdding(int num_bits_to_shift, uint16_t data)
    {
        if (num_bits_to_shift > sizeof(uint16_t) * 8) {
            throw std::runtime_error("cannot shift bits more than 16");
        }
        int bytes_to_be_inserted = num_bits_to_shift / 8;
        int num_bits_to_shift_for_each_byte = num_bits_to_shift % 8;
        int num_bits_remainings_from_container = container_.size() * 8 - num_bits_;
        // ensure there are enough bytes
        if (num_bits_to_shift_for_each_byte > num_bits_remainings_from_container) {
            container_.push_back(0);
        }
        uint8_t mask = 0xff >> (8 - num_bits_to_shift_for_each_byte);
        uint8_t remaining_bits = (data & mask) << (8 - num_bits_to_shift_for_each_byte);
        for (int i = 0; i < container_.size(); ++i) {
            uint8_t this_remaining_bits = (container_[i] & mask) << (8 - num_bits_to_shift_for_each_byte);
            container_[i] = (container_[i] >> num_bits_to_shift_for_each_byte) + remaining_bits;
            remaining_bits = this_remaining_bits;
        }
        if (num_bits_to_shift > 8) {
            data >>= num_bits_to_shift_for_each_byte;
            container_.push_front(data);
        }
        num_bits_ += num_bits_to_shift;
    }

    Container const& GetData() const
    {
        return container_;
    }

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
