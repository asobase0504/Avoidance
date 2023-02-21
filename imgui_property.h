//**************************************************
// 
// imugi_property.h
// Author  : katsuki mizuki
// Author  : Yuda Kaito
// 
//**************************************************
#ifndef _IMGUI_PROPERTY_H_	// このマクロが定義されてなかったら
#define _IMGUI_PROPERTY_H_	// 2重インクルード防止のマクロ定義

//==================================================
// インクルード
//==================================================
#include "imgui.h"
#include "task.h"
#include <d3dx9.h>

//--------------------------------------------------
// 入力検知
//--------------------------------------------------
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//--------------------------------------------------
// imgui表示クラス
//--------------------------------------------------
class CImguiProperty : public CTask
{
private:	// 定義
	static const char* FontPath;			// 使用するフォント
	static const float FontSize;			// フォントサイズ
	static const ImVec4 TitleBarColor;		// タイトルバーの色
	static const ImVec4 SliderBarColor;		// スライダーバーの色
	static const ImVec4 CheckMarkColor;		// チェックマークの色
	static const ImVec4 ScrollBarColor;		// スクロールバーの色
	static const ImVec4 HeaderColor;		// ヘッダーの基本色
	static const ImVec4 HeaderHoveredColor;	// ヘッダーにマウスカーソルを合わせた時の色
	static const ImVec4 HeaderActiveColor;	// ヘッダークリック時の色

	//static const int	MAX_TEXT;		// テキストの最大文字数
	static const char*	WINDOW_NAME;	// ウインドウの名前 (キャプションに表示)
public:
	CImguiProperty();
	~CImguiProperty();

	HRESULT Init() { return S_OK; }
	virtual HWND Init(HWND hWnd, LPDIRECT3DDEVICE9 pDevice);
	virtual void Uninit(HWND hWnd, WNDCLASSEX wcex);
	virtual void Update();
	virtual void Draw();

	char* GetFileName();
	void SetFileName(char*FileStringData);

protected:
	// カラーパレット
	bool ColorPalette4(const char* label, float col[4]);
	bool ColorPalette(float color[4], float backup_color[4], ImGuiColorEditFlags flags);

	// 型の変換
	ImVec4 ColorToImVec4(const D3DXCOLOR& color);
	D3DXCOLOR ImVec4ToColor(const ImVec4& vec);

protected:
	bool s_window = false;	// ウインドウを使用するかどうか
private:
	char FileString[MAX_PATH * 256];	// ファイル名
};

#endif // !_IMGUI_PROPERTY_H_
