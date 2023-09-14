#include "random.h"

#include <openssl/rand.h>

namespace bip39 {

RandomBytes::RandomBytes(int n)
    : n_(n)
{
}

std::vector<uint8_t> RandomBytes::Random()
{
    std::vector<uint8_t> res(n_);
    RAND_bytes(res.data(), n_);
    return res;
}

} // namespace bip39
