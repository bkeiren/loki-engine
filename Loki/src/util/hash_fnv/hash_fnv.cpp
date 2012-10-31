#include <string>
#include "util/hash/hash_fnv/hash_fnv.h"

#define FNV_PRIME_32	16777619
#define FNV_OFFSET_32	2166136261U

namespace loki
{

namespace util
{

namespace hash
{

uint32_t Hash_FNV32( const char* s )
{
	uint32_t hash = FNV_OFFSET_32;

	for(uint32_t i = 0; i < strlen(s); i++)
	{
		hash = hash ^ (s[i]); // xor next byte into the bottom of the hash
		hash = hash * FNV_PRIME_32; // Multiply by prime number found to work well
	}

	return hash;
} 

}

}

}