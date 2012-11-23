#pragma once

#ifndef COMPONENT_H
#define COMPONENT_H

#include "core/eventsystem/eventlistener/eventlistener.h"
#include "util/typeinfo/typeinfo.h"

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

	//////////////////////////////////////////////////////////////////////////
	// This functions is specialized for components using the 
	// COMPONENT_SINGLE_INSTANCE() macro.
	//////////////////////////////////////////////////////////////////////////
	template< class _T >
	inline bool ComponentAllowsMultipleInstancesOnEntity();
}

#define REGISTER_COMPONENT_NAMED(componentclass, name)																					\
	namespace loki {																													\
		namespace components {																											\
			template<>																													\
			inline const std::string& GetComponentTypeName<componentclass>()															\
			{																															\
				static const std::string _Name = std::string(name); return _Name;														\
			}																															\
			template<>																													\
			inline const ::loki::util::general::TypeInfo& GetComponentTypeInfo<componentclass>()										\
			{																															\
				static const ::loki::util::general::TypeInfo _TI = ::loki::util::general::TypeInfo(typeid(componentclass)); return _TI; \
			}																															\
		}																																\
	}

#define REGISTER_COMPONENT(componentclass)	REGISTER_COMPONENT_NAMED(componentclass, #componentclass)

#define COMPONENT_SINGLE_INSTANCE(componentclass)												\
	namespace loki {																			\
		namespace components {																	\
			template<>																			\
			inline bool ComponentAllowsMultipleInstancesOnEntity<componentclass>()				\
			{																					\
				return false;																	\
			}																					\
		}																						\
	}

class Entity;

//////////////////////////////////////////////////////////////////////////
// The Component class is the base class for any components that 
// an entity could be composed of.
// Examples for component types are: PhysicsComponent, RenderComponent,
// MoveableComponent.
// !!!	IMPLEMENTATION NOTE:
//		In order to speed up type-info generation, some awesome stuff
//		is done using a macro and static member functions and such.
//		When making your own component, be sure to put the 
//		DECLARE_COMPONENT macro in the public field of the class,
//		passing your component class type as argument.
//		Without this macro, the class will yield compiler errors telling you
//		that 'GetTypeInfo' is not a member of your class!
//		Example:
//		class MyComponent : public Component
//		{
//		public:
//			DECLARE_COMPONENT
//		};
//		Note that you should NOT use this macro in any custom 'base' component
//		classes. For instance, say you want to have a base component class
//		to provide functionality that a number of additional child components
//		would use. In that case you would only use this macro on the child classes
//		because it's of no use to do so on the base class (Although it should not
//		provide errors just be safe and try to avoid it ;) ).
// !!!
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
protected:
	Component();
	virtual ~Component() = 0;

	//////////////////////////////////////////////////////////////////////////
	// Utility functions that simply call GetEntity() and then GetComponent(). These are just here to 
	// make it easier to find components attached to the same entity.
	//////////////////////////////////////////////////////////////////////////
	template< typename _ComponentType >
	_ComponentType* GetComponent() const;

	const Transform& GetTransform() const;
	Transform& GetTransform();
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