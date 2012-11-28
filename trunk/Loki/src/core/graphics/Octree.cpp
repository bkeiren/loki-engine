#include "core/graphics/Octree.h"

namespace loki
{

namespace graphics
{

Octree::Octree()	:
	OctreeOctant(0, OctreeOctant::_OCTANT_ROOT, BoundingBox(0.5f))
{
	ILLEGAL_CTOR_ERROR("Octree")
}

Octree::Octree( f32 _Size )	:
	OctreeOctant(0, OctreeOctant::_OCTANT_ROOT, BoundingBox(_Size * 0.5f))
{

}

Octree::~Octree()
{

}

}

}
