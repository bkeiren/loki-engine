#include "core/window.h"
#include <GLEW//glew.h>
#include "core/input/input.h"

namespace loki
{

Window::Window( int32 _Width, int32 _Height, char* _Title, int32 _Bits, bool _Fullscreen, WindowProc _WindowCallback, int32 _PositionX /*= 0*/, int32 _PositionY /*= 0*/ )	:
	m_Fullscreen(_Fullscreen),
	m_hDC(0),
	m_hRC(0),
	m_hWnd(0),
	m_WindowCreated(false),
	m_Cursor(0),
	m_Icon(0),
	m_WindowIsOwned(false),
	m_WindowCallback(_WindowCallback),
	m_X(0),
	m_Y(0),
	m_Width(0),
	m_Height(0),
	m_LastWindowedWidth(0),
	m_LastWindowedHeight(0)
{
	_Width = (_Width <= 0)?(1):(_Width);
	_Height = (_Height <= 0)?(1):(_Height);

	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left = (long)0;			// Set Left Value To 0
	WindowRect.right = (long)_Width;		// Set Right Value To Requested Width
	WindowRect.top = (long)0;				// Set Top Value To 0
	WindowRect.bottom = (long)_Height;		// Set Bottom Value To Requested Height

	m_hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= _WindowProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= m_hInstance;							// Set The Instance
	wc.hIcon			= 0;
	wc.hCursor			= 0;
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= L"OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL, L"Failed to register window class.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return;											// Return FALSE
	}

	if (m_Fullscreen)												// Attempt Fullscreen Mode?
	{
		DEVMODE dmScreenSettings;								// Device Mode
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth	= _Width;				// Selected Screen Width
		dmScreenSettings.dmPelsHeight	= _Height;				// Selected Screen Height
		dmScreenSettings.dmBitsPerPel	= _Bits;					// Selected Bits Per Pixel
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
			if (MessageBox(NULL, L"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?", L"Loki", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
			{
				m_Fullscreen = FALSE;		// Windowed Mode Selected.
			}
			else
			{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL, L"Program Will Now Close.", L"ERROR", MB_OK | MB_ICONSTOP);
				return;
			}
		}
	}

