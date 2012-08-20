#pragma once

#ifndef TRIANGLE_H
#define TRIANGLE_H

namespace loki
{

namespace renderer
{

/*
	The Triangle class stores vertex indices that map to elements in an array of
	Vertex objects.
*/
struct LkTriangle
{
	union
	{
		struct { unsigned int v0; unsigned int v1; unsigned int v2; };
		struct { unsigned int verts[3]; };
	};
};

}

}

#endif