#pragma once

#ifndef CHECKBOX_H
#define CHECKBOX_H

namespace gui
{

class CheckBox	: public Widget
{
public:

	// Stores a bool pointer that is queried and altered whenever
	// necessary according to the classic behavior of a checkbox.
	void SetStateVariable( bool* _Reference );
private:
	friend class Container;	// The Container class should be the only class able to instantiate and delete CheckBox objects.

	CheckBox( Container* _Parent );
	CheckBox();
	virtual ~CheckBox();

	bool* m_StateVariable;
};

}

#endif