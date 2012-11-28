#pragma once

#ifndef OCTREE_OCTANT_H
#define OCTREE_OCTANT_H

#include "core/boundingbox/BoundingBox.h"

namespace loki
{

class BoundingBox;

namespace graphics
{

class OctreeOctant
{
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

	OctreeOctant* GetParent() const;
	OctreeOctant* GetOctant( EOctant _Octant ) const;
	const BoundingBox& GetBoundingBox() const;

	bool IsRoot() const;
	bool IsLeaf() const;
protected:
	OctreeOctant( OctreeOctant* _Parent, EOctant _PlaceInParent, const BoundingBox& _BoundingBox );
	~OctreeOctant();

private:
	OctreeOctant();

	void _CreateOctants();
	void _CopyContentsFromOctant( OctreeOctant* _Octant );

	union
	{
		struct { OctreeOctant* m_Octants[8]; };
		struct { OctreeOctant* m_Octant_TopFrontRight;
				 OctreeOctant* m_Octant_TopBackRight;
				 OctreeOctant* m_Octant_TopBackLeft;
				 OctreeOctant* m_Octant_TopFrontLeft;
				 OctreeOctant* m_Octant_BottomFrontRight;
				 OctreeOctant* m_Octant_BottomBackRight;
				 OctreeOctant* m_Octant_BottomBackLeft;
				 OctreeOctant* m_Octant_BottomFrontLeft; };
	};

	OctreeOctant* m_Parent;
	EOctant m_PlaceInParent;

	BoundingBox m_BoundingBox;
};

}

}

#endif