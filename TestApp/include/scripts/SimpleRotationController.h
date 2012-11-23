#pragma once

#ifndef SIMPLEROTATIONCONTROLLER_H
#define SIMPLEROTATIONCONTROLLER_H

#include "core/entitysystem/component/default/CppScript.h"
#include "core/input/input.h"

using namespace loki;

class SimpleRotationController	: public components::CppScript
{
public:
	SimpleRotationController()
	{

	}

	~SimpleRotationController()
	{

	}

	void SetRotationVector( const vec3& _Vector )
	{
		m_RotationVector = math::normalize(_Vector);
	}

	void SetVector( const vec3& _Vector )
	{
		m_Vector = _Vector;
	}
private:
	void Awake()
	{
		m_Vector = vec3(4.0f, 0.0f, 0.0f);
		m_RotationVector = vec3(0.0f, 1.0f, 0.0f);
	}

	void Update()
	{
		Transform& t = GetEntity()->GetTransform();

		m_Vector = math::rotate(m_Vector, 2.0f, m_RotationVector);
		t.SetPosition(m_Vector);
	}

	void Stop()
	{

	}

	vec3 m_Vector;
	vec3 m_RotationVector;
};

REGISTER_COMPONENT(SimpleRotationController)

#endif