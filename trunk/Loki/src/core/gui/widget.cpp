#include "widget.h"

namespace gui
{

Widget::Widget( Container* _Parent )	:
	m_Parent(_Parent)
{
	if (!m_Parent)
	{
		// Output warning that m_Parent is NULL.
	}

	m_Position.x = 0.0f;
	m_Position.z = 0.0f;
	m_Position.y = 0.0f;

	for (int i = 0; i < _EVENT_COUNT; ++i)
	{
		m_Callbacks[i] = 0;
	}
}

Widget::Widget()	:
	m_Parent(0)
{
	// Output warning that a widget was instantiated incorrectly.
}

Widget::~Widget()
{

}

void Widget::SetPosition( gui::Vec3& _Position )
{
	m_Position = _Position;
}

const gui::Vec3& Widget::GetPosition()
{
	return m_Position;
}

void Widget::SetEventCallback( EEventType _Event, EventCallback _Callback )
{
	m_Callbacks[_Event] = _Callback;
}

}