//=============================================================================
//
// ゲーム画面
// Author : Yuda Kaito
//
//=============================================================================
//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "stage_edit.h"
/* Sestem系統 */
#include "application.h"
#include "fade.h"
#include "sound.h"
#include "input.h"
#include "color.h"
#include "utility.h"
#include "delay_process.h"
#include "stage_imgui.h"

/* 3D系統 */
#include "camera_edit.h"
#include "light.h"
#include "stage.h"
#include "load_stage.h"

/* Object系統 */
#include "edit_operator.h"

#include "object_polygon3d.h"
#include "player.h"
#include "player_died.h"
#include "goal.h"
#include "pause.h"
#include "bg_box.h"
#include "countdown.h"
#include "mouse_object.h"
#include "select.h"

//-----------------------------------------------------------------------------
// コンストラクタ
// Author : Yuda Kaito
//-----------------------------------------------------------------------------
CStageEdit::CStageEdit() :
	m_stage(nullptr),
	m_stageSectionID(0)
{
	m_stagePath.clear();
}

//-----------------------------------------------------------------------------
// デストラクタ
// Author : Yuda Kaito
//-----------------------------------------------------------------------------
CStageEdit::~CStageEdit()
{
}

//-----------------------------------------------------------------------------
// 初期化
// Author : Yuda Kaito
//-----------------------------------------------------------------------------
HRESULT CStageEdit::Init(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	imgui = new CStageImgui;
	imgui->Init(CApplication::GetInstance()->GetWindow(), pDevice);

	CInput::GetKey()->GetMouse()->SetShowCursor(true);	// マウスカーソルの表示
	CApplication::GetInstance()->GetColor()->SetTheme(3);

	// ライト
	CLight* light = new CLight;
	light->Init();

	// カメラ
	CCamera* camera = new CCameraEdit;
	camera->Init();

	// 背景
	{
		CObject2d* bg = CObject2d::Create();
		bg->SetPos(CApplication::CENTER_POS);
		bg->SetSize(CApplication::CENTER_POS);
		D3DXCOLOR color = CApplication::GetInstance()->GetColor()->GetColor(CColor::COLOR_0);
		bg->SetColor(color);
	}

	m_stagePath = LoadPath("data/FILE/stageGroup.json");

	m_stage = LoadAll(m_stagePath[m_stageSectionID]);
	m_stage->SetStart(false);
	m_stage->StopPopEnemy();

	{
		m_operator = CEditOperator::Create();
		m_operator->LoadModel("BOX");
		m_operator->SetPos(D3DXVECTOR3(0.0f, 60.0f, 0.0f));
		D3DXCOLOR color = CApplication::GetInstance()->GetColor()->GetColor(CColor::COLOR_2);
		m_operator->SetMaterialDiffuse(0, color);
		m_operator->CalculationVtx();
	}

	return S_OK;
}

//-----------------------------------------------------------------------------
// 終了
// Author : Yuda Kaito
//-----------------------------------------------------------------------------
void CStageEdit::Uninit(void)
{
	CApplication::GetInstance()->GetSound()->Stop();	// 音楽の停止
	CInput::GetKey()->GetMouse()->SetShowCursor(false);	// マウスカーソルの表示

	Release();
}

//-----------------------------------------------------------------------------
// 更新
// Author : Yuda Kaito
//-----------------------------------------------------------------------------
void CStageEdit::Update(void)
{
	// タイトルに戻る
	if (CInput::GetKey()->Trigger(DIK_BACK))
	{
		CApplication::GetInstance()->GetFade()->NextMode(CApplication::MODE_TITLE);
	}

	D3DXCOLOR color;

	// 編集モードの切り替え
	if (CInput::GetKey()->Trigger(DIK_TAB))
	{
		switch (m_type)
		{
		case CStageEdit::NEW_ENEMY:
			m_type = NEW_BLOCK;
			color = CApplication::GetInstance()->GetColor()->GetColor(CColor::COLOR_0);
			m_operator->SetMaterialDiffuse(0, color);
			m_operator->SetScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
			m_operator->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

			break;
		case CStageEdit::NEW_BLOCK:
			m_type = RE_GOAL;
			m_operator->SetScale(D3DXVECTOR3(0.1f, 0.1f, 0.1f));

			break;
		case CStageEdit::RE_GOAL:
			m_type = RE_ENEMY;
			color = CApplication::GetInstance()->GetColor()->GetColor(CColor::COLOR_3);
			m_operator->SetMaterialDiffuse(0, color);
			m_operator->SetScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
			m_operator->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			break;
		case CStageEdit::RE_ENEMY:
			m_type = RE_BLOCK;
			color = CApplication::GetInstance()->GetColor()->GetColor(CColor::COLOR_0);
			m_operator->SetMaterialDiffuse(0, color);
			m_operator->SetScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
			m_operator->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			break;
		case CStageEdit::RE_BLOCK:
			m_type = FREE;
			color = CApplication::GetInstance()->GetColor()->GetColor(CColor::COLOR_2);
			m_operator->SetMaterialDiffuse(0, color);
			m_operator->SetScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
			m_operator->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			break;
		case CStageEdit::FREE:
			m_type = NEW_ENEMY;
			color = CApplication::GetInstance()->GetColor()->GetColor(CColor::COLOR_3);
			m_operator->SetMaterialDiffuse(0, color);
			m_operator->SetScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
			m_operator->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			break;
		default:
			break;
		}

		m_operator->SetEditType(m_type);
	}
}
