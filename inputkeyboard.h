//=============================================================================
//
// 入力処理 [inputkeyboard.h]
// Author1 : KOZUNA HIROHITO
//
//=============================================================================

#ifndef _INPUTKEYBOARD_H_		//このマクロ定義がされなかったら
#define _INPUTKEYBOARD_H_		//2重インクルード防止のマクロ定義

//----------------------------------------------------------------------------
//インクルードファイル
//----------------------------------------------------------------------------
#include "DirectInput.h"
//----------------------------------------------------------------------------
//クラス定義
//----------------------------------------------------------------------------
class CInputKeyboard : public CDirectInput
{
private:
	static const int NUM_KEY_MAX = 256;	//キーの最大数（キーボード）
public:
	CInputKeyboard();
	~CInputKeyboard()override;
	HRESULT Init(HINSTANCE hInstance, HWND hWnd)override;		// 入力処理全部の初期化
	void Uninit(void)override;									// 入力処理全部の終了処理
	void Update(void)override;									// 入力処理全部の更新処理

	bool GetPress(int nKey);		// キーボードプレス処理
	bool GetTrigger(int nKey);		// キーボードトリガー処理
	bool GetRelease(int nKey);		// キーボードリリース処理
	bool GetAllPress(void);			// キーボード全キープレス処理
	bool GetAllTrigger(void);		// キーボード全キートリガー処理
	bool GetAllRelease(void);		// キーボード全キーリリース処理

private:
	LPDIRECTINPUTDEVICE8 m_pDevKeyboard;	// 入力デバイス（キーボード（コントローラー用は別に作る））へのポインタ
	BYTE m_aKeyState[NUM_KEY_MAX];			// キーボードのプレス情報
	BYTE m_aKeyStateTrigger[NUM_KEY_MAX];	// キーボードのトリガー情報
	BYTE m_aKeyStateRelease[NUM_KEY_MAX];	// キーボードのリリース情報
};
#endif