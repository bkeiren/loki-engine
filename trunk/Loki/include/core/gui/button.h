#pragma once

#ifndef BUTTON_H
#define BUTTON_H

namespace gui
{

class Container;

class Button	: public Widget
{
public:

private:
	friend class Container;	// The Container class should be the only class able to instantiate and delete Button objects.

	Button( Container* _Parent );
	Button();
	virtual ~Button();
};

}

#endif