#pragma once

#ifndef SIMPLECONTROLLER_H
#define SIMPLECONTROLLER_H

#include "core/entitysystem/component/default/CppScript.h"
#include "core/input/input.h"

using namespace loki;

class SimpleController	: public components::CppScript
{
public:
	DECLARE_COMPONENT_TYPEINFO(SimpleController);

	SimpleController()
	{

	}

	~SimpleController()
	{

	}

private:

	void Awake()
	{
		m_AutoRotate = false;
		m_RotateAxes = math::bool3(true, true, true);
	}

	void Update()
	{
		Transform& t = GetEntity()->GetTransform();

		if (g_Input->Get(KEY_R))
		{
			t.SetPosition(vec3(0.0f, 0.0f, 0.0f));
			t.SetOrientation(quat(1.0f, 0.0f, 0.0f, 0.0f));
		}
		else
		{
			if (g_Input->IsReleased(KEY_T))
			{
				m_AutoRotate = !m_AutoRotate;
			}

			if (g_Input->IsReleased('1'))
			{
				m_RotateAxes.x = !m_RotateAxes.x;
			}
			if (g_Input->IsReleased('2'))
			{
				m_RotateAxes.y = !m_RotateAxes.y;
			}
			if (g_Input->IsReleased('3'))
			{
				m_RotateAxes.z = !m_RotateAxes.z;
			}

			if (m_AutoRotate)
			{
				float RotateSpeed = 1.0f;
				if (m_RotateAxes.x) t.RotateX(RotateSpeed);
				if (m_RotateAxes.y) t.RotateY(RotateSpeed);
				if (m_RotateAxes.z) t.RotateZ(RotateSpeed);
			}
			
			vec3 v = vec3(0.0f, 0.0f, 0.0f);
			if (g_Input->Get(KEY_ARROWUP))
			{
				v.z = 1.0f;
			}
			else if (g_Input->Get(KEY_ARROWDOWN))
			{
				v.z = -1.0f;
			}
			if (g_Input->Get(KEY_ARROWLEFT))
			{
				v.x = 1.0f;
			}
			else if (g_Input->Get(KEY_ARROWRIGHT))
			{
				v.x = -1.0f;
			}
			if (g_Input->Get(KEY_RSHIFT))
			{
				v.y = 1.0f;
			}
			else if (g_Input->Get(KEY_RCONTROL))
			{
				v.y = -1.0f;
			}
			//t.Translate(v);
			t.position += v;
		}
	}

	void Stop()
	{

	}
	
	bool m_AutoRotate;
	math::bool3 m_RotateAxes;
};

#endif