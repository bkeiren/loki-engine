#pragma once

#ifndef WIDGET_H
#define WIDGET_H

#include "gui_types.h"

namespace gui
{

typedef int (*EventCallback)( void* _A );

enum EEventType
{
	EVENT_MOUSEENTER = 0,	// On enter.
	EVENT_MOUSEOVER,		// While over.
	EVENT_MOUSEDOWN,		// On down.
	EVENT_MOUSEUP,			// On up.
	EVENT_MOUSEEXIT,		// On exit.

	_EVENT_COUNT			// Keep as last, don't modify.
							// Prefixed underscore is intentional.
};

class Widget
{
public:

	void SetPosition( gui::Vec3& _Position );
	const gui::Vec3& GetPosition();

	void SetEventCallback( EEventType _Event, EventCallback _Callback );
protected:
	Widget( Container* _Parent );
	Widget();
	virtual ~Widget() = 0;
private:
	Container* m_Parent;
	gui::Vec3 m_Position;

	EventCallback m_Callbacks[_EVENT_COUNT];
};

}

#endif