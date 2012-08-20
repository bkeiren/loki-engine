#pragma once

#ifndef WINDOW_H
#define WINDOW_H

#include <map>

namespace loki
{

class LkWindow
{
public:
	typedef long (__stdcall *WindowProc)( LkWindow* _Window, UINT, WPARAM, LPARAM );

	LkWindow( int _Width, int _Height, char* _Title, int _Bits, bool _Fullscreen, WindowProc _WindowCallback, int _PositionX = 0, int _PositionY = 0 );
	~LkWindow();

	void MakeRenderContextCurrent();

	void SwapBuffers() const;

	//////////////////////////////////////////////////////////////////////////
	// To be called when the cursor image might have been reset by the mouse
	// leaving the window and it re-entered the window.
	// If called once per frame, should work as required.
	//////////////////////////////////////////////////////////////////////////
	void UpdateCursorImage();

	int GetWidth() const;
	int GetHeight() const;
	int GetX() const;
	int GetY() const;

	void SetWidth( int _Width );
	void SetHeight( int _Height );
	void SetDimensions( int _Width, int _Height );
	void SetX( int _X );
	void SetY( int _Y );
	void SetPosition( int _X, int _Y );

	void SetFullscreen( bool _Fullscreen );

	bool IsValid() const;

	bool LoadCursor( const char* _CursorFile, unsigned int _SizeX = 26, unsigned int _SizeY = 26 );
	bool LoadIcon( const char* _IconFile, unsigned int _SizeX = 32, unsigned int _SizeY = 32 );

	HWND GetHWND() const;

	bool IsFullscreen() const;
private:
	LkWindow();

	void _Destroy();

	void _UpdateRectangleInfo();
	
	static LRESULT CALLBACK _WindowProc( HWND _hWnd, UINT _uMsg, WPARAM _wParam, LPARAM _lParam );

	//////////////////////////////////////////////////////////////////////////
	// This function can be used to pass messages that this class should handle
	// itself as well. Examples which SHOULD be passed to this class:
	// WM_MOVE and WM_SIZE messages.
	//////////////////////////////////////////////////////////////////////////
	void _ParseWindowMessage( UINT _Msg, WPARAM _WParam, LPARAM _LParam );

	bool m_Fullscreen;
	HDC m_hDC;		// Private GDI Device Context.
	HGLRC m_hRC;		// Permanent Rendering Context.
	HWND m_hWnd;		// The window Handle.
	HINSTANCE m_hInstance;
	bool m_WindowCreated;
	HCURSOR m_Cursor;
	HICON m_Icon;
	bool m_WindowIsOwned;
	WindowProc m_WindowCallback;
	
	// This information is kept up to date by updating it each time the window's position or dimensions change.
	// This is done so that the data does not need to be queried each time it is request, but rather
	// only each time it changes. _UpdateRectangleInfo() can be called to explicitely update.
	int m_X;
	int m_Y;
	int m_Width;
	int m_Height;

	// These two are used to set the window's dimensions back to what they were before switching to fullscreen mode from windowed mode.
	// If the window has no history of being in windowed mode, the window will simply take on the resolution of the display.
	int m_LastWindowedWidth;
	int m_LastWindowedHeight;
};

}

#endif