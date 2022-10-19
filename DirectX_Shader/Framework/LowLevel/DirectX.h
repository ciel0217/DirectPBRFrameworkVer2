#pragma warning(push)
#pragma warning(disable:4005)

#define _CRT_SECURE_NO_WARNINGS			// scanf ‚Ìwarning–h~
#include <stdio.h>

#include <DirectXTex/header/DirectXTex.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <d3dx9.h>
#include <wrl/client.h>
#define DIRECTINPUT_VERSION 0x0800		// Œx‘Îˆ
#include "dinput.h"
#include "mmsystem.h"

#pragma warning(pop)


//*****************************************************************************
// ƒ‰ƒCƒuƒ‰ƒŠ‚ÌƒŠƒ“ƒN
//*****************************************************************************
#pragma comment (lib, "d3d11.lib")		
#pragma comment (lib, "d3dcompiler.lib")
#pragma comment (lib, "d3dx11.lib")	
#pragma comment (lib, "d3dx9.lib")	
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "dxerr.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dinput8.lib")

#if _DEBUG
#pragma comment (lib, "DirectXTex/x86/Debug/DirectXTex.lib")
#else
#pragma comment (lib, "DirectXTex/x86/Release/DirectXTex.lib")
#endif

#define SCREEN_WIDTH	(960)
#define SCREEN_HEIGHT	(540)	