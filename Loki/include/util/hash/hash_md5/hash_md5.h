#pragma once

#ifndef HASH_MD5_H
#define HASH_MD5_H

#include "util/uint128.h"

namespace loki
{

namespace util
{

namespace hash
{

typedef uint128 MD5Hash;

MD5Hash Hash_MD5( const char* s ); 

void MD5HashToString( MD5Hash _Hash, std::string& _String );

}

}

}

#endif