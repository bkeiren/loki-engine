#include "core/graphics/Mesh.h"
#include "glew/glew.h"
#include "core/graphics/IndexBuffer.h"
#include "core/graphics/VertexBuffer.h"
#include "core/graphics/Vertex.h"

namespace loki
{

namespace graphics
{

// Cg input semantic names and values. The comment on each line provides the semantic name as it should appear in the
// Cg shader. Each value is simply the index to which Cg binds a semantic and can be used
// in the C++ application code to bind data to locations.
enum ECgSemantics
{
	ATTR0 = 0,		POSITION = 0,
	ATTR1 = 1,		BLENDWEIGHT = 1,
	ATTR2 = 2,		NORMAL = 2,
	ATTR3 = 3,		COLOR0 = 3,			DIFFUSE = 3,
	ATTR4 = 4,		COLOR1 = 4,			SPECULAR = 4,
	ATTR5 = 5,		FOGCOORD = 5,		TESSFACTOR = 5,
	ATTR6 = 6,		POINTSIZE = 6,		// NOTE: Cg shader semantic for point size is actually 'PSIZE', but can't be used here because it's typedeffed.
	ATTR7 = 7,		BLENDINDICES = 7,
	ATTR8 = 8,		TEXCOORD0 = 8,
	ATTR9 = 9,		TEXCOORD1 = 9,
	ATTR10 = 10,	TEXCOORD2 = 10,
	ATTR11 = 11,	TEXCOORD3 = 11,
	ATTR12 = 12,	TEXCOORD4 = 12,
	ATTR13 = 13,	TEXCOORD5 = 13,
	ATTR14 = 14,	TEXCOORD6 = 14,		TANGENT = 14,
	ATTR15 = 15,	TEXCOORD7 = 15,		BINORMAL = 15
};

Mesh::Mesh()	:
	m_IBO(0),
	m_VBO(0)
{
	
}

Mesh::~Mesh()
{
	delete m_IBO;
	delete m_VBO;
}

Mesh* Mesh::Create( IndexBuffer* _IBO, VertexBuffer* _VBO )
{
	assert(_IBO != 0);
	assert(_VBO != 0);

	Mesh* mesh = new Mesh();

	mesh->m_IBO = _IBO;
	mesh->m_VBO = _VBO;

	return mesh;
}

void Mesh::Draw() const
{
#define MEMBER_OFFSET(s,m) ((char *)NULL + (offsetof(s,m)))
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

	// Bind the vertices' VBO buffer.
	m_VBO->Bind();

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

	// Bind the indices' VBO buffer.
	m_IBO->Bind();

	glDrawElements(GL_TRIANGLES, m_IBO->GetNumIndices(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));

	// Unbind both buffers.
	m_IBO->Unbind();
	m_VBO->Unbind();

#undef BUFFER_OFFSET
#undef MEMBER_OFFSET
}

const IndexBuffer* Mesh::GetIndexBuffer() const
{
	return m_IBO;
}

const VertexBuffer* Mesh::GetVertexBuffer() const
{
	return m_VBO;
}

}

}
