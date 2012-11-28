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
private:
	Octree();
	
	void _UpdateOctant( OctreeOctant<_T>* _Octant );
	
	uint32 m_MaxMembersPerOctant;
};

}

}

#include "core/graphics/Octree.inl"

#endif