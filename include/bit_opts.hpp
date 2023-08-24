#ifndef BIT_OPTS_HPP
#define BIT_OPTS_HPP

#include <cstdint>

#include <vector>

namespace bip39 {

template <typename T>
class Bits {
public:
    static constexpr int BITS_T = sizeof(T) * 8;

    explicit Bits(T val, int num_bits = BITS_T)
        : num_bits_(num_bits)
    {
        data_.push_back(val);
    }

    Bits(void const* data, uint32_t num_bits)
        : num_bits_(num_bits)
    {
        if (data == nullptr || num_bits == 0) {
            throw std::runtime_error("invalid incoming data");
        }
        int n = num_bits / BITS_T;
        if (num_bits % BITS_T != 0) {
            ++n;
        }
        data_.resize(n, 0);
        memcpy(data_.data(), data, n);
    }

    T FrontBitsToInt(int num_front_bits) const
    {
        if (num_front_bits > BITS_T) {
            throw std::runtime_error("too many bits to convert to 64-bit int value");
        }
        if (num_front_bits > num_bits_) {
            num_front_bits = num_bits_;
        }
        T first = data_[0];
        int num_shift = BITS_T - num_front_bits;
        return (first & BuildMask(num_front_bits)) >> num_shift;
    }

    void ShiftToLeft(int num_bits_to_shift)
    {
        int rm_n = (num_bits_to_shift / BITS_T);
        if (rm_n > 0) {
            data_.erase(std::begin(data_), std::begin(data_) + rm_n);
        }
        int n = num_bits_to_shift % BITS_T;
        int nn = BITS_T - n;
        T mask = BuildMask(n);
        for (int i = 0; i < data_.size(); ++i) {
            if (i > 0) {
                T remaining_bits = (data_[i] & mask) >> nn;
                data_[i - 1] += remaining_bits;
            }
            data_[i] <<= n;
        }
    }

    std::vector<T> const& GetData() const
    {
        return data_;
    }

private:
    static T SetAllBits()
    {
        T val;
        memset(&val, 0xff, sizeof(val));
        return val;
    }

    static uint64_t BuildMask(int num_bits)
    {
        assert(num_bits <= BITS_T);
        T res = SetAllBits();
        res <<= (BITS_T - num_bits);
        return res;
    }

    std::vector<T> data_;
    int num_bits_;
};

} // namespace bip39

#endif
