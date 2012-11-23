namespace loki
{

namespace components
{
	template< class _T >
	inline const std::string& GetComponentTypeName() 
	{ 
		static const std::string _Str = std::string("COMPONENT_NOT_REGISTERED"); return _Str; 
	}

	template< class _T >
	inline const ::loki::util::general::TypeInfo& GetComponentTypeInfo() 
	{ 
		static const ::loki::util::general::TypeInfo& _TI = ::loki::util::general::TypeInfo(typeid(Component)); return _TI;
	}

	template< class _T >
	inline bool ComponentAllowsMultipleInstancesOnEntity()	
	{ 
		return true; 
	}
}

Entity* Component::GetEntity()
{
	return m_Entity;
}

const Entity* Component::GetEntity() const
{
	return m_Entity;
}

template< typename _ComponentType >
_ComponentType* Component::GetComponent() const
{
	return GetEntity()->GetComponent<_ComponentType>();
}

}