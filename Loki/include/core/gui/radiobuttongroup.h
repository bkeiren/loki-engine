#pragma once

#ifndef RADIOBUTTONGROUP_H
#define RADIOBUTTONGROUP_H

namespace gui
{

class RadioButton;

class RadioButtonGroup	: public Widget
{
public:

	RadioButton* AddRadioButton();
private:
	friend class Container;	// The Container class should be the only class able to instantiate and delete RadioButtonGroup objects.

	RadioButtonGroup( Container* _Parent );
	RadioButtonGroup();
	virtual ~RadioButtonGroup();

	std::list<RadioButton*> m_RadioButtons;
	unsigned int m_SelectedIndex;	// The index of the radiobutton
									// that has been selected.
};

}

#endif