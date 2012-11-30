#pragma once

#ifndef OCTREE_OCTANT_H
#define OCTREE_OCTANT_H

#include "core/boundingbox/BoundingBox.h"
#include "core/entitysystem/component/default/MeshRenderer.h"

#define OCTREE_OPERATION_LOGGING

namespace loki
{

class BoundingBox;

namespace graphics
{

//////////////////////////////////////////////////////////////////////////
// These three functions are to be specialized for Octree types.
// They are used by the OctreeOctant class to obtain data that is required
// to properly position members in the octree.
//////////////////////////////////////////////////////////////////////////
template< class _T >
inline void GetOctantMemberPosition( _T _Member, vec3& _Output );

template< class _T >
inline void GetOctantMemberBoundingBox( _T _Member, BoundingBox& _Output );

template< class _T >
inline f32 GetOctantMemberBoundingRadius( _T _member );

template< class _T >
inline bool GetOctantMemberIsDirty( _T _Member );
//////////////////////////////////////////////////////////////////////////


template< class _T >
class Octree;

template< class _T >
class OctreeOctant
{
	CONTAINER_MACRO_TEMPLATE_LIST(_T, Members)
public:
	enum EOctant 
	{
		OCTANT_TOP_FRONT_RIGHT = 0,
		OCTANT_TOP_BACK_RIGHT,
		OCTANT_TOP_BACK_LEFT,
		OCTANT_TOP_FRONT_LEFT,
		OCTANT_BOTTOM_FRONT_RIGHT,
		OCTANT_BOTTOM_BACK_RIGHT,
		OCTANT_BOTTOM_BACK_LEFT,
		OCTANT_BOTTOM_FRONT_LEFT,

		_OCTANT_ROOT
	};

	OctreeOctant<_T>* GetParent() const;
	OctreeOctant<_T>* GetOctant( EOctant _Octant ) const;
	const BoundingBox& GetBoundingBox() const;

	bool IsRoot() const;
	bool IsLeaf() const;

	uint32 GetMemberCount() const;
protected:
	OctreeOctant( Octree<_T>* _Octree, OctreeOctant<_T>* _Parent, EOctant _PlaceInParent, const BoundingBox& _BoundingBox );
	~OctreeOctant();

	//////////////////////////////////////////////////////////////////////////
	// This function is called by other OctreeOctants in order to insert a member.
	// If the octant has already reached it's capacity, it will call 
	// this function recursively on the appropriate child octant. This process
	// will repeat until an octant accepts the member because it has not reached it's capacity yet.
	// NOTE: Only leaf octants will accept members. Other octants will NOT
	// contain any members. If an octant is a leaf and at capacity, 
	// it will create child octants and transfer all of it's members
	// to the appropriate child octants. This results in 8 new leafs, the old one
	// 'downgrading' to a regular node without any members but with 8 child octants.
	//////////////////////////////////////////////////////////////////////////
	void _InsertMember( _T _Member );
	void _RemoveMember( _T _Member );

	void _Update();

	//////////////////////////////////////////////////////////////////////////
	// This function checks the number of members in the current octant
	// in order to clean up and merge octanst if required. The algorithm
	// works as follows:
	// 
	//////////////////////////////////////////////////////////////////////////
	void _CheckAndMergeOctants();
private:
	OctreeOctant();

	void _CreateOctants();
	void _RemoveOctants();	// This also merges the members of the child octants with our own.
	void _StealMembersFromOctant( OctreeOctant<_T>* _Octant );

	//////////////////////////////////////////////////////////////////////////
	// Finds in which octant a point _P lies. Does not check outer bounds.
	// This means that if a point lies outside of the octant's entire space,
	// it still counts as being in that octant.
	//////////////////////////////////////////////////////////////////////////
	EOctant _FindLocalOctant( const vec3& _P ) const;

	bool _HasChanged() const;

	union
	{
		struct { OctreeOctant<_T>* m_Octants[9]; };
		struct { OctreeOctant<_T>* m_Octant_TopFrontRight;
				 OctreeOctant<_T>* m_Octant_TopBackRight;
				 OctreeOctant<_T>* m_Octant_TopBackLeft;
				 OctreeOctant<_T>* m_Octant_TopFrontLeft;
				 OctreeOctant<_T>* m_Octant_BottomFrontRight;
				 OctreeOctant<_T>* m_Octant_BottomBackRight;
				 OctreeOctant<_T>* m_Octant_BottomBackLeft;
				 OctreeOctant<_T>* m_Octant_BottomFrontLeft;
				 OctreeOctant<_T>* m_Octant_Root; };
	};

	Octree<_T>* m_Octree;
	OctreeOctant<_T>* m_Parent;
	EOctant m_PlaceInParent;

	BoundingBox m_BoundingBox;

	Members m_Members;
	bool m_HasChanged;
};

}

}

#include "core/graphics/OctreeOctant.inl"

#endif