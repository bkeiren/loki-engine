#include "core/ui/overlaymanager.h"
#include "core/ui/overlay.h"
#include "util/hash/hash.h"
#include "core/ui/overlaystyle.h"

namespace loki
{

namespace ui
{

LkOverlayManager* g_OverlayManager = NULL;

LkOverlayManager::LkOverlayManager()
{
	SubscribeToEvent(EVENT_POSTRENDER);
}

LkOverlayManager::~LkOverlayManager()
{
	
}

LkOverlay* LkOverlayManager::GetOverlay( const char* _Overlay )
{
	assert(_Overlay != NULL);

	OverlayID hash = HASH(_Overlay);
	return _GetOverlay(hash);
}

LkOverlay* LkOverlayManager::CreateOverlay( const char* _Overlay, const char* _OverlayStyle )
{
	assert(_Overlay != NULL);

	OverlayID hash = HASH(_Overlay);
	LkOverlay* overlay = _GetOverlay(hash);
	if (overlay)
	{
		LOG(VL_WARN, "OverlayManager::CreateOverlay: An overlay named '%s' already exists", _Overlay);
		return NULL;
	}

	OverlayStyleID stylehash = HASH(_OverlayStyle);
	LkOverlayStyle* overlaystyle = _GetOverlayStyle(stylehash);
	if (!overlaystyle)
	{
		LOG(VL_WARN, "OverlayManager::CreateOverlay: An overlay style named '%s' does not exist", _OverlayStyle);
		return NULL;
	}

	overlay = new LkOverlay(overlaystyle);
	m_Overlays.insert(OverlaysPair(hash, overlay));
	return overlay;
}

bool LkOverlayManager::LoadOverlayStyle( const char* _OverlayStyleName, const char* _OverlayStyleFile )
{
	OverlayStyleID hash = HASH(_OverlayStyleName);
	LkOverlayStyle* style = _GetOverlayStyle(hash);
	if (style)
	{
		LOG(VL_WARN, "OverlayManager::LoadOverlayStyle: An overlay style named '%s' already exists", _OverlayStyleName);
		return false;
	}

	bool success = false;
	style = new LkOverlayStyle(_OverlayStyleName, _OverlayStyleFile, success);
	if (!success)
	{
		LOG(VL_WARN, "OverlayManager::LoadOverlayStyle: Overlay style file '%s' could not be succesfully used", _OverlayStyleFile);
		delete style;
		return false;
	}
	
	m_OverlayStyles.insert(OverlayStylesPair(hash, style));
	LOG(VL_NORMAL, "OverlayManager::LoadOverlayStyle: Overlay style '%s' was succesfully loaded", _OverlayStyleName);
	return true;
}

LkOverlay* LkOverlayManager::_GetOverlay( OverlayID _OverlayID )
{
	Overlays::iterator it = m_Overlays.find(_OverlayID);
	if (it != m_Overlays.end())
	{
		return (*it).second;
	}
	return NULL;
}

LkOverlayStyle* LkOverlayManager::_GetOverlayStyle( OverlayStyleID _OverlayStyleID )
{
	OverlayStyles::iterator it = m_OverlayStyles.find(_OverlayStyleID);
	if (it != m_OverlayStyles.end())
	{
		return (*it).second;
	}
	return NULL;
}

void LkOverlayManager::_OnEvent( const LkEvent& _Event )
{
	switch (_Event.GetEventType())
	{
	case EVENT_POSTRENDER:
		{
			for (Overlays::iterator it = m_Overlays.begin(); it != m_Overlays.end(); ++it)
			{
				(*it).second->Render();
			}
			
			break;
		}
	}
}

}

}