#pragma once

#ifndef VERTEXBUFFEROBJECT_H
#define VERTEXBUFFEROBJECT_H

namespace loki
{

namespace graphics
{

struct Vertex;

class VertexBufferObject
{
public:
	~VertexBufferObject();

	static VertexBufferObject* Create( Vertex* _Vertices, uint32 _NumVertices );

	uint32 GetNumVertices() const;
	uint32 GetBufferHandle() const;
	const Vertex* GetVerticesRAM() const;
private:
	VertexBufferObject();

	uint32 m_NumVertices;
	uint32 m_GLBufferHandle;
	Vertex* m_VerticesRAM;
};

}

}

#endif