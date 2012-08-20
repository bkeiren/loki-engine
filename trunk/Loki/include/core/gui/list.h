#pragma once

#ifndef LIST_H
#define LIST_H

namespace gui
{

class ListItem;

class List	: public Widget
{
public:

	ListItem* AddItem();
private:
	friend class Container;	// The Container class should be the only class able to instantiate and delete List objects.

	List( Container* _Parent );
	List();
	virtual ~List();

	std::list<ListItem*> m_Items;
};

}

#endif