//=============================================================================
//
// XInputを使用したコントローラー処理
// Author1 : HAMADA RYUUGA
// input.hを参照を推奨
//
//=============================================================================

//-----------------------------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------------------------
#include "inputcontroller.h"

//*************************************************************************************
//コンストラクタ
//*************************************************************************************
CInputController::CInputController()
{
	m_pInput = nullptr;
	m_pDevJoypad = nullptr;
}

//*************************************************************************************
//デストラクタ
//*************************************************************************************
CInputController::~CInputController()
{
}

//*************************************************************************************
//初期化
//*************************************************************************************
HRESULT CInputController::Init(HINSTANCE hInstance, HWND hWnd)
{
	//XInputのステートを設定（有効にする）
	XInputEnable(true);

	for (int nCnt = 0; nCnt < PLAYER_MAX; nCnt++)
	{
		//メモリーのクリア
		memset(&m_JoyKeyState[nCnt], 0, sizeof(XINPUT_STATE));
		memset(&m_JoyKeyStateTrigger[nCnt], 0, sizeof(XINPUT_STATE));

		//ジョイパッドの振動制御の０クリア
		ZeroMemory(&m_JoyMoter[nCnt], sizeof(XINPUT_VIBRATION));

		//振動制御用の初期化
		m_nStrength[nCnt] = 0;
		m_nTime[nCnt] = 0;
	}
	return S_OK;
}

//*************************************************************************************
//終了処理
//*************************************************************************************
void CInputController::Uninit(void)
{
	//XInputのステートを設定（無効にする）
	XInputEnable(false);
}

//*************************************************************************************
//更新処理
//*************************************************************************************
void CInputController::Update(void)
{
	XINPUT_STATE JoyKeyState[PLAYER_MAX];		//ジョイパッド入力情報

	for (int nCnt = 0; nCnt < PLAYER_MAX; nCnt++)
	{
		//ジョイパッドの状態を取得
		if (XInputGetState(nCnt, &JoyKeyState[nCnt]) == ERROR_SUCCESS)
		{
			m_JoyKeyStateTrigger[nCnt].Gamepad.wButtons
				= ~m_JoyKeyState[nCnt].Gamepad.wButtons
				& JoyKeyState[nCnt].Gamepad.wButtons; //トリガー情報を保存
			m_JoyKeyState[nCnt] = JoyKeyState[nCnt];  //プレス処理
		}

		//ジョイパッドの振動
		m_JoyMoter[nCnt].wLeftMotorSpeed = m_nStrength[nCnt];
		m_JoyMoter[nCnt].wRightMotorSpeed = m_nStrength[nCnt];
		XInputSetState(nCnt, &m_JoyMoter[nCnt]);

		if (m_nTime[nCnt] > 0)
		{
			m_nTime[nCnt]--;
		}
		else
		{
			m_nStrength[nCnt] = 0;
			m_nTime[nCnt] = 0;
		}
	}
}

//キーボードプレス処理
bool CInputController::GetPress(JOYKEY Key, int nPlayer)
{
	return (m_JoyKeyState[nPlayer].Gamepad.wButtons & (0x01 << Key)) ? true : false;
}
//キーボードトリガー処理
bool CInputController::GetTrigger(JOYKEY Key, int nPlayer)
{
	return (m_JoyKeyStateTrigger[nPlayer].Gamepad.wButtons & (0x01 << Key)) ? true : false;
}

//ジョイパット（スティックプレス）処理
D3DXVECTOR3 CInputController::GetJoypadStick(JOYKEY Key, int nPlayer)
{
	switch (Key)
	{
	case JOYKEY_LEFT_STICK:
		m_JoyStickPos[nPlayer] = D3DXVECTOR3(m_JoyKeyState[nPlayer].Gamepad.sThumbLX / 32767.0f, -m_JoyKeyState[nPlayer].Gamepad.sThumbLY / 32767.0f, 0.0f);
		break;
	case JOYKEY_RIGHT_STICK:
		m_JoyStickPos[nPlayer] = D3DXVECTOR3(m_JoyKeyState[nPlayer].Gamepad.sThumbRX / 32767.0f, -m_JoyKeyState[nPlayer].Gamepad.sThumbRY / 32767.0f, 0.0f);
		break;
	}

	return m_JoyStickPos[nPlayer];
}

//ジョイパット（トリガーペダル）処理
int  CInputController::GetJoypadTriggerPedal(JOYKEY Key, int nPlayer)
{
	int nJoypadTriggerPedal = 0;
	switch (Key)
	{
	case JOYKEY_LEFT_TRIGGER:
		nJoypadTriggerPedal = m_JoyKeyState[nPlayer].Gamepad.bLeftTrigger;
		break;
	case JOYKEY_RIGHT_TRIGGER:
		nJoypadTriggerPedal = m_JoyKeyState[nPlayer].Gamepad.bRightTrigger;
		break;
	}

	return nJoypadTriggerPedal;
}

//コントローラーの振動制御
void  CInputController::JoypadVibration(int nTime, WORD nStrength, int nPlayer)
{
	m_nTime[nPlayer] = nTime;			//振動持続時間
	m_nStrength[nPlayer] = nStrength;	//振動の強さ
}