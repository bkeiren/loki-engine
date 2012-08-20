#include "core/eventsystem/eventmanager.h"
#include "core/engine.h"
#include "core/renderer/renderer.h"
#include "core/window.h"

namespace loki
{

LkInput* g_Input = NULL;

LkInput::LkInput()	:
	m_Mouse(glm::int2(0, 0)),
	m_MousePrevious(glm::int2(0, 0)),
	m_MouseDelta(glm::int2(0, 0)),
	m_MouseMoved(false),
	m_MouseRestDelta(glm::int2(400, 400)),
	m_MouseWheelDelta(0.0f),
	m_MouseAccelerationEnabled(true),
	m_MouseAccelerationParameters(glm::vec2(1.5f, 0.0f))
{
	_Init();
}

LkInput::~LkInput()
{

}

bool LkInput::_Init()
{
	LOG(VL_ALWAYS, "Input::Init: Initialized");
	return true;
}

void LkInput::_PerformMouseAcceleration()
{
	//2) calculate the total distance this corresponds to: dr = sqrt(dx^2+dy^2)
	float dr = sqrtf((float)(m_MouseDelta.x * m_MouseDelta.x + m_MouseDelta.y * m_MouseDelta.y));

	//3) determine how much time has passed, and calculate the speed of the movement: v = dr/dt
	float dt = g_Engine->GetFrameTime();
	float v = dr / dt;

	//4) perform some non-linear transform on the velocity, 
	//   eg: v_new = a * v + b * v^2 (start with a=1 and b=0 for no acceleration, and then experiment for optimal values)
	float v_new = m_MouseAccelerationParameters.x * v + m_MouseAccelerationParameters.y * (v * v);

	//5) calculate a new distance: dr_new = v_new * dt
	float dr_new = v_new * dt;

	//6) calculate new distances in x / y direction: 
	//   dx_new = dx * dr_new / dr and dy_new = dy * dr_new / dr
	float dx_new = m_MouseDelta.x * dr_new / dr;
	float dy_new = m_MouseDelta.y * dr_new / dr;

	m_Mouse.x = (int)(m_MousePrevious.x + dx_new);
	m_Mouse.y = (int)(m_MousePrevious.y + dy_new);
	m_MouseDelta = m_Mouse - m_MousePrevious;
	m_MouseMoved = (m_MousePrevious != m_Mouse);
}

void LkInput::Capture()
{
	for (int i = 0; i < KEY_LAST; ++i)
	{
		short state = GetAsyncKeyState(i) >> 8;	// Shifted by 8 because the most significant bit is used to indicate the
												// state of the key.
		// If key is down...
		if (state)
		{
			// If key was not down last frame...
			if (m_Keys[i] == KEYSTATE_UP || m_Keys[i] == KEYSTATE_RELEASED)
			{
				// Key has just been pressed.
				m_Keys[i] = KEYSTATE_PRESSED;
			}
			// Else, key was down or being pressed last frame...
			else
			{
				m_Keys[i] = KEYSTATE_DOWN;
			}
		}
		// Else if key is not down...
		else
		{
			// If key was down last frame...
			if (m_Keys[i] == KEYSTATE_DOWN || m_Keys[i] == KEYSTATE_PRESSED)
			{
				m_Keys[i] = KEYSTATE_RELEASED;
			}
			// Else, key was up or being released last frame...
			else
			{
				m_Keys[i] = KEYSTATE_UP;
			}
		}
	}

	POINT pos;
	GetCursorPos(&pos);
	ScreenToClient(g_Engine->GetWindow()->GetHWND(), &pos);

	m_MousePrevious = m_Mouse;
	m_Mouse.x = pos.x;
	m_Mouse.y = pos.y;
	m_MouseDelta = m_Mouse - m_MousePrevious;

	m_MouseMoved = (m_MousePrevious != m_Mouse);

	m_MouseWheelDelta = 0.0f;

	if (m_MouseAccelerationEnabled && m_MouseMoved)
	{
		_PerformMouseAcceleration();
	}

	if (m_MouseMoved)
	{
		g_EventManager->Post(LkEvent(EVENT_MOUSEMOVE));
	}

	switch (m_Keys[BUTTON_MOUSELEFT])
	{
	case KEYSTATE_DOWN:
		g_EventManager->Post(LkEvent(EVENT_MB_LEFT_DOWN));
		break;
	case KEYSTATE_PRESSED:
		g_EventManager->Post(LkEvent(EVENT_MB_LEFT_PRESSED));
		break;
	case KEYSTATE_RELEASED:
		g_EventManager->Post(LkEvent(EVENT_MB_LEFT_RELEASED));
		break;
	}

	switch (m_Keys[BUTTON_MOUSEMIDDLE])
	{
	case KEYSTATE_DOWN:
		g_EventManager->Post(LkEvent(EVENT_MB_MIDDLE_DOWN));
		break;
	case KEYSTATE_PRESSED:
		g_EventManager->Post(LkEvent(EVENT_MB_MIDDLE_PRESSED));
		break;
	case KEYSTATE_RELEASED:
		g_EventManager->Post(LkEvent(EVENT_MB_MIDDLE_RELEASED));
		break;
	}

	switch (m_Keys[BUTTON_MOUSERIGHT])
	{
	case KEYSTATE_DOWN:
		g_EventManager->Post(LkEvent(EVENT_MB_RIGHT_DOWN));
		break;
	case KEYSTATE_PRESSED:
		g_EventManager->Post(LkEvent(EVENT_MB_RIGHT_PRESSED));
		break;
	case KEYSTATE_RELEASED:
		g_EventManager->Post(LkEvent(EVENT_MB_RIGHT_RELEASED));
		break;
	}
}

EKeyState LkInput::Get( EKeys _Key ) const
{
	return m_Keys[_Key];
}

const glm::int2 LkInput::GetMousePosition() const
{
	glm::int2 m = glm::int2(renderer::g_Renderer->GetPixelScale() * glm::vec2(m_Mouse));
	return m;
}

int LkInput::GetMouseX() const
{
	return m_Mouse.x;
}

int LkInput::GetMouseY() const
{
	return m_Mouse.y;
}

bool LkInput::GetMouseMoved() const
{
	return m_MouseMoved;
}

const glm::int2& LkInput::GetMouseDelta() const
{
	return m_MouseDelta;
}

int LkInput::GetMouseDeltaX() const
{
	return m_MouseDelta.x;
}

int LkInput::GetMouseDeltaY() const
{
	return m_MouseDelta.y;
}

float LkInput::GetMouseWheelDelta() const
{
	return m_MouseWheelDelta;
}

void LkInput::SetMouseAccelerationEnabled( bool _Enabled )
{
	m_MouseAccelerationEnabled = _Enabled;
}

bool LkInput::GetMouseAccelerationEnabled() const
{
	return m_MouseAccelerationEnabled;
}

void LkInput::SetMouseAccelerationParameters( const glm::vec2& _Parameters )
{
	m_MouseAccelerationParameters = _Parameters;
}

const glm::vec2& LkInput::GetMouseAccelerationParameters() const
{
	return m_MouseAccelerationParameters;
}

}	// Namespace loki