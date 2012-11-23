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
public:
	FreeCam();
	~FreeCam();

private:
	void Awake();
	void Update();
	void Stop();
	void Enabled();
	void Disabled();
};

}

}

}

REGISTER_COMPONENT_NAMED(::loki::components::scripts::FreeCam, "FreeCam")

#endif