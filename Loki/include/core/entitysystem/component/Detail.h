#pragma once

#ifndef COMPONENT_DETAIL_H
#define COMPONENT_DETAIL_H

namespace loki
{

class Component;

namespace components
{

namespace detail
{

typedef Component*(*CreateComponentFunction)();
CONTAINER_MACRO_MAP(std::string, CreateComponentFunction, ComponentRegistry)

inline ComponentRegistry& GetComponentRegistry()
{
	static ComponentRegistry _Registry;
	return _Registry;
}

template< class _T >
Component* CreateComponent()
{
	return new _T;
}

template< class _T >
class RegistryEntry
{
public:
	static RegistryEntry<_T>& Instance( const std::string& _Name )
	{
		static RegistryEntry<_T> _Instance(_Name);
		return _Instance;
	}

private:
	RegistryEntry( const std::string& _Name )
	{
		ComponentRegistry& _Registry = GetComponentRegistry();
		CreateComponentFunction _Function = CreateComponent<_T>;

		std::pair<ComponentRegistryIter, bool> _Ret = 
			_Registry.insert(ComponentRegistryPair(_Name, _Function));

		if (_Ret.second == false)
		{
			// A component was already registered with this name.
			LOG(VL_ERROR, "A component type with name '%s' is already registered", _Name.c_str());
			assert("Check log" && 0);
		}
	}

// 	RegistryEntry(const RegistryEntry<T>&) = delete; // C++11 feature
// 	RegistryEntry& operator=(const RegistryEntry<T>&) = delete;
};

}

}

}

#endif