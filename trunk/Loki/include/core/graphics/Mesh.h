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

private:
	IndexBufferObject* m_IndexBufferObject;
	VertexBufferObject* m_VertexBufferObject;
};

}

}

#endif