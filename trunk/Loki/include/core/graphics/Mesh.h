#pragma once

#ifndef MESH_H
#define MESH_H

namespace loki
{

namespace graphics
{

class IndexBufferObject;
class VertexBufferObject;

class Mesh
{
public:
	~Mesh();

	static Mesh* Create( IndexBufferObject* _IBO, VertexBufferObject* _VBO );

	//////////////////////////////////////////////////////////////////////////
	// Makes the draw calls required to draw the mesh. Shaders and materials
	// and other settings must be set before calling this.
	//////////////////////////////////////////////////////////////////////////
	void Draw() const;

	const IndexBufferObject* GetIndexBufferObject() const;
	const VertexBufferObject* GetVertexBufferObject() const;
private:
	Mesh();

	IndexBufferObject* m_IBO;
	VertexBufferObject* m_VBO;
};

}

}

#endif