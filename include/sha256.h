#ifndef SHA256_HPP
#define SHA256_HPP

#include <cstdint>

#include <vector>

struct evp_md_ctx_st;

namespace bip39 {

class SHA256 {
public:
    SHA256();

    ~SHA256();

    void Update(void const* data, int n);

    std::vector<uint8_t> const& GetResult();

private:
    evp_md_ctx_st* mdctx_{nullptr};
    std::vector<uint8_t> res_;
};

} // namespace bip39

#endif
