#include "core/actor/components/rendercomponent/rendercomponent.h"

namespace loki
{

std::list<LkRenderComponent*> LkRenderComponent::m_RenderComponents;

LkRenderComponent::LkRenderComponent()	:
	m_Model(NULL)
{
	m_RenderComponents.push_back(this);
}

LkRenderComponent::~LkRenderComponent()
{
	m_RenderComponents.remove(this);
}

void LkRenderComponent::Update()
{

}

void LkRenderComponent::SetModel( renderer::LkModel* _Model )
{
	m_Model = _Model;
}

renderer::LkModel* LkRenderComponent::GetModel()
{
	return m_Model;
}

}