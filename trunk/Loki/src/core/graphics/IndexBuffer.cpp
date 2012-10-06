#include "core/graphics/IndexBuffer.h"
#include "GLEW/glew.h"

namespace loki
{

namespace graphics
{

IndexBuffer::IndexBuffer()	:
	Buffer(Buffer::BUFFER_TARGET_ELEMENT_ARRAY_BUFFER),
	m_NumIndices(0),
	m_IndicesRAM(0)
{

}

IndexBuffer::~IndexBuffer()
{
	delete[] m_IndicesRAM;
}

IndexBuffer* IndexBuffer::Create( uint32* _Indices, uint32 _NumIndices )
{
	assert(_Indices != NULL);
	assert(_NumIndices >= 3);

	IndexBuffer* ibo = new IndexBuffer();

	ibo->m_NumIndices = _NumIndices;
	ibo->m_IndicesRAM = _Indices;	// We take ownership of this data.

	ibo->_UploadData(sizeof(uint32) * ibo->m_NumIndices, _Indices, Buffer::BUFFER_USAGE_STATIC_DRAW);

	return ibo;
}

uint32 IndexBuffer::GetNumIndices() const
{
	return m_NumIndices;
}
const uint32* IndexBuffer::GetIndicesRAM() const
{
	return m_IndicesRAM;
}

}

}