#include "core/graphics/VertexBufferObject.h"
#include "GLEW/glew.h"
#include "core/graphics/Vertex.h"

namespace loki
{

namespace graphics
{

VertexBufferObject::VertexBufferObject()	:
	m_NumVertices(0),
	m_GLBufferHandle(0),
	m_VerticesRAM(0)
{

}

VertexBufferObject::~VertexBufferObject()
{
	if (m_GLBufferHandle != 0)
	{
		glDeleteBuffers(1, &m_GLBufferHandle);
		m_GLBufferHandle = 0;
	}
	delete[] m_VerticesRAM;
}

VertexBufferObject* VertexBufferObject::Create( Vertex* _Vertices, unsigned int _NumVertices )
{
	assert(_Vertices != NULL);
	assert(_NumVertices > 0);

	VertexBufferObject* vbo = new VertexBufferObject();

	vbo->m_NumVertices = _NumVertices;
	vbo->m_VerticesRAM = _Vertices;	// We take ownership of this data.

	// Create the Vertex-Buffer-Objects.
	glGenBuffers(1, &vbo->m_GLBufferHandle);

	// Copy the vertex data to the vertex VBO.
	glBindBuffer(GL_ARRAY_BUFFER, vbo->m_GLBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vbo->m_NumVertices, _Vertices, GL_STATIC_DRAW);	// Copy data from RAM to VRAM.

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return vbo;
}

unsigned int VertexBufferObject::GetNumVertices() const
{
	return m_NumVertices;
}

unsigned int VertexBufferObject::GetBufferHandle() const
{
	return m_GLBufferHandle;
}

const Vertex* VertexBufferObject::GetVerticesRAM() const
{
	return m_VerticesRAM;
}

}

}