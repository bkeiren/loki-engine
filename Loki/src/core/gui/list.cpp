#include <list>		// STL list.
#include "widget.h"
#include "list.h"
#include "listitem.h"

namespace gui
{

List::List( Container* _Parent )	:
	Widget(_Parent)
{

}

List::List()
{

}

List::~List()
{

}

ListItem* List::AddItem()
{
	ListItem* item = new ListItem(this);
	m_Items.push_back(item);
	return item;
}

}