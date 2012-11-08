#include "core/entitysystem/component/default/scripts/FreeCam.h"
#include "core/entitysystem/component/default/CameraComponent.h"
#include "core/entitysystem/Entity.h"

#include "core/html/htmlcore.h"
#include "core/input/input.h"

namespace loki
{

namespace components
{

namespace scripts
{

FreeCam::FreeCam()
{

}

FreeCam::~FreeCam()
{

}

void FreeCam::Awake()
{
	m_Camera = GetEntity()->GetComponent<loki::components::CameraComponent>();
	if (!m_Camera)
	{
		LOG(VL_WARN, "FreeCam::Awake: Entity does not have a camera component attached!");
	}
}

void FreeCam::Update()
{
	if (!g_HTMLCore->GetInputDetected())
	{
		if (g_Input->Get('T') == KEYSTATE_DOWN)
		{
			m_Camera->LookAt(vec3(0.0f, 0.0f, 0.0f));
		}

		// 			LkHTMLView* tab = g_HTMLCore->GetWebTabInFocus();
		// 			if (g_Input->Get(BUTTON_MOUSELEFT) == KEYSTATE_DOWN && (tab && tab->GetAlphaAtCursor() == 0.0f))
		// 			{
		// 				int2 mouseDelta = -g_Input->GetMouseDelta();
		// 				movcomp->RotateX((f32)mouseDelta.y / 3);
		// 				movcomp->RotateLocalY((f32)mouseDelta.x / 3);
		// 			}

		Transform& t = GetTransform();

		if (g_Input->Get(BUTTON_MOUSELEFT))
		{
			int2 mouseDelta = -g_Input->GetMouseDelta();
			t.RotateX((f32)mouseDelta.y / 3);
			t.LocalRotateY((f32)mouseDelta.x / 3);
		}

		static f32 camSpeed = 0.3f;
		camSpeed = max(camSpeed + (g_Input->GetMouseWheelDelta() * 0.25f), 0.1f);

		t.LocalTranslate( -FORWARD * ((((bool)g_Input->Get('W')) * camSpeed) - (((bool)g_Input->Get('S')) * camSpeed)) );
		t.LocalTranslate( SIDE * ((((bool)g_Input->Get('D')) * camSpeed) - (((bool)g_Input->Get('A')) * camSpeed)) );
		t.Translate( UP * ((((bool)g_Input->Get('X')) * camSpeed) - (((bool)g_Input->Get('Z')) * camSpeed)) );	
	}
}

void FreeCam::Stop()
{

}

void FreeCam::Enabled()
{

}

void FreeCam::Disabled()
{

}

}

}

}