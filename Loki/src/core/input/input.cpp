#include "core/input/input.h"
#include "core/eventsystem/eventmanager.h"
#include "core/engine.h"
#include "core/renderer/renderer.h"
#include "core/window.h"
#include "core/console/console.h"
#include "core/time/Time.h"

namespace loki
{

LkInput* g_Input = NULL;

LkInput::LkInput()	:
	m_Mouse(int2(0, 0)),
	m_MousePrevious(int2(0, 0)),
	m_MouseDelta(int2(0, 0)),
	m_MouseMoved(false),
	m_MouseRestDelta(int2(400, 400)),
	m_MouseWheelDelta(0.0f),
	m_MouseAccelerationEnabled(true),
	m_MouseAccelerationParameters(vec2(1.5f, 0.0f))
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
	f32 dr = sqrtf((f32)(m_MouseDelta.x * m_MouseDelta.x + m_MouseDelta.y * m_MouseDelta.y));

	//3) determine how much time has passed, and calculate the speed of the movement: v = dr/dt
	f32 dt = g_Time->GetActualFrameTime();
	f32 v = dr / dt;

	//4) perform some non-linear transform on the velocity, 
	//   eg: v_new = a * v + b * v^2 (start with a=1 and b=0 for no acceleration, and then experiment for optimal values)
	f32 v_new = m_MouseAccelerationParameters.x * v + m_MouseAccelerationParameters.y * (v * v);

	//5) calculate a new distance: dr_new = v_new * dt
	f32 dr_new = v_new * dt;

	//6) calculate new distances in x / y direction: 
	//   dx_new = dx * dr_new / dr and dy_new = dy * dr_new / dr
	f32 dx_new = m_MouseDelta.x * dr_new / dr;
	f32 dy_new = m_MouseDelta.y * dr_new / dr;

	m_Mouse.x = (int32)(m_MousePrevious.x + dx_new);
	m_Mouse.y = (int32)(m_MousePrevious.y + dy_new);
	m_MouseDelta = m_Mouse - m_MousePrevious;
	m_MouseMoved = (m_MousePrevious != m_Mouse);
}

void LkInput::_CaptureKeyState( int32 _Key )
{
	short state = GetKeyState(_Key) >> 8;	// Shifted by 8 because the most significant bit is used to indicate the
											// state of the key.

	// If key is down...
	if (state)
	{
		// If key was not down last frame...
		if (m_Keys[_Key] == KEYSTATE_UP || m_Keys[_Key] == KEYSTATE_RELEASED)
		{
			// Key has just been pressed.
			m_Keys[_Key] = KEYSTATE_PRESSED;
		}
		// Else, key was down or being pressed last frame...
		else
		{
			m_Keys[_Key] = KEYSTATE_DOWN;
		}
	}
	// Else if key is not down...
	else
	{
		// If key was down last frame...
		if (m_Keys[_Key] == KEYSTATE_DOWN || m_Keys[_Key] == KEYSTATE_PRESSED)
		{
			m_Keys[_Key] = KEYSTATE_RELEASED;
		}
		// Else, key was up or being released last frame...
		else
		{
			m_Keys[_Key] = KEYSTATE_UP;
		}
	}
}

void LkInput::_Capture()
{
	if (!g_Engine->GetWindow()->HasFocus())
	{
		return;
	}

	if (!g_Console->IsVisible())
	{
		for (int32 i = 0; i < _KEY_LAST; ++i)
		{
			_CaptureKeyState(i);
		}
	}
	else
	{
		EKeyState states[2] = { m_Keys[KEY_TILDE], m_Keys[KEY_ESCAPE] };

		for (int32 i = 0; i < _KEY_LAST; ++i)
		{
			m_Keys[i] = KEYSTATE_UP;
		}

		// Restore previous state for tilde and escape.
		m_Keys[KEY_TILDE] = states[0];
		m_Keys[KEY_ESCAPE] = states[1];

		// These must be passed to the console in C++ so we check these.
		_CaptureKeyState(KEY_TILDE);
		_CaptureKeyState(KEY_ESCAPE);
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

EKeyState LkInput::Get( int32 _Key ) const
{
	assert(_Key > 0 && _Key < _KEY_LAST);
	return m_Keys[_Key];
}

bool LkInput::IsUp( int32 _Key ) const
{
	return KEY_UP(_Key);
}

bool LkInput::IsPressed( int32 _Key ) const
{
	return KEY_PRESSED(_Key);
}

bool LkInput::IsDown( int32 _Key ) const
{
	return KEY_DOWN(_Key);
}

bool LkInput::IsReleased( int32 _Key ) const
{
	return KEY_RELEASED(_Key);
}

const int2 LkInput::GetMousePosition() const
{
	int2 m = int2(renderer::g_Renderer->GetPixelScale() * vec2(m_Mouse));
	return m;
}

int32 LkInput::GetMouseX() const
{
	return m_Mouse.x;
}

int32 LkInput::GetMouseY() const
{
	return m_Mouse.y;
}

bool LkInput::GetMouseMoved() const
{
	return m_MouseMoved;
}

const int2& LkInput::GetMouseDelta() const
{
	return m_MouseDelta;
}

int32 LkInput::GetMouseDeltaX() const
{
	return m_MouseDelta.x;
}

int32 LkInput::GetMouseDeltaY() const
{
	return m_MouseDelta.y;
}

f32 LkInput::GetMouseWheelDelta() const
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

void LkInput::SetMouseAccelerationParameters( const vec2& _Parameters )
{
	m_MouseAccelerationParameters = _Parameters;
}

const vec2& LkInput::GetMouseAccelerationParameters() const
{
	return m_MouseAccelerationParameters;
}

}	// Namespace loki