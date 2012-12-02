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
	OctreeOctant<_T>::_Update();
	_ReinsertDirtyMembers();

	// Clean up any unused octants.
	_CheckAndMergeOctants();
}

template< class _T >
void Octree<_T>::DebugDraw() const
{
	_DebugDrawOctant((OctreeOctant<_T>*)this);
}

template< class _T >
void Octree<_T>::_DebugDrawOctant( OctreeOctant<_T>* _Octant ) const
{
	if (_Octant->IsLeaf())
	{
		const BoundingBox& bb = _Octant->GetBoundingBox();
		renderer::debug::DrawCube(bb.GetCenter(), bb.GetMax().x - bb.GetMin().x, true, vec3(1.0f, 1.0f, 1.0f), true);
	}
	else
	{
		for (uint32 i = 0; i < 8; ++i)
		{
			_DebugDrawOctant(_Octant->GetOctant((OctreeOctant<_T>::EOctant)i));
		}
	}
}

template< class _T >
void Octree<_T>::_ReinsertDirtyMembers()
{
	for (DirtyMembersIter it = m_DirtyMembers.begin(); it != m_DirtyMembers.end(); ++it)
	{
		Insert((*it));
	}
	m_DirtyMembers.clear();
}

template< class _T >
void Octree<_T>::_RegisterDirtyMember( _T _Member )
{
	m_DirtyMembers.push_back(_Member);
}

}

}