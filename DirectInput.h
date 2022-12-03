//=============================================================================
//
// 入力処理 [directinput.h]
// Author1 : KOZUNA HIROHITO
//
//=============================================================================

#ifndef _DIRECTINPUT_H_		//このマクロ定義がされなかったら
#define _DIRECTINPUT_H_		//2重インクルード防止のマクロ定義

//----------------------------------------------------------------------------
// ライブラリーファイルのリンク
//----------------------------------------------------------------------------
#pragma comment(lib,"dinput8.lib")//入力処理に必要(キーボード)
#pragma comment(lib,"dxguid.lib")//DirectXコンポーネントに使用

//----------------------------------------------------------------------------
// include
//----------------------------------------------------------------------------
#include <Windows.h>
#include <d3dx9.h>
#define	 DIRECTINPUT_VERSION (0x0800)	//ビルド時の警告対策用マクロ(この位置から動かさない)
#include <dinput.h>						//入力処理に必要

//----------------------------------------------------------------------------
// クラス定義
//----------------------------------------------------------------------------
class CDirectInput
{
public:
	CDirectInput();
	virtual ~CDirectInput();

	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd) = 0;	// 派生先用の初期化
	virtual void Uninit(void) = 0;								// 派生先用の終了処理
	virtual void Update(void) = 0;								// 派生先用の更新処理

	static HRESULT Create(HINSTANCE hInstance, HWND hWnd);		// DirectInputオブジェクトの生成
	static void Break(void);									// DirectInputオブジェクトの破棄

protected:
	static LPDIRECTINPUT8 m_pInput;	// DirectInputオブジェクトへのポインタ

private:

};
#endif