#pragma once

#ifndef RESIZEABLE_H
#define RESIZEABLE_H

namespace gui
{

class Resizeable
{
public:
	Resizeable();
	virtual ~Resizeable() = 0;	// Class should not be instantiated.

	void SetSize( gui::Vec2& _Size );
	const gui::Vec2& GetSize();
private:

	gui::Vec2 m_Size;
};

}

#endif