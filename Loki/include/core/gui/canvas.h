#pragma once

#ifndef CANVAS_H
#define CANVAS_H

namespace gui
{

class Window;

class Canvas	: public Container
{
public:

	Window* AddWindow();
private:
	friend Canvas* CreateCanvas();

	Canvas();
	virtual ~Canvas();

	std::list<Window*> m_Windows;
};

}

#endif