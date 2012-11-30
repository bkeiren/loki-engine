#pragma once

#ifndef OCTREE_H
#define OCTREE_H

#include "core/graphics/OctreeOctant.h"

namespace loki
{

namespace graphics
{

template< class _T >
class Octree	: public OctreeOctant<_T>
{
	CONTAINER_MACRO_TEMPLATE_LIST(_T, DirtyMembers)
	friend void OctreeOctant<_T>::_Update();
public:
	// The size is the half-length of the root-quadrant in world-units.
	// Take a sufficiently large amount for this so that the entire scene fits 
	// inside of that quadrant.
	Octree( f32 _Size, uint32 _MaxMembersPerOctant );
	~Octree();

	uint32 GetMaxMembersPerOctant() const;

	void Insert( _T _Member );
	void Remove( _T _Member );

	void Update();

	void DebugDraw() const;
private:
	Octree();

	void _DebugDrawOctant( OctreeOctant<_T>* _Octant ) const;
	void _ReinsertDirtyMembers();
	void _RegisterDirtyMember( _T _Member );
	
	uint32 m_MaxMembersPerOctant;
	DirtyMembers m_DirtyMembers;
};

}

}

#include "core/graphics/Octree.inl"

#endif