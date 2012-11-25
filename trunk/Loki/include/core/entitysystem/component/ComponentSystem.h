#pragma once

#ifndef COMPONENT_SYSTEM_H
#define COMPONENT_SYSTEM_H

namespace loki
{

template< typename _ComponentType >
class ComponentSystem
{
	CONTAINER_MACRO_TEMPLATE_LIST(_ComponentType*, Components)
public:
	ComponentSystem();
	~ComponentSystem();

	static void UpdateComponents();

	static void RegisterComponent( _ComponentType* _Component );
	static void UnregisterComponent( _ComponentType* _Component );
private:
	Components m_Components;
};

}

#include "core/entitysystem/component/ComponentSystem.inl"

#endif
