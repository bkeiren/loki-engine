#include "radiobutton.h"

namespace gui
{

RadioButton::RadioButton( RadioButtonGroup* _Group )	:
	m_Group(_Group)
{
	if (!m_Group)
	{
		// Output warning that m_Group is NULL.
	}
}

RadioButton::RadioButton()
{

}

RadioButton::~RadioButton()
{

}

}