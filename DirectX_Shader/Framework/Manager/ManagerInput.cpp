#include "../Manager/ManagerInput.h"
#include "../LowLevel/DirectX.h"

ManagerInput* ManagerInput::m_Instance;

void ManagerInput::InitInput(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr;

	if (!m_DInput)
	{
		// DirectInputオブジェクトの作成
		hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
			IID_IDirectInput8, (void**)&m_DInput, NULL);
	}
	InitKeyboard(hInstance, hWnd);
	InitPad(hInstance, hWnd);
	InitMouse(hInstance, hWnd);

}

void ManagerInput::UpdateInput()
{
	UpdateKeyboard();
	UpdatePad();
	UpdateMouse();
}

void ManagerInput::UninitInput()
{
	UninitKeyboard();
	UninitPad();
	UninitMouse();
}


void ManagerInput::InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr;

	// デバイスオブジェクトを作成
	hr = m_DInput->CreateDevice(GUID_SysKeyboard, &m_DIDevKeyboard, NULL);
	if (FAILED(hr) || m_DIDevKeyboard == NULL)
	{
		MessageBox(hWnd, "キーボードがねぇ！", "警告！", MB_ICONWARNING);
		return;
	}

	// データフォーマットを設定
	hr = m_DIDevKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "キーボードのデータフォーマットを設定できませんでした。", "警告！", MB_ICONWARNING);
		return;
	}

	// 協調モードを設定（フォアグラウンド＆非排他モード）
	hr = m_DIDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(hr))
	{
		MessageBox(hWnd, "キーボードの協調モードを設定できませんでした。", "警告！", MB_ICONWARNING);
		return ;
	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	m_DIDevKeyboard->Acquire();
}


BOOL CALLBACK ManagerInput::SearchGamePadCallback(LPDIDEVICEINSTANCE lpddi, LPVOID)
{
	HRESULT result;

	int* count = ManagerInput::GetInstance()->GetPadCount();
	result = ManagerInput::GetInstance()->GetDevice()->CreateDevice(lpddi->guidInstance, &ManagerInput::GetInstance()->GetGamePad()[*count++], NULL);
	return DIENUM_CONTINUE;	// 次のデバイスを列挙
}

void ManagerInput::InitPad(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT result;

	m_PadCount = 0;

	m_DInput->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)SearchGamePadCallback, NULL, DIEDFL_ATTACHEDONLY);

	for (int i = 0; i < m_PadCount; i++) {
		// ジョイスティック用のデータ・フォーマットを設定
		result = m_GamePad[i]->SetDataFormat(&c_dfDIJoystick);
		if (FAILED(result))
			return; // データフォーマットの設定に失敗

		// モードを設定（フォアグラウンド＆非排他モード）
//		result = pGamePad[i]->SetCooperativeLevel(hWindow, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
//		if ( FAILED(result) )
//			return false; // モードの設定に失敗

		// 軸の値の範囲を設定
		// X軸、Y軸のそれぞれについて、オブジェクトが報告可能な値の範囲をセットする。
		// (max-min)は、最大10,000(?)。(max-min)/2が中央値になる。
		// 差を大きくすれば、アナログ値の細かな動きを捕らえられる。(パッドの性能による)
		DIPROPRANGE				diprg;
		ZeroMemory(&diprg, sizeof(diprg));
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = RANGE_MIN;
		diprg.lMax = RANGE_MAX;
		// X軸の範囲を設定
		diprg.diph.dwObj = DIJOFS_X;
		m_GamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Y軸の範囲を設定
		diprg.diph.dwObj = DIJOFS_Y;
		m_GamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);

		// 各軸ごとに、無効のゾーン値を設定する。
		// 無効ゾーンとは、中央からの微少なジョイスティックの動きを無視する範囲のこと。
		// 指定する値は、10000に対する相対値(2000なら20パーセント)。
		DIPROPDWORD				dipdw;
		dipdw.diph.dwSize = sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
		dipdw.diph.dwHow = DIPH_BYOFFSET;
		dipdw.dwData = DEADZONE;
		//X軸の無効ゾーンを設定
		dipdw.diph.dwObj = DIJOFS_X;
		m_GamePad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		//Y軸の無効ゾーンを設定
		dipdw.diph.dwObj = DIJOFS_Y;
		m_GamePad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);

		//ジョイスティック入力制御開始
		m_GamePad[i]->Acquire();
	}
	return;
}

