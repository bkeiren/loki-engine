namespace loki
{

namespace components
{

template< class _T >
typename System<_T>::Components System<_T>::m_Components;

template< class _T >
const typename System<_T>::Components& System<_T>::GetComponents()
{
	return m_Components;
}

template< class _T >
void System<_T>::_AddComponent( _T* _Component )
{
	m_Components.push_back((void*)_Component);
}

template< class _T >
void System<_T>::_RemoveComponent( _T* _Component )
{
	m_Components.remove((void*)_Component);
}

}

}