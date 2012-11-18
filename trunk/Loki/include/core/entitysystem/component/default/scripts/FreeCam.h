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
	DECLARE_COMPONENT(FreeCam);

	FreeCam();
	~FreeCam();

private:
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

#endif