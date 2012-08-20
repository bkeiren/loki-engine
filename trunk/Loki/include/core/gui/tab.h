#pragma once

#ifndef TAB_H
#define TAB_H

namespace gui
{

class TabGroup;

class Tab	: public Container
{
public:

protected:
	friend class TabGroup;	// Tabs should only be instantiated by the TabGroup class.
private:
	Tab( TabGroup* _Group );
	Tab();
	virtual ~Tab();

	TabGroup* m_Group;
};

}

#endif