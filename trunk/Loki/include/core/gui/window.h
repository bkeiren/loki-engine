#pragma once

#ifndef WINDOW_H
#define WINDOW_H

namespace gui
{

class Window	: public Container, public Resizeable
{
public:

	void SetPosition( gui::Vec3& _Position );
	const gui::Vec3& GetPosition();
protected:
	friend class Canvas;	// Windows should only be created by the Canvas class.
private:
	Window( Canvas* _Parent );
	Window();
	virtual ~Window();

	Canvas* m_Parent;
	gui::Vec3 m_Position;
};

}

#endif