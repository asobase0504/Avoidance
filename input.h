//=============================================================================
//
// 入力処理 [input.h]
// Author : KOZUNA HIROHITO
// Author2 : YUDA KAITO
// 
//=============================================================================

#ifndef _INPUT_H_		//このマクロ定義がされなかったら
#define _INPUT_H_		//2重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "inputkeydata.h"
#include "DirectInput.h"
#include "mouse.h"
#include <vector>

//----------------------------------------------------------------------------
//前方宣言
//----------------------------------------------------------------------------
class CInputKeyboard;
class CInputJoyPad;
class CMouse;

//----------------------------------------------------------------------------
//クラス定義
//----------------------------------------------------------------------------
class CInput
{
public:
	//*インプットが必要な時呼び出す
	static CInput *GetKey() { return m_pInput; }		//プレイやトリガーなどのアドレスの取得
private:
	static CInput *m_pInput;	// このクラスの情報
public:

	CInput();
	~CInput();

	//*アプリケーション処理に書くやつ
	static CInput *Create();						// 入力処理系のクリエイト、Initの前に書く
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);	// 入力処理全部の初期化
	void Uninit();									// 入力処理全部の終了処理
	void Update();									// 入力処理全部の更新処理

	// 全てのデバイス
	bool Press(STAN_DART_INPUT_KEY key) { return KeyChackAll(key, 1); }	// 総合プレス
	bool Trigger(STAN_DART_INPUT_KEY key) { return KeyChackAll(key, 2); }	// 総合トリガー
	bool Release(STAN_DART_INPUT_KEY key) { return KeyChackAll(key, 3); }	// 総合リリース

	// 入力しているデバイスを指定
	bool Press(STAN_DART_INPUT_KEY key, int nNum) { return KeyChackNum(key, 1, nNum); }		// 総合プレス
	bool Trigger(STAN_DART_INPUT_KEY key, int nNum) { return KeyChackNum(key, 2, nNum); }	// 総合トリガー
	bool Release(STAN_DART_INPUT_KEY key, int nNum) { return KeyChackNum(key, 3, nNum); }	// 総合リリース

	// 入力しているデバイスを返す
	std::vector<int> PressDevice(STAN_DART_INPUT_KEY key);		// 総合プレス
	std::vector<int> TriggerDevice(STAN_DART_INPUT_KEY key);	// 総合トリガー
	std::vector<int> ReleaseDevice(STAN_DART_INPUT_KEY key);	// 総合リリース

	/* Keyboard */
	bool Press(int nKey);	// プレス
	bool Trigger(int nkey);	// トリガー
	bool Release(int nkey);	// リリース

	/* JoyPad */
	bool Press(DirectJoypad key);				// プレス
	bool Press(DirectJoypad key, int nNum);		// プレス
	bool Trigger(DirectJoypad key);				// トリガー
	bool Trigger(DirectJoypad key, int nNum);	// トリガー
	bool Release(DirectJoypad key);				// リリース
	bool Release(DirectJoypad key, int nNum);	// リリース

	D3DXVECTOR3 VectorMoveKey();	// 十字キー式のベクトル取得
	D3DXVECTOR3 VectorMoveJoyStick(bool bleftandright,int nNum = 0);	// ジョイスティックのベクトル取得

	// 繋がってるJoyPadの数
	int GetAcceptJoyPadCount();

	/* Mouse */
	CMouse* GetMouse() { return m_mouse; }	// マウス全体の取得
	bool Press(CMouse::MOUSE_KEY key) { return m_mouse->GetPress(key); }		// プレス
	bool Trigger(CMouse::MOUSE_KEY key) { return m_mouse->GetTrigger(key); }	// トリガー
	bool Release(CMouse::MOUSE_KEY key) { return m_mouse->GetRelease(key); }	// リリース

private:
	bool KeyChackAll(STAN_DART_INPUT_KEY key, int type);			// 全デバイスの入力を確認
	bool KeyChackNum(STAN_DART_INPUT_KEY key, int type, int nNum);	// 指定したデバイスの入力を確認

private:

	CInputKeyboard* m_pKeyboard;	// キーボードの情報
	CInputJoyPad* m_pJoyPad;		// ジョイパッドの情報
	CMouse* m_mouse;				// マウス
};

#endif

