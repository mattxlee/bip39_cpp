#include "sha256.h"

#include <stdexcept>

#include <openssl/evp.h>

namespace bip39 {

SHA256::SHA256()
    : mdctx_(EVP_MD_CTX_new())
{
    if (mdctx_ == nullptr) {
        throw std::runtime_error("create new md context object is failed");
    }
    if (1 != EVP_DigestInit_ex(mdctx_, EVP_sha256(), nullptr)) {
        throw std::runtime_error("cannot initialize md context object");
    }
}

SHA256::~SHA256()
{
    if (mdctx_) {
        EVP_MD_CTX_free(mdctx_);
    }
}

void SHA256::Update(void const* data, int n)
{
    if (1 != EVP_DigestUpdate(mdctx_, data, n)) {
        throw std::runtime_error("cannot update data to md context object");
    }
}

std::vector<uint8_t> const& SHA256::GetResult()
{
    if (res_.empty()) {
        int md_size = EVP_MD_size(EVP_sha256());
        uint8_t* pmd_out = reinterpret_cast<uint8_t*>(OPENSSL_malloc(md_size));
        if (nullptr == pmd_out) {
            throw std::runtime_error("failed to allocate memory");
        }
        uint32_t len;
        if (1 != EVP_DigestFinal_ex(mdctx_, pmd_out, &len)) {
            throw std::runtime_error("cannot calculate sha256 digest");
        }
        assert(len == md_size);
        res_.resize(md_size);
        memcpy(res_.data(), pmd_out, md_size);
        OPENSSL_free(pmd_out);
    }
    return res_;
}

} // namespace bip39
