#pragma once

#ifndef WIN32MENU_H
#define WIN32MENU_H

namespace loki
{

class Win32MenuItem;

class Win32SubMenu
{
	friend class Window;
	CONTAINER_MACRO_MAP(std::string, Win32SubMenu*, Menus)
	CONTAINER_MACRO_MAP(std::string, Win32MenuItem*, Items)
public:
	enum
	{
		// Flag values from http://msdn.microsoft.com/en-us/library/windows/desktop/ms647616(v=vs.85).aspx
		ITEM_FLAG_DISABLED = 0x00000002L,
		ITEM_FLAG_ENABLED = 0x00000000L,
		ITEM_FLAG_GRAYED = 0x00000001L,
		ITEM_FLAG_MENUBARBREAK = 0x00000020L,
		ITEM_FLAG_MENUBREAK = 0x00000040L,
		ITEM_FLAG_SEPARATOR = 0x00000800L
	};

	uint32 GetID() const;
	Win32SubMenu* CreateSubMenu( const std::string& _Name );
	Win32MenuItem* CreateItem( const std::string& _Name, uint32 _Flags );
	Win32SubMenu* FindSubMenu( const std::string& _Name );
	Win32MenuItem* FindItem( const std::string& _Name );
private:
	Win32SubMenu();
	Win32SubMenu( const std::string& _Name );
	~Win32SubMenu();

	static uint32 _ReserveGUID();

	Menus m_SubMenus;
	Items m_Items;
	uint32 m_ID;
	std::string m_Name;
	HMENU m_HMENUHandle;
};

}

#endif