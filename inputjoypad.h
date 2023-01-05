//=============================================================================
//
// 入力処理 [directjoypad.h]
// Author1 : KOZUNA HIROHITO
//
//=============================================================================
#ifndef _INPUTJOYPAD_H_		//このマクロ定義がされなかったら
#define _INPUTJOYPAD_H_		//2重インクルード防止のマクロ定義

//----------------------------------------------------------------------------
//インクルードファイル
//----------------------------------------------------------------------------
#include "DirectInput.h"
#include "inputkeydata.h"

//----------------------------------------------------------------------------
//クラス定義
//----------------------------------------------------------------------------
class CInputJoyPad : public CDirectInput
{

private:
	static const int MAX_JOY_KEY = 32;		// ジョイパッドの使ってないキーを含めた最大数
	static const int JOYPAD_DATA_MAX = 4;	// 同時接続可能最大数

	//ジョイパッドのひとつに必要な情報の構造体
	struct SJoyPad
	{
		LPDIRECTINPUTDEVICE8 pInputDevice;	// 入力デバイスへのポインタ
		DIJOYSTATE aKeyState;				// ジョイパッドのプレス情報
		DIJOYSTATE aKeyStateTrigger;		// ジョイパッドのトリガー情報
		DIJOYSTATE aKeyStateRelease;		// ジョイパッドのリリース情報
		DirectJoypad aOldKey;				// 前回押されたキーの種類
		int nCrossPressRot;					// ジョイパッドの十字キーの押されている方向
	};

public:
	CInputJoyPad();
	~CInputJoyPad()override;
	HRESULT Init(HINSTANCE hInstance, HWND hWnd) override;	// ジョイパッドの初期化
	void Uninit(void) override;								// ジョイパッドの終了処理
	void Update(void) override;								// ジョイパッドの更新処理

	//入力デバイスへのポインタの取得
	LPDIRECTINPUTDEVICE8 GetInputDevice() { return m_JoyPadData[m_nJoyNumCnt].pInputDevice; }
	LPDIRECTINPUTDEVICE8 GetInputDevice(int nNum) { return m_JoyPadData[nNum].pInputDevice; }

	//入力デバイスへのポインタの設定
	void SetInputDevice(LPDIRECTINPUTDEVICE8 pInputDeviceint) { m_JoyPadData[m_nJoyNumCnt].pInputDevice = pInputDeviceint; m_nJoyNumCnt++;}

	//現在接続されているジョイパッドの数の取得
	int GetJoyPadNumMax() { return m_nJoyNumCnt; }

	//*接続が確認時に行うコールバック関数
	//デバイスが確認できたら情報を生成するコールバック関数
	static BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE *pdidInstance, VOID *pContext);
	//確認が出来たジョイパッドのスティックの傾きの値の幅設定
	static BOOL CALLBACK EnumAxesCallback(const DIDEVICEOBJECTINSTANCE *pdidoi, VOID *pContext);
	//*

	bool GetPress(DirectJoypad eKey, int nNum);			// プレス処理
	bool GetTrigger(DirectJoypad eKey, int nNum);		// トリガー処理
	bool GetRelease(DirectJoypad eKey, int nNum);		// リリース処理
	bool GetCrossPress(DirectJoypad eKey, int nNum);	// 十字キープレス処理
	bool GetCrossTrigger(DirectJoypad eKey, int nNum);	// 十字キートリガー処理
	bool GetCrossRelease(DirectJoypad eKey, int nNum);	// 十字キーリリース処理
	bool GetPressAll(DirectJoypad eKey);				// オールプレス処理		（キー指定あり、プレイヤー指定なし）
	bool GetTriggerAll(DirectJoypad eKey);				// オールトリガー処理	（キー指定あり、プレイヤー指定なし）
	bool GetReleaseAll(DirectJoypad eKey);				// オールリリース処理	（キー指定あり、プレイヤー指定なし）
	bool GetPressAll(int nNum);							// オールプレス処理		（キー指定なし、プレイヤー指定あり）
	bool GetTriggerAll(int nNum);						// オールトリガー処理	（キー指定なし、プレイヤー指定あり）
	bool GetReleaseAll(int nNum);						// オールリリース処理	（キー指定なし、プレイヤー指定あり）
	bool GetPressAll();									// オールプレス処理		（キー指定なし、プレイヤー指定なし）
	bool GetTriggerAll();								// オールトリガー処理	（キー指定なし、プレイヤー指定なし）
	bool GetReleaseAll();								// オールリリース処理	（キー指定なし、プレイヤー指定なし）

	D3DXVECTOR3 GetJoyStickData(bool bleftandright, int nNum = 0);			//ジョイスティックの傾き値を返す(true  = 右、false = 左)
	int GetCross(int nNum = 0) { return m_JoyPadData[nNum].nCrossPressRot; }	//ジョイパッドの十字キーの押されている方向(例　45°など)

private:
	SJoyPad m_JoyPadData[JOYPAD_DATA_MAX];	// ジョイパッドのひとつに必要な情報の構造体
	DirectJoypad m_AllOldKey;				// 全ジョイパッド共通の前回されたキー
	int m_nJoyNumCnt;						// 現在接続の接続数
};
#endif