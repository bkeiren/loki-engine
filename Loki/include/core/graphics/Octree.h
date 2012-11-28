#pragma once

#ifndef OCTREE_H
#define OCTREE_H

#include "core/graphics/OctreeOctant.h"

namespace loki
{

namespace graphics
{

class Octree	: public OctreeOctant
{

public:
	// The size is the half-length of the root-quadrant in world-units.
	// Take a sufficiently large amount for this so that the entire scene fits 
	// inside of that quadrant.
	Octree( f32 _Size );
	~Octree();

private:
	Octree();
	
};

}

}

#endif