namespace loki
{

template< typename _ComponentType >
ComponentSystem<_ComponentType>::ComponentSystem()
{

}

template< typename _ComponentType >
ComponentSystem<_ComponentType>::~ComponentSystem()
{

}

template< typename _ComponentType >
void ComponentSystem<_ComponentType>::UpdateComponents()
{
	for (ComponentsConstIter it = m_Components.begin(); it != m_Components.end(); ++it)
	{
		
	}
}

template< typename _ComponentType >
void ComponentSystem<_ComponentType>::RegisterComponent( _ComponentType* _Component )
{

}

template< typename _ComponentType >
void ComponentSystem<_ComponentType>::UnregisterComponent( _ComponentType* _Component )
{

}

}