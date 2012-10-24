#pragma once

#ifndef RENDERCOMPONENT_H
#define RENDERCOMPONENT_H

#include "core/entitysystem/component/Component.h"

namespace loki
{

namespace graphics
{
class Model;
}

namespace renderer
{
class LkRenderer;
}

namespace components
{

class RenderComponent	: public Component
{
	friend class renderer::LkRenderer;
	
	CONTAINER_MACRO_LIST(RenderComponent*, RenderComponents);
public:
	DECLARE_COMPONENT_TYPEINFO(RenderComponent)		// Required!

	RenderComponent();
	~RenderComponent();

	void SetModel( graphics::Model* _Model );
	graphics::Model* GetModel();
private:
	void _OnEvent( const LkEvent& _Event );
	void _Init();
	void _Terminate();
	
	graphics::Model* m_Model;

	static RenderComponents m_RenderComponents;
};

}

}

#endif