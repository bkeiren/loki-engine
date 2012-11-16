#pragma once

#ifndef HTMLVIEW_H
#define HTMLVIEW_H

#include <Awesomium/JSValue.h>
#include <hash_map>

// Forward declarations.
namespace Awesomium
{
	class WebView;
	class RenderBuffer;
}

namespace loki
{

namespace renderer
{
	class LkImage;
}

class LkHTMLViewListener;

typedef Awesomium::JSArguments		JSArguments;
typedef Awesomium::JSValue			JSValue;
typedef Awesomium::FutureJSValue	FutureJSValue;
typedef void(*JSDelegate)( const JSArguments& );	// JS Delegate function signature.

class LkHTMLView
{
	friend class LkHTMLCore;
	friend class NullHTMLCore;
	friend class NullHTMLView;

	typedef stdext::hash_map<std::wstring, JSDelegate>			JSDelegateMapTier1;
	typedef JSDelegateMapTier1::iterator						JSDelegateMapIterTier1;
	typedef JSDelegateMapTier1::const_iterator					JSDelegateMapConstIterTier1;
	typedef std::pair<std::wstring, JSDelegate>					JSDelegateMapPairTier1;
	typedef stdext::hash_map<std::wstring, JSDelegateMapTier1>	JSDelegateMapTier0;
	typedef JSDelegateMapTier0::iterator						JSDelegateMapIterTier0;
	typedef JSDelegateMapTier0::const_iterator					JSDelegateMapConstIterTier0;
	typedef std::pair<std::wstring, JSDelegateMapTier1>			JSDelegateMapPairTier0;
public:
	//////////////////////////////////////////////////////////////////////////
	// Loads the specified URL.
	// The _Username and _Password parameters are optional and
	// can be used to pass authentication information to the webpage if
	// this is required.
	//////////////////////////////////////////////////////////////////////////
	virtual void LoadURL( const std::string& _URL, const std::string& _Username = "", const std::string& _Password = "" );

	//////////////////////////////////////////////////////////////////////////
	// Loads the specified HTML code.
	//////////////////////////////////////////////////////////////////////////
	virtual void LoadHTML( const std::string& _HTML );

	//////////////////////////////////////////////////////////////////////////
	// Loads the specified file.
	//////////////////////////////////////////////////////////////////////////
	virtual void LoadFile( const std::string& _File );

	//////////////////////////////////////////////////////////////////////////
	// Executes Javascript code in the context of the current page.
	//////////////////////////////////////////////////////////////////////////
	virtual void ExecuteJavascript( const std::string& _Javascript );

	//////////////////////////////////////////////////////////////////////////
	// Calls a Javascript function on the given object with the given
	// arguments.
	// If the function resides in the global namespace, _Object should be
	// empty ("").
	//////////////////////////////////////////////////////////////////////////
	virtual void CallJavascriptFunction( const std::wstring& _FunctionName, const std::wstring& _Object = L"" /* Global scope if empty */, const JSArguments& _Arguments = JSArguments() );

	//////////////////////////////////////////////////////////////////////////
	// Creates a Javascript object.
	//////////////////////////////////////////////////////////////////////////
	virtual void CreateJavascriptObject( const std::wstring& _ObjectName );

	//////////////////////////////////////////////////////////////////////////
	// Destroys a previously created Javascript object.
	//////////////////////////////////////////////////////////////////////////
	virtual void DestroyJavascriptObject( const std::wstring& _ObjectName );

	//////////////////////////////////////////////////////////////////////////
	// Sets a callback name for a Javascript object. This name will be 
	// used to identify the callback when LkWebTabListener::onCallback() is
	// fired.
	//////////////////////////////////////////////////////////////////////////
	virtual void SetJavascriptCallback( const std::wstring& _ObjectName, const std::wstring& _CallbackName );

	//////////////////////////////////////////////////////////////////////////
	// Sets a property of a Javascript object.
	//////////////////////////////////////////////////////////////////////////
	virtual void SetJavascriptProperty( const std::wstring& _ObjectName, const std::wstring& _PropertyName, const JSValue& _Value );

