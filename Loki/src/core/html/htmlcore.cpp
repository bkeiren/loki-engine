#include <Awesomium/WebCore.h>
#include "core/html/htmlcore.h"
#include "core/html/htmlview/htmlview.h"
#include "core/renderer/renderer.h"
#include "core/renderer/image/image.h"

#include "core/engine.h"
#include "core/game/game.h"
#include "core/input/input.h"
#include "core/renderer/debugrenderer.h"

namespace loki
{

LkHTMLCore* g_HTMLCore = NULL;

LkHTMLCore::LkHTMLCore()	:
	m_WebCore(NULL),
	m_HTMLViewInFocus(NULL),
	m_InputDetected(false)
{
	_Init();
}

LkHTMLCore::~LkHTMLCore()
{
	_Terminate();
}

bool LkHTMLCore::_Init()
{
	SubscribeToEvent(EVENT_ONUPDATE);

	// Create a webcore with custom settings.
	Awesomium::WebCoreConfig config;
	
	// This CSS file can be used to alter the global default appearance of things.
	// It is mainly used for altering the look of scrollbars though.
	config.setCustomCSSFromFile("resources//ui//customcss.css");

	m_WebCore = new Awesomium::WebCore(config);

	if (!m_WebCore)
	{
		LOG(VL_ERROR, "HTMLCore::Init: Failed to create WebCore instance");
		return false;
	}

	LOG(VL_ALWAYS, "HTMLCore::Init: Initialized");
	return true;
}

void LkHTMLCore::_Terminate()
{
	for (HTMLViewsIter it = m_HTMLViews.begin(); it != m_HTMLViews.end(); ++it)
	{
		delete (*it);
	}
	m_HTMLViews.clear();

	// Awesomium schedules clean ups through atexit(), which is
	// important to note. If atexit() is overridden by anything other
	// than Awesomium, not everything might be cleaned up properly.
	delete m_WebCore;

	LOG(VL_ALWAYS, "HTMLCore::Terminate: Terminated");
}

void LkHTMLCore::_OnEvent( const LkEvent& _Event )
{
	switch (_Event.GetEventType())
	{
	case EVENT_ONUPDATE:
		{
			// By default, we assume we've detected no input.
			m_InputDetected = false;

			// If user pressed a mouse button or scrolled...
			if (g_Input->Get(BUTTON_MOUSELEFT) || g_Input->Get(BUTTON_MOUSEMIDDLE) || g_Input->Get(BUTTON_MOUSERIGHT) || g_Input->GetMouseWheelDelta() != 0.0f)
			{
				LkHTMLView* view = _GetTopMostViewUnderMouse(true, 0.0f);
				
				if (m_HTMLViewInFocus != view)
				{
					// Unfocus the previous view if it exists.
					if (m_HTMLViewInFocus)
					{
						m_HTMLViewInFocus->m_WebView->unfocus();
					}

					// Focus the new view.
					if (view)
					{
						view->m_WebView->focus();
						//LOG(VL_NORMAL, "New focus");
					}
// 					else
// 					{
// 						LOG(VL_NORMAL, "No focus");
// 					}
					m_HTMLViewInFocus = view;
				}

				if (view)
				{
					m_InputDetected = true;
				}
			}

			m_WebCore->update();

			if (m_HTMLViewInFocus)
			{
				//LOG(VL_NORMAL, "Alpha: %f", m_HTMLViewInFocus->GetAlphaAtCursor());

				switch (g_Input->Get(BUTTON_MOUSELEFT))
				{
				case KEYSTATE_PRESSED:
					{
						m_HTMLViewInFocus->m_WebView->injectMouseDown(Awesomium::LEFT_MOUSE_BTN);
						break;
					}
				case KEYSTATE_RELEASED:
					{
						m_HTMLViewInFocus->m_WebView->injectMouseUp(Awesomium::LEFT_MOUSE_BTN);
						break;
					}
				}

				switch (g_Input->Get(BUTTON_MOUSEMIDDLE))
				{
				case KEYSTATE_PRESSED:
					{
						m_HTMLViewInFocus->m_WebView->injectMouseDown(Awesomium::MIDDLE_MOUSE_BTN);
						break;
					}
				case KEYSTATE_RELEASED:
					{
						m_HTMLViewInFocus->m_WebView->injectMouseUp(Awesomium::MIDDLE_MOUSE_BTN);
						break;
					}
				}

				switch (g_Input->Get(BUTTON_MOUSERIGHT))
				{
				case KEYSTATE_PRESSED:
					{
						m_HTMLViewInFocus->m_WebView->injectMouseDown(Awesomium::RIGHT_MOUSE_BTN);
						break;
					}
				case KEYSTATE_RELEASED:
					{
						m_HTMLViewInFocus->m_WebView->injectMouseUp(Awesomium::RIGHT_MOUSE_BTN);
						break;
					}
				}

				f32 d = g_Input->GetMouseWheelDelta();
				if (d != 0.0f)
				{
					m_HTMLViewInFocus->m_WebView->injectMouseWheel((int32)(d * 100), 0);	// (100 pixels per 'tick' of the wheel).
				}

				if (m_HTMLViewInFocus->GetAllowHistoryBrowsing())
				{
					if (KEY_RELEASED(BUTTON_5))
					{
						m_HTMLViewInFocus->GoToHistoryOffset(-1);
					}
					else if (KEY_RELEASED(BUTTON_6))
					{
						m_HTMLViewInFocus->GoToHistoryOffset(1);
					}
				}
			}

			// Mouse move-events need to be injected into every view.
			if (g_Input->GetMouseMoved())
			{
				for (HTMLViewsConstIter it = m_HTMLViews.begin(); it != m_HTMLViews.end(); ++it)
				{
					LkHTMLView* view = (*it);

					if (!view->m_RenderImage->Is3D())
					{
						int2 mousepos = view->TranslateGlobalMousePositionToLocal(g_Input->GetMousePosition());

						view->m_WebView->injectMouseMove(mousepos.x, mousepos.y);
					}
//					// NOTE: EXPERIMENTAL!
// 					else
// 					{
// 						int2 mousepos = g_Input->GetMousePosition();
// 
// 						// NOTE: injectMouseMove expects x and y coordinates relative to the webtab.
// 
// 						vec3 pos = view->m_RenderImage->GetRelativePosition();
// 						pos = ((vec3(pos.x, 1.0f - pos.y, pos.z) + vec3(view->m_RenderImage->GetRelativeAnchorOffset(), 0.0f)) * 2.0f) - 1.0f;
// 
// 						vec2 size = view->m_RenderImage->GetRelativeSize() * 2.0f;			
// 
// 						// Calculate the world-space position of the mouse cursor (On the near plane).
// 						CameraComponent* cam = CameraComponent::GetActiveCamera();
// 						Transform& comp = cam->GetEntity()->GetTransform();
// 
// 						// Normalized vector indicating the direction from the camera origin to the cursor's position on the near-plane.
// 						vec3 r = cam->GetCameraToViewportVector(mousepos);
// 						vec3 r_origin = comp->GetPosition();
// 
// 						// 3 corner points in model space of the quad.
// 						vec3 PA = vec3(pos.x, pos.y + size.y, pos.z);
// 						vec3 PC = vec3(pos.x + size.x, pos.y, pos.z);
// 						vec3 PD = vec3(pos.x, pos.y, pos.z);
// 
// 						// The quad's normal and position in world-space.
// 						vec3 planenormal = vec3(view->m_RenderImage->GetModelMatrix() * vec4(math::cross(PC - PD, PA - PD), 1.0f));
// 						vec3 planepos = vec3(view->m_RenderImage->GetModelMatrix() * vec4(pos, 1.0f));
// 
// 						// Intersect the ray with the quad to find the intersection distance.
// 						f32 rayDirDotNormal = math::dot(r, planenormal);
// 						if (rayDirDotNormal != 0) 
// 						{
// 							// Intersection.
// 							f32 dist = math::dot(planepos - r_origin, planenormal) / rayDirDotNormal;
// 
// 							vec3 intersectpos = r_origin + (r * dist);
// 
// 							//renderer::debug::DrawAxes(point3D, quat(), 1.0f, false);
// 							renderer::debug::DrawLine3D(r_origin, r_origin + (r * dist), false, vec3(1.0f, 0.0f, 0.0f));
// 
// 							// Now transform intersectpos to model space by multiplying the inverse model matrix with intersectpos.
// 							vec3 modelspacepos = vec3(math::inverse(view->m_RenderImage->GetModelMatrix()) * vec4(intersectpos, 1.0f));
// 
// 							// Yay.
// 
// 							int32 x = int32(math::clamp(modelspacepos.x / size.x, 0.0f, 1.0f) * view->m_RenderImage->GetWidth());
// 							int32 y = int32(math::clamp(modelspacepos.y / size.y, 0.0f, 1.0f) * view->m_RenderImage->GetHeight());
// 							view->m_WebView->injectMouseMove(x, y);
// 							LOG(VL_NORMAL, "x: %i\ty: %i", x, y);
// 						}
// 						// No intersection.
// 					}
				}
			}

			break;
		}
	}
}

LkHTMLView* LkHTMLCore::CreateView( int32 _Width, int32 _Height, int32 _Layer /*= 0*/ )
{
	LkHTMLView* view = new LkHTMLView(m_WebCore->createWebView(_Width, _Height), this, _Width, _Height);

	view->_SetLayer(_Layer);

	if (m_HTMLViews.empty())
	{
		m_HTMLViews.push_back(view);
	}
	else
	{
		bool inserted = false;
		for (HTMLViewsIter it = m_HTMLViews.begin(); it != m_HTMLViews.end(); ++it)
		{
			// If the current element's layer index during iteration equals the layer index, we insert here and move all subsequent
			// views down by one layer.
			// If the current element's layer index during iteration is greater than the layer index,
			// we can insert here. No need to move all subsequent layers because there is some room
			// between those and the new view.
			if ((*it)->_GetLayer() >= _Layer)
			{
				it = m_HTMLViews.insert(it, view);
				inserted = true;
				
				if ((*it)->_GetLayer() == _Layer && it != m_HTMLViews.end())
				{
					// Move all subsequent layers down by one.
					for (HTMLViewsIter it2 = ++it; it2 != m_HTMLViews.end(); ++it2)
					{
						(*it2)->_SetLayer((*it2)->_GetLayer() - 1);
					}
				}
			}
			// If the current element's layer index during iteration is less than the layer index,
			// we just continue iterating because the higher ones are further down the list.
		}

		if (!inserted)
		{
			// Just add to the end. This is reached if none of we passed all elements with a layer that was less
			// than the layer index, and did not encounter any layers of equal or greater index.
			m_HTMLViews.push_back(view);
		}
	}

	return view;
}

void LkHTMLCore::DestroyView( LkHTMLView** _Tab )
{
	if ((*_Tab) == m_HTMLViewInFocus)
	{
		m_HTMLViewInFocus = 0;
	}

	m_HTMLViews.remove((*_Tab));
	delete (*_Tab);
	*_Tab = 0;	// NOTE: This doesn't actually set the pointer to 0, but rather to 1? :/
}

void LkHTMLCore::InjectKeyboardEvent( UINT _Msg, WPARAM _WParam, LPARAM _LParam )
{
	if (m_HTMLViewInFocus)
	{
		m_HTMLViewInFocus->m_WebView->injectKeyboardEvent(Awesomium::WebKeyboardEvent(_Msg, _WParam, _LParam));
	}
}

void LkHTMLCore::SetViewInFocus( LkHTMLView* _View )
{
	m_HTMLViewInFocus = _View;
}

LkHTMLView* LkHTMLCore::GetViewInFocus() const
{
	return m_HTMLViewInFocus;
}

bool LkHTMLCore::GetInputDetected() const
{
	return m_InputDetected;
}

LkHTMLView* LkHTMLCore::_GetTopMostViewUnderMouse( bool _ConsiderTransparency /*= true*/, f32 _AlphaThreshold /*= 0.0f*/, f32 _ExceptionAlpha /*= -1.0f*/ ) const
{
#define EPSILON 0.02f

	// Find which view the cursor is over, starting at the top layer.
	for (HTMLViewsConstRevIter it = m_HTMLViews.rbegin(); it != m_HTMLViews.rend(); ++it)
	{
		if ((*it)->IsActive())
		{
			vec3 p = (*it)->m_RenderImage->GetAbsolutePosition();
			vec2 s = (*it)->m_RenderImage->GetAbsoluteSize();
			int2 m = g_Input->GetMousePosition();

			if (!(m.x < p.x || m.x > p.x + s.x || m.y < p.y || m.y > p.y + s.y))
			{
				f32 a = (*it)->GetAlphaAt((int32)(m.x - p.x), (int32)(m.y - p.y));
				if ((_ConsiderTransparency && ((a > _AlphaThreshold /*- EPSILON*/) || (a >= _ExceptionAlpha - EPSILON && a <= _ExceptionAlpha + EPSILON))) || !_ConsiderTransparency)
				{
					// This is the topmost view that the mouse is over and the pixel that the mouse is over in this view
					// meets the transparency tests.
					return (*it);
				}
			}
		}
	}
	return 0;

#undef EPSILON
}

}
