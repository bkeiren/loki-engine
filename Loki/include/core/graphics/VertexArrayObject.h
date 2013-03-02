#pragma once

#ifndef VertexArrayObject_H
#define VertexArrayObject_H

namespace loki
{

namespace graphics
{

class IndexBuffer;
class VertexBufferObject;

//////////////////////////////////////////////////////////////////////////
// Vertex Array Objects (VAO) are objects that speed up the use of
// index- and VertexBufferObjects. Just like index and VertexBufferObjects speed up
// passing vertex data to be rendered, a VAO 'binds' to these two buffers
// and allows us to simply bind the VAO once before each render call
// without having to rebind attribute locations etc.
// NOTE: This VertexArrayObject class expects the vertex buffer contents
// to contain data as loki::graphics::Vertex objects.
//////////////////////////////////////////////////////////////////////////
class VertexArrayObject
{
public:
	~VertexArrayObject();

	static VertexArrayObject* Create( IndexBuffer* _IndexBuffer, VertexBufferObject* _VertexBufferObject );

	uint32 GetArrayHandle() const;

	void Bind() const;
	void Unbind() const;
private:
	VertexArrayObject( IndexBuffer* _IndexBuffer, VertexBufferObject* _VertexBufferObject );
	VertexArrayObject();

	uint32 m_GLArrayHandle;
};

}

}

#endif