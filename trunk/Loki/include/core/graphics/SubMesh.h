#pragma once

#ifndef SUBMESH_H
#define SUBMESH_H

namespace loki
{

namespace graphics
{

class IndexBuffer;
class VertexBufferObject;
class VertexArrayObject;

class SubMesh
{
public:
	~SubMesh();

	static SubMesh* Create( IndexBuffer* _IBO, VertexBufferObject* _VBO );

	//////////////////////////////////////////////////////////////////////////
	// Makes the draw calls required to draw the mesh. Shaders and materials
	// and other settings must be set before calling this.
	//////////////////////////////////////////////////////////////////////////
	void Draw() const;

	const IndexBuffer* GetIBO() const;
	const VertexBufferObject* GetVBO() const;
	const VertexArrayObject* GetVAO() const;
private:
	SubMesh();

	IndexBuffer* m_IBO;
	VertexBufferObject* m_VBO;
	VertexArrayObject* m_VAO;
};

}

}

#endif