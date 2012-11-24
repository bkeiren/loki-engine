#pragma once

#ifndef ENTITY_INL
#define ENTITY_INL

#ifdef _DEBUG
	#define CHECK_IF_REGISTERED_AND_LOG(TYPE)					\
		if (!::loki::components::IsComponentRegistered<TYPE>())				\
		{												\
			LOG(VL_ERROR, "Entity::"__FUNCTION__": Component type has not been registered. Register a component with macro REGISTER_COMPONENT.");	\
			return 0;									\
		}
#else
	#define CHECK_IF_REGISTERED_AND_LOG()
#endif

namespace loki
{

template< typename _ComponentType >
_ComponentType* Entity::InstantiateComponent()
{
	CHECK_IF_REGISTERED_AND_LOG(_ComponentType)

	Component* comp = (Component*)new _ComponentType();
	//Component* comp = (Component*)malloc(sizeof(_ComponentType));

	ComponentsIter it = m_Components.find(::loki::components::GetComponentTypeInfo<_ComponentType>());
	if (!::loki::components::ComponentAllowsMultipleInstancesOnEntity<_ComponentType>() && it != m_Components.end() && (*it).second.size() >= 1)
	{
		LOG(VL_ERROR, "Entity::InstantiateComponent: Entities can not have more than one instance of the '%s' component. Use macro COMPONENT_SINGLE_INSTANCE to change.", ::loki::components::GetComponentTypeName<_ComponentType>().c_str());
		return 0;
	}
	m_Components[::loki::components::GetComponentTypeInfo<_ComponentType>()].push_back(comp);
	_OnComponentInstantiated<_ComponentType>((_ComponentType*)comp);
	comp->SetEntity(this);
	comp->_BaseInit();
	return (_ComponentType*)comp;
}

template< typename _ComponentType >
void Entity::RemoveComponent()
{
	CHECK_IF_REGISTERED_AND_LOG(_ComponentType)

	_ComponentType* comp = 0;
	util::general::TypeInfo typeinfo = ::loki::components::GetComponentTypeInfo<_ComponentType>();
	ComponentsIter it = m_Components.find(typeinfo);
	if (it == m_Components.end())
	{
		std::string t(typeinfo.GetTypeName());
		LOG(VL_ERROR, "Entity::RemoveComponent: Entity does not have a component of type %s.", t.c_str());
		return;
	}
	comp = (_ComponentType*)&(*((*it).second.back()));
	(*it).second.pop_back();
	comp->_BaseTerminate();
	_OnComponentRemoved<_ComponentType>(comp);
	delete comp;
	//free(comp);

	if ((*it).second.size() <= 0)
	{
		m_Components.erase(it);
	}
}

template< typename _ComponentType >
void Entity::RemoveComponent( _ComponentType* _Instance )
{
	CHECK_IF_REGISTERED_AND_LOG(_ComponentType)

	_ComponentType* comp = 0;
	util::general::TypeInfo typeinfo = ::loki::components::GetComponentTypeInfo<_ComponentType>();
	ComponentsIter it = m_Components.find(typeinfo);
	if (it == m_Components.end())
	{
		std::string t(typeinfo.GetTypeName());
		LOG(VL_ERROR, "Entity::RemoveComponent: Entity does not have any component of type %s.", t.c_str());
		return;
	}
	for (ComponentsVectorIter it2 = (*it).second.begin(); it2 != (*it).second.end(); ++it2)
	{
		if ((*it2) == _Instance)
		{
			comp = (*it2);
			(*it).second.remove(it2);
			comp->_BaseTerminate();
			_OnComponentRemoved<_ComponentType>(comp);
			delete comp;
			//free(comp);

			if ((*it).second.size() <= 0)
			{
				m_Components.erase(it);
			}
			return;
		}
	}
	LOG(VL_ERROR, "Entity::RemoveComponent: Entity does not have specified component instance.");
}

template< typename _ComponentType >
void Entity::RemoveComponents()
{
	CHECK_IF_REGISTERED_AND_LOG(_ComponentType)

	_ComponentType* comp = 0;
	util::general::TypeInfo typeinfo = ::loki::components::GetComponentTypeInfo<_ComponentType>();
	ComponentsIter it = m_Components.find(typeinfo);
	if (it == m_Components.end())
	{
		std::string t(typeinfo.GetTypeName());
		LOG(VL_ERROR, "Entity::RemoveComponent: Entity does not have components of type %s.", t.c_str());
		return;
	}
	for (std::vector<_ComponentType*>::iterator it2 = (*it).second.begin(); it2 != (*it).second.end(); ++it2)
	{
		comp = (*it2);
		comp->_BaseTerminate();
		_OnComponentRemoved<_ComponentType>(comp);
		delete comp;
		//free(comp);
	}
	(*it).second.clear();
	m_Components.erase(it);
}

template<>
void Entity::RemoveComponent<Transform>()
{
	LOG(VL_WARN, "Entity::RemoveComponent: It is not possible to remove the Transform component.");
}

template< typename _ComponentType >
bool Entity::HasComponent() const
{
	CHECK_IF_REGISTERED_AND_LOG(_ComponentType)

	ComponentsConstIter it = m_Components.find(::loki::components::GetComponentTypeInfo<_ComponentType>());
	return (it != m_Components.end()) && ((*it).size() > 0);
}

template< typename _ComponentType >
_ComponentType* Entity::GetComponent() const
{
	CHECK_IF_REGISTERED_AND_LOG(_ComponentType)

	util::general::TypeInfo typeinfo = ::loki::components::GetComponentTypeInfo<_ComponentType>();
	ComponentsConstIter it = m_Components.find(typeinfo);
	if (it != m_Components.end())
	{
		return (_ComponentType*)&(*((*it).second.back()));
	}
// 	std::string t(typeinfo.GetTypeName());
// 	LOG(VL_ERROR, "Entity::GetComponent: Entity does not have a component of type %s.", t.c_str());
	return 0;
}

template< typename _ComponentType >
const std::vector<_ComponentType*>* Entity::GetComponents() const
{
	CHECK_IF_REGISTERED_AND_LOG(_ComponentType)

	util::general::TypeInfo typeinfo = ::loki::components::GetComponentTypeInfo<_ComponentType>();
	ComponentsConstIter it = m_Components.find(typeinfo);
	if (it != m_Components.end())
	{
		return (std::vector<_ComponentType*>*)(&((*it).second));
	}
	// 	std::string t(typeinfo.GetTypeName());
	// 	LOG(VL_ERROR, "Entity::GetComponent:s Entity does not have a component of type %s.", t.c_str());
	return 0;
}

template<>
Transform* Entity::GetComponent<Transform>() const
{
	return m_Transform;
}

template< typename _ComponentType >
void Entity::_OnComponentInstantiated( _ComponentType* _Component )
{
	// Do nothing.
}

ONINSTANTIATESPEC(Transform)
{
	if (m_Transform == 0)
	{
		m_Transform = _Component;
	}
}

ONINSTANTIATESPEC(components::CameraComponent)
{
	if (m_Camera == 0)
	{
		m_Camera = _Component;
	}
}

ONINSTANTIATESPEC(components::Light)
{
	if (m_Light == 0)
	{
		m_Light = _Component;
	}
}

ONINSTANTIATESPEC(components::MeshRenderer)
{
	if (m_Renderer == 0)
	{
		m_Renderer = _Component;
	}
}

template< typename _ComponentType >
void Entity::_OnComponentRemoved( _ComponentType* _Component )
{
	// Do nothing.
}

ONREMOVESPEC(Transform)
{
	if (m_Transform == _Component)
	{
		m_Transform = 0;
	}
}

ONREMOVESPEC(components::CameraComponent)
{
	if (m_Camera == _Component)
	{
		m_Camera = 0;
	}
}

ONREMOVESPEC(components::Light)
{
	if (m_Light == _Component)
	{
		m_Light = 0;
	}
}

ONREMOVESPEC(components::MeshRenderer)
{
	if (m_Renderer == _Component)
	{
		m_Renderer = 0;
	}
}

}

#endif