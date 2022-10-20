#pragma once
#include "../LowLevel/DirectX.h"
#define WINDOW_NAME		"‚ ‚ ‚ "
#define CLASS_NAME		"App"

class CDxWindow {
private:
	HWND m_hWnd;
	HINSTANCE m_hInstance;
	WNDCLASSEX m_wndClass;

public:
	CDxWindow() = delete;
	CDxWindow(HINSTANCE hInstance) : m_hInstance(hInstance) {};
	~CDxWindow() {};

	HRESULT RegisterWindowClass(WNDCLASSEX *wnd);
	HRESULT CreateMainWindow();
	LRESULT LocalWndProc(HWND, UINT, WPARAM, LPARAM);

	HWND GetHWND() { return m_hWnd; }
};