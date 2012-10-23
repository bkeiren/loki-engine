#include "core/html/htmlview/htmlviewlistener.h"
#include "core/html/htmlview/htmlview.h"

namespace loki
{

LkHTMLViewListener::LkHTMLViewListener( LkHTMLView* _WebTab )	:
	m_WebTab(_WebTab)
{
	assert(m_WebTab != 0);
}

LkHTMLViewListener::LkHTMLViewListener()	:
	m_WebTab(0)
{
	//ILLEGAL_CTOR_ERROR("HTMLViewListener");	// Not anymore...
}

LkHTMLViewListener::~LkHTMLViewListener()
{

}

void LkHTMLViewListener::onBeginNavigation(Awesomium::WebView* caller, 
							   const std::string& url, 
							   const std::wstring& frameName)
{

}

void LkHTMLViewListener::onBeginLoading(Awesomium::WebView* caller, 
							const std::string& url, 
							const std::wstring& frameName, 
							int32 statusCode, 
							const std::wstring& mimeType)
{

}

void LkHTMLViewListener::onFinishLoading(Awesomium::WebView* caller)
{

}

void LkHTMLViewListener::onCallback(Awesomium::WebView* caller, 
						const std::wstring& objectName, 
						const std::wstring& callbackName, 
						const Awesomium::JSArguments& args)
{
	if (!m_WebTab->CallJSDelegate(objectName, callbackName, args))
	{
		std::wstring str = L"Awesomium JS Unhandled Callback: ";
		str += objectName;
		str += L".";
		str += callbackName;
		str += L"( ";
		for (Awesomium::JSArguments::const_iterator it = args.begin(); it != args.end(); ++it)
		{
			if (it != args.begin())
			{
				str += L", ";
			}
			std::wstring type;
			if (it->isInteger())
			{
				type = L"int32";
			}
			else if (it->isBoolean())
			{
				type = L"bool";
			}
			else if (it->isDouble())
			{
				type = L"double";
			}
			else if (it->isString())
			{
				type = L"string";
			}
			else if (it->isNull())
			{
				type = L"null";
			}
			else if (it->isObject())
			{
				type = L"object";
			}
			else if (it->isArray())
			{
				type = L"array";
			}

			str += L"(";
			str += type;
			str += L") ";
			str += (*it).toString();
		}
		str += L" );";
		LOG(VL_NORMAL, util::ToMultiByteString(str).c_str());
	}
}

void LkHTMLViewListener::onReceiveTitle(Awesomium::WebView* caller, 
							const std::wstring& title, 
							const std::wstring& frameName)
{

}

void LkHTMLViewListener::onChangeTooltip(Awesomium::WebView* caller, 
									   const std::wstring& tooltip)
{

}

void LkHTMLViewListener::onChangeCursor(Awesomium::WebView* caller, 
									  Awesomium::CursorType cursor)
{

}

void LkHTMLViewListener::onChangeKeyboardFocus(Awesomium::WebView* caller, 
											 bool isFocused)
{

}

void LkHTMLViewListener::onChangeTargetURL(Awesomium::WebView* caller, 
										 const std::string& url)
{

}

void LkHTMLViewListener::onOpenExternalLink(Awesomium::WebView* caller, 
								const std::string& url, 
								const std::wstring& source)
{

}

void LkHTMLViewListener::onRequestDownload(Awesomium::WebView* caller,
										 const std::string& url)
{

}

void LkHTMLViewListener::onWebViewCrashed(Awesomium::WebView* caller)
{

}

void LkHTMLViewListener::onPluginCrashed(Awesomium::WebView* caller, 
									   const std::wstring& pluginName)
{

}

void LkHTMLViewListener::onRequestMove(Awesomium::WebView* caller, 
									 int32 x, int32 y)
{

}

void LkHTMLViewListener::onGetPageContents(Awesomium::WebView* caller, 
							   const std::string& url, 
							   const std::wstring& contents)
{

}


void LkHTMLViewListener::onDOMReady(Awesomium::WebView* caller)
{

}

void LkHTMLViewListener::onRequestFileChooser(Awesomium::WebView* caller,
								  bool selectMultipleFiles,
								  const std::wstring& title,
								  const std::wstring& defaultPath)
{

}

void LkHTMLViewListener::onGetScrollData(Awesomium::WebView* caller,
							 int32 contentWidth,
							 int32 contentHeight,
							 int32 preferredWidth,
							 int32 scrollX,
							 int32 scrollY)
{

}

void LkHTMLViewListener::onJavascriptConsoleMessage(Awesomium::WebView* caller,
										const std::wstring& message,
										int32 lineNumber,
										const std::wstring& source)
{
	std::string str = "Awesomium JS: ";
	str += util::ToMultiByteString(message);
	str += " - Line: ";
	str += lineNumber;
	if (source.length() > 0)
	{
		str += " (Source: ";
		str += util::ToMultiByteString(source);
	}
	LOG(VL_NORMAL, str.c_str());
}

void LkHTMLViewListener::onGetFindResults(Awesomium::WebView* caller,
							  int32 requestID,
							  int32 numMatches,
							  const Awesomium::Rect& selection,
							  int32 curMatch,
							  bool finalUpdate)
{

}

void LkHTMLViewListener::onUpdateIME(Awesomium::WebView* caller,
						 Awesomium::IMEState imeState,
						 const Awesomium::Rect& caretRect)
{

}

void LkHTMLViewListener::onShowContextMenu(Awesomium::WebView* caller,
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
							   int32 editFlags)
{

}

void LkHTMLViewListener::onRequestLogin(Awesomium::WebView* caller,
							int32 requestID,
							const std::string& requestURL,
							bool isProxy,
							const std::wstring& hostAndPort,
							const std::wstring& scheme,
							const std::wstring& realm)
{

}

void LkHTMLViewListener::onChangeHistory(Awesomium::WebView* caller,
							 int32 backCount,
							 int32 forwardCount)
{

}

void LkHTMLViewListener::onFinishResize(Awesomium::WebView* caller,
							int32 width,
							int32 height)
{

}

void LkHTMLViewListener::onShowJavascriptDialog(Awesomium::WebView* caller,
									int32 requestID,
									int32 dialogFlags,
									const std::wstring& message,
									const std::wstring& defaultPrompt,
									const std::string& frameURL)
{

}

}
