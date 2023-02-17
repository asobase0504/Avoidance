//=============================================================================
//
// マウス入力処理(mouse.h)
// Author : 唐﨑結斗
// 概要 : マウス生成を行う
//
//=============================================================================

//***************************************************************************
// インクルード
//***************************************************************************
#include <assert.h>
#include <stdio.h>

#include "mouse.h"
#include "renderer.h" // スクリーンの大きさを取得するため

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CMouse::CMouse()
{
	m_device = nullptr;
	m_isPosLock = false;
	memset(&m_aKeyState, 0, sizeof(m_aKeyState));
	memset(&m_aKeyStateTrigger, 0, sizeof(m_aKeyStateTrigger));
	memset(&m_aKeyStateRelease, 0, sizeof(m_aKeyStateRelease));
}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CMouse::~CMouse()
{

}

//=============================================================================
// 初期化
// Author : 唐﨑結斗
// 概要 : マウスのアクセス権を獲得できた場合、処理を継続する
//=============================================================================
HRESULT CMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	// 入力デバイス（マウス）の生成
	if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_device, NULL)))
	{
		return E_FAIL;
	}

	//データフォーマットを設定
	if (FAILED(m_device->SetDataFormat(&c_dfDIMouse2)))
	{
		return E_FAIL;
	}

	// 協調モードを設定
	if (FAILED(m_device->SetCooperativeLevel(hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// ウィンドウハンドルの格納
	m_hWnd = hWnd;

	// Deviceへのアクセス権を獲得
	m_device->Acquire();
	return S_OK;
}

//=============================================================================
// 終了
// Author : Yuda Kaito
// 概要 : マウスのアクセス権を解放し、デバイスとオブジェクトを解放する
//=============================================================================
void CMouse::Uninit(void)
{
	//入力デバイスの放棄
	if (m_device != NULL)
	{
		m_device->Unacquire();		// アクセス権を放棄
		m_device->Release();
		m_device = NULL;
	}
}

//=============================================================================
// 更新
// Author : 唐﨑結斗
// 概要 : マウス入力された情報を保存する
//=============================================================================
void CMouse::Update(void)
{
	// 変数宣言
	DIMOUSESTATE2 aKeyState;		// マウス入力情報
	int nCntKey;

	// 入力デバイスからデータを取得
	if (SUCCEEDED(m_device->GetDeviceState(sizeof(aKeyState), &aKeyState)))
	{
		for (nCntKey = 0; nCntKey < MAX_MOUSE_KEY; nCntKey++)
		{
			// マウストリガー情報を保存
			m_aKeyStateTrigger.rgbButtons[nCntKey] = (m_aKeyState.rgbButtons[nCntKey] ^ aKeyState.rgbButtons[nCntKey]) & aKeyState.rgbButtons[nCntKey];

			// マウスリリース情報を保存
			m_aKeyStateRelease.rgbButtons[nCntKey] = (m_aKeyState.rgbButtons[nCntKey] ^ aKeyState.rgbButtons[nCntKey]) & m_aKeyState.rgbButtons[nCntKey];

			// マウスプレス情報を保存
			m_aKeyState = aKeyState;
		}
	}
	else
	{
		// マウスへのアクセス権を獲得
		m_device->Acquire();
	}

	if (m_isPosLock)
	{
		SetCursorPosLock();
	}
}

//=============================================================================
//  マウスプレス情報の取得
// Author : 唐﨑結斗
// 概要 : 引数のキーの情報を返す
//=============================================================================
bool CMouse::GetPress(MOUSE_KEY nKey)
{
	return (m_aKeyState.rgbButtons[nKey] & 0x80) ? true : false;
}

//=============================================================================
// マウストリガー情報の取得
// Author : 唐﨑結斗
// 概要 : 引数のキーの情報を返す
//=============================================================================
bool CMouse::GetTrigger(MOUSE_KEY nKey)
{
	return (m_aKeyStateTrigger.rgbButtons[nKey] & 0x80) ? true : false;
}

//=============================================================================
// マウスリリース情報の取得
// Author : 唐﨑結斗
// 概要 : 引数のキーの情報を返す
//=============================================================================
bool CMouse::GetRelease(MOUSE_KEY nKey)
{
	return (m_aKeyStateRelease.rgbButtons[nKey] & 0x80) ? true : false;
}

//=============================================================================
// マウスカーソルの位置の取得
// Author : 唐﨑結斗
// 概要 : マウスカーソルの位置を取得し、数値を返す
//=============================================================================
D3DXVECTOR3 CMouse::GetMouseCursorPos()
{
	// スクリーン座標上のマウスカーソルの位置の取得
	GetCursorPos(&m_mouseCursor);

	// ウィンドウ上のマウスカーソルの位置の取得
	ScreenToClient(m_hWnd, &m_mouseCursor);

	return D3DXVECTOR3((float)m_mouseCursor.x, (float)m_mouseCursor.y, 0.0f);
}

//=============================================================================
// マウスホイールの移動量取得
// Author : 唐﨑結斗
// 概要 : マウスホイールの移動量の数値を返す
//=============================================================================
int CMouse::GetWheelPower(void)
{
	return (int)m_aKeyState.lZ;
}

//=============================================================================
// マウスの移動量取得
// Author : 唐﨑結斗
// 概要 : マウスの移動量の数値を返す
//=============================================================================
D3DXVECTOR3 CMouse::GetMouseCursorMove(void)
{
	return D3DXVECTOR3((float)(m_aKeyState.lX), (float)(m_aKeyState.lY), (float)(m_aKeyState.lZ));
}

//=============================================================================
// カーソルのロック
// Author : YudaKaito
// 概要 : カーソルの位置をロックする。
//=============================================================================
void CMouse::SetCursorPosLock()
{
	D3DXVECTOR2 pos;

	pos.x = SCREEN_WIDTH / 2;
	pos.y = SCREEN_HEIGHT / 2;

	WINDOWINFO windowInfo;

	//ウィンドウの位置を取得
	windowInfo.cbSize = sizeof(WINDOWINFO);
	GetWindowInfo(m_hWnd, &windowInfo);

	//マウスの移動先の絶対座標（モニター左上からの座標）
	pos.x += windowInfo.rcWindow.left;
	pos.y += windowInfo.rcWindow.top + 35; //ウィンドウのタイトルバーの分（35px）をプラス

	SetCursorPos((int)pos.x, (int)pos.y);
}
