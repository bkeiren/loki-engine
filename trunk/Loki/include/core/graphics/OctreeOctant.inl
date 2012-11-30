namespace loki
{

namespace graphics
{

template< class _T >
inline void GetOctantMemberPosition( _T _Member, vec3& _Output )
{
	LOG(VL_ERROR, "GetOctantMemberPosition: You have not specialized this function for the type of your Octree.");
}

template< class _T >
inline void GetOctantMemberBoundingBox( _T _Member, BoundingBox& _Output )
{
	LOG(VL_ERROR, "GetOctantMemberBoundingBox: You have not specialized this function for the type of your Octree.");
}

template< class _T >
inline f32 GetOctantMemberBoundingRadius( _T _member )
{
	LOG(VL_ERROR, "GetOctantMemberBoundingRadius: You have not specialized this function for the type of your Octree.");
}

template< class _T >
inline bool GetOctantMemberIsDirty( _T _member )
{
	LOG(VL_ERROR, "GetOctantMemberIsDirty: You have not specialized this function for the type of your Octree.");
}

template< class _T >
OctreeOctant<_T>::OctreeOctant()	:
	m_BoundingBox(vec3(-1.0f, -1.0f, -1.0f), vec3(1.0f, 1.0f, 1.0f))
{
	ILLEGAL_CTOR_ERROR("OctreeOctant")
}

template< class _T >
OctreeOctant<_T>::OctreeOctant( Octree<_T>* _Octree, OctreeOctant<_T>* _Parent, EOctant _PlaceInParent, const BoundingBox& _BoundingBox )	:
	m_Octree(_Octree)
	,m_Parent(_Parent)
	,m_PlaceInParent(_PlaceInParent)
	,m_BoundingBox(_BoundingBox)
	,m_HasChanged(false)
{
	assert(m_Octree != 0);

	for (uint32 i = 0; i < 8; ++i)
	{
		m_Octants[i] = 0;
	}
	m_Octants[_OCTANT_ROOT] = (OctreeOctant<_T>*)m_Octree;	// This is valid because Octree inherits from OctreeOctant and acts as root.
}

template< class _T >
OctreeOctant<_T>::~OctreeOctant()
{
// 	if (!IsRoot())
// 	{
// 		// Move all octants to the parent.
// 		for (uint32 i = 0; i < 8; ++i)
// 		{
// 			m_Parent->_StealMembersFromOctant( m_Octants[i] );
// 		}
// 	}

	for (uint32 i = 0; i < 8; ++i)
	{
		delete m_Octants[i];
	}
}

template< class _T >
OctreeOctant<_T>* OctreeOctant<_T>::GetParent() const
{
	return m_Parent;
}

template< class _T >
OctreeOctant<_T>* OctreeOctant<_T>::GetOctant( EOctant _Octant ) const
{
	return m_Octants[_Octant];
}

template< class _T >
const BoundingBox& OctreeOctant<_T>::GetBoundingBox() const
{
	return m_BoundingBox;
}

template< class _T >
bool OctreeOctant<_T>::IsRoot() const
{
	bool HasParent = m_Parent != 0;
	bool PlaceIsRoot = m_PlaceInParent == _OCTANT_ROOT;
	// If this assert fails, something went wrong because if either is true, the other must also be true.
	// The Octree class calls the OctreeOctant c-tor in it's own c-tor, passing 0 for the parent pointer
	// and _OCTANT_ROOT as the place in the parent. At NO other location in the code should _OCTANT_ROOT
	// be used or should the parent pointer be 0 when constructing an OctreeOctant.
	assert( (!HasParent && PlaceIsRoot) || (HasParent && !PlaceIsRoot) );
	return PlaceIsRoot;
}

template< class _T >
bool OctreeOctant<_T>::IsLeaf() const
{
	// If this assert fails, something went wrong because octants should either have 8 child octants or none at all.
	assert((m_Octants[0] == 0 &&
			m_Octants[1] == 0 &&
			m_Octants[2] == 0 && 
			m_Octants[3] == 0 &&
			m_Octants[4] == 0 &&
			m_Octants[5] == 0 &&
			m_Octants[6] == 0 &&
			m_Octants[7] == 0) ||
		   (m_Octants[0] != 0 &&
			m_Octants[1] != 0 &&
			m_Octants[2] != 0 && 
			m_Octants[3] != 0 &&
			m_Octants[4] != 0 &&
			m_Octants[5] != 0 &&
			m_Octants[6] != 0 &&
			m_Octants[7] != 0));
	return (m_Octants[0] == 0);
}

template< class _T >
uint32 OctreeOctant<_T>::GetMemberCount() const
{
	return m_Members.size();
}

template< class _T >
void OctreeOctant<_T>::_InsertMember( _T _Member )
{
	if (IsLeaf())
	{
		if (m_Members.size() >= m_Octree->GetMaxMembersPerOctant()/*m_Members.capacity()*/)
		{
#ifdef OCTREE_OPERATION_LOGGING
			LOG(VL_NORMAL, "OctreeOctant: Expanding octant because member size has exceeded maximum at octant %i", (int)m_PlaceInParent);
#endif
			// Create child octants.
			_CreateOctants();

			// Insert all members into the appropriate new octants.
			for (MembersIter it = m_Members.begin(); it != m_Members.end(); ++it)
			{
				_T m = (*it);
				vec3 Pos;
				GetOctantMemberPosition<_T>(m, Pos);
				m_Octants[_FindLocalOctant(Pos)]->_InsertMember(_Member);
			}
			// Clear our own list of members because they've been put into child octants.
			m_Members.clear();

			// Resize the members to 0 so we're not wasting memory on it. It's empty now anyway.
			m_Members.resize(0, 0);
		}
		else
		{
// 			if (m_Members.capacity() < m_Octree->GetMaxMembersPerOctant())
// 			{
// 				m_Members.resize(m_Octree->GetMaxMembersPerOctant(), 0);
// 			}
#ifdef OCTREE_OPERATION_LOGGING
			LOG(VL_NORMAL, "OctreeOctant: Inserting member at leaf octant %i", (int)m_PlaceInParent);
#endif
			m_Members.push_back(_Member);
		}
	}
	else
	{
		// Find which child octant _Member should be in, and call _InsertMember on that octant.
		vec3 Pos;
		GetOctantMemberPosition<_T>(_Member, Pos);
		m_Octants[_FindLocalOctant(Pos)]->_InsertMember(_Member);
	}
}

template< class _T >
void OctreeOctant<_T>::_RemoveMember( _T _Member )
{
	if (IsLeaf())
	{
#ifdef OCTREE_OPERATION_LOGGING
		LOG(VL_NORMAL, "OctreeOctant: Removing member at leaf octant %i", (int)m_PlaceInParent);
#endif
		for (MembersIter it = m_Members.begin(); it != m_Members.end(); ++it)
		{
			if ((*it) == _Member)
			{
				m_Members.erase(it);
				break;
			}
		}
	}
	else
	{
		vec3 Pos;
		GetOctantMemberPosition<_T>(_Member, Pos);
		m_Octants[_FindLocalOctant(Pos)]->_RemoveMember(_Member);
	}
}

template< class _T >
void OctreeOctant<_T>::_Update()
{
	m_HasChanged = false;
	if (IsLeaf())
	{
		// Iterate over all members and check if they are dirty.
		for (MembersIter it = m_Members.begin(); it != m_Members.end(); ++it)
		{
			_T& m = (*it);

			if (GetOctantMemberIsDirty<_T>(m))
			{
#ifdef OCTREE_OPERATION_LOGGING
				LOG(VL_NORMAL, "OctreeOctant: Detected dirty member at octant %i", (int)m_PlaceInParent);
#endif
				m_HasChanged = true;
				
				// Remove and re-insert by registering it as dirty in the octree.
				// The octree will re-insert all dirty members after updating all octants.
				m_Octree->_RegisterDirtyMember(m);

				// Erase the element and store new iterator. If the iterator points to the end now, we should exit the loop
				// or the next iteration will increment the end iterator (An invalid operation).
				it = m_Members.erase(it);
				if (it == m_Members.end())
				{
					break;
				}
			}
		}
	}
	else
	{
		for (uint32 i = 0; i < 8; ++i)
		{
			m_Octants[i]->_Update();
		}
	}
}

template< class _T >
void OctreeOctant<_T>::_CheckAndMergeOctants()
{
	if (!IsLeaf())
	{
		// We're not a leaf, so we have child octants. And if the first child octant is a leaf, all child octants must be leaves.
		if (m_Octants[0]->IsLeaf())
		{
			// Count the total number of members of the child octants. If this is less than or equal to the maximum allowed number
			// per octant, we don't need the child octants anymore and we can copy their member over to us and delete them.
			uint32 TotalMemberCount = 0;
			for (uint32 i = 0; i < 8; ++i)
			{
				TotalMemberCount += m_Octants[i]->m_Members.size();
			}

			if (TotalMemberCount >= m_Octree->GetMaxMembersPerOctant())
			{
				// _RemoveOctants automatically merges the members of the child octants with our own and cleans
				// up the child octants.
				_RemoveOctants();
			}
		}
		// We're not a leaf, so we have child octants. But the child octants are not leaves either, so we must propagate this
		// function call to them.
		else
		{
			for (uint32 i = 0; i < 8; ++i)
			{
				m_Octants[i]->_CheckAndMergeOctants();
			}
		}
	}
}

template< class _T >
void OctreeOctant<_T>::_CreateOctants()
{
#ifdef OCTREE_OPERATION_LOGGING
	LOG(VL_NORMAL, "OctreeOctant: Creating child octants at octant %i", (int)m_PlaceInParent);
#endif

	// If the current octant's bounding box ranges from 'A' to 'B' where 'C' is the center point and r equals 
	// the half-length (Since we're using cubes, this is the same for all dimensions), the 8 child octants have bounding boxes as follows:
	// 0: min: C, max: B
	// 1: min: [C.x, C.y, A.z], max: [B.x, B.y, C.z]
	// 2: min: [A.x, C.y, A.z], max: [C.x, B.y, C.z]
	// 3: min: [A.x, C.y, C.z], max: [C.x, B.y, B.z]
	// 4: min: [C.x, A.y, C.z], max: [B.x, C.y, B.z]
	// 5: min: [C.x, A.y, A.z], max: [B.x, C.y, C.z]
	// 6: min: A, max: C
	// 7: min: [A.x, A.y, C.z], max: [C.x, C.y, B.z]

	vec3 A = m_BoundingBox.GetMin();
	vec3 B = m_BoundingBox.GetMax();
	vec3 C = m_BoundingBox.GetCenter();
	vec3 min;
	vec3 max;
	for (uint32 i = 0; i < 8; ++i)
	{
		switch (i)
		{
		case 0:
			{
				min = C;
				max = B;
				break;
			}
		case 1:
			{
				min = vec3(C.x, C.y, A.z);
				max = vec3(B.x, B.y, C.z);
				break;
			}
		case 2:
			{
				min = vec3(A.x, C.y, A.z);
				max = vec3(C.x, B.y, C.z);
				break;
			}
		case 3:
			{
				min = vec3(A.x, C.y, C.z);
				max = vec3(C.x, B.y, B.z);
				break;
			}
		case 4:
			{
				min = vec3(C.x, A.y, C.z); 
				max = vec3(B.x, C.y, B.z);
				break;
			}
		case 5:
			{
				min = vec3(C.x, A.y, A.z); 
				max = vec3(B.x, C.y, C.z);
				break;
			}
		case 6:
			{
				min = A;
				max = C;
				break;
			}
		case 7:
			{
				min = vec3(A.x, A.y, C.z);
				max = vec3(C.x, C.y, B.z);
				break;
			}
		}

		m_Octants[i] = new OctreeOctant<_T>(m_Octree, this, (EOctant)i, BoundingBox(min, max));
	}
}

template< class _T >
void OctreeOctant<_T>::_RemoveOctants()
{
#ifdef OCTREE_OPERATION_LOGGING
	LOG(VL_NORMAL, "OctreeOctant: Removing child octants and merging members at octant %i", (int)m_PlaceInParent);
#endif
	for (uint32 i = 0; i < 8; ++i)
	{
		_StealMembersFromOctant(m_Octants[i]);
		delete m_Octants[i];
		m_Octants[i] = 0;
	}
}

template< class _T >
void OctreeOctant<_T>::_StealMembersFromOctant( OctreeOctant<_T>* _Octant )
{
	m_Members.splice(m_Members.begin(), _Octant->m_Members);
}

template< class _T >
typename OctreeOctant<_T>::EOctant OctreeOctant<_T>::_FindLocalOctant( const vec3& _P ) const
{
	const vec3& C = m_BoundingBox.GetCenter();
	if (_P.y < C.y)
	{
		// On the -Y side.
		if (_P.z < C.z)
		{
			// On the -Z side.
			if (_P.x < C.x)
			{
				// On the -X side.
				return OCTANT_BOTTOM_BACK_LEFT;
			}
			else
			{
				// On the +X side.
				return OCTANT_BOTTOM_BACK_RIGHT;
			}
		}
		else
		{
			// On the +Z side;
			if (_P.x < C.x)
			{
				// On the -X side.
				return OCTANT_BOTTOM_FRONT_LEFT;
			}
			else
			{
				// On the +X side.
				return OCTANT_BOTTOM_FRONT_RIGHT;
			}
		}
	}
	else
	{
		// On the +Y side.
		if (_P.z < C.x)
		{
			// On the -Z side.
			if (_P.x < C.x)
			{
				// On the -X side.
				return OCTANT_TOP_BACK_LEFT;
			}
			else
			{
				// On the +X side.
				return OCTANT_TOP_BACK_RIGHT;
			}
		}
		else
		{
			// On the +Z side;
			if (_P.x < C.x)
			{
				// On the -X side.
				return OCTANT_TOP_FRONT_LEFT;
			}
			else
			{
				// On the +X side.
				// We handle this case at the bottom so that the function always returns something.
				// If any of the other cases was handled, the function would have already returned. Since it
				// reached this part of the code it will reach the end and return the appropriate value.
			}
		}
	}

	return OCTANT_TOP_FRONT_RIGHT;
}

template< class _T >
bool OctreeOctant<_T>::_HasChanged() const
{
	return m_HasChanged;
}

}

}