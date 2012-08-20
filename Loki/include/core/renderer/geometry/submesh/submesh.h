#pragma once

#ifndef SUBMESH_H
#define SUBMESH_H

#include <GLEW\\glew.h>

namespace loki
{

namespace renderer
{

struct LkVertex;

class LkSubMesh
{
	friend class LkMesh;
public:

	unsigned int GetNumIndices() const;
	unsigned int GetNumVertices() const;
	GLuint GetIndicesVBO() const;
	GLuint GetVerticesVBO() const;
	const unsigned int* GetIndicesRAM() const;
	const LkVertex* GetVerticesRAM() const;

	void Draw() const;
private:
	bool _CreateIndexBuffer( unsigned int* _Indices, unsigned int _NumIndices );
	bool _CreateVertexBuffer( LkVertex* _Vertices, unsigned int _NumVertices );
	
	LkSubMesh();
	~LkSubMesh();

	unsigned int m_NumIndices;
	unsigned int m_NumVertices;

	GLuint m_IndicesVBO;
	GLuint m_VerticesVBO;

	// Data in main RAM instead of GPU memory. Is used to access the source data even when it is located on the GPU.
	unsigned int* m_Indices;
	LkVertex* m_Vertices;
};

}

}

#endif