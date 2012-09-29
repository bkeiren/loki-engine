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

	static VertexBufferObject* Create( Vertex* _Vertices, unsigned int _NumVertices );

	unsigned int GetNumVertices() const;
	unsigned int GetBufferHandle() const;
	const Vertex* GetVerticesRAM() const;
private:
	VertexBufferObject();

	unsigned int m_NumVertices;
	unsigned int m_GLBufferHandle;
	Vertex* m_VerticesRAM;
};

}

}

#endif