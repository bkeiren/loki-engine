#pragma once

#ifndef COMPONENTSYSTEM_H
#define COMPONENTSYSTEM_H

namespace loki
{

namespace components
{

template< class _T >
class System
{
	friend class Entity;
public:
	CONTAINER_MACRO_LIST(void*, Components)	// Void* instead of _T because we can't create lists with 
											// abstract classes such as Component (apparently).
	
	static const Components& GetComponents();
private:
	static void _AddComponent( _T* _Component );
	static void _RemoveComponent( _T* _Component );

	static Components m_Components;
};

}

}

#include "core/entitysystem/component/system/System.inl"

#endif