	//////////////////////////////////////////////////////////////////////////
	// Binds a C++ Javascript delegate function to a JS object and callback.
	// The function pointer is stored in set of maps, mapping it to
	// the object and the function name. When the WebViewListener's onCallback
	// function is invoked, the map is checked for any registered functions
	// and these are then executed.
	// Binding a function here will allow Javascript to call it.
	// Example: 
	//		C++:	void MyFunction( const JSArguments& _Args ) {}
	//				BindJSDelegate(L"MyObject", L"MyDelegateFunction", &MyFunction);
	//
	//		JS:		MyObject.MyDelegateFunction();	// Optional arguments.
	//////////////////////////////////////////////////////////////////////////
	virtual void BindJSDelegate( const std::wstring& _ObjectName, const std::wstring& _FunctionName, JSDelegate _CFunction );

	//////////////////////////////////////////////////////////////////////////
	// Returns a C++ Javascript delegate function if one exists for the
	// given object and function name.
	//////////////////////////////////////////////////////////////////////////
	virtual JSDelegate GetJSDelegate( const std::wstring& _ObjectName, const std::wstring& _FunctionName );

	//////////////////////////////////////////////////////////////////////////
	// Utility function that simply calls GetJSDelegate and checks whether
	// it returned an actual function address.
	// Equivalent to doing:
	// JSDelegate f = GetJSDelegate();
	// if (f) f();
	// Returns true if a delegate function was called, false if not.
	//////////////////////////////////////////////////////////////////////////
	virtual bool CallJSDelegate( const std::wstring& _ObjectName, const std::wstring& _FunctionName, const JSArguments& _Args );

	//////////////////////////////////////////////////////////////////////////
	// Stops the current navigation.
	//////////////////////////////////////////////////////////////////////////
	virtual void Stop();

	//////////////////////////////////////////////////////////////////////////
	// Reloads the page.
	//////////////////////////////////////////////////////////////////////////
	virtual void Reload();

	//////////////////////////////////////////////////////////////////////////
	// Returns the current URL of the page.
	//////////////////////////////////////////////////////////////////////////
	virtual const std::string& GetURL() const;

	//////////////////////////////////////////////////////////////////////////
	// Renders this tab to a render buffer.
	//////////////////////////////////////////////////////////////////////////
	virtual void Render();

	//////////////////////////////////////////////////////////////////////////
	// Checks whether the tab wants to be re-rendered.
	//////////////////////////////////////////////////////////////////////////
	virtual bool IsDirty();

	//////////////////////////////////////////////////////////////////////////
	// Checks whether the tab is loading.
	//////////////////////////////////////////////////////////////////////////
	virtual bool IsLoading();

	//////////////////////////////////////////////////////////////////////////
	// Sets whether rendering is done with transparency preserved.
	//////////////////////////////////////////////////////////////////////////
	virtual void SetTransparent( bool _Transparent );

	//////////////////////////////////////////////////////////////////////////
	// Returns whether rendering is done with transparency preserved.
	//////////////////////////////////////////////////////////////////////////
	virtual bool IsTransparent() const;

	virtual f32 GetAlphaAt( int32 _X, int32 _Y ) const;
	virtual f32 GetAlphaAtCursor() const;

	virtual int2 TranslateGlobalMousePositionToLocal( const int2& _GlobalPosition ) const;

	virtual void Resize( int32 _Width, int32 _Height, bool _WaitForRepaint = true, int32 _RepaintTimeoutMs = 300 );

	virtual int32 GetWidth() const;
	virtual int32 GetHeight() const;

	virtual bool IsActive() const;
	virtual void SetActive( bool _Active );

	virtual void SetListener( LkHTMLViewListener* _Listener );

	virtual bool GetAllowHistoryBrowsing() const;
	virtual void SetAllowHistoryBrowsing( bool _Allow );

	virtual void GoToHistoryOffset( int32 _Offset );
private:
	LkHTMLView( Awesomium::WebView* _WebView, LkHTMLCore* _ParentBrowser, int32 _Width, int32 _Height );
	LkHTMLView(); // Private default c-tor.
	~LkHTMLView(); // Private default d-tor. Clients should not have access to it.

	virtual LkHTMLViewListener* _GetListener();

	virtual void _SetLayer( int32 _Layer );
	virtual int32 _GetLayer() const;

	Awesomium::WebView* m_WebView;
	renderer::LkImage* m_RenderImage;
	LkHTMLCore* m_ParentCore;

	JSDelegateMapTier0 m_JSDelegates;

	const Awesomium::RenderBuffer* m_LastRenderbuffer;

	int32 m_Width;
	int32 m_Height;

	bool m_Active;

	int32 m_Layer;

	bool m_AllowHistoryBrowsing;
};

}

#endif
