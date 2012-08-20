#include <list>
#include "widget.h"
#include "radiobuttongroup.h"
#include "radiobutton.h"

namespace gui
{

RadioButtonGroup::RadioButtonGroup( Container* _Parent )	:
	Widget(_Parent),
	m_SelectedIndex(-1)
{

}

RadioButtonGroup::RadioButtonGroup()	:
	m_SelectedIndex(-1)
{

}

RadioButtonGroup::~RadioButtonGroup()
{

}

RadioButton* RadioButtonGroup::AddRadioButton()
{
	RadioButton* radiobutton = new RadioButton(this);
	m_RadioButtons.push_back(radiobutton);
	return radiobutton;
}

}