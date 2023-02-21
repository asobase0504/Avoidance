//=============================================================================
//
// 入力処理 [inputjoypad.cpp]
// Author1 : KOZUNA HIROHITO
// input.hを参照を推奨
//
//=============================================================================

//-----------------------------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------------------------
#include "inputjoypad.h"
#include "debug_proc.h"

//*************************************************************************************
//コンストラクタ
//*************************************************************************************
CInputJoyPad::CInputJoyPad()
{
	ZeroMemory(&m_JoyPadData, sizeof(m_JoyPadData));
	m_nJoyNumCnt = 0;
	m_AllOldKey = DirectJoypad::JOYPAD_MAX;
}

//*************************************************************************************
//デストラクタ
//*************************************************************************************
CInputJoyPad::~CInputJoyPad()
{
}

//デバイスを列挙してデバイスを作成してくれるコールバック関数
BOOL CALLBACK CInputJoyPad::EnumJoysticksCallback(const DIDEVICEINSTANCE *pdidInstance, VOID *pContext)
{
	HRESULT hr;

	CInputJoyPad *pThis = (CInputJoyPad*)pContext;
	LPDIRECTINPUTDEVICE8 pInputDevice = pThis->GetInputDevice();

	if (pInputDevice != nullptr)
	{
		return E_FAIL;
	}

	hr = m_pInput->CreateDevice(pdidInstance->guidInstance, &pInputDevice, NULL);

	if (FAILED(hr))
	{
		return E_FAIL;
	}
	
	pThis->SetInputDevice(pInputDevice);

	//次のデバイスを調べるときはDIENUM_CONTINUE最初の一回のみの場合はDIENUM_STOP
	return DIENUM_CONTINUE;
}

//デバイスに対してスティックの範囲等を指定
BOOL CALLBACK CInputJoyPad::EnumAxesCallback(const DIDEVICEOBJECTINSTANCE */*pdidoi*/, VOID *pContext)
{
	LPDIRECTINPUTDEVICE8 pInputDevice = (LPDIRECTINPUTDEVICE8)pContext;
	// 入力範囲のセット
	DIPROPRANGE diprg;
	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwHow = DIPH_BYOFFSET;
	diprg.lMax = 1000;
	diprg.lMin = -1000;

	// X軸
	diprg.diph.dwObj = DIJOFS_X;
	pInputDevice->SetProperty(DIPROP_RANGE, &diprg.diph);

	// Y軸
	diprg.diph.dwObj = DIJOFS_Y;
	pInputDevice->SetProperty(DIPROP_RANGE, &diprg.diph);

	// Z軸
	diprg.diph.dwObj = DIJOFS_Z;
	pInputDevice->SetProperty(DIPROP_RANGE, &diprg.diph);

	// RX軸
	diprg.diph.dwObj = DIJOFS_RX;
	pInputDevice->SetProperty(DIPROP_RANGE, &diprg.diph);

	// RY軸
	diprg.diph.dwObj = DIJOFS_RY;
	pInputDevice->SetProperty(DIPROP_RANGE, &diprg.diph);

	// RZ軸
	diprg.diph.dwObj = DIJOFS_RZ;
	pInputDevice->SetProperty(DIPROP_RANGE, &diprg.diph);
	return DIENUM_CONTINUE;
}

//*************************************************************************************
//初期化
//*************************************************************************************
HRESULT CInputJoyPad::Init(HINSTANCE hInstance, HWND hWnd)
{
	
	// デバイスの列挙
	if (FAILED(m_pInput->EnumDevices(
		DI8DEVCLASS_GAMECTRL,
		EnumJoysticksCallback,
		this,
		DIEDFL_ATTACHEDONLY)))
	{
		return E_FAIL;
	}

	for (int nCnt = 0; nCnt < JOYPAD_DATA_MAX; nCnt++)
	{
		if (m_JoyPadData[nCnt].pInputDevice == nullptr)
		{
			continue;
		}

		// デバイスのフォーマットの設定
		HRESULT hr = m_JoyPadData[nCnt].pInputDevice->SetDataFormat(&c_dfDIJoystick);

		if (FAILED(hr))
		{
			return E_FAIL;
		}

		// 協調モードの設定
		if (FAILED(m_JoyPadData[nCnt].pInputDevice->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND)))
		{
			return E_FAIL;
		}

		//デバイスに対して十字キーの範囲等を指定
		if (FAILED(m_JoyPadData[nCnt].pInputDevice->EnumObjects(EnumAxesCallback,
			m_JoyPadData[nCnt].pInputDevice,
			DIDFT_AXIS)))
		{
			return E_FAIL;
		}

	}
	return S_OK;
}

