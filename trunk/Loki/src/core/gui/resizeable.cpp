#include "gui_types.h"
#include "resizeable.h"

namespace gui
{

Resizeable::Resizeable()
{
	m_Size.x = 0.1f;
	m_Size.y = 0.1f;
}

Resizeable::~Resizeable()
{

}

void Resizeable::SetSize( gui::Vec2& _Size )
{
	m_Size = _Size;
}

const gui::Vec2& Resizeable::GetSize()
{
	return m_Size;
}

}