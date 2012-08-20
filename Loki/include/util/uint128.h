#pragma once

#ifndef UINT128_H
#define UINT128_H

typedef unsigned __int64 uint64;
class uint128
{
public:
	uint128();
	uint128( uint64 _high, uint64 _low );
	uint128( unsigned int _i0, unsigned int _i1, unsigned int _i2, unsigned int _i3 );

	union
	{
		struct { uint64 high; uint64 low; };
		struct { unsigned int i0; unsigned int i1; unsigned int i2; unsigned int i3; };
		struct { char c[16]; };
	};

	bool operator == ( uint128 _n );
	bool operator != ( uint128 _n );
};

#endif