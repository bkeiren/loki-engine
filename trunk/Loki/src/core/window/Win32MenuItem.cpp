#include "core/window/Win32MenuItem.h"

namespace loki
{

namespace
{
	void DummyCallback() {}
}

Win32MenuItem::Callbacks Win32MenuItem::m_Callbacks;

Win32MenuItem::Win32MenuItem( const std::string& _Name )	:
	m_ID(_ReserveGUID()),
	m_Name(_Name)
{
	m_Callbacks.insert(CallbacksPair(GetID(), &DummyCallback));
}

Win32MenuItem::Win32MenuItem()
{
	ILLEGAL_CTOR_ERROR("Win32MenuItem")
}

Win32MenuItem::~Win32MenuItem()
{

}

uint32 Win32MenuItem::GetID() const
{
	return m_ID;
}

void Win32MenuItem::SetCallback( Win32MenuItemCallback _Callback )
{
	m_Callbacks[GetID()] = _Callback == 0 ? &DummyCallback : _Callback;
}

uint32 Win32MenuItem::_ReserveGUID()
{
	static uint32 GUIDCounter = 8008;
	return GUIDCounter++;
}

void Win32MenuItem::_CallCallback( uint32 _ID )
{
	Win32MenuItemCallback cb = m_Callbacks[_ID];
	if (cb)
	{
		cb();
	}
}

}