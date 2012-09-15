#include "core/html/htmlview/htmlview.h"
#include "core/html/htmlcore.h"
#include "core/html/htmlview/htmlviewlistener.h"
#include <Awesomium/WebCore.h>
#include "core/renderer/renderer.h"
#include "core/renderer/image/image.h"
#include "util/clock/clock.h"
#include "core/input/input.h"

namespace loki
{

LkHTMLView::LkHTMLView( Awesomium::WebView* _WebView, LkHTMLCore* _ParentBrowser, int _Width, int _Height )	:
	m_WebView(NULL),
	m_ParentCore(NULL),
	m_Width(_Width),
	m_Height(_Height),
	m_RenderImage(new renderer::LkImage(0, vec2(0.0f, 0.0f), vec2(_Width, _Height), false, true)),
	m_LastRenderbuffer(0),
	m_Active(true),
	m_Layer(0),
	m_AllowHistoryBrowsing(true)
{
	m_WebView = _WebView;
	m_ParentCore = _ParentBrowser;

	m_WebView->setTransparent(true);

	// Call _GetListener() in order to instantiate a LkWebTabListener object.
	_GetListener();

	if (!m_WebView)
	{
		LOG(VL_ERROR, "HTMLView::HTMLView: Created instance with null WebView");
	}

	if (!m_ParentCore)
	{
		LOG(VL_ERROR, "HTMLView::HTMLView: Created instance with null parent browser");
	}

	m_RenderImage->SetFlipY(true);
	m_RenderImage->Set3D(false);
	
	m_WebView->focus();
}

LkHTMLView::LkHTMLView()
{
	LOG(VL_ERROR, "HTMLView::HTMLView: Created instance with invalid c-tor");
}

LkHTMLView::~LkHTMLView()
{
	delete m_RenderImage;
	m_RenderImage = NULL;

	// Queues the web view for deletion.
	m_WebView->destroy();
}

void LkHTMLView::LoadURL( const std::string& _URL, const std::string& _Username /* =  */, const std::string& _Password /* = */ )
{
	m_WebView->loadURL(_URL, L"", _Username, _Password);
}

void LkHTMLView::LoadHTML( const std::string& _HTML )
{
	m_WebView->loadHTML(_HTML);
}

void LkHTMLView::LoadFile( const std::string& _File )
{
	m_WebView->loadFile(_File);
}

void LkHTMLView::ExecuteJavascript( const std::string& _Javascript )
{
	if (m_WebView->isLoadingPage())
	{
		return;
	}
	m_WebView->executeJavascript(_Javascript);
}

void LkHTMLView::CallJavascriptFunction( const std::wstring& _FunctionName, const std::wstring& _Object /* = L"" */, const JSArguments& _Arguments /* = JSArguments() */ )
{
	if (m_WebView->isLoadingPage())
	{
		return;
	}
	m_WebView->callJavascriptFunction(_Object, _FunctionName, _Arguments);
}

void LkHTMLView::CreateJavascriptObject( const std::wstring& _ObjectName )
{
	m_WebView->createObject(_ObjectName);
}

void LkHTMLView::DestroyJavascriptObject( const std::wstring& _ObjectName )
{
	m_WebView->destroyObject(_ObjectName);
}

void LkHTMLView::SetJavascriptCallback( const std::wstring& _ObjectName, const std::wstring& _CallbackName )
{
	m_WebView->setObjectCallback(_ObjectName, _CallbackName);
}

void LkHTMLView::SetJavascriptProperty( const std::wstring& _ObjectName, const std::wstring& _PropertyName, const JSValue& _Value )
{
	m_WebView->setObjectProperty(_ObjectName, _PropertyName, _Value);
}

void LkHTMLView::BindJSDelegate( const std::wstring& _ObjectName, const std::wstring& _FunctionName, JSDelegate _CFunction )
{
	(m_JSDelegates[_ObjectName])[_FunctionName] = _CFunction;
	SetJavascriptCallback(_ObjectName, _FunctionName);
}

JSDelegate LkHTMLView::GetJSDelegate( const std::wstring& _ObjectName, const std::wstring& _FunctionName )
{
	JSDelegateMapConstIterTier0 p0 = m_JSDelegates.find(_ObjectName);
	if (p0 != m_JSDelegates.end())
	{
		JSDelegateMapConstIterTier1 p1 = (*p0).second.find(_FunctionName);
		if (p1 != (*p0).second.end())
		{
			return (*p1).second;
		}
	}
	return 0;
}

bool LkHTMLView::CallJSDelegate( const std::wstring& _ObjectName, const std::wstring& _FunctionName, const JSArguments& _Args )
{
	JSDelegate f = GetJSDelegate(_ObjectName, _FunctionName);
	if (f)
	{
		f(_Args);
		return true;
	}
	return false;
}

void LkHTMLView::Stop()
{
	m_WebView->stop();
}

void LkHTMLView::Reload()
{
	m_WebView->reload();
}

const std::string& LkHTMLView::GetURL() const
{
	return m_WebView->getURL();
}

void LkHTMLView::Render()
{
	// If we're still loading, do nothing.
	// If the webview hasn't been created, do nothing.
	// If the webview isn't dirty, don't render.
	if (!m_WebView || IsLoading())
	{
		return;
	}

	if (IsDirty())
	{
		Awesomium::Rect dirtybounds = m_WebView->getDirtyBounds();
		//util::Clock clock;
		//clock.Start();
		m_LastRenderbuffer = m_WebView->render();
		
		if (!m_LastRenderbuffer)
		{
			return;
		}
		//LOG(VL_NORMAL, "Render Time: %f", clock.Lap_ms());

		if (m_RenderImage->GetNumTextureIndices() == 0)
		{
			//clock.Start();
			m_RenderImage->AddTextureFromMemory((void*)(m_LastRenderbuffer->buffer), m_LastRenderbuffer->width, m_LastRenderbuffer->height, renderer::RBIF_RGBA, renderer::RBF_BGRA, renderer::RBT_UNSIGNED_INT_8_8_8_8_REV);

			//LOG(VL_NORMAL, "AddTextureFromMemory: %f", clock.Lap_ms());
		}
		else
		{
			//clock.Start();
//#define ONLY_DIRTY_RECT
#ifdef ONLY_DIRTY_RECT
			void* bufferaddress = (void*)(((char*)(renderBuffer->buffer)) + ((dirtybounds.x + (dirtybounds.y * renderBuffer->width)) * 4));

			m_RenderImage->SetSubTextureFromMemory(bufferaddress,	dirtybounds.x, 
																	dirtybounds.y, 
																	dirtybounds.width, 
																	dirtybounds.height, 
																	renderer::RBF_BGRA, renderer::RBT_UNSIGNED_INT_8_8_8_8_REV);
#else
			m_RenderImage->SetSubTextureFromMemory((void*)m_LastRenderbuffer->buffer, 0, 0, m_LastRenderbuffer->width, m_LastRenderbuffer->height, renderer::RBF_BGRA, renderer::RBT_UNSIGNED_INT_8_8_8_8_REV);
#endif

			//LOG(VL_NORMAL, "SetSubTextureFromMemory: %f", clock.Lap_ms());
		}
	}
	m_RenderImage->Render();
}

bool LkHTMLView::IsDirty()
{
	return m_WebView->isDirty();
}

bool LkHTMLView::IsLoading()
{
	return m_WebView->isLoadingPage();
}

void LkHTMLView::SetTransparent( bool _Transparent )
{
	m_WebView->setTransparent(_Transparent);
}

bool LkHTMLView::IsTransparent() const
{
	return m_WebView->isTransparent();
}

float LkHTMLView::GetAlphaAt( int _X, int _Y ) const
{
	if (m_LastRenderbuffer)
	{
		//return ((float)m_LastRenderbuffer->getAlphaAtPoint(_X, _Y) / 255.0f);	// This does not actually work correctly for some reason, so
																				// we just access the buffer manually.
		return (((float)m_LastRenderbuffer->buffer[((_X + _Y * m_LastRenderbuffer->width) * 4) + 3]) / 255.0f);
	}
	return 0.0f;
}

float LkHTMLView::GetAlphaAtCursor()	const
{
	int2 m = TranslateGlobalMousePositionToLocal(g_Input->GetMousePosition());
	return GetAlphaAt(m.x, m.y);
}

int2 LkHTMLView::TranslateGlobalMousePositionToLocal( const int2& _GlobalPosition ) const
{
	return (g_Input->GetMousePosition() - int2(m_RenderImage->GetAbsolutePosition()));
}

void LkHTMLView::Resize( int _Width, int _Height, bool _WaitForRepaint /*= true*/, int _RepaintTimeoutMs /*= 300*/ )
{
	m_WebView->resize(m_Width, m_Height, _WaitForRepaint, _RepaintTimeoutMs);
	m_Width = _Width;
	m_Height = _Height;

	m_RenderImage->SetAbsoluteSize(vec2(m_Width, m_Height));
}

int LkHTMLView::GetWidth() const
{
	return m_Width;
}

int LkHTMLView::GetHeight() const
{
	return m_Height;
}

bool LkHTMLView::IsActive() const
{
	return m_Active;
}

void LkHTMLView::SetActive( bool _Active )
{
	m_Active = _Active;
}

void LkHTMLView::SetListener( LkHTMLViewListener* _Listener )
{
	_Listener->m_WebTab = this;
	m_WebView->setListener(_Listener);
}

bool LkHTMLView::GetAllowHistoryBrowsing() const
{
	return m_AllowHistoryBrowsing;
}

void LkHTMLView::SetAllowHistoryBrowsing( bool _Allow )
{
	m_AllowHistoryBrowsing = _Allow;
}

void LkHTMLView::GoToHistoryOffset( int _Offset )
{
	m_WebView->goToHistoryOffset(_Offset);
}

LkHTMLViewListener* LkHTMLView::_GetListener()
{
	LkHTMLViewListener* listener = (LkHTMLViewListener*)m_WebView->getListener();
	if (!listener)
	{
		listener = new LkHTMLViewListener(this);
		m_WebView->setListener(listener);
	}
	return listener;
}

void LkHTMLView::_SetLayer( int _Layer )
{
	m_Layer = _Layer;
}

int LkHTMLView::_GetLayer() const
{
	return m_Layer;
}

}
