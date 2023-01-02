//=============================================================================
//
// XInputを使用したコントローラー処理
// Author1 : KOZUNA HIROHITO
//
//=============================================================================
#ifndef _INPUTCONTROLLER_H_		//このマクロ定義がされなかったら
#define _INPUTCONTROLLER_H_		//2重インクルード防止のマクロ定義

//----------------------------------------------------------------------------
//ライブラリーファイルのリンク
//----------------------------------------------------------------------------
#pragma comment(lib,"dinput8.lib")	// 入力処理に必要(キーボード)
#pragma comment(lib,"xinput.lib")	// 入力処理に必要（ジョイパット）
#pragma comment(lib,"dxguid.lib")	// DirectXコンポーネントに使用

//----------------------------------------------------------------------------
//インクルードファイル
//----------------------------------------------------------------------------
#include <Windows.h>
#pragma push_macro("new")
#undef new
#include <d3dx9.h>
#pragma pop_macro("new")
#define	 DIRECTINPUT_VERSION (0x0800)		//ビルド時の警告対策用マクロ(この位置から動かさない)
#include <Xinput.h>							//ジョイパット処理
#include <dinput.h>							//入力処理に必要

//----------------------------------------------------------------------------
//クラス定義
//----------------------------------------------------------------------------
class CInputController
{
private:
	static const int NUM_KEY_MAX = 256;		// キーの最大数（キーボード）
	static const int PLAYER_MAX = 4;		// プレイヤーの最大人数
public:

	//キー種類(ジョイパッド)
	typedef enum
	{
		JOYKEY_UP = 0,			// 十字ボタン上
		JOYKEY_DOWN,			// 十字ボタン下
		JOYKEY_LEFT,			// 十字ボタン左
		JOYKEY_RIGHT,			// 十字ボタン右
		JOYKEY_START,			// スタートボタン
		JOYKEY_BACK,			// バックボタン
		JOYKEY_LEFT_THUMB,		// 左スティック押込み
		JOYKEY_RIGHT_THUMB,		// 右スティック押込み
		JOYKEY_LEFT_SHOULDER,	// L１ボタン
		JOYKEY_RIGHT_SHOULDER,	// R１ボタン
		JOYKEY_LEFT_TRIGGER,	// L２トリガー
		JOYKEY_RIGHT_TRIGGER,	// R２トリガー
		JOYKEY_A,				// Aボタン
		JOYKEY_B,				// Bボタン
		JOYKEY_X,				// Xボタン
		JOYKEY_Y,				// Yボタン
		JOYKEY_LEFT_STICK,		// 左スティック
		JOYKEY_RIGHT_STICK,		// 右スティック
		JOYKEY_MAX
	}JOYKEY;

	CInputController();
	~CInputController();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);		// 入力処理全部の初期化
	void Uninit(void);									// 入力処理全部の終了処理
	void Update(void);									// 入力処理全部の更新処理

	bool GetPress(JOYKEY Key, int nPlayer);					// ジョイパッドプレス処理
	bool GetTrigger(JOYKEY Key, int nPlayer);				// ジョイパッドトリガー処理
	D3DXVECTOR3 GetJoypadStick(JOYKEY Key, int nPlayer);			// ジョイパッドスティック処理
	int GetJoypadTriggerPedal(JOYKEY Key, int nPlayer);				// ジョイパッドトリガーペダル処理
	void JoypadVibration(int nTime, WORD nStrength, int nPlayer);	// ジョイパッド振動制御

private:

	LPDIRECTINPUT8 m_pInput;				// DirectInputオブジェクトへのポインタ
	LPDIRECTINPUTDEVICE8 m_pDevJoypad;		// 入力デバイス（キーボード（コントローラー用は別に作る））へのポインタ

	//ジョイパッド
	XINPUT_STATE m_JoyKeyState[PLAYER_MAX];				// ジョイパットのプレス情報
	XINPUT_STATE m_JoyKeyStateTrigger[PLAYER_MAX];		// ジョイパットのトリガー情報
	D3DXVECTOR3 m_JoyStickPos[PLAYER_MAX];				// ジョイスティックの傾き
	XINPUT_VIBRATION m_JoyMoter[PLAYER_MAX];			// ジョイパッドのモーター
	int m_nTime[PLAYER_MAX];							// 振動持続時間
	WORD m_nStrength[PLAYER_MAX];						// 振動の強さ (0 - 65535)

};
#endif