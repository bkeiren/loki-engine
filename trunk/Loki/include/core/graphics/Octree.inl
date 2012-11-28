namespace loki
{

namespace graphics
{

template< class _T >
Octree<_T>::Octree()	:
	OctreeOctant<_T>(this, 0, OctreeOctant::_OCTANT_ROOT, BoundingBox(0.5f))
{
	ILLEGAL_CTOR_ERROR("Octree")
}

template< class _T >
Octree<_T>::Octree( f32 _Size, uint32 _MaxMembersPerOctant )	:
	OctreeOctant<_T>(this, 0, OctreeOctant::_OCTANT_ROOT, BoundingBox(_Size * 0.5f))
	,m_MaxMembersPerOctant(_MaxMembersPerOctant)
{

}

template< class _T >
Octree<_T>::~Octree()
{

}

template< class _T >
uint32 Octree<_T>::GetMaxMembersPerOctant() const
{
	return m_MaxMembersPerOctant;
}

template< class _T >
void Octree<_T>::Insert( _T _Member )
{
	if (_Member == 0)
	{
		return;
	}
	_InsertMember(_Member);
}

template< class _T >
void Octree<_T>::Remove( _T _Member )
{
	if (_Member == 0)
	{
		return;
	}
	_RemoveMember(_Member);
}

template< class _T >
void Octree<_T>::Update()
{

}

template< class _T >
void Octree<_T>::_UpdateOctant( OctreeOctant<_T>* _Octant )
{
	if (_Octant->IsLeaf())
	{
		// Iterate over all members and check if they are dirty.
		for (OctreeOctant<_T>::Members it = _Octant->m_Members.begin(); it != _Octant->m_Members.end(); ++it)
		{
			_T& m = (*it);

			if (GetOctantMemberIsDirty<_T>(m))
			{
				// Remove and re-insert.
				_Octant->_RemoveMember(m);
				Insert(m);
			}
		}
	}
	else
	{
		for (uint32 i = 0; i < 8; ++i)
		{
			_UpdateOctant(_Octant->GetOctant((OctreeOctant<_T>::EOctant)i));
		}
	}
}

}

}