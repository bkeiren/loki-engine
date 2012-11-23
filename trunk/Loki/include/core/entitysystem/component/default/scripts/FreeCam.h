#pragma once

#ifndef FREECAM_H
#define FREECAM_H

#include "core/entitysystem/component/default/CppScript.h"

namespace loki
{

namespace components
{

class CameraComponent;

namespace scripts
{

class FreeCam	: public components::CppScript
{
	friend class ::loki::Entity;
public:
private:
	FreeCam();
	~FreeCam();

	void Awake();
	void Update();
	void Stop();
	void Enabled();
	void Disabled();
	
	loki::components::CameraComponent* m_Camera;
};

}

}

}

REGISTER_COMPONENT_NAMED(::loki::components::scripts::FreeCam, "FreeCam")

#endif