#include "container.h"
#include "canvas.h"
#include "resizeable.h"
#include "window.h"

namespace gui
{

Canvas::Canvas()
{

}

Canvas::~Canvas()
{

}

Window* Canvas::AddWindow()
{
	Window* window = new Window(this);
	m_Windows.push_back(window);
	return window;
}

}