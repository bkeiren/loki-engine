//////////////////////////////////////////////////////////////////////////
// This file can be included by source code in order to provide access
// to all hashing functions at once, or to provide access to the HASH()
// macro. The HASH() macro uses one of the available hash functions
// to hash it's argument, specified by the define DEFAULT_HASH.
// In order to change which hashing function HASH() defaults to, you
// can redefine DEFAULT_HASH to one of the HASH_* pre-processor defines.
// Note that if an invalid value is assigned to DEFAULT_HASH, a compilation
// error will occur.
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef HASH_H
#define HASH_H

#include "util/hash/hash_fnv/hash_fnv.h"
#include "util/hash/hash_md5/hash_md5.h"
#include "util/hash/hash_murmurhash3/hash_murmurhash3.h"

namespace loki
{

namespace util
{

#define HASH_FNV				0
#define HASH_MD5				1
#define HASH_MURMUR3			2
#define HASH_MURMUR3_128		3
#define HASH_MURMUR3_128_x64	4

// Can be redefined to any 
#define DEFAULT_HASH	HASH_MURMUR3


#if DEFAULT_HASH == HASH_FNV

	#define HASH(a)	(loki::util::hash::Hash_FNV32(a))

#elif DEFAULT_HASH == HASH_MD5

	#define HASH(a)	(loki::util::hash::Hash_MD5(a))

#elif DEFAULT_HASH == HASH_MURMUR3

	#define HASH(a)	(loki::util::hash::Hash_MurmurHash3(a))

#elif DEFAULT_HASH == HASH_MURMUR3_128

	#define HASH(a)	(loki::util::hash::Hash_MurmurHash3_128(a))

#elif DEFAULT_HASH == HASH_MURMUR3_128_x64

	#define HASH(a)	(loki::util::hash::Hash_MurmurHash3_128_x64(a))

#else
	// Any other value for DEFAULT_HASH is invalid.
	#error Pre-processor define DEFAULT_HASH is defined as an invalid value.

#endif

}

}

#endif