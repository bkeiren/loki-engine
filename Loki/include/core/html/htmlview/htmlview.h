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
	void LoadURL( const std::string& _URL, const std::string& _Username = "", const std::string& _Password = "" );

	//////////////////////////////////////////////////////////////////////////
	// Loads the specified HTML code.
	//////////////////////////////////////////////////////////////////////////
	void LoadHTML( const std::string& _HTML );

	//////////////////////////////////////////////////////////////////////////
	// Loads the specified file.
	//////////////////////////////////////////////////////////////////////////
	void LoadFile( const std::string& _File );

	//////////////////////////////////////////////////////////////////////////
	// Executes Javascript code in the context of the current page.
	//////////////////////////////////////////////////////////////////////////
	void ExecuteJavascript( const std::string& _Javascript );

	//////////////////////////////////////////////////////////////////////////
	// Calls a Javascript function on the given object with the given
	// arguments.
	// If the function resides in the global namespace, _Object should be
	// empty ("").
	//////////////////////////////////////////////////////////////////////////
	void CallJavascriptFunction( const std::wstring& _FunctionName, const std::wstring& _Object = L"" /* Global scope if empty */, const JSArguments& _Arguments = JSArguments() );

	//////////////////////////////////////////////////////////////////////////
	// Creates a Javascript object.
	//////////////////////////////////////////////////////////////////////////
	void CreateJavascriptObject( const std::wstring& _ObjectName );

	//////////////////////////////////////////////////////////////////////////
	// Destroys a previously created Javascript object.
	//////////////////////////////////////////////////////////////////////////
	void DestroyJavascriptObject( const std::wstring& _ObjectName );

	//////////////////////////////////////////////////////////////////////////
	// Sets a callback name for a Javascript object. This name will be 
	// used to identify the callback when LkWebTabListener::onCallback() is
	// fired.
	//////////////////////////////////////////////////////////////////////////
	void SetJavascriptCallback( const std::wstring& _ObjectName, const std::wstring& _CallbackName );

	//////////////////////////////////////////////////////////////////////////
	// Sets a property of a Javascript object.
	//////////////////////////////////////////////////////////////////////////
	void SetJavascriptProperty( const std::wstring& _ObjectName, const std::wstring& _PropertyName, const JSValue& _Value );

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
	void BindJSDelegate( const std::wstring& _ObjectName, const std::wstring& _FunctionName, JSDelegate _CFunction );

	//////////////////////////////////////////////////////////////////////////
	// Returns a C++ Javascript delegate function if one exists for the
	// given object and function name.
	//////////////////////////////////////////////////////////////////////////
	JSDelegate GetJSDelegate( const std::wstring& _ObjectName, const std::wstring& _FunctionName );

	//////////////////////////////////////////////////////////////////////////
	// Utility function that simply calls GetJSDelegate and checks whether
	// it returned an actual function address.
	// Equivalent to doing:
	// JSDelegate f = GetJSDelegate();
	// if (f) f();
	// Returns true if a delegate function was called, false if not.
	//////////////////////////////////////////////////////////////////////////
	bool CallJSDelegate( const std::wstring& _ObjectName, const std::wstring& _FunctionName, const JSArguments& _Args );

	//////////////////////////////////////////////////////////////////////////
	// Stops the current navigation.
	//////////////////////////////////////////////////////////////////////////
	void Stop();

	//////////////////////////////////////////////////////////////////////////
	// Reloads the page.
	//////////////////////////////////////////////////////////////////////////
	void Reload();

	//////////////////////////////////////////////////////////////////////////
	// Returns the current URL of the page.
	//////////////////////////////////////////////////////////////////////////
	const std::string& GetURL() const;

	//////////////////////////////////////////////////////////////////////////
	// Renders this tab to a render buffer.
	//////////////////////////////////////////////////////////////////////////
	void Render();

	//////////////////////////////////////////////////////////////////////////
	// Checks whether the tab wants to be re-rendered.
	//////////////////////////////////////////////////////////////////////////
	bool IsDirty();

	//////////////////////////////////////////////////////////////////////////
	// Checks whether the tab is loading.
	//////////////////////////////////////////////////////////////////////////
	bool IsLoading();

	//////////////////////////////////////////////////////////////////////////
	// Sets whether rendering is done with transparency preserved.
	//////////////////////////////////////////////////////////////////////////
	void SetTransparent( bool _Transparent );

	//////////////////////////////////////////////////////////////////////////
	// Returns whether rendering is done with transparency preserved.
	//////////////////////////////////////////////////////////////////////////
	bool IsTransparent() const;

	float GetAlphaAt( int _X, int _Y ) const;
	float GetAlphaAtCursor() const;

	glm::int2 TranslateGlobalMousePositionToLocal( const glm::int2& _GlobalPosition ) const;

	void Resize( int _Width, int _Height, bool _WaitForRepaint = true, int _RepaintTimeoutMs = 300 );

	int GetWidth() const;
	int GetHeight() const;

	bool IsActive() const;
	void SetActive( bool _Active );

	void SetListener( LkHTMLViewListener* _Listener );

	bool GetAllowHistoryBrowsing() const;
	void SetAllowHistoryBrowsing( bool _Allow );

	void GoToHistoryOffset( int _Offset );
private:
	LkHTMLView( Awesomium::WebView* _WebView, LkHTMLCore* _ParentBrowser, int _Width, int _Height );
	LkHTMLView(); // Private default c-tor.
	~LkHTMLView(); // Private default d-tor. Clients should not have access to it.

	LkHTMLViewListener* _GetListener();

	void _SetLayer( int _Layer );
	int _GetLayer() const;

	Awesomium::WebView* m_WebView;
	renderer::LkImage* m_RenderImage;
	LkHTMLCore* m_ParentCore;

	JSDelegateMapTier0 m_JSDelegates;

	const Awesomium::RenderBuffer* m_LastRenderbuffer;

	int m_Width;
	int m_Height;

	bool m_Active;

	int m_Layer;

	bool m_AllowHistoryBrowsing;
};

}

#endif