//*************************************************************************************
//終了処理
//*************************************************************************************
void CInputJoyPad::Uninit(void)
{
	for (int nCnt = 0; nCnt < JOYPAD_DATA_MAX; nCnt++)
	{
		//入力デバイスの放棄
		if (m_JoyPadData[nCnt].pInputDevice != nullptr)
		{
			m_JoyPadData[nCnt].pInputDevice->Unacquire();
			m_JoyPadData[nCnt].pInputDevice->Release();
			m_JoyPadData[nCnt].pInputDevice = nullptr;
		}
	}
}

//*************************************************************************************
//更新処理
//*************************************************************************************
void CInputJoyPad::Update(void)
{
	for (int nCnt = 0; nCnt < JOYPAD_DATA_MAX; nCnt++)
	{
		if (m_JoyPadData[nCnt].pInputDevice == nullptr)
		{
			continue;
		}
		HRESULT hr;
		//デバイスからデータを取得できることを確認し、確認できなかった場合にはアクセス権が取得
		hr = m_JoyPadData[nCnt].pInputDevice->Poll();
		if (FAILED(hr))
		{
			hr = m_JoyPadData[nCnt].pInputDevice->Acquire();
			while (hr == DIERR_INPUTLOST)
			{
				hr = m_JoyPadData[nCnt].pInputDevice->Acquire();
			}
		}

		DIJOYSTATE JoyKey;
		//コントローラーの状態を取得
		if (SUCCEEDED(m_JoyPadData[nCnt].pInputDevice->GetDeviceState(sizeof(DIJOYSTATE), &JoyKey)))
		{
			for (int nButtons = 0; nButtons < MAX_JOY_KEY; nButtons++)
			{
				m_JoyPadData[nCnt].aKeyStateTrigger.rgbButtons[nButtons] = ~m_JoyPadData[nCnt].aKeyState.rgbButtons[nButtons] & JoyKey.rgbButtons[nButtons]; //トリガー情報を保存
				m_JoyPadData[nCnt].aKeyStateRelease.rgbButtons[nButtons] = m_JoyPadData[nCnt].aKeyState.rgbButtons[nButtons] & ~JoyKey.rgbButtons[nButtons]; //トリガー情報を保存
			}
			m_JoyPadData[nCnt].aKeyState = JoyKey;//プレス処理の保管
			m_JoyPadData[nCnt].nCrossPressRot = (int)(m_JoyPadData[nCnt].aKeyState.rgdwPOV[0] / 100.0f);//ジョイパッドの十字キーの押されている方向
		}
	}
}

//プレス処理
bool CInputJoyPad::GetPress(DirectJoypad eKey,int nNum)
{
	if (m_JoyPadData[nNum].pInputDevice == nullptr)
	{
		return false;
	}

	if (eKey > JOYPAD_HOME)
	{
		return GetCrossPress(eKey, nNum);
	}
	return (m_JoyPadData[nNum].aKeyState.rgbButtons[eKey] & 0x80) ? true : false;
}

//トリガー処理
bool CInputJoyPad::GetTrigger(DirectJoypad eKey, int nNum)
{
	if (m_JoyPadData[nNum].pInputDevice == nullptr)
	{
		return false;
	}

	if (eKey > JOYPAD_HOME)
	{
		return GetCrossTrigger(eKey, nNum);
	}

	return (m_JoyPadData[nNum].aKeyStateTrigger.rgbButtons[eKey] & 0x80) ? true : false;
}

// リリース処理
bool CInputJoyPad::GetRelease(DirectJoypad eKey, int nNum)
{
	if (m_JoyPadData[nNum].pInputDevice == nullptr)
	{
		return false;
	}

	if (eKey > JOYPAD_HOME)
	{
		return GetCrossRelease(eKey, nNum);
	}

	return (m_JoyPadData[nNum].aKeyStateRelease.rgbButtons[eKey] & 0x80) ? true : false;
}

