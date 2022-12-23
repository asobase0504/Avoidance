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

	m_stage = LoadAll(L"data/FILE/stage.json");
	m_stage->SetStart(true);
	m_stageNext = LoadAll(L"data/FILE/stage.json",D3DXVECTOR3(0.0f,-1200.0f,0.0f));

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
	if (m_stage->IsEnd())
	{
		m_stage->Release();
		m_stage = m_stageNext;
		D3DXVECTOR3 pos = m_stage->GetPos();
		m_stage->SetStart(true);

		if (pos.y < -4200.0f)
		{
			m_stageNext = LoadAll(L"data/FILE/stage.json", D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}
		else
		{
			m_stageNext = LoadAll(L"data/FILE/stage.json", D3DXVECTOR3(0.0f, pos.y - 1200.0f, 0.0f));
		}
	}
}

//-----------------------------------------------------------------------------
// 描画
//-----------------------------------------------------------------------------
void CGame::Draw(void)
{

}
