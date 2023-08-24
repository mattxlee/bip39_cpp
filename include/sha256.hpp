#ifndef SHA256_HPP
#define SHA256_HPP

#include <cstdint>

#include <vector>

#include <stdexcept>

#include <openssl/sha.h>

namespace bip39 {

class SHA256 {
public:
    SHA256()
    {
        if (!SHA256_Init(&ctx_)) {
            throw std::runtime_error("cannot initialize sha256 context");
        }
    }

    void Update(uint8_t const* data, int n)
    {
        if (!SHA256_Update(&ctx_, data, n)) {
            throw std::runtime_error("cannot update data to sha256");
        }
    }

    std::vector<uint8_t> const& GetResult()
    {
        if (res_.empty()) {
            res_.resize(SHA256_DIGEST_LENGTH);
            if (!SHA256_Final(res_.data(), &ctx_)) {
                throw std::runtime_error("cannot get sha256 digest");
            }
        }
        return res_;
    }

private:
    SHA256_CTX ctx_;
    std::vector<uint8_t> res_;
};

} // namespace bip39

#endif