//十字キープレス処理
bool CInputJoyPad::GetCrossPress(DirectJoypad eKey, int nNum)
{
	if (m_JoyPadData[nNum].pInputDevice == nullptr)
	{
		return false;
	}

	switch (eKey)
	{
	case JOYPAD_UP:
		if (m_JoyPadData[nNum].aKeyState.rgdwPOV[0] == JOYKEY_DIRECT_CROSS::JOYKEY_CROSS_UP
			|| m_JoyPadData[nNum].aKeyState.lY <= -700
			|| m_JoyPadData[nNum].aKeyState.lRz <= -700)
		{
			return true;
		}
		break;
	case JOYPAD_DOWN:
		if (m_JoyPadData[nNum].aKeyState.rgdwPOV[0] == JOYKEY_DIRECT_CROSS::JOYKEY_CROSS_DOWN
			|| m_JoyPadData[nNum].aKeyState.lY >= 700
			|| m_JoyPadData[nNum].aKeyState.lRz >= 700)
		{
			return true;
		}
		break;
	case JOYPAD_LEFT:
		if (m_JoyPadData[nNum].aKeyState.rgdwPOV[0] == JOYKEY_DIRECT_CROSS::JOYKEY_CROSS_LEFT
			|| m_JoyPadData[nNum].aKeyState.lX <= -700
			|| m_JoyPadData[nNum].aKeyState.lZ <= -700)
		{
			return true;
		}
		break;
	case JOYPAD_RIGHT:
		if (m_JoyPadData[nNum].aKeyState.rgdwPOV[0] == JOYKEY_DIRECT_CROSS::JOYKEY_CROSS_RIGHT
			|| m_JoyPadData[nNum].aKeyState.lX >= 700
			|| m_JoyPadData[nNum].aKeyState.lZ >= 700)
		{
			return true;
		}
		break;
	default:
		return false;
		break;
	}
	return false;
}

//十字キートリガー処理
bool CInputJoyPad::GetCrossTrigger(DirectJoypad eKey, int nNum)
{
	if (m_JoyPadData[nNum].pInputDevice == nullptr)
	{
		return false;
	}

	if (m_JoyPadData[nNum].aOldKey != eKey
		&& GetCrossPress(eKey, nNum))
	{
		m_JoyPadData[nNum].aOldKey = eKey;
		return true;
	}
	else if (m_JoyPadData[nNum].aOldKey == eKey
		&& GetCrossPress(eKey, nNum))
	{
		return false;
	}

	m_JoyPadData[nNum].aOldKey = DirectJoypad::JOYPAD_MAX;
	return false;
}

//十字キーリリース処理
bool CInputJoyPad::GetCrossRelease(DirectJoypad eKey, int nNum)
{
	if (m_JoyPadData[nNum].pInputDevice == nullptr)
	{
		return false;
	}

	if (m_JoyPadData[nNum].aOldKey != eKey && !GetCrossPress(eKey, nNum))
	{
		return false;
	}

	else if (m_JoyPadData[nNum].aOldKey == eKey && !GetCrossPress(eKey, nNum))
	{
		m_JoyPadData[nNum].aOldKey = eKey;
		return true;
	}

	m_JoyPadData[nNum].aOldKey = DirectJoypad::JOYPAD_MAX;
	return false;
}

//オールプレス処理	（キー指定あり、プレイヤー指定なし）
bool CInputJoyPad::GetPressAll(DirectJoypad eKey)
{
	for (int nCnt = 0; nCnt < JOYPAD_DATA_MAX; nCnt++)
	{
		if (GetPress(eKey, nCnt))
		{
			return true;
		}
	}
	return false;
}

//オールトリガー処理（キー指定あり、プレイヤー指定なし）
bool CInputJoyPad::GetTriggerAll(DirectJoypad eKey)
{
	for (int nCnt = 0; nCnt < JOYPAD_DATA_MAX; nCnt++)
	{
		if (GetPress(eKey, nCnt) && m_AllOldKey == JOYPAD_MAX)
		{
			m_AllOldKey = eKey;
			return true;
		}
	}

	if (GetPressAll())
	{//誰かがキーを押していたら
		return false;
	}

	m_AllOldKey = JOYPAD_MAX;
	return false;
}

//オールリリース処理	（キー指定あり、プレイヤー指定なし）
bool CInputJoyPad::GetReleaseAll(DirectJoypad eKey)
{
	for (int nCnt = 0; nCnt < JOYPAD_DATA_MAX; nCnt++)
	{
		if (GetRelease(eKey, nCnt) && m_AllOldKey == JOYPAD_MAX)
		{
			m_AllOldKey = eKey;
			return true;
		}
	}

	if (GetReleaseAll())
	{//誰かがキーを押していたら
		return false;
	}

	m_AllOldKey = JOYPAD_MAX;
	return false;
}

//オールプレス処理	（キー指定なし、プレイヤー指定あり）
bool CInputJoyPad::GetPressAll(int nNum)
{
	if (m_JoyPadData[nNum].pInputDevice == nullptr)
	{
		return false;
	}

	for (int nCntKey = 0; nCntKey < JOYPAD_MAX; nCntKey++)
	{
		if (GetPress((DirectJoypad)nCntKey, nNum))
		{
			return true;
		}
	}
	return false;
}

