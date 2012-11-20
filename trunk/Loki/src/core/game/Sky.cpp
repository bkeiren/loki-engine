#include "core/game/Sky.h"
#include "core/graphics/TextureCube.h"
#include "core/graphics/DisplayList.h"
#include "core/graphics/effect/EffectManager.h"
#include "core/graphics/UtilityPrimitives.h"
#include "core/entitysystem/component/default/CameraComponent.h"

namespace loki
{

namespace game
{

graphics::TextureCube* Sky::m_CubeMap = 0;
graphics::DisplayList* Sky::m_DisplayList = 0;
graphics::Effect* Sky::m_Shader = 0;

Sky::Sky()
{

}

Sky::~Sky()
{

}

void Sky::SetCubeMap( graphics::TextureCube* _CubeMap )
{
	m_CubeMap = _CubeMap;
}

graphics::TextureCube* Sky::GetCubeMap()
{
	return m_CubeMap;
}

void Sky::Render()
{
	if (!m_Shader)
	{
		m_Shader = graphics::g_EffectManager->CreateEffectFromFile(DEFAULT_RESOURCE("shaders//sky.cgfx"), "SkyShader");
	}
	if (!m_DisplayList)
	{
		m_DisplayList = graphics::DisplayList::Create(1);
		m_DisplayList->BeginList();
		graphics::DrawIcoSphere(1.0f, 2);
		m_DisplayList->EndList();
		if (!m_DisplayList->IsCompiled())
		{
			LOG(VL_ERROR, "Sky::Render: Failed to compile display list");
		}
	}

	if (!m_CubeMap || !m_Shader || !m_DisplayList)
	{
		return;
	}

	graphics::EffectParameter* Param = 0;
	Param = m_Shader->GetParameterBySemantic("LKSKYSAMPLER");
	if (Param)
	{
		Param->Set(m_CubeMap->GetTextureHandle());
	}
	Param = m_Shader->GetParameterBySemantic("LKMODELVIEWPROJ");
	if (Param)
	{
		components::CameraComponent* Camera = components::CameraComponent::GetActiveCamera();
		mat4 ViewMatrix = Camera->GetViewMatrix();
		ViewMatrix[3] = vec4(0.0f, 0.0f, 0.0f, 1.0f);	// Remove translation of view matrix.
		mat4 m = Camera->GetProjectionMatrix() * ViewMatrix;
		Param->Set(m);
	}

	while (m_Shader->HasNextPass())
	{
		m_DisplayList->Draw();
	}
}

}

}