void ManagerInput::InitMouse(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT result;
	// デバイス作成
	result = m_DInput->CreateDevice(GUID_SysMouse, &m_Mouse, NULL);
	if (FAILED(result) || m_Mouse == NULL)
	{
		MessageBox(hWnd, "No mouse", "Warning", MB_OK | MB_ICONWARNING);
		return;
	}
	// データフォーマット設定
	result = m_Mouse->SetDataFormat(&c_dfDIMouse2);
	if (FAILED(result))
	{
		MessageBox(hWnd, "Can't setup mouse", "Warning", MB_OK | MB_ICONWARNING);
		return;
	}
	// 他のアプリと協調モードに設定
	result = m_Mouse->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(result))
	{
		MessageBox(hWnd, "Mouse mode error", "Warning", MB_OK | MB_ICONWARNING);
		return;
	}

	// デバイスの設定
	DIPROPDWORD prop;

	prop.diph.dwSize = sizeof(prop);
	prop.diph.dwHeaderSize = sizeof(prop.diph);
	prop.diph.dwObj = 0;
	prop.diph.dwHow = DIPH_DEVICE;
	prop.dwData = DIPROPAXISMODE_REL;		// マウスの移動値　相対値

	result = m_Mouse->SetProperty(DIPROP_AXISMODE, &prop.diph);
	if (FAILED(result))
	{
		MessageBox(hWnd, "Mouse property error", "Warning", MB_OK | MB_ICONWARNING);
		return;
	}

	// アクセス権を得る
	m_Mouse->Acquire();
	return;
}



void ManagerInput::UpdateKeyboard()
{
	HRESULT hr;
	BYTE keyStateOld[256];

	// 前回のデータを保存
	memcpy(keyStateOld, m_KeyState, NUM_KEY_MAX);

	// デバイスからデータを取得
	hr = m_DIDevKeyboard->GetDeviceState(sizeof(m_KeyState), m_KeyState);
	if (SUCCEEDED(hr))
	{
		for (int cnt = 0; cnt < NUM_KEY_MAX; cnt++)
		{
			m_KeyStateTrigger[cnt] = (keyStateOld[cnt] ^ m_KeyState[cnt]) & m_KeyState[cnt];
			m_KeyStateRelease[cnt] = (keyStateOld[cnt] ^ m_KeyState[cnt]) & ~m_KeyState[cnt];
			m_KeyStateRepeat[cnt] = m_KeyStateTrigger[cnt];

			if (m_KeyState[cnt])
			{
				m_KeyStateRepeatCnt[cnt]++;
				if (m_KeyStateRepeatCnt[cnt] >= 20)
				{
					m_KeyStateRepeat[cnt] = m_KeyState[cnt];
				}
			}
			else
			{
				m_KeyStateRepeatCnt[cnt] = 0;
				m_KeyStateRepeat[cnt] = 0;
			}
		}
	}
	else
	{
		// キーボードへのアクセス権を取得
		m_DIDevKeyboard->Acquire();
	}

}

