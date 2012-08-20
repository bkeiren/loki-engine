#pragma once

#ifndef TABGROUP_H
#define TABGROUP_H

namespace gui
{

class Container;

class TabGroup	: public Widget
{
public:

	Tab* AddTab();
private:
	friend class Container;	// The Container class should be the only class able to instantiate and delete TabGroup objects.

	TabGroup( Container* _Parent );
	TabGroup();
	virtual ~TabGroup();

	std::list<Tab*> m_Tabs;
};

}

#endif