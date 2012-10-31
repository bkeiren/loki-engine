#pragma once

#ifndef HASH_MURMURHASH3_H
#define HASH_MURMURHASH3_H

#include "util/uint128.h"

typedef __int32 int32_t;
typedef unsigned long uint32_t;

namespace loki
{

namespace util
{

namespace hash
{

uint32_t Hash_MurmurHash3( const char* s );		// 32-bit platform, 32-bit output.
uint128 Hash_MurmurHash3_128( const char* s);	// 32-bit platform, 128-bit output.
uint128 Hash_MurmurHash3_128_x64( const char* s);	// 64-bit platform, 128-bit output.

}

}

}

#endif