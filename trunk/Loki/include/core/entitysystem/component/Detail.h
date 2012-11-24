#pragma once

#ifndef COMPONENT_DETAIL_H
#define COMPONENT_DETAIL_H

#include "util/typeinfo/typeinfo.h"

//////////////////////////////////////////////////////////////////////////
//
// The implementation of automatically registering components has been
// implemented using:
// http://gamedev.stackexchange.com/questions/17746/entity-component-systems-in-c-how-do-i-discover-types-and-construct-component/17759#17759
// (Accessed 24-11-2012)
//
//////////////////////////////////////////////////////////////////////////

namespace loki
{

class Component;

namespace components
{

namespace detail
{

class RegistryEntry;

//////////////////////////////////////////////////////////////////////////
// Typedefs.
//////////////////////////////////////////////////////////////////////////
typedef Component*(*CreateComponentFunction)();
CONTAINER_MACRO_MAP(std::string, RegistryEntry, ComponentRegistry)

//////////////////////////////////////////////////////////////////////////
// Functions.
//////////////////////////////////////////////////////////////////////////
inline ComponentRegistry& GetComponentRegistry();

//////////////////////////////////////////////////////////////////////////
// Classes.
//////////////////////////////////////////////////////////////////////////
class RegistryEntry 
{
public:
	RegistryEntry( CreateComponentFunction _Function, const util::general::TypeInfo& _TypeInfo );

	CreateComponentFunction m_Function;
	const util::general::TypeInfo& m_TypeInfo;
};

template< class _T >
class RegistryEntryHelper
{
public:
	static RegistryEntryHelper<_T>& Instance( const std::string& _Name );

private:
	RegistryEntryHelper( const std::string& _Name );

// 	RegistryEntryHelper(const RegistryEntryHelper<T>&) = delete; // C++11 feature
// 	RegistryEntryHelper& operator=(const RegistryEntryHelper<T>&) = delete;
};

}

}

}

#include "core/entitysystem/component/Detail.inl"

#endif