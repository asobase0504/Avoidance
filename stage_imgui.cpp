//**************************************************
//
// imgui_property.cpp
// Author  : Yuda Kaito
//
//**************************************************

//--------------------------------------------------
// include
//--------------------------------------------------
#include "stage_imgui.h"

//------------------------------
// imgui
//------------------------------
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"
#include <implot.h>

//------------------------------
// json
//------------------------------
#include <fstream>
#include "file.h"

//------------------------------
// stage
//------------------------------
#include "main.h"
#include "application.h"
#include "game.h"
#include "player.h"
#include "load_stage.h"
#include "objectX_group.h"
#include "object.h"
#include "stage.h"
#include "input.h"
#include "color.h"

//--------------------------------------------------
// コンストラクタ
//--------------------------------------------------
CStageImgui::CStageImgui() :
	s_bEffectEnable(true),
	gpu_id(0),
	m_player(nullptr)
{
}

//--------------------------------------------------
// デストラクタ
//--------------------------------------------------
CStageImgui::~CStageImgui()
{
}

//--------------------------------------------------
// 初期化
//--------------------------------------------------
HWND CStageImgui::Init(HWND hWnd, LPDIRECT3DDEVICE9 pDevice)
{
	// 初期化
	HWND outWnd = CImguiProperty::Init(hWnd, pDevice);

	return outWnd;
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void CStageImgui::Uninit(HWND hWnd, WNDCLASSEX wcex)
{
	CImguiProperty::Uninit(hWnd, wcex);
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void CStageImgui::Update()
{
	CImguiProperty::Update();

	if (!s_window)
	{// ウインドウを使用しない
		return;
	}

	static int sliderInt = 0;
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save"))
			{
				//SaveAll();
			}
			else if (ImGui::MenuItem("Load"))
			{
				//LoadAll(m_player->GetPos());
			}

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	//パーティクルのデータ出力＆読み込み
	if (ImGui::Button("LOAD"))
	{
		LoadAll("data/FILE/stage.json");
	}
	ImGui::SameLine();
	if (ImGui::Button("SAVE"))
	{
		CStage* stage = (CStage*)CApplication::GetInstance()->GetTaskGroup()->SearchRoleTop(CTask::ERole::ROLE_STAGE, CTaskGroup::EPriority::LEVEL_SYSTEM);
		SaveAll(stage,"data/FILE/stage.json");
	}
	ImGui::SameLine();

	// FPSの表示
	ImGui::Text("FPS  : %.2f", ImGui::GetIO().Framerate);

	ImGui::Separator();	// 区切り線

	//エフェクト関係
	static bool a = false;
	static std::vector<std::string> itemList;

	if(!a)
	{
		a = true;
		itemList.push_back("Stage");
		itemList.push_back("Enemy");
	}

	static const char* s_currentItem = "Stage";

	if (ImGui::BeginCombo("Combo", s_currentItem))
	{
		for (int i = 0; i < itemList.size(); ++i)
		{
			const bool is_selected = (s_currentItem == itemList[i].c_str());
			
			if (ImGui::Selectable(itemList[i].c_str(), is_selected))
			{
				s_currentItem = itemList[i].c_str();
			}

			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	if (strcmp(s_currentItem, "Stage") == 0)
	{
		StageProperty();
	}
	else if(strcmp(s_currentItem, "Enemy") == 0)
	{
		EnemyProperty();
	}

	ImGui::End();
}

//--------------------------------------------------
// ステージの編集
//--------------------------------------------------
bool CStageImgui::StageProperty()
{
	if (m_player == nullptr)
	{
		m_player = (CPlayer*)CObject::SearchType(CObject::PLAYER, CTaskGroup::EPriority::LEVEL_3D_1);
		return false;
	}

	bool existsChange = false;	// 変更があるか調べる

	auto changeDetection = [&existsChange](bool function)
	{
		if (existsChange)
		{
			return;
		}
		existsChange = function;
	};

	ImGui::Text("/* StageEdit */");

	ImGui::Separator();

	SetStageBase();
	StageArrangement();

	return existsChange;
}

//--------------------------------------------------
// ステージの基礎設定
//--------------------------------------------------
void CStageImgui::SetStageBase()
{
	CStage* stage = (CStage*)CApplication::GetInstance()->GetTaskGroup()->SearchRoleTop(ROLE_STAGE, CTaskGroup::EPriority::LEVEL_SYSTEM);

	{
		ImGui::Text("---- Scale ----");
		float scale = stage->GetScale();
		ImGui::SliderFloat("Scale", &scale, 0.0f, scale + 1.0f);
		stage->SetScale(scale);
	}
}

//--------------------------------------------------
// ステージの配置
//--------------------------------------------------
void CStageImgui::StageArrangement()
{
	if (CInput::GetKey()->Trigger(DIK_RETURN))
	{
		CStage* stage = (CStage*)CApplication::GetInstance()->GetTaskGroup()->SearchRoleTop(CTask::ERole::ROLE_STAGE, CTaskGroup::EPriority::LEVEL_SYSTEM);
		stage->AddFloor(m_player->GetPos(), m_player->GetRot(), m_player->GetScale());
	}

	D3DXCOLOR color(CApplication::GetInstance()->GetColor()->GetColor(CColor::COLOR_0));
	color.a = 0.55f;
	m_player->SetMaterialDiffuse(0,color);

	ImGui::Text("/* PosReset */");
	if (ImGui::Button("AllReset"))
	{
		m_player->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}
	ImGui::SameLine();
	if (ImGui::Button("XReset"))
	{
		D3DXVECTOR3 pos = m_player->GetPos();
		m_player->SetPos(D3DXVECTOR3(0.0f, pos.y, pos.z));
	}
	ImGui::SameLine();
	if (ImGui::Button("YReset"))
	{
		D3DXVECTOR3 pos = m_player->GetPos();
		m_player->SetPos(D3DXVECTOR3(pos.x, 0.0f, pos.z));
	}
	ImGui::SameLine();
	if (ImGui::Button("ZReset"))
	{
		D3DXVECTOR3 pos = m_player->GetPos();
		m_player->SetPos(D3DXVECTOR3(pos.x, pos.y, 0.0f));
	}

	ImGui::Separator();

	{
		ImGui::Text("---- Pos ----");
		D3DXVECTOR3 pos = m_player->GetPos();
		ImGui::SliderFloat("X", &pos.x, pos.x - 5.0f, pos.x + 5.0f);
		ImGui::SameLine();
		if (ImGui::Button("X.0"))
		{
			pos.x = floor(pos.x);
		}
		ImGui::SliderFloat("Y", &pos.y, pos.y - 5.0f, pos.y + 5.0f);
		if (ImGui::Button("Y.0"))
		{
			pos.y = floor(pos.y);
		}
		ImGui::SameLine();
		if (ImGui::Button("Y:0.0"))
		{
			pos.y = 0.0f;
		}
		ImGui::SliderFloat("Z", &pos.z, pos.z - 5.0f, pos.z + 5.0f);
		ImGui::SameLine();
		if (ImGui::Button("Z.0"))
		{
			pos.z = floor(pos.z);
		}
		m_player->SetPos(pos);
	}

	{
		ImGui::Text("---- Rot ----");
		D3DXVECTOR3 rot = m_player->GetRot();
		ImGui::SliderFloat("Rot_X", &rot.x, -D3DX_PI, D3DX_PI);
		ImGui::SameLine();
		if (ImGui::Button("Rot_X.0"))
		{
			rot.x = floor(rot.x);
		}
		ImGui::SliderFloat("Rot_Y", &rot.y, -D3DX_PI, D3DX_PI);
		if (ImGui::Button("Rot_Y.0"))
		{
			rot.y = floor(rot.y);
		}
		ImGui::SameLine();
		if (ImGui::Button("Rot_Y:0.0"))
		{
			rot.y = 0.0f;
		}
		ImGui::SliderFloat("Rot_Z", &rot.z, -D3DX_PI, D3DX_PI);
		ImGui::SameLine();
		if (ImGui::Button("Rot_Z.0"))
		{
			rot.z = floor(rot.z);
		}
		m_player->SetRot(rot);
	}

	{
		ImGui::Text("---- Scale ----");
		D3DXVECTOR3 scale = m_player->GetScale();
		ImGui::SliderFloat("Scale_X", &scale.x, 0.0f, scale.x + 1.0f);
		ImGui::SameLine();
		if (ImGui::Button("Scale_X.0"))
		{
			scale.x = floor(scale.x);
		}
		ImGui::SliderFloat("Scale_Y", &scale.y, 0.0f, scale.y + 1.0f);
		if (ImGui::Button("Scale_Y.0"))
		{
			scale.y = floor(scale.y);
		}
		ImGui::SameLine();
		ImGui::SliderFloat("Scale_Z", &scale.z, 0.0f, scale.z + 1.0f);
		ImGui::SameLine();
		if (ImGui::Button("Scale_Z.0"))
		{
			scale.z = floor(scale.z);
		}
		m_player->SetScale(scale);
	}
}

//--------------------------------------------------
// エネミーの配置
//--------------------------------------------------
void CStageImgui::EnemyProperty()
{
	if (CInput::GetKey()->Trigger(DIK_RETURN))
	{
		CStage* stage = (CStage*)CApplication::GetInstance()->GetTaskGroup()->SearchRoleTop(CTask::ERole::ROLE_STAGE, CTaskGroup::EPriority::LEVEL_SYSTEM);
		stage->AddEnemy(0, m_player->GetPos(), m_player->GetRot(), 0);
		stage->PopEnemy(0, m_player->GetPos(), m_player->GetRot());
	}

	D3DXCOLOR color(CApplication::GetInstance()->GetColor()->GetColor(CColor::COLOR_3));
	color.a = 0.55f;
	m_player->SetMaterialDiffuse(0, color);

	ImGui::Text("/* PosReset */");
	if (ImGui::Button("AllReset"))
	{
		m_player->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}
	ImGui::SameLine();
	if (ImGui::Button("XReset"))
	{
		D3DXVECTOR3 pos = m_player->GetPos();
		m_player->SetPos(D3DXVECTOR3(0.0f, pos.y, pos.z));
	}
	ImGui::SameLine();
	if (ImGui::Button("YReset"))
	{
		D3DXVECTOR3 pos = m_player->GetPos();
		m_player->SetPos(D3DXVECTOR3(pos.x, 0.0f, pos.z));
	}
	ImGui::SameLine();
	if (ImGui::Button("ZReset"))
	{
		D3DXVECTOR3 pos = m_player->GetPos();
		m_player->SetPos(D3DXVECTOR3(pos.x, pos.y, 0.0f));
	}

	ImGui::Separator();

	{
		ImGui::Text("---- Pos ----");
		D3DXVECTOR3 pos = m_player->GetPos();
		ImGui::SliderFloat("X", &pos.x, pos.x - 5.0f, pos.x + 5.0f);
		ImGui::SameLine();
		if (ImGui::Button("X.0"))
		{
			pos.x = floor(pos.x);
		}
		ImGui::SliderFloat("Y", &pos.y, pos.y - 5.0f, pos.y + 5.0f);
		if (ImGui::Button("Y.0"))
		{
			pos.y = floor(pos.y);
		}
		ImGui::SameLine();
		if (ImGui::Button("Y:0.0"))
		{
			pos.y = 0.0f;
		}
		ImGui::SliderFloat("Z", &pos.z, pos.z - 5.0f, pos.z + 5.0f);
		ImGui::SameLine();
		if (ImGui::Button("Z.0"))
		{
			pos.z = floor(pos.z);
		}
		m_player->SetPos(pos);
	}

	{
		ImGui::Text("---- Rot ----");
		D3DXVECTOR3 rot = m_player->GetRot();
		ImGui::SliderFloat("Rot_X", &rot.x, -D3DX_PI, D3DX_PI);
		ImGui::SameLine();
		if (ImGui::Button("Rot_X.0"))
		{
			rot.x = floor(rot.x);
		}
		ImGui::SliderFloat("Rot_Y", &rot.y, -D3DX_PI, D3DX_PI);
		if (ImGui::Button("Rot_Y.0"))
		{
			rot.y = floor(rot.y);
		}
		ImGui::SameLine();
		if (ImGui::Button("Rot_Y:0.0"))
		{
			rot.y = 0.0f;
		}
		ImGui::SliderFloat("Rot_Z", &rot.z, -D3DX_PI, D3DX_PI);
		ImGui::SameLine();
		if (ImGui::Button("Rot_Z.0"))
		{
			rot.z = floor(rot.z);
		}
		m_player->SetRot(rot);
	}
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void CStageImgui::Draw()
{
	CImguiProperty::Draw();
}
