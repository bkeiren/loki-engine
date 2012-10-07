#include "core/graphics/VertexArray.h"
#include <cassert>
#include "core/graphics/Enums.h"
#include "core/graphics/IndexBuffer.h"
#include "core/graphics/VertexBuffer.h"
#include "core/graphics/Vertex.h"
#include "glew/glew.h"

namespace loki
{

namespace graphics
{

VertexArray::VertexArray( IndexBuffer* _IndexBuffer, VertexBuffer* _VertexBuffer )
{
	glGenVertexArrays(1, &m_GLArrayHandle);

	Bind();

#define MEMBER_OFFSET(s,m) ((char *)NULL + (offsetof(s,m)))
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

	// Bind the VBO.
	_VertexBuffer->Bind();

	//////////////////////////////////////////////////////////////////////////
	// IMPORTANT LIFE LESSON: glVertexAttribPointer must be called each time
	// glBindBuffer is called because it is only then that it maps
	// vertex attributes to the currently bound buffer.
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
	glVertexAttribPointer(		ATTR15,	3, GL_FLOAT, false, sizeof(Vertex), MEMBER_OFFSET(Vertex, binormal));
	//////////////////////////////////////////////////////////////////////////

	// Bind the IBO.
	_IndexBuffer->Bind();

#undef BUFFER_OFFSET
#undef MEMBER_OFFSET

	Unbind();
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_GLArrayHandle);
}

VertexArray* VertexArray::Create( IndexBuffer* _IndexBuffer, VertexBuffer* _VertexBuffer )
{
	assert(_IndexBuffer != 0);
	assert(_VertexBuffer != 0);

	VertexArray* va = new VertexArray(_IndexBuffer, _VertexBuffer);
	return va;
}

uint32 VertexArray::GetArrayHandle() const
{
	return m_GLArrayHandle;
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_GLArrayHandle);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}

}

}
