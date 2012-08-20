#include <list>
#include "widget.h"
#include "tabgroup.h"
#include "container.h"
#include "tab.h"

namespace gui
{

TabGroup::TabGroup( Container* _Parent )	:
	Widget(_Parent)
{

}

TabGroup::TabGroup()
{

}

TabGroup::~TabGroup()
{

}

Tab* TabGroup::AddTab()
{
	Tab* tab = new Tab(this);
	m_Tabs.push_back(tab);
	return tab;
}

}