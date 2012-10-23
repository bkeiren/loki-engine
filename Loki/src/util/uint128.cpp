#include "util/uint128.h"

uint128::uint128()	:
	i0(0),
	i1(0),
	i2(0),
	i3(0)
{

}

uint128::uint128( uint64 _high, uint64 _low )	:
	high(_high),
	low(_low)
{

}

uint128::uint128( uint32 _i0, uint32 _i1, uint32 _i2, uint32 _i3 )	:
	i0(_i0),
	i1(_i1),
	i2(_i2),
	i3(_i3)
{

}

bool uint128::operator == ( uint128 _n )
{
	return (i0 == _n.i0 && i1 == _n.i1 && i2 == _n.i2 && i3 == _n.i3);
}

bool uint128::operator != ( uint128 _n )
{
	return (i0 != _n.i0 || i1 != _n.i1 || i2 != _n.i2 || i3 != _n.i3);
}
