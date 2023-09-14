#ifndef BYTESWAP_HPP
#define BYTESWAP_HPP

#include <cstdint>

namespace bip39 {

uint16_t __bswap_16(uint16_t x);

uint32_t __bswap_32(uint32_t x);

uint64_t __bswap_64(uint64_t x);

uint16_t ByteSwap(uint16_t val);

uint32_t ByteSwap(uint32_t val);

uint64_t ByteSwap(uint64_t val);

} // namespace bip39

#endif
