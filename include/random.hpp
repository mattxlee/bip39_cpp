#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <cstdint>

#include <vector>

#include <openssl/rand.h>

namespace bip39 {

class RandomBytes {
public:
    explicit RandomBytes(int n)
        : n_(n)
    {
    }

    std::vector<uint8_t> Random()
    {
        std::vector<uint8_t> res(n_);
        RAND_bytes(res.data(), n_);
        return res;
    }

private:
    int n_{0};
};

} // namespace bip39

#endif
