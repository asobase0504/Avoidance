//=============================================================================
//
// レンダラー
// Author : 浜田琉雅
//
//=============================================================================
#ifndef _RENDERER_H_			// このマクロ定義がされてなかったら
#define _RENDERER_H_			// 二重インクルード防止のマクロ定義

//-----------------------------------------------------------------------------
// ライブラリーリンク
//-----------------------------------------------------------------------------
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"winmm.lib")

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include <Windows.h>
#include <tchar.h> // _T
#include <string>

#pragma push_macro("new")
#undef new
#include <d3dx9.h>
#pragma pop_macro("new")

//*****************************************************************************
// 定数定義
//*****************************************************************************
namespace
{
	// ウインドウのクラス名
	LPCTSTR CLASS_NAME = _T("AppClass");
	// ウインドウのキャプション名
	LPCTSTR WINDOW_NAME = _T("AvoidBox");
}

const int SCREEN_WIDTH = 1280;	// スクリーンの幅
const int SCREEN_HEIGHT = 720;	// スクリーンの高さ

#ifdef _DEBUG
int GetTime(void);
#endif // _DEBUG

//=============================================================================
// 前方宣言
//=============================================================================
class CDebugProc;

//=============================================================================
// 描画の設定
//=============================================================================
class CRenderer
{
public:	// シングルトン用のインスタンス
	static CRenderer* GetInstance();
	static CRenderer* m_renderer;
private:
	CRenderer();
public:
	~CRenderer();
	HRESULT Init(HWND hWnd, bool bWindow);
	void Uninit();
	void Update();
	void Draw();

	// Getter
	LPDIRECT3DDEVICE9 GetDevice() { return m_pD3DDevice; }

private:	// メンバー変数
	LPDIRECT3D9 m_pD3D;				// Direct3Dオブジェクト
	LPDIRECT3DDEVICE9 m_pD3DDevice;	// Deviceオブジェクト

#ifdef _DEBUG
	CDebugProc* m_debugProc;
#endif // _DEBUG

};
#endif