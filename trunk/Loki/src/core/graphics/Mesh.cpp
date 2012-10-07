#include "core/graphics/Mesh.h"
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

Mesh::Mesh()	:
	m_IBO(0),
	m_VBO(0),
	m_VAO(0)
{
	
}

Mesh::~Mesh()
{
	delete m_IBO;
	delete m_VBO;
	delete m_VAO;
}

Mesh* Mesh::Create( IndexBuffer* _IBO, VertexBuffer* _VBO )
{
	assert(_IBO != 0);
	assert(_VBO != 0);

	Mesh* mesh = new Mesh();

	mesh->m_VAO = VertexArray::Create(_IBO, _VBO);

	return mesh;
}

void Mesh::Draw() const
{
#define MEMBER_OFFSET(s,m) ((char *)NULL + (offsetof(s,m)))
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

	m_VAO->Bind();

	glDrawElements(GL_TRIANGLES, m_IBO->GetNumIndices(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));

	m_VAO->Unbind();

#undef BUFFER_OFFSET
#undef MEMBER_OFFSET
}

}

}