//オールトリガー処理（キー指定なし、プレイヤー指定あり）
bool CInputJoyPad::GetTriggerAll(int nNum)
{
	if (m_JoyPadData[nNum].pInputDevice == nullptr)
	{
		return false;
	}
	
	for (int nCntKey = 0; nCntKey < JOYPAD_MAX; nCntKey++)
	{
		if (GetPress((DirectJoypad)nCntKey,nNum) && m_JoyPadData[nNum].aOldKey != nCntKey)
		{
			m_JoyPadData[nNum].aOldKey = (DirectJoypad)nCntKey;
			return true;
		}
		else if (GetPress((DirectJoypad)nCntKey, nNum) && m_JoyPadData[nNum].aOldKey == nCntKey)
		{
			return false;
		}
	}

	m_JoyPadData[nNum].aOldKey = JOYPAD_MAX;
	return false;
}

//オールリリース処理（キー指定なし、プレイヤー指定あり）
bool CInputJoyPad::GetReleaseAll(int nNum)
{
	if (m_JoyPadData[nNum].pInputDevice == nullptr)
	{
		return false;
	}

	for (int nCntKey = 0; nCntKey < JOYPAD_MAX; nCntKey++)
	{
		if (GetRelease((DirectJoypad)nCntKey, nNum) && m_JoyPadData[nNum].aOldKey != nCntKey)
		{
			m_JoyPadData[nNum].aOldKey = (DirectJoypad)nCntKey;
			return true;
		}
		else if (GetRelease((DirectJoypad)nCntKey, nNum) && m_JoyPadData[nNum].aOldKey == nCntKey)
		{
			return false;
		}
	}

	m_JoyPadData[nNum].aOldKey = JOYPAD_MAX;
	return false;
}

//オールプレス処理	（キー指定なし、プレイヤー指定なし）
bool CInputJoyPad::GetPressAll()
{
	for (int nCnt = 0; nCnt < JOYPAD_DATA_MAX; nCnt++)
	{
		if (m_JoyPadData[nCnt].pInputDevice == nullptr)
		{
			return false;
		}

		if (GetPressAll(nCnt))
		{
			return true;
		}
	}
	return false;
}

//オールトリガー処理（キー指定なし、プレイヤー指定なし）
bool CInputJoyPad::GetTriggerAll()
{
	for (int nCnt = 0; nCnt < JOYPAD_DATA_MAX; nCnt++)
	{
		if (m_JoyPadData[nCnt].pInputDevice == nullptr)
		{
			continue;
		}

		for (int nCntKey = 0; nCntKey < JOYPAD_MAX; nCntKey++)
		{
			if (GetPress((DirectJoypad)nCntKey, nCnt))
			{
				if (m_AllOldKey != nCntKey)
				{
					m_AllOldKey = (DirectJoypad)nCntKey;
					return true;
				}
				else if (m_AllOldKey == nCntKey)
				{
					return false;
				}				
			}
		}
	}

	m_AllOldKey = DirectJoypad::JOYPAD_MAX;
	return false;
}

//オールリリース処理（キー指定なし、プレイヤー指定なし）
bool CInputJoyPad::GetReleaseAll()
{
	for (int nCnt = 0; nCnt < JOYPAD_DATA_MAX; nCnt++)
	{
		if (m_JoyPadData[nCnt].pInputDevice == nullptr)
		{
			continue;
		}

		for (int nCntKey = 0; nCntKey < JOYPAD_MAX; nCntKey++)
		{
			if (GetRelease((DirectJoypad)nCntKey, nCnt))
			{
				if (m_AllOldKey != nCntKey)
				{
					m_AllOldKey = (DirectJoypad)nCntKey;
					return true;
				}
				else if (m_AllOldKey == nCntKey)
				{
					return false;
				}
			}
		}
	}

	m_AllOldKey = DirectJoypad::JOYPAD_MAX;
	return false;
}

//ジョイスティックの値を返す
D3DXVECTOR3 CInputJoyPad::GetJoyStickData(bool bleftandright, int nNum)
{
	if (bleftandright)
	{//スティックの右左(true  = 右、false = 左)
		if (m_JoyPadData[nNum].aKeyState.lRx != 0
			|| m_JoyPadData[nNum].aKeyState.lRy != 0)
		{
			return D3DXVECTOR3((float)m_JoyPadData[nNum].aKeyState.lRx, (float)m_JoyPadData[nNum].aKeyState.lRy, 0.0f);
		}

		return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	if (m_JoyPadData[nNum].aKeyState.lY != 0 
		|| m_JoyPadData[nNum].aKeyState.lX != 0)
	{
		return D3DXVECTOR3((float)m_JoyPadData[nNum].aKeyState.lX, (float)m_JoyPadData[nNum].aKeyState.lY,0.0f);
	}
	
	return D3DXVECTOR3(0.0f,0.0f,0.0f);
}
