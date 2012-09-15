#include "core/actor/camera/trackballcam/trackballcam.h"
#include "core/input/input.h"

namespace loki
{

LkCamera* CameraFactoryTrackBallCam( const char* _Name, game::LkLevel* _Level )
{
	return new LkTrackBallCam(_Name, _Level);
}

LkTrackBallCam::LkTrackBallCam( const char* _Name, game::LkLevel* _Level )	:
	LkCamera(_Name, _Level),
	m_Distance(5.0f),
	m_Center(vec3(0.0f, 0.0f, 0.0f))
{
	SubscribeToEvent(EVENT_ONUPDATE);
}

LkTrackBallCam::LkTrackBallCam()
{
	ILLEGAL_CTOR_ERROR("TrackBallCam");
}

LkTrackBallCam::~LkTrackBallCam()
{

}

void LkTrackBallCam::SetTrackBallDistance( float _Distance )
{
	m_Distance = _Distance;
}

float LkTrackBallCam::GetTrackBallDistance() const
{
	return m_Distance;
}

void LkTrackBallCam::SetTrackBallCenter( const vec3& _Center )
{
	m_Center = _Center;
}

const vec3& LkTrackBallCam::GetTrackBallCenter() const
{
	return m_Center;
}

void LkTrackBallCam::_OnEvent( const LkEvent& _Event )
{
	LkCamera::_OnEvent(_Event);

	switch (_Event.GetEventType())
	{
	case EVENT_ONUPDATE:
		{
			loki::LkMovableComponent* movcomp = GetComponent<LkMovableComponent>();

			movcomp->SetPosition(m_Center);

			if (g_Input->Get(BUTTON_MOUSELEFT) == KEYSTATE_DOWN)
			{
				int2 mouseDelta = -g_Input->GetMouseDelta();
				movcomp->RotateX((float)mouseDelta.y / 3);
				movcomp->RotateLocalY((float)mouseDelta.x / 3);
			}

			SetTrackBallDistance(GetTrackBallDistance() - (g_Input->GetMouseWheelDelta() * 0.6f));

			// As with the freecam, not sure why I need to use SIDE instead of FORWARD here.
			// I'm guessing it has something to do with OpenGL pointing the camera down the Z-axis by default, while
			// the X-axis is used as the forward axis in this engine.
			movcomp->SetPosition(m_Center + ((movcomp->GetOrientation() * SIDE) * m_Distance));

			break;
		}
	}
}

}