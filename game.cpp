//=============================================================================
//
// ゲーム画面
// Author : Yuda Kaito
//
//=============================================================================
//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "game.h"
/* Sestem系統 */
#include "application.h"
#include "fade.h"
#include "sound.h"
#include "input.h"
#include "color.h"
/* 3D系統 */
#include "camera_game.h"
#include "light.h"

#include "stage.h"
#include "load_stage.h"
/* Object系統 */
#include "object_polygon3d.h"
#include "player.h"
#include "goal.h"
#include "pause.h"
/* Enemy系統 */
#include "enemy_oneway.h"

//-----------------------------------------------------------------------------
// 静的メンバ変数の宣言
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
CGame::CGame()
{
}

//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
CGame::~CGame()
{
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
HRESULT CGame::Init(void)
{
	// ライト
	CLight* light = new CLight;
	light->Init();

	// カメラ
	CCamera* camera = new CCameraGame;
	camera->Init();

	// 音
	CApplication::GetInstance()->GetSound()->Play(CSound::LABEL_BGM_GAME);

	{// プレイヤー
		CPlayer* player = CPlayer::Create();
		player->LoadModel("BOX");
		player->SetPos(D3DXVECTOR3(0.0f, 30.0f, 0.0f));
		player->SetMaterialDiffuse(0, CApplication::GetInstance()->GetColor()->GetColor(CColor::COLOR_0));
		player->CalculationVtx();
	}

	{// ゴール
		CGoal* goal = CGoal::Create();
		goal->LoadModel("BOX");
		goal->SetPos(D3DXVECTOR3(60.0f, 15.0f, 0.0f));
		goal->SetRot(D3DXVECTOR3(D3DX_PI * 0.25f, D3DX_PI * 0.25f, D3DX_PI * 0.25f));
		goal->SetMaterialDiffuse(0, CApplication::GetInstance()->GetColor()->GetColor(CColor::COLOR_2));
		goal->CalculationVtx();
	}

	m_stage = LoadAll(L"data/FILE/stage.json");

	return S_OK;
}

//-----------------------------------------------------------------------------
// 終了
//-----------------------------------------------------------------------------
void CGame::Uninit(void)
{
	CApplication::GetInstance()->GetSound()->Stop();
	Release();
}

//-----------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------
void CGame::Update(void)
{
	CPlayer* player = (CPlayer*)CObject::SearchType(CObject::PLAYER,CTaskGroup::EPriority::LEVEL_3D_1);

	if (player != nullptr)
	{
		if (player->GetIsGoal())
		{
			CApplication::GetInstance()->GetFade()->NextMode(CApplication::MODE_RESULT);
		}
	}
}

//-----------------------------------------------------------------------------
// 描画
//-----------------------------------------------------------------------------
void CGame::Draw(void)
{

}
