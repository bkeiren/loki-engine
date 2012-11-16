#pragma once

#ifndef NULLHTMLVIEW_H
#define NULLHTMLVIEW_H

#include "core/html/htmlview/htmlview.h"

namespace loki
{

namespace
{
	void DummyJSDelegate( const JSArguments& ) {}
}

class NullHTMLView	: public LkHTMLView
{
	friend class NullHTMLCore;
public:
	void LoadURL( const std::string& _URL, const std::string& _Username = "", const std::string& _Password = "" ) {}
	void LoadHTML( const std::string& _HTML ) {}
	void LoadFile( const std::string& _File ) {}
	void ExecuteJavascript( const std::string& _Javascript ) {}
	void CallJavascriptFunction( const std::wstring& _FunctionName, const std::wstring& _Object = L"" /* Global scope if empty */, const JSArguments& _Arguments = JSArguments() ) {}
	void CreateJavascriptObject( const std::wstring& _ObjectName ) {}
	void DestroyJavascriptObject( const std::wstring& _ObjectName ) {}
	void SetJavascriptCallback( const std::wstring& _ObjectName, const std::wstring& _CallbackName ) {}
	void SetJavascriptProperty( const std::wstring& _ObjectName, const std::wstring& _PropertyName, const JSValue& _Value ) {}
	void BindJSDelegate( const std::wstring& _ObjectName, const std::wstring& _FunctionName, JSDelegate _CFunction ) {}
	JSDelegate GetJSDelegate( const std::wstring& _ObjectName, const std::wstring& _FunctionName ) { return &DummyJSDelegate; }
	bool CallJSDelegate( const std::wstring& _ObjectName, const std::wstring& _FunctionName, const JSArguments& _Args ) { return true; }
	void Stop() {}
	void Reload() {}
	const std::string& GetURL() const { static std::string URL = std::string("NullHTMLView"); return URL; }
	void Render() {}
	bool IsDirty() { return false; }
	bool IsLoading() { return false; }
	void SetTransparent( bool _Transparent ) {}
	bool IsTransparent() const { return false; }
	f32 GetAlphaAt( int32 _X, int32 _Y ) const { return 0.0f; }
	f32 GetAlphaAtCursor() const { return 0.0f; }
	int2 TranslateGlobalMousePositionToLocal( const int2& _GlobalPosition ) const { return int2(0, 0); }
	void Resize( int32 _Width, int32 _Height, bool _WaitForRepaint = true, int32 _RepaintTimeoutMs = 300 ) {}
	int32 GetWidth() const { return 1; }
	int32 GetHeight() const { return 1; }
	bool IsActive() const { return false; }
	void SetActive( bool _Active ) {}
	void SetListener( LkHTMLViewListener* _Listener ) {}
	bool GetAllowHistoryBrowsing() const { return false; }
	void SetAllowHistoryBrowsing( bool _Allow ) {}
	void GoToHistoryOffset( int32 _Offset ) {}
private:
	NullHTMLView( Awesomium::WebView* _WebView, LkHTMLCore* _ParentBrowser, int32 _Width, int32 _Height ) {}
	NullHTMLView() {}
	~NullHTMLView() {}

	LkHTMLViewListener* _GetListener() { return 0; }
	void _SetLayer( int32 _Layer ) {}
	int32 _GetLayer() const { return 0; }
};

}

#endif
