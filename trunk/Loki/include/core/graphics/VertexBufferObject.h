#pragma once

#ifndef VertexBufferObjectOBJECT_H
#define VertexBufferObjectOBJECT_H

#include "core/graphics/Buffer.h"

namespace loki
{

namespace graphics
{

struct Vertex;

class VertexBufferObject	: public Buffer
{
public:
	~VertexBufferObject();

	static VertexBufferObject* Create( Vertex* _Vertices, uint32 _NumVertices );

	uint32 GetNumVertices() const;
	const Vertex* GetVerticesRAM() const;

private:
	VertexBufferObject();

	uint32 m_NumVertices;
	Vertex* m_VerticesRAM;
};

}

}

#endif