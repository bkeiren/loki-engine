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

}