#pragma once

#ifndef ENTITY_INL
#define ENTITY_INL

namespace loki
{

template< typename _ComponentType >
void Entity::InstantiateComponent()
{
	Component* comp = (Component*)new _ComponentType();
	//void* buffer = operator new (sizeof(_ComponentType));
	//Component* comp = new (buffer) _ComponentType;
#if RTTI_TYPE == RTTI_TYPEID
	m_Components.insert(ComponentsPair(util::TypeInfo(typeid(_ComponentType)), comp));
#elif RTTI_TYPE == RTTI_DYNAMIC_CAST
	m_Components.push_back(comp);
#endif
	comp->SetEntity(this);
	comp->_Init();
}

template< typename _ComponentType >
void Entity::RemoveComponent()
{
	_ComponentType* comp = 0;
#if RTTI_TYPE == RTTI_TYPEID
	util::TypeInfo typeinfo = util::TypeInfo(typeid(_ComponentType));
	Components::iterator it = m_Components.find(typeinfo);
	if (it == m_Components.end())
	{
		std::string t(typeinfo.GetTypeName());
		LOG(VL_ERROR, "Entity::RemoveComponent: Entity does not have a component of type %s.", t.c_str());
		return;
	}
	comp = (_ComponentType*)((*it).second);
	m_Components.erase(it);
#elif RTTI_TYPE == RTTI_DYNAMIC_CAST
	for (ComponentsIter it = m_Components.begin(); it != m_Components.end(); ++it)
	{
		comp = dynamic_cast<_ComponentType*>((*it));
		if (comp)
		{
			m_Components.erase(it);
			break;
		}
	}
	if (!comp)
	{
		std::string t(util::TypeInfo(typeid(_ComponentType)).GetTypeName());
		LOG(VL_ERROR, "Entity::RemoveComponent: Entity does not have a component of type %s.", t.c_str());
		return;
	}
#endif
	comp->_Terminate();
	delete comp;
	//comp->~_ComponentType();
	//delete[] ((void*)comp);
	//free(comp);
}

template< typename _ComponentType >
bool Entity::HasComponent() const
{
#if RTTI_TYPE == RTTI_TYPEID
	return (m_Components.find(util::TypeInfo(typeid(_ComponentType))) != m_Components.end());
#elif RTTI_TYPE == RTTI_DYNAMIC_CAST
	_ComponentType* comp = 0;
	for (ComponentsIter it = m_Components.begin(); it != m_Components.end(); ++it)
	{
		comp = dynamic_cast<_ComponentType*>((*it));
		if (comp)
		{
			return true;
		}
	}
	return false;
#endif
}

template< typename _ComponentType >
_ComponentType* Entity::GetComponent() const
{
#if RTTI_TYPE == RTTI_TYPEID
	util::TypeInfo typeinfo = util::TypeInfo(typeid(_ComponentType));
	Components::const_iterator it = m_Components.find(typeinfo);
	if (it != m_Components.end())
	{
		return (_ComponentType*)((*it).second);
	}
#elif RTTI_TYPE == RTTI_DYNAMIC_CAST
	for (ComponentsConstIter it = m_Components.begin(); it != m_Components.end(); ++it)
	{
		_ComponentType* comp = dynamic_cast<_ComponentType*>((*it));
		if (comp)
		{
			return comp;
		}
	}
	util::TypeInfo typeinfo = util::TypeInfo(typeid(_ComponentType));
#endif
	std::string t(typeinfo.GetTypeName());
	LOG(VL_ERROR, "Entity::GetComponent: Entity does not have a component of type %s.", t.c_str());
	return 0;
}

}

#endif