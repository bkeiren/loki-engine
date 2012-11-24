#include "core/entitysystem/Entity.h"
#include "core/entitysystem/component/Component.h"
#include "core/entitysystem/component/default/Transform.h"

namespace loki
{

EntityID::EntityID()	:
	m_ID(0)
{

}

bool EntityID::operator == ( const EntityID& _ID ) const
{
	return (m_ID == _ID.m_ID);
}

bool EntityID::operator != ( const EntityID& _ID ) const
{
	return (m_ID != _ID.m_ID);
}

bool EntityID::operator < ( const EntityID& _ID ) const
{
	return (m_ID < _ID.m_ID);
}

bool EntityID::operator > ( const EntityID& _ID ) const
{
	return (m_ID > _ID.m_ID);
}

bool EntityID::operator <= ( const EntityID& _ID ) const
{
	return (m_ID <= _ID.m_ID);
}

bool EntityID::operator >= ( const EntityID& _ID ) const
{
	return (m_ID >= _ID.m_ID);
}

Entity::Entity()	:
	m_Transform(0)
	,m_Camera(0)
	,m_Light(0)
	,m_Renderer(0)
{
	// An entity always has a transform component.
	InstantiateComponent<Transform>();
}

Entity::~Entity()
{
	_ClearComponents();
}

EntityID Entity::GetID() const
{
	return m_EntityID;
}

const std::string& Entity::GetName() const
{
	return m_Name;
}

const Transform& Entity::GetTransform() const
{
	return *m_Transform;
}

Transform& Entity::GetTransform()
{
	// NOTE: Post event to check translation in scene management system?
	// There is also a version of GetTransform which returns a const reference, so hopefully the compiler will use that version
	// in places where the transform is only read and not modified. That will ensure that if we only want to read the transform
	// we won't place an unnecessary event for this entity.
	return *m_Transform;
}

const components::CameraComponent* Entity::GetCamera() const
{
	return m_Camera;
}

components::CameraComponent* Entity::GetCamera()
{
	return m_Camera;
}

const components::Light* Entity::GetLight() const
{
	return m_Light;
}

components::Light* Entity::GetLight()
{
	return m_Light;
}

const components::MeshRenderer* Entity::GetRenderer() const
{
	return m_Renderer;
}

components::MeshRenderer* Entity::GetRenderer()
{
	return m_Renderer;
}

void Entity::SetName( const char* _Name )
{
	m_Name = std::string(_Name);
}

void Entity::SetID( EntityID _ID )
{
	m_EntityID = _ID;
}

Component* Entity::_CreateComponentByTypeName( const std::string& _Name )
{
	::loki::components::detail::ComponentRegistry& _Registry = ::loki::components::detail::GetComponentRegistry();
	::loki::components::detail::ComponentRegistryIter it = _Registry.find(_Name);

	if (it == _Registry.end())
	{
		return 0;
	}

	::loki::components::detail::CreateComponentFunction _Function = it->second.m_Function;
	return _Function();
}

void Entity::_ClearComponents()
{
	for (Components::iterator it = m_Components.begin(); it != m_Components.end(); ++it)
	{
		ComponentsVector Vector = (*it).second;
		for (ComponentsVectorIter it2 = Vector.begin(); it2 != Vector.end(); ++it2)
		{
			delete (*it2);
		}
		Vector.clear();
	}
	m_Components.clear();
}

}