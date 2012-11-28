#include "core/graphics/OctreeOctant.h"
#include "core/boundingbox/BoundingBox.h"

namespace loki
{

namespace graphics
{

OctreeOctant::OctreeOctant()	:
	m_BoundingBox(vec3(-1.0f, -1.0f, -1.0f), vec3(1.0f, 1.0f, 1.0f))
{
	ILLEGAL_CTOR_ERROR("OctreeOctant")
}

OctreeOctant::OctreeOctant( OctreeOctant* _Parent, EOctant _PlaceInParent, const BoundingBox& _BoundingBox )	:
	 m_Parent(_Parent)
	,m_PlaceInParent(_PlaceInParent)
	,m_BoundingBox(_BoundingBox)
{
	for (uint32 i = 0; i < 8; ++i)
	{
		m_Octants[i] = 0;
	}
}

OctreeOctant::~OctreeOctant()
{
	if (!IsRoot())
	{
		// Move all octants to the parent.
		for (uint32 i = 0; i < 8; ++i)
		{
			m_Parent->_CopyContentsFromOctant( m_Octants[i] );
		}
	}

	for (uint32 i = 0; i < 8; ++i)
	{
		delete m_Octants[i];
	}
}

OctreeOctant* OctreeOctant::GetParent() const
{
	return m_Parent;
}

OctreeOctant* OctreeOctant::GetOctant( EOctant _Octant ) const
{
	return m_Octants[_Octant];
}

const BoundingBox& OctreeOctant::GetBoundingBox() const
{
	return m_BoundingBox;
}

bool OctreeOctant::IsRoot() const
{
	bool HasParent = m_Parent == 0;
	bool PlaceIsRoot = m_PlaceInParent == _OCTANT_ROOT;
	// If this assert fails, something went wrong because if either is true, the other must also be true.
	// The Octree class calls the OctreeOctant c-tor in it's own c-tor, passing 0 for the parent pointer
	// and _OCTANT_ROOT as the place in the parent. At NO other location in the code should _OCTANT_ROOT
	// be used or should the parent pointer be 0 when constructing an OctreeOctant.
	assert( (!HasParent && PlaceIsRoot) || (HasParent && !PlaceIsRoot) );
	return PlaceIsRoot;
}

bool OctreeOctant::IsLeaf() const
{
	// If this assert fails, something went wrong because octants should either have 8 child octants or none at all.
	assert( (m_Octants[0] == 0 &&
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
			 m_Octants[7] != 0) );
	return (m_Octants[0] == 0);
}

void OctreeOctant::_CreateOctants()
{
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

		m_Octants[i] = new OctreeOctant(this, (EOctant)i, BoundingBox(min, max));
	}
}

void OctreeOctant::_CopyContentsFromOctant( OctreeOctant* _Octant )
{
	// Copy contents from _Octant over to this.
}

}

}
