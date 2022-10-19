#pragma once
#include "DirectX.h"


#define	NUM_KEY_MAX			(256)
#define GAME_PAD_MAX		(4)
// game pad用設定値
#define DEADZONE		2500			// 各軸の25%を無効ゾーンとする
#define RANGE_MAX		1000			// 有効範囲の最大値
#define RANGE_MIN		-1000			// 有効範囲の最小値


/* game pad情報 */
#define BUTTON_UP		0x00000001l	// 方向キー上(.IY<0)
#define BUTTON_DOWN		0x00000002l	// 方向キー下(.IY>0)
#define BUTTON_LEFT		0x00000004l	// 方向キー左(.IX<0)
#define BUTTON_RIGHT	0x00000008l	// 方向キー右(.IX>0)
#define BUTTON_A		0x00000010l	// Ａボタン(.rgbButtons[0]&0x80)
#define BUTTON_B		0x00000020l	// Ｂボタン(.rgbButtons[1]&0x80)
#define BUTTON_C		0x00000040l	// Ｃボタン(.rgbButtons[2]&0x80)
#define BUTTON_X		0x00000080l	// Ｘボタン(.rgbButtons[3]&0x80)
#define BUTTON_Y		0x00000100l	// Ｙボタン(.rgbButtons[4]&0x80)
#define BUTTON_Z		0x00000200l	// Ｚボタン(.rgbButtons[5]&0x80)
#define BUTTON_L		0x00000400l	// Ｌボタン(.rgbButtons[6]&0x80)
#define BUTTON_R		0x00000800l	// Ｒボタン(.rgbButtons[7]&0x80)
#define BUTTON_START	0x00001000l	// ＳＴＡＲＴボタン(.rgbButtons[8]&0x80)
#define BUTTON_M		0x00002000l	// Ｍボタン(.rgbButtons[9]&0x80)



class ManagerInput {
private:
	static ManagerInput* m_Instance;
	LPDIRECTINPUT8 m_DInput;
	LPDIRECTINPUTDEVICE8 m_DIDevKeyboard;

	BYTE m_KeyState[NUM_KEY_MAX];
	BYTE m_KeyStateTrigger[NUM_KEY_MAX];
	BYTE m_KeyStateRepeat[NUM_KEY_MAX];
	BYTE m_KeyStateRelease[NUM_KEY_MAX];
	BYTE m_KeyStateRepeatCnt[NUM_KEY_MAX];

	LPDIRECTINPUTDEVICE8 m_Mouse;
	DIMOUSESTATE2 m_MouseState;
	DIMOUSESTATE2 m_MouseTrigger;

	LPDIRECTINPUTDEVICE8 m_GamePad[GAME_PAD_MAX] = {NULL, NULL, NULL, NULL};
	DWORD m_PadState[GAME_PAD_MAX];
	DWORD m_PadTrigger[GAME_PAD_MAX];
	int m_PadCount = 0;

	void InitKeyboard(HINSTANCE hInstance, HWND hWnd);
	void InitPad(HINSTANCE hInstance, HWND hWnd);
	void InitMouse(HINSTANCE hInstance, HWND hWnd);

	static BOOL CALLBACK SearchGamePadCallback(LPDIDEVICEINSTANCE lpddi, LPVOID);

	void UpdateKeyboard();
	void UpdatePad();
	void UpdateMouse();

	void UninitKeyboard();
	void UninitPad();
	void UninitMouse();

public:
	
	static ManagerInput* Create() {
		if (m_Instance == nullptr) {
			m_Instance = new ManagerInput();
		}

		return m_Instance;
	}

	void InitInput(HINSTANCE hInstance,HWND hWnd);
	void UpdateInput();

	void UninitInput();


	LPDIRECTINPUT8 GetDevice() { return m_DInput; }
	LPDIRECTINPUTDEVICE8* GetGamePad() { return m_GamePad; }
	int* GetPadCount() { return &m_PadCount; }
	

	static ManagerInput* GetInstance() { return m_Instance; }

	//= ============================================================================
		// キーボードのプレス状態を取得
		//=============================================================================
		bool GetKeyboardPress(int key)
	{
		return (m_KeyState[key] & 0x80) ? true : false;
	}

	//=============================================================================
	// キーボードのトリガー状態を取得
	//=============================================================================
	bool GetKeyboardTrigger(int key)
	{
		return (m_KeyStateTrigger[key] & 0x80) ? true : false;
	}

	//=============================================================================
	// キーボードのリピート状態を取得
	//=============================================================================
	bool GetKeyboardRepeat(int key)
	{
		return (m_KeyStateRepeat[key] & 0x80) ? true : false;
	}

	//=============================================================================
	// キーボードのリリ－ス状態を取得
	//=============================================================================
	bool GetKeyboardRelease(int key)
	{
		return (m_KeyStateRelease[key] & 0x80) ? true : false;
	}

	//----------------------------------------------
	BOOL IsMouseLeftPressed(void)
	{
		return (BOOL)(m_MouseState.rgbButtons[0] & 0x80);	// 押されたときに立つビットを検査
	}
	BOOL IsMouseLeftTriggered(void)
	{
		return (BOOL)(m_MouseTrigger.rgbButtons[0] & 0x80);
	}
	BOOL IsMouseRightPressed(void)
	{
		return (BOOL)(m_MouseState.rgbButtons[1] & 0x80);
	}
	BOOL IsMouseRightTriggered(void)
	{
		return (BOOL)(m_MouseTrigger.rgbButtons[1] & 0x80);
	}
	BOOL IsMouseCenterPressed(void)
	{
		return (BOOL)(m_MouseState.rgbButtons[2] & 0x80);
	}
	BOOL IsMouseCenterTriggered(void)
	{
		return (BOOL)(m_MouseTrigger.rgbButtons[2] & 0x80);
	}
	//------------------
	long GetMouseX(void)
	{
		return m_MouseState.lX;
	}
	long GetMouseY(void)
	{
		return m_MouseState.lY;
	}
	long GetMouseZ(void)
	{
		return m_MouseState.lZ;
	}

	//----------------------------------------------- 検査
	BOOL IsButtonPressed(int padNo, DWORD button)
	{
		return (button & m_PadState[padNo]);
	}

	BOOL IsButtonTriggered(int padNo, DWORD button)
	{
		return (button & m_PadTrigger[padNo]);
	}
};
