#pragma once

#ifndef INPUT_H
#define INPUT_H

#pragma warning( disable : 4800 )	// Force value to bool warning. Ignored because
									// using Input::Get()'s return value as a boolean will result in this warning.

#include "Types.h"	// Need this for client apps including this file.

namespace loki
{

enum
{
	BUTTON_MOUSELEFT = 1,
	BUTTON_MOUSERIGHT, // 2
	BUTTON_MOUSEMIDDLE = 4,
	BUTTON_5, // 5
	BUTTON_6, // 6
	KEY_BACKSPACE = 8,
	KEY_TAB = 9,
	KEY_RETURN = 13,
	KEY_ESCAPE = 27,
	KEY_SPACE = 32,
	KEY_PAGEUP, // 33
	KEY_PAGEDOWN, // 34
	KEY_END, // 35
	KEY_HOME, // 36
	KEY_ARROWLEFT, // 37
	KEY_ARROWUP, // 38
	KEY_ARROWRIGHT, // 39
	KEY_ARROWDOWN, // 40
	KEY_PRINTSCREEN = 44,
	KEY_INSERT, // 45
	KEY_DELETE, // 46
	KEY_0 = 48,
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9, // 57
	KEY_A = 65, 
	KEY_B, 
	KEY_C, 
	KEY_D, 
	KEY_E, 
	KEY_F, 
	KEY_G, 
	KEY_H, 
	KEY_I, 
	KEY_J, 
	KEY_K, 
	KEY_L, 
	KEY_M, 
	KEY_N, 
	KEY_O, 
	KEY_P, 
	KEY_Q, 
	KEY_R, 
	KEY_S, 
	KEY_T, 
	KEY_U, 
	KEY_V, 
	KEY_W, 
	KEY_X, 
	KEY_Y, 
	KEY_Z, // 90
	KEY_MENU = 93,
	// NOTE: 97 thru 122 are the ASCII codes of the lower-case versions of the letters of the alphabet.
	// But at 112, the Fn keys start enumerating. Note that trying to access the state of alphabet-keys 
	// by using their lower-case character variants will return the state of other keys (Not the ones expected).
	KEY_F1 = 112,	
	KEY_F2,
	KEY_F3,
	KEY_F4,
	KEY_F5,
	KEY_F6,
	KEY_F7,
	KEY_F8,
	KEY_F9,
	KEY_F10,
	KEY_F11,
	KEY_F12, // 123
	KEY_LSHIFT = 160,
	KEY_RSHIFT,	// 161
	KEY_LCONTROL, // 162
	KEY_RCONTROL, // 163
	KEY_LALT, // 164
	KEY_RALT, // 165
	KEY_SEMICOLON = 186,
	KEY_PLUS = 187,
	KEY_COMMA = 188,
	KEY_MINUS = 189,
	KEY_DOT = 190,
	KEY_SLASHFORWARD = 191,
	KEY_TILDE = 192,
	KEY_BRACKETLEFT = 219,
	KEY_SLASHBACKWARD = 220,
	KEY_BRACKETRIGHT = 221,
	KEY_APOSTROPHE = 222,

	// TODO: Numeric keys.

	_KEY_LAST = 255	// Keep as last!
};

enum EKeyState
{
	KEYSTATE_UP			= 0,			// Key is up.
	KEYSTATE_PRESSED	= 0x1,			// Key has just been pressed.
	KEYSTATE_DOWN		= 0x1 << 1,		// Key is being held down.
	KEYSTATE_RELEASED	= 0x1 << 2,		// Key has just been released.
};

//////////////////////////////////////////////////////////////////////////
// Convenience macros.
//////////////////////////////////////////////////////////////////////////
#define KEY_UP(key)			(loki::g_Input->Get(key) == loki::KEYSTATE_UP)
#define KEY_PRESSED(key)	(loki::g_Input->Get(key) == loki::KEYSTATE_PRESSED)
#define KEY_DOWN(key)		(loki::g_Input->Get(key) == loki::KEYSTATE_DOWN)
#define KEY_RELEASED(key)	(loki::g_Input->Get(key) == loki::KEYSTATE_RELEASED)

class LkInput
{
	friend class LokiEngine;
public:	
	EKeyState Get( int32 _Key ) const;
	bool IsUp( int32 _Key ) const;
	bool IsPressed( int32 _Key ) const;
	bool IsDown( int32 _Key ) const;
	bool IsReleased( int32 _Key ) const;
	const int2 GetMousePosition() const;
	int32 GetMouseX() const;
	int32 GetMouseY() const ;
	bool GetMouseMoved() const;
	const int2& GetMouseDelta() const;
	int32 GetMouseDeltaX() const;
	int32 GetMouseDeltaY() const;
	f32 GetMouseWheelDelta() const;
	void SetMouseAccelerationEnabled( bool _Enabled );
	bool GetMouseAccelerationEnabled() const;
	void SetMouseAccelerationParameters( const vec2& _Parameters );
	const vec2& GetMouseAccelerationParameters() const;
private:
	LkInput();
	~LkInput();

	//////////////////////////////////////////////////////////////////////////
	// Takes care of initializing the Input class. For example, it makes sure
	// that the mouse cursor is set to a certain position at startup so that
	// there is no strange movement registered if the mouse isn't at the
	// correct position during start up.
	//////////////////////////////////////////////////////////////////////////
	bool _Init();

	//////////////////////////////////////////////////////////////////////////
	// Captures key input and sets the key states. Must be called by the engine 
	// each frame in order to have up-to-date key information.
	//////////////////////////////////////////////////////////////////////////
	void _Capture();

	void _PerformMouseAcceleration();

	inline void _CaptureKeyState( int32 _Key );

	EKeyState m_Keys[_KEY_LAST];
	int2 m_Mouse;
	int2 m_MousePrevious;
	int2 m_MouseDelta;
	bool m_MouseMoved;
	int2 m_MouseRestDelta;	// Mouse resting delta position.
	f32 m_MouseWheelDelta;	// Each 'tick' of the mouse wheel is 1 unit. Continuous mouse wheels can have intermediate values.
	bool m_MouseAccelerationEnabled;
	vec2 m_MouseAccelerationParameters;	// Testing has showed that parameter X should not exceed 1.5f (Because that will result in over-compensation and thus jittering when the cursor comes to a stop).
												
};

extern LkInput* g_Input;

}

#endif // INPUT_H