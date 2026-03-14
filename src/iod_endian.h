//
// Created by larry on 3/14/26.
//

#ifndef IOD_ENDIAN_H
#define IOD_ENDIAN_H

#include <stdint.h>

#if defined(__GNUC__) || defined(__clang__)
#define bswap16 __builtin_bswap16
#define bswap32 __builtin_bswap32
#define bswap64 __builtin_bswap64
#else

static inline uint16_t bswap16(uint16_t x)
{
    return (x << 8) | (x >> 8);
}

static inline uint32_t bswap32(uint32_t x)
{
    return ((x & 0x000000FFU) << 24) |
           ((x & 0x0000FF00U) << 8)  |
           ((x & 0x00FF0000U) >> 8)  |
           ((x & 0xFF000000U) >> 24);
}

static inline uint64_t bswap64(uint64_t x)
{
    return ((uint64_t)bswap32(x) << 32) |
           bswap32(x >> 32);
}

#endif //IOD_ENDIAN_H
