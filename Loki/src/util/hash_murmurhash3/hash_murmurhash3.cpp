#include <string>
#include "util/hash/hash_murmurhash3/hash_murmurhash3.h"
#include "util/hash/hash_murmurhash3/MurmurHash3.h"

namespace loki
{

namespace util
{

namespace
{

uint32_t seed = 0x4A094C4;

}

uint32_t Hash_MurmurHash3( const char* s )
{
	uint32_t out;
	MurmurHash3_x86_32((void*)s, strlen(s), seed, (void*)&out);
	return out;
} 

uint128 Hash_MurmurHash3_128( const char* s )
{
	uint128 out;
	MurmurHash3_x86_128((void*)s, strlen(s), seed, (void*)&out);
	return out;
}

uint128 Hash_MurmurHash3_128_x64( const char* s )
{
	uint128 out;
	MurmurHash3_x64_128((void*)s, strlen(s), seed, (void*)&out);
	return out;
}

}

}