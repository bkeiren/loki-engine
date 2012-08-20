#include "widget.h"
#include "checkbox.h"

namespace gui
{

CheckBox::CheckBox( Container* _Parent )	:
	Widget(_Parent),
	m_StateVariable(0)
{

}

CheckBox::CheckBox()	:
	m_StateVariable(0)
{

}

CheckBox::~CheckBox()
{

}

void CheckBox::SetStateVariable( bool* _Reference )
{
	m_StateVariable = _Reference;
}

}