#include "core/entitysystem/component/default/MeshRenderer.h"
#include "core/graphics/Mesh.h"
#include "core/graphics/Material.h"

namespace loki
{

namespace components
{

MeshRenderer::RenderComponents MeshRenderer::m_RenderComponents;

MeshRenderer::MeshRenderer()	:
	m_CastShadows(false),
	m_ReceiveShadows(false)
{
	m_RenderComponents.push_back(this);
}

MeshRenderer::~MeshRenderer()
{
	m_RenderComponents.remove(this);
}

void MeshRenderer::SetCastShadows( bool _CastShadows )
{
	m_CastShadows = _CastShadows;
}

bool MeshRenderer::CastsShadows() const
{
	return m_CastShadows;
}

void MeshRenderer::SetReceiveShadows( bool _ReceiveShadows )
{
	m_ReceiveShadows = _ReceiveShadows;
}

bool MeshRenderer::ReceivesShadows() const
{
	return m_ReceiveShadows;
}

void MeshRenderer::SetMesh( const std::string& _MeshFile )
{
	m_Mesh = graphics::Mesh::LoadMesh(_MeshFile);

	m_Materials.resize(m_Mesh->GetSubMeshCount(), 0);
}

const graphics::Mesh* MeshRenderer::GetMesh() const
{
	return m_Mesh;
}

void MeshRenderer::SetMaterial( const std::string& _MaterialFile, int32 _Index /*= 0*/ )
{
	m_Materials[_Index] = graphics::Material::LoadMaterial(_MaterialFile);
}

const graphics::Material* MeshRenderer::GetMaterial( int32 _Index /*= 0*/ )
{
	return m_Materials[_Index];
}

void MeshRenderer::_HandleEvent( const LkEvent& _Event )
{

}

void MeshRenderer::_Init()
{

}

void MeshRenderer::_Terminate()
{

}

}

}