	if (m_Fullscreen)												// Are We Still In Fullscreen Mode?
	{
		dwExStyle = WS_EX_APPWINDOW;								// Window Extended Style
		dwStyle = WS_POPUP;										// Windows Style
		//ShowCursor(FALSE);										// Hide Mouse Pointer
	}
	else
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle = WS_OVERLAPPEDWINDOW;							// Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(m_hWnd = CreateWindowEx(	dwExStyle,							// Extended Style For The Window
									L"OpenGL",							// Class Name
									loki::util::strings::ToWideString(_Title).c_str(),								// Window Title
									dwStyle |							// Defined Window Style
									WS_CLIPSIBLINGS |					// Required Window Style
									WS_CLIPCHILDREN,					// Required Window Style
									0, 0,								// Window Position
									WindowRect.right-WindowRect.left,	// Calculate Window Width
									WindowRect.bottom-WindowRect.top,	// Calculate Window Height
									NULL,								// No Parent Window
									NULL,								// No Menu
									m_hInstance,						// Instance
									(void*)this)))						// Pass the this pointer to WM_CREATE.
	{
		_Destroy();								// Reset The Display
		MessageBox(NULL, L"Window Creation Error.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	// Store the this pointer in the user data field.
	SetWindowLongPtr(m_hWnd, GWL_USERDATA, (long)((void*)(this)));

	static PIXELFORMATDESCRIPTOR pfd =				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		_Bits,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		24,											// 24Bit Z-Buffer (Depth Buffer)  
		8,											// Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};

	if (!(m_hDC = GetDC(m_hWnd)))							// Did We Get A Device Context?
	{
		_Destroy();								// Reset The Display
		MessageBox(NULL, L"Can't Create A GL Device Context.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	if (!(PixelFormat = ChoosePixelFormat(m_hDC, &pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		_Destroy();								// Reset The Display
		MessageBox(NULL, L"Can't Find A Suitable PixelFormat.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	if(!SetPixelFormat(m_hDC, PixelFormat, &pfd))		// Are We Able To Set The Pixel Format?
	{
		_Destroy();								// Reset The Display
		MessageBox(NULL, L"Can't Set The PixelFormat.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	ShowWindow(m_hWnd, SW_SHOW);						// Show The Window
	SetForegroundWindow(m_hWnd);						// Slightly Higher Priority
	SetFocus(m_hWnd);									// Sets Keyboard Focus To The Window

	m_WindowCreated = true;

	LoadCursor("resources//cursor2.cur");
	LoadIcon("resources//icon.ico", 64, 64);

	_UpdateRectangleInfo();
}

Window::Window()
{
	ILLEGAL_CTOR_ERROR("Window");
}

Window::~Window()
{
	_Destroy();
}

void Window::MakeRenderContextCurrent()
{
	if (m_hRC == 0)
	{
		if (!(m_hRC = wglCreateContext(m_hDC)))				// Are We Able To Get A Rendering Context?
		{
			MessageBox(NULL, L"Can't Create A GL Rendering Context.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		}
	}
	if(!wglMakeCurrent(m_hDC, m_hRC))					// Try To Activate The Rendering Context
	{
		MessageBox(NULL, L"Can't Activate The GL Rendering Context.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
	}
}

void Window::SwapBuffers() const
{
	::SwapBuffers(m_hDC);	// Swap Buffers (Double Buffering)
}

void Window::UpdateCursorImage()
{
	// Update the cursor image if the cursor has left the window and re-entered it.
	static bool MouseHasLeft = false;
	int2 MousePos = g_Input->GetMousePosition();
	if (MousePos.x < 0 || MousePos.x > GetWidth() || MousePos.y < 0 || MousePos.y > GetHeight())
	{
		MouseHasLeft = true;
	}
	else
	{
		if (MouseHasLeft)
		{
			MouseHasLeft = false;
			SetCursor(m_Cursor);
		}
	}
}

int32 Window::GetWidth() const
{
	return m_Width;
}

int32 Window::GetHeight() const
{
	return m_Height;
}

int32 Window::GetX() const
{
	return m_X;
}

int32 Window::GetY() const
{
	return m_Y;
}

void Window::SetWidth( int32 _Width )
{
	SetDimensions(_Width, GetHeight());
}

void Window::SetHeight( int32 _Height )
{
	SetDimensions(GetWidth(), _Height);
}

void Window::SetDimensions( int32 _Width, int32 _Height )
{
	RECT w;
	RECT c;
	GetWindowRect(m_hWnd, &w);
	GetClientRect(m_hWnd, &c);

	::SetWindowPos(m_hWnd, 0, 0, 0, _Width + (c.left - w.left) + (w.right - c.right), 
									_Height + (c.top - w.top) + (w.bottom - c.bottom), SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	_UpdateRectangleInfo();
}

void Window::SetX( int32 _X )
{
	SetPosition(_X, GetY());
}

void Window::SetY( int32 _Y )
{
	SetPosition(GetX(), _Y);
}

void Window::SetPosition( int32 _X, int32 _Y )
{
	::SetWindowPos(m_hWnd, 0, _X, _Y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOSENDCHANGING);
	_UpdateRectangleInfo();
}

void Window::SetFullscreen( bool _Fullscreen )
{
	if (m_Fullscreen == _Fullscreen)
	{
		return;	
	}

	if (_Fullscreen)
	{
		DEVMODE fullscreenSettings;
		bool isChangeSuccessful;

		m_LastWindowedWidth = m_Width;
		m_LastWindowedHeight = m_Height;

		EnumDisplaySettings(NULL, 0, &fullscreenSettings);
		fullscreenSettings.dmPelsWidth        = m_Width;
		fullscreenSettings.dmPelsHeight       = m_Height;
		fullscreenSettings.dmFields           = DM_PELSWIDTH |
												DM_PELSHEIGHT;

		SetWindowLongPtr(m_hWnd, GWL_EXSTYLE, WS_EX_APPWINDOW | WS_EX_TOPMOST);
		SetWindowLongPtr(m_hWnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
		SetWindowPos(m_hWnd, HWND_TOP, 0, 0, m_Width, m_Height, SWP_SHOWWINDOW);
		isChangeSuccessful = ChangeDisplaySettings(&fullscreenSettings, CDS_FULLSCREEN) == DISP_CHANGE_SUCCESSFUL;
		ShowWindow(m_hWnd, SW_MAXIMIZE);

		if (!isChangeSuccessful)
		{
			LOG(VL_ERROR, "Window::SetFullscreen: Failed to switch to fullscreen mode");
		}
		else
		{
			m_Fullscreen = true;
		}
	}
	else
	{
		bool isChangeSuccessful;

		SetWindowLongPtr(m_hWnd, GWL_EXSTYLE, WS_EX_LEFT);
		SetWindowLongPtr(m_hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
		isChangeSuccessful = ChangeDisplaySettings(NULL, CDS_RESET) == DISP_CHANGE_SUCCESSFUL;

		int32 width = m_LastWindowedWidth;
		int32 height = m_LastWindowedHeight;

		if (width == 0 || height == 0)
		{
			width = GetDeviceCaps(m_hDC, HORZRES);
			height = GetDeviceCaps(m_hDC, VERTRES);
		}

		SetWindowPos(m_hWnd, HWND_NOTOPMOST, 0, 0, width, height, SWP_SHOWWINDOW);
		ShowWindow(m_hWnd, SW_RESTORE);

		if (!isChangeSuccessful)
		{
			LOG(VL_ERROR, "Window::SetFullscreen: Failed to switch to windowed mode");
		}
		else
		{
			SetDimensions(width, height);

			m_Fullscreen = false;
		}
	}
}

bool Window::IsValid() const
{
	return m_WindowCreated;
}

bool Window::LoadCursor( const char* _CursorFile, uint32 _SizeX /* = 26 */, uint32 _SizeY /* = 26 */ )
{
	m_Cursor = (HCURSOR)LoadImageA(NULL, _CursorFile, IMAGE_CURSOR, _SizeX, _SizeY, LR_LOADFROMFILE);
	SetCursor(m_Cursor);

	return (bool)m_Cursor;
}

bool Window::LoadIcon( const char* _IconFile, uint32 _SizeX /* = 32 */, uint32 _SizeY /* = 32 */ )
{
	m_Icon = (HICON)LoadImageA(NULL, _IconFile, IMAGE_ICON, _SizeX, _SizeY, LR_LOADFROMFILE);
	SendMessage(m_hWnd, WM_SETICON, ICON_SMALL, (LPARAM)m_Icon );

	return (bool)m_Icon;
}

HWND Window::GetHWND() const
{
	return m_hWnd;
}

bool Window::IsFullscreen() const
{
	return m_Fullscreen;
}

bool Window::HasFocus() const
{
	return (GetFocus() == m_hWnd);
}

void Window::_Destroy()
{
	// Check wether a window was actually created.
	if (!m_WindowCreated)
	{
		return;
	}

	if (m_Fullscreen)										// Are We In Fullscreen Mode?
	{
		ChangeDisplaySettings(NULL, 0);					// If So Switch Back To The Desktop
		ShowCursor(TRUE);								// Show Mouse Pointer
	}

	if (m_hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL, L"Release Of DC And RC Failed.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(m_hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL, L"Release Rendering Context Failed.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}
		m_hRC = NULL;										// Set RC To NULL
	}

	if (m_hDC && !ReleaseDC(m_hWnd, m_hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL, L"Release Device Context Failed.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		m_hDC = NULL;										// Set DC To NULL
	}

	if (m_hWnd && !DestroyWindow(m_hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL, L"Could Not Release hWnd.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		m_hWnd = NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass(L"OpenGL", m_hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL, L"Could Not Unregister Class.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		m_hInstance = NULL;									// Set hInstance To NULL
	}
}

void Window::_UpdateRectangleInfo()
{
	RECT out;
	GetClientRect(m_hWnd, &out);

	m_Width = (int32)(out.right - out.left);
	m_Height = (int32)(out.bottom - out.top);
	m_X = (int32)(out.left);
	m_Y = (int32)(out.top);
}

LRESULT CALLBACK Window::_WindowProc( HWND _hWnd, UINT _uMsg, WPARAM _wParam, LPARAM _lParam )
{
	//////////////////////////////////////////////////////////////////////////
	// NOTE: In order to be able to catch messages here but still have them
	// sent to, for example, OpenGL, this function needs to return 0 after
	// catching such a message within the switch statement of this function.
	// If the function does not return, DefWindowProc() will parse the
	// message and it will be lost.
	//////////////////////////////////////////////////////////////////////////

	if (_uMsg != WM_NCCREATE && _uMsg != WM_NCCALCSIZE && _uMsg != WM_GETMINMAXINFO)
	{
		Window* window = (_uMsg != WM_CREATE)?((Window*)GetWindowLongPtr(_hWnd, GWL_USERDATA)):((Window*)_lParam);
		if (!window)
		{
			// Shouldn't happen.
			assert("Window::_WindowProc: Window user data was 0 while _WindowProc was registered. Something is wrong..." && 0);
		}

		if (_uMsg == WM_MOVE || _uMsg == WM_SIZE)
		{
			window->_ParseWindowMessage(_uMsg, _wParam, _lParam);
		}
		if (window->m_WindowCallback)
		{
			long l = window->m_WindowCallback(window, _uMsg, _wParam, _lParam);
			if (l != -1)
			{
				return l;
			}
		}
	}

	// Pass all unhandled messages to DefWindowProc
	return DefWindowProc(_hWnd, _uMsg, _wParam, _lParam);
}

void Window::_ParseWindowMessage( UINT _Msg, WPARAM _WParam, LPARAM _LParam )
{
	switch (_Msg)
	{
	case WM_MOVE:
		{
			m_X = (int32)LOWORD(_LParam);
			m_Y = (int32)HIWORD(_LParam);
			break;
		}
	case WM_SIZE:
		{
			m_Width = (int32)LOWORD(_LParam);
			m_Height = (int32)HIWORD(_LParam);
			break;
		}
	}
}

}
