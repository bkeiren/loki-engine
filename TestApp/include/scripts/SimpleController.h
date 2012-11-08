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

	}

	void Update()
	{
		Transform& t = GetEntity()->GetTransform();

		if (g_Input->Get(KEY_R))
		{
			t.SetPosition(vec3(0.0f, 0.0f, 0.0f));
		}
		else
		{
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
			t.Translate(v);
		}
	}

	void Stop()
	{

	}
};

#endif