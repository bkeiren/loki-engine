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
	glm::vec3 pos;
	glm::vec2 uv;
	glm::vec3 normal;
	glm::vec3 tangent;
	glm::vec3 binormal;
};

}

}

#endif