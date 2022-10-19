#pragma once
#include "CDxWindow.h"
#include <memory>


class CDxRenderer;
class ManagerTexture;

class CDxMainApplication {
private:
	HINSTANCE m_hInstance;
	WNDCLASSEX m_WndClassWx;
	std::unique_ptr<CDxWindow> m_DxWindow;
	CDxRenderer *m_Renderer;
	ManagerTexture *m_ManagerTexture;


	
	//デバッグ用
	int m_CountFPS;
	char m_DebugStr[2048] = "aaaa";

public:
	CDxMainApplication();
	~CDxMainApplication() {};
	int Run(HINSTANCE hInstance);
	static LRESULT CALLBACK GrobalWndProc(HWND, UINT, WPARAM, LPARAM);
	
	HINSTANCE GetHInstance() { return m_hInstance; }
	void GetWndClass(WNDCLASSEX *wc) {
		*wc = m_WndClassWx;
	}

	CDxWindow* GetDxWindow() { return m_DxWindow.get(); }

private:
	BOOL RegistWndClass();
	BOOL CreateMainWnd();
	BOOL Initialize();
	void Loop();
	void ReleaseApp();
	void Update();
	void Draw();

	//デバッグ用
	char* GetDebugStr(void) {
		return m_DebugStr;
	}
};