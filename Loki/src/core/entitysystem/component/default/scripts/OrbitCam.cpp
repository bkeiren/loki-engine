#include "core/entitysystem/component/default/scripts/OrbitCam.h"
#include "core/entitysystem/component/default/CameraComponent.h"
#include "core/entitysystem/Entity.h"
#include "core/input/input.h"

namespace loki
{

namespace components
{

namespace scripts
{

OrbitCam::OrbitCam()	:
	m_Center(vec3(0.0f, 0.0f, 0.0f)),
	m_Distance(5.0f)
{

}

OrbitCam::~OrbitCam()
{

}

float OrbitCam::GetDistance() const
{
	return m_Distance;
}

void OrbitCam::SetDistance( float _Distance )
{
	m_Distance = _Distance;
}

const vec3& OrbitCam::GetCenter() const
{
	return m_Center;
}

void OrbitCam::SetCenter( const vec3& _Center )
{
	m_Center = _Center;
}

void OrbitCam::Awake()
{
	m_Camera = GetEntity()->GetComponent<loki::components::CameraComponent>();
	if (!m_Camera)
	{
		LOG(VL_WARN, "OrbitCam::Awake: Entity does not have a camera component attached!");
	}
}

void OrbitCam::Update()
{
	Transform& t = GetTransform();
	t.SetPosition(m_Center);

	if (g_Input->Get(BUTTON_MOUSELEFT) == KEYSTATE_DOWN)
	{
		int2 mouseDelta = -g_Input->GetMouseDelta();
		t.RotateX((f32)mouseDelta.y / 3);
		t.LocalRotateY((f32)mouseDelta.x / 3);
	}

	SetDistance(GetDistance() - (g_Input->GetMouseWheelDelta() * 0.6f));

	t.SetPosition(m_Center + ((t.GetOrientation() * FORWARD) * GetDistance()));
}

void OrbitCam::Stop()
{

}

void OrbitCam::Enabled()
{

}

void OrbitCam::Disabled()
{

}

}

}

}