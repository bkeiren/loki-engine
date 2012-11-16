#pragma once

#ifndef WIN32MENUITEM_H
#define WIN32MENUITEM_H

namespace loki
{

class Win32MenuItem
{
	friend class LokiEngine;
	friend class Win32SubMenu;
public:
	typedef void(*Win32MenuItemCallback)();

	uint32 GetID() const;

	void SetCallback( Win32MenuItemCallback _Callback );
private:
	CONTAINER_MACRO_MAP(uint32, Win32MenuItemCallback, Callbacks)

	Win32MenuItem( const std::string& _Name );
	Win32MenuItem();
	~Win32MenuItem();

	static uint32 _ReserveGUID();

	static void _CallCallback( uint32 _ID );

	uint32 m_ID;
	std::string m_Name;

	static Callbacks m_Callbacks;
};

}

#endif