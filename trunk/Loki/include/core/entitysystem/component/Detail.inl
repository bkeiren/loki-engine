namespace loki
{

namespace components
{

namespace detail
{

namespace
{

template< class _T >
Component* CreateComponent()
{
	return new _T;
}

}

inline ComponentRegistry& GetComponentRegistry()
{
	static ComponentRegistry _Registry;
	return _Registry;
}

template< class _T >
RegistryEntryHelper<_T>::RegistryEntryHelper( const std::string& _Name )
{
	ComponentRegistry& _Registry = GetComponentRegistry();
	CreateComponentFunction _Function = CreateComponent<_T>;

	std::pair<ComponentRegistryIter, bool> _Ret = 
		_Registry.insert(ComponentRegistryPair(_Name, RegistryEntry(_Function, GetComponentTypeInfo<_T>())));

	if (_Ret.second == false)
	{
		// A component was already registered with this name.
		LOG(VL_ERROR, "A component type with name '%s' is already registered", _Name.c_str());
		assert("Check log" && 0);
	}
}

template< class _T >
RegistryEntryHelper<_T>& RegistryEntryHelper<_T>::Instance( const std::string& _Name )
{
	static RegistryEntryHelper<_T> _Instance(_Name);
	return _Instance;
}

}

}

}