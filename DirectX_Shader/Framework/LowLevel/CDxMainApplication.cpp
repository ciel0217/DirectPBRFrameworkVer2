#include "CDxMainApplication.h"
#include <time.h>
#include "../LowLevel/CDxRenderer.h"
#include "../Manager/ManagerTexture.h"
#include "../Resources/Sprite.h"
#include "../Manager/ManagerScene.h"
#include "../Manager/ManagerInput.h"
#include "../Manager/ManagerShader.h"
#include "../Renderer/CameraRenderer.h"
#include "../Manager/ManagerCollsionDetection.h"
#include "../Resources/CGameObject.h"
#include "../Renderer/CRenderer.h"

CDxMainApplication *pApp = nullptr;

CDxMainApplication::CDxMainApplication()
{
	pApp = this;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) 
{
	UNREFERENCED_PARAMETER(hPrevInstance);	// 無くても良いけど、警告が出る（未使用宣言）
	UNREFERENCED_PARAMETER(lpCmdLine);		// 無くても良いけど、警告が出る（未使用宣言）
	CDxMainApplication tmpApp;

	pApp->Run(hInstance);
}

int CDxMainApplication::Run(HINSTANCE hInstance)
{

	m_hInstance = hInstance;
	m_DxWindow = std::make_unique<CDxWindow>(hInstance);
	

	if (!RegistWndClass()) {
		return -1;
	}

	if (!CreateMainWnd()) {
		return -1;
	}

	if (!Initialize()) {
		return -1;
	}

	Loop();

	ReleaseApp();


	return S_OK;
}


BOOL CDxMainApplication::RegistWndClass()
{
	HRESULT hr = m_DxWindow->RegisterWindowClass(&m_WndClassWx);
	return 1;
}

BOOL CDxMainApplication::CreateMainWnd()
{
	HRESULT hr = m_DxWindow->CreateMainWindow();
	return 1;
}

BOOL CDxMainApplication::Initialize()
{
	//初期化処理するやつ
	//renderとか
	m_Renderer = new CDxRenderer(m_DxWindow->GetHWND(), m_hInstance);
	m_Renderer->InitRenderer();
	
	Sprite2D::InitSprite();
	ManagerInput::Create();
	ManagerInput::GetInstance()->InitInput(m_hInstance, m_DxWindow->GetHWND());
	ManagerShader::CompileShader();
	ManagerModel::Init();
	ManagerCollisionDetection::Create();
	CRenderer::SetUpRenderer();

	//CGameObject::SetUpGameObject();
	CameraRenderer::SetUpCameraRenderer();
	ManagerScene::Create();
	

	return 1;
}

void CDxMainApplication::Loop()
{
	srand((unsigned int)time(NULL));

	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;
	
	MSG msg;
	
	// フレームカウント初期化
	timeBeginPeriod(1);	// 分解能を設定
	dwExecLastTime = dwFPSLastTime = timeGetTime();	// システム時刻をミリ秒単位で取得
	dwCurrentTime = dwFrameCount = 0;
	

	// メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{// PostQuitMessage()が呼ばれたらループ終了
				break;
			}
			else
			{
				// メッセージの翻訳と送出
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			dwCurrentTime = timeGetTime();

			if ((dwCurrentTime - dwFPSLastTime) >= 1000)	// 1秒ごとに実行
			{
#ifdef _DEBUG
				m_CountFPS = dwFrameCount;
#endif
				dwFPSLastTime = dwCurrentTime;				// FPSを測定した時刻を保存
				dwFrameCount = 0;							// カウントをクリア
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))	// 1/60秒ごとに実行
			{
				dwExecLastTime = dwCurrentTime;	// 処理した時刻を保存

#ifdef _DEBUG	// デバッグ版の時だけFPSを表示する
				wsprintf(m_DebugStr, WINDOW_NAME);
				wsprintf(&m_DebugStr[strlen(m_DebugStr)], " FPS:%d", m_CountFPS);
				SetWindowText(m_DxWindow->GetHWND(), m_DebugStr);
#endif

				Update();			// 更新処理
				Draw();				// 描画処理

				//CheckScene();

				dwFrameCount++;
			}
		}
	}
}

//消す
LRESULT CALLBACK CDxMainApplication::GrobalWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (pApp) {
		CDxWindow* wnd = pApp->GetDxWindow();
		wnd->LocalWndProc(hWnd, message, wParam, lParam);
	}
	
	return 0;
}

void CDxMainApplication::ReleaseApp()
{
	timeEndPeriod(1);				// 分解能を戻す

	ManagerInput::GetInstance()->UninitInput();
	//CGameObject3D::ReleaseGameObject();
	CameraRenderer::Release();
	ManagerCollisionDetection::Release();


	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, m_WndClassWx.hInstance);
}

void CDxMainApplication::Update()
{
	ManagerInput::GetInstance()->UpdateInput();
	ManagerScene::GetInstance()->Update();
}

void CDxMainApplication::Draw()
{
	ManagerScene::GetInstance()->Draw();

	//m_Renderer->ClearRenderTextureSceneByDeffard();
	//m_Renderer->SetRenderTargetByDeffard();
	////描画
	//test->Draw();

	//m_Renderer->ClearBackBuffor();
	//m_Renderer->SetRenderTargetBackBuffor();
	////test->Draw();
	////ポストプロセス用処理

	//post->Draw();

	m_Renderer->Present();
}
