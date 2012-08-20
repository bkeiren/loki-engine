#pragma once

#ifndef LISTITEM_H 
#define LITSITEM_H

namespace gui
{

class ListItem
{
public:

protected:
	friend class List;	// ListItem's should only be instantiated and deleted by the List class.
private:
	ListItem( List* _List );
	ListItem();
	virtual ~ListItem();

	List* m_List;
};

}

#endif