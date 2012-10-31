#pragma once

#ifndef HTMLCORE_H
#define HTMLCORE_H

#include "core/html/htmlview/htmlview.h"
#include "core/eventsystem/eventlistener/eventlistener.h"

// Forward declarations.
namespace Awesomium
{
	class WebCore;
}

namespace loki
{

class LkHTMLCore	: public LkEventListener
{
	friend class LokiEngine;

	typedef std::list<LkHTMLView*>				HTMLViews;
	typedef HTMLViews::iterator					HTMLViewsIter;
	typedef HTMLViews::reverse_iterator			HTMLVIewsRevIter;
	typedef HTMLViews::const_iterator			HTMLViewsConstIter;
	typedef HTMLViews::const_reverse_iterator	HTMLViewsConstRevIter;
public:
	//////////////////////////////////////////////////////////////////////////
	// Creates a new HTML view. _Layer indicates the layer on which the new
	// view is supposed to be. If there already exist a view with on the same
	// layer, we assume that the newest view should get priority and it is inserted
	// on top of the older view.
	//////////////////////////////////////////////////////////////////////////
	LkHTMLView* CreateView( int32 _Width, int32 _Height, int32 _Layer = 0 );

	void DestroyView( LkHTMLView** _Tab );

	//////////////////////////////////////////////////////////////////////////
	// Injects input directly from windows events.
	//////////////////////////////////////////////////////////////////////////
	void InjectKeyboardEvent( UINT _Msg, WPARAM _WParam, LPARAM _LParam );

	void SetViewInFocus( LkHTMLView* _View );
	LkHTMLView* GetViewInFocus() const;

	bool GetInputDetected() const;
private:
	LkHTMLCore();
	~LkHTMLCore();

	bool _Init();
	void _Terminate();
	
	void _OnEvent( const LkEvent& _Event );

	//////////////////////////////////////////////////////////////////////////
	// Finds the topmost view under the mouse position.
	// If _ConsiderTransparency is true, pixels whose alpha value is less than
	// or equal to _AlphaThreshold are considered see-through and will be
	// treated as if they do not belong to a view.
	// _ExceptionAlpha can be used to force an exception to be made when that
	// value is detected as the alpha value. Even when the threshold test fails
	// the exception test can still ensure a certain pixel is considered
	// as a valid mouse hit. Values such as -1.0f (The default value) ensure
	// that the exception rule is never met (Since negative values are invalid
	// alpha values). Note that _ExceptionAlpha is only considered when
	// _ConsiderTransparancy is true.
	// NOTE: There is a small epsilon value that is used to provide some
	// buffer room around _ExceptionAlpha because
	// the alpha values might have an issue with precision. For example,
	// when using the value 0.5 as alpha in your HTML page, the actual value
	// as returned from the rendered buffer might be 0.49...
	// For this reason, there is a epsilon value of 0.02 which is added on 
	// both sides of each value as passed to this function.
	// This means that:
	// For an alpha value of 0.5 for _ExceptionAlpha, any value in 
	// the range [0.48, 0.52] will be accepted.
	//////////////////////////////////////////////////////////////////////////
	LkHTMLView* _GetTopMostViewUnderMouse( bool _ConsiderTransparency = true, f32 _AlphaThreshold = 0.0f, f32 _ExceptionAlpha = -1.0f ) const;

	Awesomium::WebCore* m_WebCore;

	HTMLViews m_HTMLViews;
	LkHTMLView* m_HTMLViewInFocus;
	bool m_InputDetected;
};

extern LkHTMLCore* g_HTMLCore;

}

#endif