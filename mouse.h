//=============================================================================
//
// マウス入力処理(mouse.h)
// Author : 唐﨑結斗
// 概要 : マウス生成を行う
//
//=============================================================================
#ifndef _MOUSE_H_		// このマクロ定義がされてなかったら
#define _MOUSE_H_		// 二重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "DirectInput.h"

//=============================================================================
// マウス入力処理
// Author : 唐﨑結斗
// 概要 : マウス生成を行う
//=============================================================================
class CMouse : public CDirectInput
{
public:
	//--------------------------------------------------------------------
	// マウスのキー種別
	// Author : 唐﨑結斗
	// 概要 : マウスのキー種別
	//--------------------------------------------------------------------
	enum MOUSE_KEY
	{
		MOUSE_KEY_LEFT = 0,		// マウス左ボタン
		MOUSE_KEY_RIGHT,		// マウス右ボタン
		MOUSE_KEY_MIDDLE,		// マウス中央ボタン
		MOUSE_KEY_SIDE_0,		// マウスサイドボタン0
		MOUSE_KEY_SIDE_1,		// マウスサイドボタン1
		MOUSE_KEY_SIDE_2,		// マウスサイドボタン2
		MOUSE_KEY_SIDE_3,		// マウスサイドボタン3
		MOUSE_KEY_SIDE_4,		// マウスサイドボタン4
		MAX_MOUSE_KEY,			// マウスのボタンの最大数
	};

	//--------------------------------------------------------------------
	// コンストラクタとデストラクタ
	//--------------------------------------------------------------------
	CMouse();
	~CMouse() override;

	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	HRESULT Init(HINSTANCE hInstance, HWND hWnd) override;	// 初期化
	void Uninit(void) override;								// 終了
	void Update(void) override;								// 更新
	bool GetPress(MOUSE_KEY nKey);							// プレス情報の取得
	bool GetTrigger(MOUSE_KEY nKey);						// トリガー情報の取得
	bool GetRelease(MOUSE_KEY nKey);						// リリース情報の取得
	int GetWheelPower(void);								// マウスのホイールの動き処理

	// カーソル
	D3DXVECTOR3 GetMouseCursorPos(void);					// スクリーン座標の取得
	D3DXVECTOR3 GetMouseCursorMove(void);					// 移動量を出力処理
	void SetShowCursor(bool bShow) { ShowCursor(bShow); }	// 表示の有無
	void UseSetPosLock(bool use) { m_isPosLock = use; }
	void SetCursorPosLock();

private:
	//--------------------------------------------------------------------
	// メンバ変数
	//--------------------------------------------------------------------
	LPDIRECTINPUTDEVICE8 m_device;		// 入力デバイスへのポインタ
	DIMOUSESTATE2 m_aKeyState;			// プレス情報
	DIMOUSESTATE2 m_aKeyStateTrigger;	// トリガー情報
	DIMOUSESTATE2 m_aKeyStateRelease;	// リリース情報
	POINT m_mouseCursor;				// マウスカーソルの位置
	bool m_isPosLock;
	HWND m_hWnd;						// ウィンドウハンドを格納する変数
};

#endif