void ManagerInput::UpdatePad()
{
	HRESULT			result;
	DIJOYSTATE2		dijs;
	int				i;

	for (i = 0; i < m_PadCount; i++)
	{
		DWORD lastPadState;
		lastPadState = m_PadState[i];
		m_PadState[i] = 0x00000000l;	// 初期化

		result = m_GamePad[i]->Poll();	// ジョイスティックにポールをかける
		if (FAILED(result)) {
			result = m_GamePad[i]->Acquire();
			while (result == DIERR_INPUTLOST)
				result = m_GamePad[i]->Acquire();
		}

		result = m_GamePad[i]->GetDeviceState(sizeof(DIJOYSTATE), &dijs);	// デバイス状態を読み取る
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED) {
			result = m_GamePad[i]->Acquire();
			while (result == DIERR_INPUTLOST)
				result = m_GamePad[i]->Acquire();
		}

		// ３２の各ビットに意味を持たせ、ボタン押下に応じてビットをオンにする
		//* y-axis (forward)
		if (dijs.lY < 0)					m_PadState[i] |= BUTTON_UP;
		//* y-axis (backward)
		if (dijs.lY > 0)					m_PadState[i] |= BUTTON_DOWN;
		//* x-axis (left)
		if (dijs.lX < 0)					m_PadState[i] |= BUTTON_LEFT;
		//* x-axis (right)
		if (dijs.lX > 0)					m_PadState[i] |= BUTTON_RIGHT;
		//* Ａボタン
		if (dijs.rgbButtons[0] & 0x80)	m_PadState[i] |= BUTTON_A;
		//* Ｂボタン
		if (dijs.rgbButtons[1] & 0x80)	m_PadState[i] |= BUTTON_B;
		//* Ｃボタン
		if (dijs.rgbButtons[2] & 0x80)	m_PadState[i] |= BUTTON_C;
		//* Ｘボタン
		if (dijs.rgbButtons[3] & 0x80)	m_PadState[i] |= BUTTON_X;
		//* Ｙボタン
		if (dijs.rgbButtons[4] & 0x80)	m_PadState[i] |= BUTTON_Y;
		//* Ｚボタン
		if (dijs.rgbButtons[5] & 0x80)	m_PadState[i] |= BUTTON_Z;
		//* Ｌボタン
		if (dijs.rgbButtons[6] & 0x80)	m_PadState[i] |= BUTTON_L;
		//* Ｒボタン
		if (dijs.rgbButtons[7] & 0x80)	m_PadState[i] |= BUTTON_R;
		//* ＳＴＡＲＴボタン
		if (dijs.rgbButtons[8] & 0x80)	m_PadState[i] |= BUTTON_START;
		//* Ｍボタン
		if (dijs.rgbButtons[9] & 0x80)	m_PadState[i] |= BUTTON_M;

		// Trigger設定
		m_PadTrigger[i] = ((lastPadState ^ m_PadState[i])	// 前回と違っていて
			& m_PadState[i]);					// しかも今ONのやつ

	}
}

void ManagerInput::UpdateMouse()
{
	HRESULT result;
	// 前回の値保存
	DIMOUSESTATE2 lastMouseState = m_MouseState;
	// データ取得
	result = m_Mouse->GetDeviceState(sizeof(m_MouseState), &m_MouseState);
	if (SUCCEEDED(result))
	{
		m_MouseTrigger.lX = m_MouseState.lX;
		m_MouseTrigger.lY = m_MouseState.lY;
		m_MouseTrigger.lZ = m_MouseState.lZ;
		// マウスのボタン状態
		for (int i = 0; i < 8; i++)
		{
			m_MouseTrigger.rgbButtons[i] = ((lastMouseState.rgbButtons[i] ^
				m_MouseState.rgbButtons[i]) & m_MouseState.rgbButtons[i]);
		}
	}
	else	// 取得失敗
	{
		// アクセス権を得てみる
		result = m_Mouse->Acquire();
	}
	return;
}

void ManagerInput::UninitKeyboard()
{
	if (m_DIDevKeyboard)
	{
		m_DIDevKeyboard->Release();
		m_DIDevKeyboard = NULL;
	}
}

void ManagerInput::UninitPad()
{
	for (int i = 0; i < GAME_PAD_MAX; i++) {
		if (m_GamePad[i])
		{
			m_GamePad[i]->Unacquire();
			m_GamePad[i]->Release();
		}
	}
}

void ManagerInput::UninitMouse()
{
	if (m_Mouse)
	{
		m_Mouse->Unacquire();
		m_Mouse->Release();
		m_Mouse = NULL;
	}
}


