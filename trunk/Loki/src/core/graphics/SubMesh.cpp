#include "core/graphics/SubMesh.h"
#include "glew/glew.h"
#include "core/graphics/IndexBuffer.h"
#include "core/graphics/VertexBuffer.h"
#include "core/graphics/Vertex.h"
#include "core/graphics/Enums.h"
#include "core/graphics/VertexArray.h"

namespace loki
{

namespace graphics
{

SubMesh::SubMesh()	:
	m_IBO(0),
	m_VBO(0),
	m_VAO(0)
{
	
}

SubMesh::~SubMesh()
{
	delete m_IBO;
	delete m_VBO;
	delete m_VAO;
}

SubMesh* SubMesh::Create( IndexBuffer* _IBO, VertexBuffer* _VBO )
{
	assert(_IBO != 0);
	assert(_VBO != 0);

	SubMesh* mesh = new SubMesh();

	mesh->m_IBO = _IBO;	// We take ownership of the index buffer and 
	mesh->m_VBO = _VBO;	// the vertex buffer.
	mesh->m_VAO = VertexArray::Create(_IBO, _VBO);

	return mesh;
}

void SubMesh::Draw() const
{
#define MEMBER_OFFSET(s,m) ((char *)NULL + (offsetof(s,m)))
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

	m_VAO->Bind();

	glDrawElements(GL_TRIANGLES, m_IBO->GetNumIndices(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));

	m_VAO->Unbind();

#undef BUFFER_OFFSET
#undef MEMBER_OFFSET
}

const IndexBuffer* SubMesh::GetIBO() const
{
	return m_IBO;
}

const VertexBuffer* SubMesh::GetVBO() const
{
	return m_VBO;
}

const VertexArray* SubMesh::GetVAO() const
{
	return m_VAO;
}

}

}
