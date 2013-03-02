#include "core/graphics/VertexArrayObject.h"
#include <cassert>
#include "core/graphics/Enums.h"
#include "core/graphics/IndexBuffer.h"
#include "core/graphics/VertexBufferObject.h"
#include "core/graphics/Vertex.h"
#include "glew/glew.h"

namespace loki
{

namespace graphics
{

VertexArrayObject::VertexArrayObject( IndexBuffer* _IndexBuffer, VertexBufferObject* _VertexBufferObject )
{
	glGenVertexArrays(1, &m_GLArrayHandle);

	Bind();

#define MEMBER_OFFSET(s,m) ((char *)NULL + (offsetof(s,m)))
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

	// Bind the VBO.
	_VertexBufferObject->Bind();

	//////////////////////////////////////////////////////////////////////////
	// IMPORTANT LIFE LESSON: glVertexAttribPointer must be called each time
	// glBindBuffer is called because it is only then that it maps
	// vertex attributes to the currently bound buffer. Fortunately, 
	// since the index and vertex buffers are attached to this vertex array
	// buffer, we only need to bind the attributes once, when we're linking
	// them together. From that point forward, whenever the vertex array is
	// bound, the attributes will be set correctly.
	//////////////////////////////////////////////////////////////////////////
	glEnableVertexAttribArray(	ATTR0);
	glVertexAttribPointer(		ATTR0,	3, GL_FLOAT, false, sizeof(Vertex), MEMBER_OFFSET(Vertex, pos));

	glEnableVertexAttribArray(	ATTR2);
	glVertexAttribPointer(		ATTR2,	3, GL_FLOAT, false, sizeof(Vertex), MEMBER_OFFSET(Vertex, normal));

	glEnableVertexAttribArray(	ATTR8);
	glVertexAttribPointer(		ATTR8,	2, GL_FLOAT, false, sizeof(Vertex), MEMBER_OFFSET(Vertex, uv));

	glEnableVertexAttribArray(	ATTR14);
	glVertexAttribPointer(		ATTR14,	3, GL_FLOAT, false, sizeof(Vertex), MEMBER_OFFSET(Vertex, tangent));

	glEnableVertexAttribArray(	ATTR15);
	glVertexAttribPointer(		ATTR15,	3, GL_FLOAT, false, sizeof(Vertex), MEMBER_OFFSET(Vertex, bitangent));
	//////////////////////////////////////////////////////////////////////////

	// Bind the IBO.
	_IndexBuffer->Bind();

#undef BUFFER_OFFSET
#undef MEMBER_OFFSET

	Unbind();
}

VertexArrayObject::~VertexArrayObject()
{
	glDeleteVertexArrays(1, &m_GLArrayHandle);
}

VertexArrayObject* VertexArrayObject::Create( IndexBuffer* _IndexBuffer, VertexBufferObject* _VertexBufferObject )
{
	assert(_IndexBuffer != 0);
	assert(_VertexBufferObject != 0);

	VertexArrayObject* va = new VertexArrayObject(_IndexBuffer, _VertexBufferObject);
	return va;
}

uint32 VertexArrayObject::GetArrayHandle() const
{
	return m_GLArrayHandle;
}

void VertexArrayObject::Bind() const
{
	glBindVertexArray(m_GLArrayHandle);
}

void VertexArrayObject::Unbind() const
{
	glBindVertexArray(0);
}

}

}
