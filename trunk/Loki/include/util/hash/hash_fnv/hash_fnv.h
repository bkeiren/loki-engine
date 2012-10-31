/*
	This code was obtained from http://ctips.pbworks.com/w/page/7277591/FNV%20Hash (Accessed 09-12-2011 @ 18:43)
	It provides functionality to convert a given string to a hashing function.
*/

#pragma once

#ifndef HASH_FNV_H
#define HASH_FNV_H

typedef __int32 int32_t;
typedef unsigned long uint32_t;

namespace loki
{

namespace util
{

namespace hash
{

uint32_t Hash_FNV32( const char* s ); 

}

}

}

#endif