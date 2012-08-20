#pragma once

#ifndef RENDERCOMPONENT_H
#define RENDERCOMPONENT_H

#include <list>
#include "core/actor/components/base/actorcomponent.h"

namespace loki
{

namespace renderer
{
class LkModel;
class LkRenderer;
}

class LkRenderComponent	: public LkActorComponent
{
	friend class renderer::LkRenderer;
public:
	LkRenderComponent();
	~LkRenderComponent();

	void SetModel( renderer::LkModel* _Model );
	renderer::LkModel* GetModel();
protected:
	void Update();

private:
	renderer::LkModel* m_Model;

	//////////////////////////////////////////////////////////////////////////
	// A list of all the render components.
	static std::list<LkRenderComponent*> m_RenderComponents;
};

}

#endif