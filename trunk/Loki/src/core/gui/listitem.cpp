#include "listitem.h"

namespace gui
{

ListItem::ListItem( List* _List )	:
	m_List(_List)
{
	if (!m_List)
	{
		// Output warning that m_List is NULL.
	}
}

ListItem::ListItem()
{

}

ListItem::~ListItem()
{

}

}