#pragma once

#ifndef NULLHTMLCORE_H
#define NULLHTMLCORE_H

#include "core/html/htmlcore.h"
#include "core/html/htmlview/nullhtmlview.h"

namespace Awesomium
{
	class WebCore;
}

namespace loki
{

class NullHTMLCore	: public LkHTMLCore
{
	friend class LokiEngine;
public:
	LkHTMLView* CreateView( int32 _Width, int32 _Height, int32 _Layer = 0 ) { return m_NullView; }
	void DestroyView( LkHTMLView** _Tab ) {}
	void InjectKeyboardEvent( UINT _Msg, WPARAM _WParam, LPARAM _LParam ) {}
	void SetViewInFocus( LkHTMLView* _View ) {}
	LkHTMLView* GetViewInFocus() const { return m_NullView; }
	bool GetInputDetected() const { return false; }
private:
	NullHTMLCore() : LkHTMLCore(true) { if (!m_NullView){ m_NullView = new NullHTMLView(); } }
	~NullHTMLCore() {}

	bool _Init() { return true; }
	void _Terminate() {}
	void _OnEvent( const LkEvent& _Event ) {}
	LkHTMLView* _GetTopMostViewUnderMouse( bool _ConsiderTransparency = true, f32 _AlphaThreshold = 0.0f, f32 _ExceptionAlpha = -1.0f ) const { return m_NullView; }

	static NullHTMLView* m_NullView; 
};

NullHTMLView* NullHTMLCore::m_NullView = 0;

}

#endif