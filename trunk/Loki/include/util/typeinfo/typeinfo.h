#pragma once

#ifndef TYPEINFO_H
#define TYPEINFO_H

#include <typeinfo>

namespace loki
{

namespace util
{

//////////////////////////////////////////////////////////////////////////
// Small and simple wrapper around std::type_info because we can't
// store those directly (because there is no public constructor), and 
// we can't compare references returned by typeid().
// Using this class, it's possible to store and compare types instead of
// just data in containers by storing objects of TypeInfo (Not pointers, 
// actual objects), which are constructed by passing the return value
// of a typeid() call to the public constructor of this class.
//////////////////////////////////////////////////////////////////////////
class TypeInfo
{
public:
	TypeInfo( const std::type_info& _TypeInfo );

	//////////////////////////////////////////////////////////////////////////
	// External operators because otherwise certain std containers will complain.
	//////////////////////////////////////////////////////////////////////////
	friend bool operator == ( const TypeInfo& _LHS, const TypeInfo& _RHS );
	friend bool operator != ( const TypeInfo& _LHS, const TypeInfo& _RHS );
	friend bool operator < ( const TypeInfo& _LHS, const TypeInfo& _RHS );
	friend bool operator > ( const TypeInfo& _LHS, const TypeInfo& _RHS );
	
	TypeInfo& operator = ( const TypeInfo& _RHS );

	//////////////////////////////////////////////////////////////////////////
	// Conversion operator(s). These can be useful when storing
	// TypeInfo in a hash_map, for example. This is because hash_map's
	// require their data to be convertible into values of type size_t
	// in order to be able to hash it.
	//////////////////////////////////////////////////////////////////////////
	operator size_t() const;	// NOTE: the 'const' at the end is VERY important. Without it, hash_map will not accept it.

	const char* GetTypeName() const;
private:
	TypeInfo();

	const std::type_info* m_Ptr;
};

bool operator == ( const TypeInfo& _LHS, const TypeInfo& _RHS );
bool operator != ( const TypeInfo& _LHS, const TypeInfo& _RHS );
bool operator < ( const TypeInfo& _LHS, const TypeInfo& _RHS );
bool operator > ( const TypeInfo& _LHS, const TypeInfo& _RHS );

}

}

#endif