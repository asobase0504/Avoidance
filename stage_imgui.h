//**************************************************
// 
// stage_imgui.h
// Author  : Yuda Kaito
// 
//**************************************************
#ifndef _STAGE_IMGUI_H_	// このマクロが定義されてなかったら
#define _STAGE_IMGUI_H_	// 2重インクルード防止のマクロ定義

//==================================================
// インクルード
//==================================================
#include "imgui_property.h"

//--------------------------------------------------
// 前方宣言
//--------------------------------------------------
class CPlayer;

//--------------------------------------------------
// パーティクル用のImgui表示クラス
//--------------------------------------------------
class CStageImgui : public CImguiProperty
{
private:	// 定義

public:
	CStageImgui();
	~CStageImgui();

	HWND Init(HWND hWnd, LPDIRECT3DDEVICE9 pDevice) override;
	void Uninit(HWND hWnd, WNDCLASSEX wcex) override;
	void Update() override;
	void Draw() override;

private:
	bool StageProperty();
	void SetStageBase();
	void StageArrangement();
	
	void EnemyProperty();

private:
	CPlayer* m_player;

	bool s_bEffectEnable = true;
	const unsigned int gpu_id = 0;
};

#endif // !_IMGUI_PROPERTY_H_
