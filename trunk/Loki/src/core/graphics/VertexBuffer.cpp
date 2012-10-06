#include "core/graphics/VertexBuffer.h"
#include "GLEW/glew.h"
#include "core/graphics/Vertex.h"

namespace loki
{

namespace graphics
{

VertexBuffer::VertexBuffer()	:
	Buffer(Buffer::BUFFER_TARGET_ARRAY_BUFFER),
	m_NumVertices(0),
	m_VerticesRAM(0)
{

}

VertexBuffer::~VertexBuffer()
{
	delete[] m_VerticesRAM;
}

VertexBuffer* VertexBuffer::Create( Vertex* _Vertices, uint32 _NumVertices )
{
	assert(_Vertices != NULL);
	assert(_NumVertices > 0);

	VertexBuffer* vbo = new VertexBuffer();

	vbo->m_NumVertices = _NumVertices;
	vbo->m_VerticesRAM = _Vertices;	// We take ownership of this data.

	vbo->UploadData(sizeof(Vertex) * vbo->m_NumVertices, _Vertices, Buffer::BUFFER_USAGE_STATIC_DRAW);

	return vbo;
}

uint32 VertexBuffer::GetNumVertices() const
{
	return m_NumVertices;
}

const Vertex* VertexBuffer::GetVerticesRAM() const
{
	return m_VerticesRAM;
}

}

}