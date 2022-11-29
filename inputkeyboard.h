//=============================================================================
//
// 入力処理 [inputkeyboard.h]
// Author1 : KOZUNA HIROHITO
// 
//	＜最後の行を見ることを推奨＞
//
//=============================================================================

#ifndef _INPUTKEYBOARD_H_		//このマクロ定義がされなかったら
#define _INPUTKEYBOARD_H_		//2重インクルード防止のマクロ定義

//----------------------------------------------------------------------------
//ライブラリーファイルのリンク
//----------------------------------------------------------------------------
#pragma comment(lib,"dinput8.lib")//入力処理に必要(キーボード)
//#pragma comment(lib,"xinput.lib") //入力処理に必要（ジョイパット）
#pragma comment(lib,"dxguid.lib")		//DirectXコンポーネントに使用


//----------------------------------------------------------------------------
//インクルードファイル
//----------------------------------------------------------------------------
#include <Windows.h>
#pragma push_macro("new")
#undef new
#include <d3dx9.h>
#pragma pop_macro("new")
#define	 DIRECTINPUT_VERSION (0x0800)						//ビルド時の警告対策用マクロ(この位置から動かさない)
//#include <Xinput.h>				  //ジョイパット処理
#include <dinput.h>				  //入力処理に必要
//----------------------------------------------------------------------------
//クラス定義
//----------------------------------------------------------------------------
class CInputKeyboard
{
private:
	static const int NUM_KEY_MAX = 256;	//キーの最大数（キーボード）
public:
	CInputKeyboard();
	~CInputKeyboard();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);		//入力処理全部の初期化
	void Uninit(void);									//入力処理全部の終了処理
	void Update(void);									//入力処理全部の更新処理

	bool GetKeyboardPress(int nKey);			//キーボードプレス処理
	bool GetKeyboardTrigger(int nKey);			//キーボードトリガー処理
	bool GetKeyboardAllPress(void);				//キーボード全キープレス処理
	bool GetKeyboardAllTrigger(void);			//キーボード全キートリガー処理

private:
	LPDIRECTINPUT8 m_pInput;							//DirectInputオブジェクトへのポインタ
	LPDIRECTINPUTDEVICE8 m_pDevKeyboard;				//入力デバイス（キーボード（コントローラー用は別に作る））へのポインタ
	BYTE m_aKeyState[NUM_KEY_MAX];						//キーボードのプレス情報
	BYTE m_aKeyStateTrigger[NUM_KEY_MAX];				//キーボードのトリガー情報

};
#endif