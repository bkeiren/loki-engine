#pragma once

#ifndef CONTAINER_H
#define CONTAINER_H

#include "gui_types.h"
#include <list>

namespace gui
{

class Widget;

class Container
{
public:

	TextField*			AddTextField();
	Button*				AddButton();
	List*				AddList();
	TabGroup*			AddTabGroup();
	RadioButtonGroup*	AddRadioButtonGroup();
	CheckBox*			AddCheckBox();
	ProgressBar*		AddProgressBar();
protected:
	Container();
	virtual ~Container() = 0;	// Abstract.
private:
	std::list<Widget*> m_Elements;
};

}

#endif