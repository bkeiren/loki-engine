#pragma once

#ifndef COMPONENT_H
#define COMPONENT_H

#include "core/eventsystem/eventlistener/eventlistener.h"
#include "util/typeinfo/typeinfo.h"
#include "core/entitysystem/component/Detail.h"

namespace loki
{

class Transform;

namespace components
{
	class CameraComponent;
	class Light;

	//////////////////////////////////////////////////////////////////////////
	// These following functions are specialized for components registered
	// using the REGISTER_COMPONENT() macro.
	//////////////////////////////////////////////////////////////////////////
	template< class _T >
	inline const std::string& GetComponentTypeName();

	template< class _T >
	inline const ::loki::util::general::TypeInfo& GetComponentTypeInfo();

	template< class _T >
	inline bool IsComponentRegistered();

	//////////////////////////////////////////////////////////////////////////
	// This functions is specialized for components using the 
	// COMPONENT_SINGLE_INSTANCE() macro.
	//////////////////////////////////////////////////////////////////////////
	template< class _T >
	inline bool ComponentAllowsMultipleInstancesOnEntity();
}

#define REGISTER_COMPONENT_NAMED(TYPE, NAME)																							\
	namespace loki {																													\
		namespace components {																											\
			template<>																													\
			inline const std::string& GetComponentTypeName<TYPE>()																		\
			{																															\
				static const std::string _Name = std::string(NAME); return _Name;														\
			}																															\
			template<>																													\
			inline const ::loki::util::general::TypeInfo& GetComponentTypeInfo<TYPE>()													\
			{																															\
				static const ::loki::util::general::TypeInfo _TI = ::loki::util::general::TypeInfo(typeid(TYPE)); return _TI;			\
			}																															\
			template<>																													\
			inline bool IsComponentRegistered<TYPE>()																					\
			{																															\
				return true;																											\
			}																															\
			namespace detail {																											\
				namespace {																												\
					template< class _T >																								\
					class ComponentRegistration;																						\
					template<>																											\
					class ComponentRegistration<TYPE>																					\
					{																													\
						static const ::loki::components::detail::RegistryEntryHelper<TYPE>& _Reg;										\
					};																													\
					const ::loki::components::detail::RegistryEntryHelper<TYPE>&														\
						ComponentRegistration<TYPE>::_Reg =																				\
							::loki::components::detail::RegistryEntryHelper<TYPE>::Instance(NAME);										\
				}																														\
			}																															\
		}																																\
	}

#define REGISTER_COMPONENT(TYPE)	REGISTER_COMPONENT_NAMED(TYPE, #TYPE)

#define COMPONENT_SINGLE_INSTANCE(TYPE)															\
	namespace loki {																			\
		namespace components {																	\
			template<>																			\
			inline bool ComponentAllowsMultipleInstancesOnEntity<TYPE>()						\
			{																					\
				return false;																	\
			}																					\
		}																						\
	}

class Entity;

//////////////////////////////////////////////////////////////////////////
// The Component class is the base class for any components that 
// an entity could be composed of.
// Examples for component types are: PhysicsComponent, MeshRenderer.
//////////////////////////////////////////////////////////////////////////
class Component	: public LkEventListener
{
	friend class Entity;
public:
	inline Entity* GetEntity();
	inline const Entity* GetEntity() const;
	
	bool IsEnabled() const;
	void Enable();
	void Disable();
	void SetEnabled( bool _Enabled );

	const Transform& GetTransform() const;
	Transform& GetTransform();
protected:
	Component();
	virtual ~Component() = 0;

	//////////////////////////////////////////////////////////////////////////
	// Utility functions that simply call GetEntity() and then GetComponent(). These are just here to 
	// make it easier to find components attached to the same entity.
	//////////////////////////////////////////////////////////////////////////
	template< typename _ComponentType >
	_ComponentType* GetComponent() const;

private:
	void SetEntity( Entity* _Entity );

	void _OnEvent( const LkEvent& _Event );
	virtual void _HandleEvent( const LkEvent& _Event );

	//////////////////////////////////////////////////////////////////////////
	// Called after data such as m_Entity is set.
	//////////////////////////////////////////////////////////////////////////
	void _BaseInit();
	virtual void _Init() = 0;

	//////////////////////////////////////////////////////////////////////////
	// Called before the component is deleted.
	//////////////////////////////////////////////////////////////////////////
	void _BaseTerminate();
	virtual void _Terminate() = 0;

	//////////////////////////////////////////////////////////////////////////
	// The entity to which this component belongs.
	//////////////////////////////////////////////////////////////////////////
	Entity* m_Entity;

	bool m_Enabled;
};

}

#include "core/entitysystem/component/Component.inl"

#endif