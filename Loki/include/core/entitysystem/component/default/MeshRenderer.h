#pragma once

#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include "core/entitysystem/component/Component.h"

namespace loki
{

namespace graphics
{
class Mesh;
class Material;
}

namespace renderer
{
class LkRenderer;
}

namespace components
{

class MeshRenderer	: public Component
{
	friend class renderer::LkRenderer;
	
	CONTAINER_MACRO_LIST(MeshRenderer*, MeshRenderers);
	
	CONTAINER_MACRO_VECTOR(graphics::Material*, Materials);
public:
	MeshRenderer();
	~MeshRenderer();

	void SetCastShadows( bool _CastShadows );
	bool CastsShadows() const;

	void SetReceiveShadows( bool _ReceiveShadows );
	bool ReceivesShadows() const;

	void SetMesh( const std::string& _MeshFile );
	const graphics::Mesh* GetMesh() const;

	void SetMaterial( const std::string& _MaterialFile, int32 _Index = 0 );
	const graphics::Material* GetMaterial( int32 _Index = 0 );
private:
	void _HandleEvent( const LkEvent& _Event );
	void _Init();
	void _Terminate();
	
	Materials m_Materials;
	bool m_CastShadows;
	bool m_ReceiveShadows;
	graphics::Mesh* m_Mesh;

	static MeshRenderers m_MeshRenderers;
};

}

}

REGISTER_COMPONENT(MeshRenderer)
//COMPONENT_SINGLE_INSTANCE(MeshRenderer)

#endif