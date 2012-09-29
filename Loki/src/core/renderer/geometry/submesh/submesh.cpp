#include "core/renderer/geometry/submesh/submesh.h"
#include "core/renderer/geometry/vertex/vertex.h"

#include "core/graphics/IndexBufferObject.h"
#include "core/graphics/VertexBufferObject.h"

using namespace loki;
using namespace renderer;

// Cg input semantic names and values. THe comment on each line provides the semantic name as it should appear in the
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

LkSubMesh::LkSubMesh()	:
	m_NumIndices(0),
	m_NumVertices(0),
	m_IndicesVBO(0),
	m_VerticesVBO(0),
	m_Indices(0),
	m_Vertices(0),
	m_IBO(0),
	m_VBO(0)
{

}

LkSubMesh::~LkSubMesh()
{
	if (m_VerticesVBO != 0)
	{
		glDeleteBuffers(1, &m_VerticesVBO);
		m_VerticesVBO = 0;
	}
	if (m_IndicesVBO != 0)
	{
		glDeleteBuffers(1, &m_IndicesVBO);
		m_IndicesVBO = 0;
	}
	delete[] m_Indices;
	delete[] m_Vertices;

	delete m_IBO;
	delete m_VBO;
}

unsigned int LkSubMesh::GetNumIndices() const
{
	return m_IBO->GetNumIndices();
	//return m_NumIndices;
}

unsigned int LkSubMesh::GetNumVertices() const
{
	return m_VBO->GetNumVertices();
	//return m_NumVertices;
}

GLuint LkSubMesh::GetIndicesVBO() const
{
	return m_IBO->GetBufferHandle();
	//return m_IndicesVBO;
}

GLuint LkSubMesh::GetVerticesVBO() const
{
	return m_VBO->GetBufferHandle();
	//return m_VerticesVBO;
}

const unsigned int* LkSubMesh::GetIndicesRAM() const
{
	return m_IBO->GetIndicesRAM();
	//return m_Indices;
}

const LkVertex* LkSubMesh::GetVerticesRAM() const
{
	return (LkVertex*)m_VBO->GetVerticesRAM();
	//return m_Vertices;
}

void LkSubMesh::Draw() const
{
#define MEMBER_OFFSET(s,m) ((char *)NULL + (offsetof(s,m)))
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

	// Bind the vertices' VBO buffer.
	glBindBuffer(GL_ARRAY_BUFFER, GetVerticesVBO());

	//////////////////////////////////////////////////////////////////////////
	// IMPORTANT LIFE LESSON: glVertexAttribPointer must be called each time
	// glBindBuffer is called because it is only then that it maps
	// vertex attributes to the currently bound buffer.
	//////////////////////////////////////////////////////////////////////////
	glEnableVertexAttribArray(	ATTR0);
	glVertexAttribPointer(		ATTR0,	3, GL_FLOAT, false, sizeof(LkVertex), MEMBER_OFFSET(LkVertex, pos));

	glEnableVertexAttribArray(	ATTR2);
	glVertexAttribPointer(		ATTR2,	3, GL_FLOAT, false, sizeof(LkVertex), MEMBER_OFFSET(LkVertex, normal));

	glEnableVertexAttribArray(	ATTR8);
	glVertexAttribPointer(		ATTR8,	2, GL_FLOAT, false, sizeof(LkVertex), MEMBER_OFFSET(LkVertex, uv));

	glEnableVertexAttribArray(	ATTR14);
	glVertexAttribPointer(		ATTR14,	3, GL_FLOAT, false, sizeof(LkVertex), MEMBER_OFFSET(LkVertex, tangent));

	glEnableVertexAttribArray(	ATTR15);
	glVertexAttribPointer(		ATTR15,	3, GL_FLOAT, false, sizeof(LkVertex), MEMBER_OFFSET(LkVertex, binormal));
	//////////////////////////////////////////////////////////////////////////

	// Bind the indices' VBO buffer.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GetIndicesVBO());

	glDrawElements(GL_TRIANGLES, GetNumIndices(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));

	// Unbind both buffers.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

#undef BUFFER_OFFSET
#undef MEMBER_OFFSET
}

bool LkSubMesh::_CreateIndexBuffer( unsigned int* _Indices, unsigned int _NumIndices )
{
	m_IBO = graphics::IndexBufferObject::Create(_Indices, _NumIndices);

// 	assert(_Indices != NULL);
// 	assert(_NumIndices >= 3);
// 
// 	m_NumIndices = _NumIndices;
// 	m_Indices = _Indices;	// We take ownership of this data.
// 
// 	// Create the Index-Buffer-Object.
// 	glGenBuffers(1, &m_IndicesVBO);
// 
// 	// Copy the index data to the index VBO.
// 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndicesVBO);
// 	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_NumIndices, _Indices, GL_STATIC_DRAW);	// Copy data from RAM to VRAM.
// 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return true;
}

bool LkSubMesh::_CreateVertexBuffer( LkVertex* _Vertices, unsigned int _NumVertices )
{
	m_VBO = graphics::VertexBufferObject::Create((graphics::Vertex*)_Vertices, _NumVertices);

// 	assert(_Vertices != NULL);
// 	assert(_NumVertices > 0);
// 
// 	m_NumVertices = _NumVertices;
// 	m_Vertices = _Vertices;	// We take ownership of this data.
// 	
// 	// Create the Vertex-Buffer-Objects.
// 	glGenBuffers(1, &m_VerticesVBO);
// 
// 	// Copy the vertex data to the vertex VBO.
// 	glBindBuffer(GL_ARRAY_BUFFER, m_VerticesVBO);
// 	glBufferData(GL_ARRAY_BUFFER, sizeof(LkVertex) * m_NumVertices, _Vertices, GL_STATIC_DRAW);	// Copy data from RAM to VRAM.
// 
// 	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return true;
}