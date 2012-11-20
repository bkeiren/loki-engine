#pragma once

#ifndef MESH_H
#define MESH_H

namespace loki
{

namespace renderer
{
	class LkRenderer;
}

namespace graphics
{

class SubMesh;

class Mesh
{
	friend class renderer::LkRenderer;
	CONTAINER_MACRO_HASH_MAP(std::string, Mesh*, Meshes)
public:
	CONTAINER_MACRO_VECTOR(SubMesh*, SubMeshes)

	static Mesh* LoadMesh( const std::string& _GeometryFile );

	const SubMesh* GetSubMesh( int32 _Index ) const;

	uint32 GetSubMeshCount() const;
private:
	Mesh( const SubMeshes& _SubMeshes );
	Mesh();
	~Mesh();

	static Mesh* _CreateMeshFromGeometryFile( const std::string& _GeometryFile );
	static Mesh* _FindMesh( const std::string& _MeshFile );

	SubMeshes m_SubMeshes;

	static Meshes m_Meshes;
};

}

}

#endif