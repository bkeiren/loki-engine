#pragma once

#ifndef VERTEX_H
#define VERTEX_H

namespace loki
{

namespace renderer
{

//////////////////////////////////////////////////////////////////////////
// Vertex struct with:
// Position
// Texture coordinates
// Normal
// Tangent
// Binormal
//////////////////////////////////////////////////////////////////////////
struct LkVertex
{
	vec3 pos;
	vec2 uv;
	vec3 normal;
	vec3 tangent;
	vec3 binormal;
};

}

}

#endif