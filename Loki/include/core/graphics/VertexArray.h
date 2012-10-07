#pragma once

#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

namespace loki
{

namespace graphics
{

class IndexBuffer;
class VertexBuffer;

//////////////////////////////////////////////////////////////////////////
// Vertex Array Objects (VAO) are objects that speed up the use of
// index- and vertexbuffers. Just like index and vertexbuffers speed up
// passing vertex data to be rendered, a VAO 'binds' to these two buffers
// and allows us to simply bind the VAO once before each render call
// without having to rebind attribute locations etc.
// NOTE: This VertexArray class expects the vertex buffer contents
// to contain data as loki::graphics::Vertex objects.
//////////////////////////////////////////////////////////////////////////
class VertexArray
{
public:
	~VertexArray();

	static VertexArray* Create( IndexBuffer* _IndexBuffer, VertexBuffer* _VertexBuffer );

	uint32 GetArrayHandle() const;

	void Bind() const;
	void Unbind() const;
private:
	VertexArray( IndexBuffer* _IndexBuffer, VertexBuffer* _VertexBuffer );
	VertexArray();

	uint32 m_GLArrayHandle;
};

}

}

#endif