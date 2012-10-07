#pragma once

#ifndef MESH_H
#define MESH_H

namespace loki
{

namespace graphics
{

class IndexBuffer;
class VertexBuffer;
class VertexArray;

class Mesh
{
public:
	~Mesh();

	static Mesh* Create( IndexBuffer* _IBO, VertexBuffer* _VBO );

	//////////////////////////////////////////////////////////////////////////
	// Makes the draw calls required to draw the mesh. Shaders and materials
	// and other settings must be set before calling this.
	//////////////////////////////////////////////////////////////////////////
	void Draw() const;

private:
	Mesh();

	IndexBuffer* m_IBO;
	VertexBuffer* m_VBO;
	VertexArray* m_VAO;
};

}

}

#endif