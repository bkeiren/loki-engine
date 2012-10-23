#pragma once

#ifndef OVERLAYMANAGER_H
#define OVERLAYMANAGER_H

#include <hash_map>
#include "core/eventsystem/eventlistener/eventlistener.h"

#include "core/ui/overlay.h"
#include "core/ui/overlayelement.h"

namespace loki
{

namespace ui
{

class LkOverlay;
class LkOverlayStyle;

typedef uint32	OverlayID;
typedef uint32	OverlayStyleID;

class LkOverlayManager	: public LkEventListener
{
	typedef stdext::hash_map<OverlayID, LkOverlay*>		Overlays;
	typedef std::pair<OverlayID, LkOverlay*>			OverlaysPair;

	typedef stdext::hash_map<OverlayStyleID, LkOverlayStyle*>	OverlayStyles;
	typedef std::pair<OverlayStyleID, LkOverlayStyle*>			OverlayStylesPair;
	
	friend class LkEngine;
public:
	LkOverlay* GetOverlay( const char* _Overlay );

	LkOverlay* CreateOverlay( const char* _Overlay, const char* _OverlayStyle );

	bool LoadOverlayStyle( const char* _OverlayStyleName, const char* _OverlayStyleFile );
private:
	LkOverlayManager();
	~LkOverlayManager();

	LkOverlay* _GetOverlay( OverlayID _OverlayID );
	LkOverlayStyle* _GetOverlayStyle( OverlayStyleID _OverlayStyleID );

	void _OnEvent( const LkEvent& _Event );

	Overlays m_Overlays;
	OverlayStyles m_OverlayStyles;
};

extern LkOverlayManager* g_OverlayManager;

}

}

#endif