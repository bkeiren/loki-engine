#include "core/entitysystem/component/default/scripts/FreeCam.h"
#include "core/entitysystem/component/default/CameraComponent.h"
#include "core/entitysystem/Entity.h"

#include "core/html/htmlcore.h"
#include "core/input/input.h"

namespace loki
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
	m_Camera = GetEntity()->GetComponent<components::CameraComponent>();
	if (!m_Camera)
	{
		LOG(VL_ERROR, "FreeCam::Awake: Entity does not have a camera component attached!");
	}
}

void FreeCam::Update()
{
	if (!g_HTMLCore->GetInputDetected())
	{
		if (g_Input->Get(KEY_T) == KEYSTATE_DOWN)
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

		if (g_Input->Get(BUTTON_MOUSELEFT))
		{
			int2 mouseDelta = -g_Input->GetMouseDelta();
			m_Camera->GetEntity()->GetTransform().RotateX(-(f32)mouseDelta.y / 3);
			m_Camera->GetEntity()->GetTransform().LocalRotateY(-(f32)mouseDelta.x / 3);
		}

		static f32 camSpeed = 0.3f;
		camSpeed = max(camSpeed + (g_Input->GetMouseWheelDelta() * 0.25f), 0.1f);

		m_Camera->GetEntity()->GetTransform().LocalTranslate( FORWARD * ((((bool)g_Input->Get(KEY_W)) * camSpeed) - (((bool)g_Input->Get(KEY_S)) * camSpeed)) );
		m_Camera->GetEntity()->GetTransform().LocalTranslate( SIDE * ((((bool)g_Input->Get(KEY_D)) * camSpeed) - (((bool)g_Input->Get(KEY_A)) * camSpeed)) );
		m_Camera->GetEntity()->GetTransform().Translate( UP * ((((bool)g_Input->Get(KEY_X)) * camSpeed) - (((bool)g_Input->Get(KEY_Z)) * camSpeed)) );	
	}
}

void FreeCam::Stop()
{

}

}

}