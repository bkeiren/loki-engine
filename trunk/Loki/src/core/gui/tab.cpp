#include "container.h"
#include "tab.h"

namespace gui
{

Tab::Tab( TabGroup* _Group )	:
	m_Group(_Group)
{
	if (!m_Group)
	{
		// Output warning that m_Group is NULL.
	}
}

Tab::Tab()
{

}

Tab::~Tab()
{

}

}