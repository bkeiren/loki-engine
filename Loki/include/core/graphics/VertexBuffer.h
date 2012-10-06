#pragma once

#ifndef VERTEXBUFFEROBJECT_H
#define VERTEXBUFFEROBJECT_H

#include "core/graphics/Buffer.h"

namespace loki
{

namespace graphics
{

struct Vertex;

class VertexBuffer	: public Buffer
{
public:
	~VertexBuffer();

	static VertexBuffer* Create( Vertex* _Vertices, uint32 _NumVertices );

	uint32 GetNumVertices() const;
	const Vertex* GetVerticesRAM() const;

private:
	VertexBuffer();

	uint32 m_NumVertices;
	Vertex* m_VerticesRAM;
};

}

}

#endif