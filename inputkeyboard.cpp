//=============================================================================
//
// 入力処理 [input.cpp]
// Author1 : KOZUNA HIROHITO
// input.hを参照を推奨
//
//=============================================================================

//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "inputkeyboard.h"

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
CInputKeyboard::CInputKeyboard()
{
	memset(m_aKeyState, 0, sizeof(m_aKeyState));
	memset(m_aKeyStateTrigger, 0, sizeof(m_aKeyStateTrigger));
	memset(m_aKeyStateRelease, 0, sizeof(m_aKeyStateRelease));
	m_pDevKeyboard = nullptr;
}

//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
CInputKeyboard::~CInputKeyboard()
{
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	//入力デバイス（キーボード）の生成
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}

	//データフォーマットを設定
	if (FAILED(m_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	//協調モードを設定
	if (FAILED(m_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//キーボードへのアクセス権を獲得
	m_pDevKeyboard->Acquire();

	return S_OK;
}

//-----------------------------------------------------------------------------
// 終了処理
//-----------------------------------------------------------------------------
void CInputKeyboard::Uninit(void)
{
	//入力デバイス（キーボード）の放棄
	if (m_pDevKeyboard != NULL)
	{
		m_pDevKeyboard->Unacquire();		//キーボードへのアクセス権を放棄
		m_pDevKeyboard->Release();
		m_pDevKeyboard = NULL;
	}
}

//-----------------------------------------------------------------------------
// 更新処理
//-----------------------------------------------------------------------------
void CInputKeyboard::Update(void)
{
	BYTE aKeyState[NUM_KEY_MAX];		//キーボードの入力情報
	int nCntKey;
	//入力デバイスからデータを取得
	if (SUCCEEDED(m_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			m_aKeyStateTrigger[nCntKey] = ~m_aKeyState[nCntKey] & aKeyState[nCntKey];	 // キーボードのトリガー情報を保存
			m_aKeyStateRelease[nCntKey] = m_aKeyState[nCntKey] & ~aKeyState[nCntKey];	 // キーボードのリリース情報を保存
			m_aKeyState[nCntKey] = aKeyState[nCntKey];		// キーボードのプレス情報を保存
		}
	}
	else
	{
		m_pDevKeyboard->Acquire();	// キーボードへのアクセス権を獲得
	}
}

//-----------------------------------------------------------------------------
// キーボードプレス処理
//-----------------------------------------------------------------------------
bool CInputKeyboard::GetPress(int nKey)
{
	return (m_aKeyState[nKey] & 0x80) ? true : false;
}

//-----------------------------------------------------------------------------
// キーボードトリガー処理
//-----------------------------------------------------------------------------
bool CInputKeyboard::GetTrigger(int nKey)
{
	return (m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//-----------------------------------------------------------------------------
// キーボードリリース処理
//-----------------------------------------------------------------------------
bool CInputKeyboard::GetRelease(int nKey)
{
	return (m_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//-----------------------------------------------------------------------------
// キーボード全キープレス処理
//-----------------------------------------------------------------------------
bool CInputKeyboard::GetAllPress(void)
{
	for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
	{
		if (GetPress(nCntKey))
		{
			return true;
		}
	}
	return false;
}

//-----------------------------------------------------------------------------
// キーボード全キートリガー処理
//-----------------------------------------------------------------------------
bool CInputKeyboard::GetAllTrigger(void)
{
	for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
	{
		if (GetTrigger(nCntKey))
		{
			return true;
		}
	}
	return false;
}

//-----------------------------------------------------------------------------
// キーボード全キーリリース処理
//-----------------------------------------------------------------------------
bool CInputKeyboard::GetAllRelease(void)
{
	for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
	{
		if (GetRelease(nCntKey))
		{
			return true;
		}
	}
	return false;
}
