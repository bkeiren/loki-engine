#pragma once

#ifndef UINT128_H
#define UINT128_H

using namespace loki;

class uint128
{
public:
	uint128();
	uint128( uint64 _high, uint64 _low );
	uint128( uint32 _i0, uint32 _i1, uint32 _i2, uint32 _i3 );

	union
	{
		struct { uint64 high; uint64 low; };
		struct { uint32 i0; uint32 i1; uint32 i2; uint32 i3; };
		struct { char c[16]; };
	};

	bool operator == ( uint128 _n );
	bool operator != ( uint128 _n );
};

#endif