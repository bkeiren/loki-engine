#pragma once

#ifndef ORBITCAM_H
#define ORBITCAM_H

#include "core/entitysystem/component/default/CppScript.h"

namespace loki
{

namespace components
{

class CameraComponent;

namespace scripts
{

class OrbitCam	: public components::CppScript
{
public:
	DECLARE_COMPONENT(OrbitCam)

	OrbitCam();
	~OrbitCam();

	float GetDistance() const;
	void SetDistance( float _Distance );

	const vec3& GetCenter() const;
	void SetCenter( const vec3& _Center );
private:
	void Awake();
	void Update();
	void Stop();
	void Enabled();
	void Disabled();

	vec3 m_Center;
	float m_Distance;

	loki::components::CameraComponent* m_Camera;
};

}

}

}

#endif