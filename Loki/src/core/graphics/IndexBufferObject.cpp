#include "core/graphics/IndexBufferObject.h"
#include "GLEW/glew.h"

namespace loki
{

namespace graphics
{

IndexBufferObject::IndexBufferObject()	:
	m_NumIndices(0),
	m_GLBufferHandle(0),
	m_IndicesRAM(0)
{

}

IndexBufferObject::~IndexBufferObject()
{
	if (m_GLBufferHandle != 0)
	{
		glDeleteBuffers(1, &m_GLBufferHandle);
		m_GLBufferHandle = 0;
	}
	delete[] m_IndicesRAM;
}

IndexBufferObject* IndexBufferObject::Create( unsigned int* _Indices, unsigned int _NumIndices )
{
	assert(_Indices != NULL);
	assert(_NumIndices >= 3);

	IndexBufferObject* ibo = new IndexBufferObject();

	ibo->m_NumIndices = _NumIndices;
	ibo->m_IndicesRAM = _Indices;	// We take ownership of this data.

	// Create the Index-Buffer-Object.
	glGenBuffers(1, &ibo->m_GLBufferHandle);

	// Copy the index data to the index VBO.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo->m_GLBufferHandle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * ibo->m_NumIndices, _Indices, GL_STATIC_DRAW);	// Copy data from RAM to VRAM.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return ibo;
}

unsigned int IndexBufferObject::GetNumIndices() const
{
	return m_NumIndices;
}

unsigned int IndexBufferObject::GetBufferHandle() const
{
	return m_GLBufferHandle;
}

const unsigned int* IndexBufferObject::GetIndicesRAM() const
{
	return m_IndicesRAM;
}

}

}