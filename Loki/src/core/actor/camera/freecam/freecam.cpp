#include "core/actor/camera/freecam/freecam.h"
#include "core/html/htmlcore.h"
#include "core/input/input.h"

namespace loki
{

LkCamera* CameraFactoryFreeCam( const char* _Name, game::LkLevel* _Level )
{
	return new LkFreeCam(_Name, _Level);
}

LkFreeCam::LkFreeCam( const char* _Name, game::LkLevel* _Level )	:	
	LkCamera(_Name, _Level)
{
	SubscribeToEvent(EVENT_ONUPDATE);
}

LkFreeCam::LkFreeCam()
{
	ILLEGAL_CTOR_ERROR("FreeCam");
}

LkFreeCam::~LkFreeCam()
{

}

void LkFreeCam::_OnEvent( const LkEvent& _Event )
{
	LkCamera::_OnEvent(_Event);

	switch (_Event.GetEventType())
	{
	//////////////////////////////////////////////////////////////////////////
	// On frame update, we want to update the camera position and orientation.
	//////////////////////////////////////////////////////////////////////////
	case EVENT_ONUPDATE:
		{
			if (!g_HTMLCore->GetInputDetected())
			{
				if (g_Input->Get(KEY_T) == KEYSTATE_DOWN)
				{
					LookAt(vec3(0.0f, 0.0f, 0.0f));
				}

				loki::LkMovableComponent* movcomp = GetComponent<LkMovableComponent>();

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
					movcomp->RotateX((f32)mouseDelta.y / 3);
					movcomp->RotateLocalY((f32)mouseDelta.x / 3);
				}

				static f32 camSpeed = 0.3f;
				camSpeed = max(camSpeed + (g_Input->GetMouseWheelDelta() * 0.25f), 0.1f);

				// Not sure why SIDE and FORWARD need to be switched around here...
				// I'm guessing it has something to do with OpenGL pointing the camera down the Z-axis by default, while
				// the X-axis is used as the forward axis in this engine.
				movcomp->TranslateLocal( -SIDE * ((((bool)g_Input->Get(KEY_W)) * camSpeed) - (((bool)g_Input->Get(KEY_S)) * camSpeed)) );
				movcomp->TranslateLocal( -FORWARD * ((((bool)g_Input->Get(KEY_A)) * camSpeed) - (((bool)g_Input->Get(KEY_D)) * camSpeed)) );
				movcomp->TranslateLocal( -UP * ((((bool)g_Input->Get(KEY_Z)) * camSpeed) - (((bool)g_Input->Get(KEY_X)) * camSpeed)) );	
			}
						
			break;
		}
	}
}

}