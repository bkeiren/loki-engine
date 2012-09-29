#pragma once

#ifndef MESH_H
#define MESH_H

struct aiScene;

namespace loki
{

namespace graphics
{
	class Mesh;
}

namespace renderer
{

//////////////////////////////////////////////////////////////////////////
// The Mesh class represents a group of meshes that are defined in
// a single file. A Model instance contains one instance of a Mesh.
// With this system, Model's can use the same mesh group data (to avoid
// duplication of meshes) but still have different materials associated
// with them.
//////////////////////////////////////////////////////////////////////////
class LkMesh
{
	friend class LkModel;
public:
	unsigned int GetNumSubMeshes() const;
	const graphics::Mesh* GetSubMesh( unsigned int _Index ) const;
private:
	LkMesh( const aiScene* _aiScene );
	LkMesh();
	~LkMesh();

	graphics::Mesh** m_SubMeshes;
	unsigned int m_NumSubMeshes;
};

}

}

#endif

