#include "core/entitysystem/component/default/RenderComponent.h"

namespace loki
{

namespace components
{

RenderComponent::RenderComponents RenderComponent::m_RenderComponents;

RenderComponent::RenderComponent()	:
	m_Model(0)
{
	m_RenderComponents.push_back(this);
}

RenderComponent::~RenderComponent()
{
	m_RenderComponents.remove(this);
}

void RenderComponent::SetModel( graphics::Model* _Model )
{
	m_Model = _Model;
}

graphics::Model* RenderComponent::GetModel()
{
	return m_Model;
}

void RenderComponent::_HandleEvent( const LkEvent& _Event )
{

}

void RenderComponent::_Init()
{

}

void RenderComponent::_Terminate()
{

}

}

}