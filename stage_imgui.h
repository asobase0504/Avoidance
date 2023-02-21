//**************************************************
// 
// stage_imgui.h
// Author  : Yuda Kaito
// 
//**************************************************
#ifndef _STAGE_IMGUI_H_	// ���̃}�N������`����ĂȂ�������
#define _STAGE_IMGUI_H_	// 2�d�C���N���[�h�h�~�̃}�N����`

//==================================================
// �C���N���[�h
//==================================================
#include "imgui_property.h"

//--------------------------------------------------
// �O���錾
//--------------------------------------------------
class CPlayer;

//--------------------------------------------------
// �p�[�e�B�N���p��Imgui�\���N���X
//--------------------------------------------------
class CStageImgui : public CImguiProperty
{
private:	// ��`

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
