#pragma once

#ifndef HTMLVIEWLISTENER_H
#define HTMLVIEWLISTENER_H

#include <Awesomium/WebViewListener.h>

namespace loki
{

class LkHTMLViewListener	: public Awesomium::WebViewListener
{
	friend class LkHTMLView;
public:
	LkHTMLViewListener( );	// Yes this should be public... Horrible reasons, just leave it.
	virtual ~LkHTMLViewListener();	// ^

	virtual void onBeginNavigation(Awesomium::WebView* caller, 
									   const std::string& url, 
									   const std::wstring& frameName) ;
		
	virtual void onBeginLoading(Awesomium::WebView* caller, 
								const std::string& url, 
								const std::wstring& frameName, 
								int32 statusCode, 
								const std::wstring& mimeType) ;
	
	virtual void onFinishLoading(Awesomium::WebView* caller) ;
	
	virtual void onCallback(Awesomium::WebView* caller, 
							const std::wstring& objectName, 
							const std::wstring& callbackName, 
							const Awesomium::JSArguments& args) ;
	
	virtual void onReceiveTitle(Awesomium::WebView* caller, 
								const std::wstring& title, 
								const std::wstring& frameName) ;
	
	virtual void onChangeTooltip(Awesomium::WebView* caller, 
								 const std::wstring& tooltip) ;
	
	virtual void onChangeCursor(Awesomium::WebView* caller, 
								Awesomium::CursorType cursor) ;
	
	virtual void onChangeKeyboardFocus(Awesomium::WebView* caller, 
									   bool isFocused) ;
	
	virtual void onChangeTargetURL(Awesomium::WebView* caller, 
								   const std::string& url) ;
	
	virtual void onOpenExternalLink(Awesomium::WebView* caller, 
									const std::string& url, 
									const std::wstring& source) ;

	virtual void onRequestDownload(Awesomium::WebView* caller,
									const std::string& url) ;
	
	virtual void onWebViewCrashed(Awesomium::WebView* caller) ;

	virtual void onPluginCrashed(Awesomium::WebView* caller, 
								 const std::wstring& pluginName) ;

	virtual void onRequestMove(Awesomium::WebView* caller, 
							   int32 x, int32 y) ;
	
	virtual void onGetPageContents(Awesomium::WebView* caller, 
								   const std::string& url, 
								   const std::wstring& contents) ;
	

	virtual void onDOMReady(Awesomium::WebView* caller) ;

	virtual void onRequestFileChooser(Awesomium::WebView* caller,
									  bool selectMultipleFiles,
									  const std::wstring& title,
									  const std::wstring& defaultPath) ;

	virtual void onGetScrollData(Awesomium::WebView* caller,
								 int32 contentWidth,
								 int32 contentHeight,
								 int32 preferredWidth,
								 int32 scrollX,
								 int32 scrollY) ;
	
	virtual void onJavascriptConsoleMessage(Awesomium::WebView* caller,
											const std::wstring& message,
											int32 lineNumber,
											const std::wstring& source) ;

	virtual void onGetFindResults(Awesomium::WebView* caller,
                                  int32 requestID,
                                  int32 numMatches,
                                  const Awesomium::Rect& selection,
                                  int32 curMatch,
                                  bool finalUpdate) ;

	virtual void onUpdateIME(Awesomium::WebView* caller,
                             Awesomium::IMEState imeState,
                             const Awesomium::Rect& caretRect) ;

	virtual void onShowContextMenu(Awesomium::WebView* caller,
                                   int32 mouseX,
								   int32 mouseY,
								   Awesomium::MediaType type,
								   int32 mediaState,
								   const std::string& linkURL,
								   const std::string& srcURL,
								   const std::string& pageURL,
								   const std::string& frameURL,
								   const std::wstring& selectionText,
								   bool isEditable,
								   int32 editFlags) ;

	virtual void onRequestLogin(Awesomium::WebView* caller,
                                   int32 requestID,
								   const std::string& requestURL,
								   bool isProxy,
								   const std::wstring& hostAndPort,
								   const std::wstring& scheme,
								   const std::wstring& realm) ;

	virtual void onChangeHistory(Awesomium::WebView* caller,
									int32 backCount,
									int32 forwardCount) ;

	virtual void onFinishResize(Awesomium::WebView* caller,
									int32 width,
									int32 height) ;

	virtual void onShowJavascriptDialog(Awesomium::WebView* caller,
											int32 requestID,
											int32 dialogFlags,
											const std::wstring& message,
											const std::wstring& defaultPrompt,
											const std::string& frameURL);
private:
	LkHTMLViewListener( LkHTMLView* _WebTab );

	LkHTMLView* m_WebTab;
};

}

#endif
