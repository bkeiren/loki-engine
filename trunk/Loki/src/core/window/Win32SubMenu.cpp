#include "core/window/Win32SubMenu.h"
#include "core/window/Win32MenuItem.h"

namespace loki
{

Win32SubMenu::Win32SubMenu( const std::string& _Name )	:
	m_ID(_ReserveGUID()),
	m_Name(_Name),
	m_HMENUHandle(CreatePopupMenu())
{

}

Win32SubMenu::Win32SubMenu()
{
	
}

Win32SubMenu::~Win32SubMenu()
{
	for (MenusIter it = m_SubMenus.begin(); it != m_SubMenus.end(); ++it)
	{
		delete (*it).second;
	}
	for (ItemsIter it = m_Items.begin(); it != m_Items.end(); ++it)
	{
		delete (*it).second;
	}
	DestroyMenu(m_HMENUHandle);
}

uint32 Win32SubMenu::GetID() const
{
	return m_ID;
}

Win32SubMenu* Win32SubMenu::CreateSubMenu( const std::string& _Name )
{
	Win32SubMenu* menu = FindSubMenu(_Name);
	if (menu)
	{
		LOG(VL_ERROR, "Win32SubMenu::CreateSubMenu: A submenu named '%s' already exists", _Name.c_str());
		return 0;
	}
	menu = new Win32SubMenu(_Name);
	AppendMenuA(m_HMENUHandle, MF_STRING | MF_POPUP, (uint32)menu->m_HMENUHandle, _Name.c_str());
	m_SubMenus.insert(MenusPair(_Name, menu));
	return menu;
}

Win32MenuItem* Win32SubMenu::CreateItem( const std::string& _Name, uint32 _Flags )
{
	Win32MenuItem* item = FindItem(_Name);
	if (item)
	{
		LOG(VL_ERROR, "Win32SubMenu::CreateItem: An item named '%s' already exists", _Name.c_str());
		return 0;
	}
	item = new Win32MenuItem(_Name);
	AppendMenuA(m_HMENUHandle, MF_STRING | _Flags, item->GetID(), _Name.c_str());
	m_Items.insert(ItemsPair(_Name, item));
	return item;
}

Win32SubMenu* Win32SubMenu::FindSubMenu( const std::string& _Name )
{
	MenusConstIter it = m_SubMenus.find(_Name);
	if (it == m_SubMenus.end())
	{
		return 0;
	}
	return (*it).second;
}

Win32MenuItem* Win32SubMenu::FindItem( const std::string& _Name )
{
	ItemsConstIter it = m_Items.find(_Name);
	if (it == m_Items.end())
	{
		return 0;
	}
	return (*it).second;
}

uint32 Win32SubMenu::_ReserveGUID()
{
	static uint32 GUIDCounter = 1337;
	return GUIDCounter++;
}

}