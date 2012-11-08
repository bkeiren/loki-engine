namespace loki
{

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