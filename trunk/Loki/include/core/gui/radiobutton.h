#pragma once

#ifndef RADIOBUTTON_H
#define RADIOBUTTON_H

namespace gui
{

class RadioButton
{
public:

protected:
	friend class RadioButtonGroup;	// RadioButtons should only be instantiated and deleted by the RadioButtonGroup class.
private:
	RadioButton( RadioButtonGroup* _Group );
	RadioButton();
	virtual ~RadioButton();

	RadioButtonGroup* m_Group;
};

}

#endif