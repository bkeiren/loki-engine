#include "container.h"
#include "resizeable.h"
#include "window.h"

namespace gui
{

Window::Window( Canvas* _Parent )	:
	m_Parent(_Parent)
{
	m_Position.x = 0.0f;
	m_Position.z = 0.0f;
	m_Position.y = 0.0f;
}

Window::Window()	:
	m_Parent(0)
{

}

Window::~Window()
{

}

void Window::SetPosition( gui::Vec3& _Position )
{
	m_Position = _Position;
}

const gui::Vec3& Window::GetPosition()
{
	return m_Position;
}

}