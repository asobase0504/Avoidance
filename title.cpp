//=============================================================================
//
// 制作 ( タイトル )
// Author : Yuda Kaito
//
//=============================================================================
//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "title.h"
#include "application.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "color.h"
// 3D系統
#include "camera_title.h"
#include "light.h"
// Object系統
#include "object2d.h"
#include "objectX.h"
// Particle系統
#include "particle_manager.h"
#include "particle_emitter.h"

#include "title_logo.h"
#include "mouse_object.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CTitle::CTitle()
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CTitle::~CTitle()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTitle::Init(void)
{
	CApplication::GetInstance()->GetSound()->Play(CSound::LABEL_BGM_TITLE);
	CApplication::GetInstance()->GetColor()->SetTheme(3);

	// ライト
	CLight* light = new CLight;
	light->Init();

	// カメラ
	CCamera* camera = new CCameraTitle;
	camera->Init();

	// 背景
	{
		CObject2d* titleBg = CObject2d::Create();
		titleBg->SetPos(CApplication::CENTER_POS);
		titleBg->SetRot(D3DXVECTOR3(0.0f,0.0f,0.25f));
		titleBg->SetSize(CApplication::CENTER_POS * 0.9f);
		titleBg->SetColor(CApplication::GetInstance()->GetColor()->GetColor(CColor::COLOR_0));
	}

	// タイトルロゴ
	CTitleLogo* titleLogo = new CTitleLogo(CTaskGroup::LEVEL_2D_1);
	titleLogo->Init();

	{
		CObjectX* testX = CObjectX::Create(D3DXVECTOR3(15.0f, -15.0f, 0.0f));
		testX->LoadModel("BOX");
		testX->SetMoveRot(D3DXVECTOR3(0.0f, 0.01f, 0.0f));
		testX->SetMaterialDiffuse(0, D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f));
		testX->CalculationVtx();
	}

	{
		CObjectX* testX = CObjectX::Create(D3DXVECTOR3(-15.0f, -15.0f, 0.0f));
		testX->LoadModel("BOX");
		testX->SetMoveRot(D3DXVECTOR3(0.0f, 0.01f, 0.0f));
		testX->SetMaterialDiffuse(0, CApplication::GetInstance()->GetColor()->GetColor(CColor::COLOR_1));
		testX->CalculationVtx();
	}

	{
		CObjectX* testX = CObjectX::Create(D3DXVECTOR3(15.0f, 15.0f, 0.0f));
		testX->LoadModel("BOX");
		testX->SetMoveRot(D3DXVECTOR3(0.0f, 0.01f, 0.0f));
		testX->SetMaterialDiffuse(0, CApplication::GetInstance()->GetColor()->GetColor(CColor::COLOR_2));
		testX->CalculationVtx();
	}

	{
		CObjectX* testX = CObjectX::Create(D3DXVECTOR3(-15.0f, 15.0f, 0.0f));
		testX->LoadModel("BOX");
		testX->SetMoveRot(D3DXVECTOR3(0.0f, 0.01f, 0.0f));
		testX->SetMaterialDiffuse(0, CApplication::GetInstance()->GetColor()->GetColor(CColor::COLOR_3));
		testX->CalculationVtx();
	}

	// マウスの見た目生成
	CMouseObject::Create();

	return S_OK;
}

//=============================================================================
// 破棄
//=============================================================================
void CTitle::Uninit(void)
{
	CApplication::GetInstance()->GetSound()->Stop();
}

//=============================================================================
// 更新処理
//=============================================================================
void CTitle::Update(void)
{
	if (CInput::GetKey()->Trigger(KEY_DOWN) || CInput::GetKey()->Trigger(CMouse::MOUSE_KEY_LEFT))
	{
		CApplication::GetInstance()->GetFade()->NextMode(CApplication::MODE_GAME);
	}
}
