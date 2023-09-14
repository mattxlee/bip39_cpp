#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <cstdint>

#include <vector>

namespace bip39 {

class RandomBytes {
public:
    explicit RandomBytes(int n);

    std::vector<uint8_t> Random();

private:
    int n_{0};
};

} // namespace bip39

#endif
