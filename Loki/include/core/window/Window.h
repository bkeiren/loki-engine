#pragma once

#ifndef WINDOW_H
#define WINDOW_H

#include <map>

namespace loki
{

class Win32SubMenu;
typedef Win32SubMenu Win32Menu;

class Window
{
public:
	typedef long (__stdcall *WindowProc)( Window* _Window, UINT, WPARAM, LPARAM );

	Window( int32 _Width, int32 _Height, char* _Title, int32 _Bits, bool _Fullscreen, WindowProc _WindowCallback, int32 _PositionX = 0, int32 _PositionY = 0 );
	~Window();

	void MakeRenderContextCurrent();

	void SwapBuffers() const;

	//////////////////////////////////////////////////////////////////////////
	// To be called when the cursor image might have been reset by the mouse
	// leaving the window and it re-entered the window.
	// If called once per frame, should work as required.
	//////////////////////////////////////////////////////////////////////////
	void UpdateCursorImage();

	int32 GetWidth() const;
	int32 GetHeight() const;
	int32 GetX() const;
	int32 GetY() const;

	void SetWidth( int32 _Width );
	void SetHeight( int32 _Height );
	void SetDimensions( int32 _Width, int32 _Height );
	void SetX( int32 _X );
	void SetY( int32 _Y );
	void SetPosition( int32 _X, int32 _Y );

	void SetFullscreen( bool _Fullscreen );
	void Maximize() const;

	bool IsValid() const;

	bool LoadCursor( const char* _CursorFile, uint32 _SizeX = 26, uint32 _SizeY = 26 );
	bool LoadIcon( const char* _IconFile, uint32 _SizeX = 32, uint32 _SizeY = 32 );

	HWND GetHWND() const;

	bool IsFullscreen() const;

	bool HasFocus() const;

	void SetAcceptDragDropFiles( bool _Accept );
	bool AcceptsDragDropFiles();

	Win32Menu* GetWin32Menu() const;
	void ReloadWin32Menu();
private:
	Window();

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
	bool m_AcceptsDragDropFiles;
	
	// This information is kept up to date by updating it each time the window's position or dimensions change.
	// This is done so that the data does not need to be queried each time it is request, but rather
	// only each time it changes. _UpdateRectangleInfo() can be called to explicitely update.
	int32 m_X;
	int32 m_Y;
	int32 m_Width;
	int32 m_Height;

	// These two are used to set the window's dimensions back to what they were before switching to fullscreen mode from windowed mode.
	// If the window has no history of being in windowed mode, the window will simply take on the resolution of the display.
	int32 m_LastWindowedWidth;
	int32 m_LastWindowedHeight;

	Win32Menu* m_Menu;
};

}

#endif