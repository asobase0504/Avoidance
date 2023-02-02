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
#include "utility.h"

/* 3D系統 */
#include "camera_game.h"
#include "light.h"
#include "stage.h"
#include "load_stage.h"

/* Object系統 */
#include "object_polygon3d.h"
#include "player.h"
#include "player_died.h"
#include "goal.h"
#include "pause.h"
#include "bg_box.h"
#include "countdown.h"

//-----------------------------------------------------------------------------
// 静的メンバ変数の宣言
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
CGame::CGame():
	m_stage(nullptr),
	m_stageNext(nullptr),
	m_section(0)
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
	m_section = 0;
	CApplication::GetInstance()->GetColor()->SetTheme(3);

	// ライト
	CLight* light = new CLight;
	light->Init();

	// カメラ
	CCamera* camera = new CCameraGame;
	camera->Init();

	// 音
	CApplication::GetInstance()->GetSound()->Play(CSound::LABEL_BGM_GAME);

	// 背景
	{
		CObject2d* bg = CObject2d::Create();
		bg->SetPos(CApplication::CENTER_POS);
		bg->SetSize(CApplication::CENTER_POS);
		D3DXCOLOR color = CApplication::GetInstance()->GetColor()->GetColor(CColor::COLOR_0);
		bg->SetColor(color);
	}

	{// プレイヤー
		m_player = CPlayer::Create();
		m_player->LoadModel("BOX");
		m_player->SetPos(D3DXVECTOR3(0.0f, 60.0f, 0.0f));
		D3DXCOLOR color = CApplication::GetInstance()->GetColor()->GetColor(CColor::COLOR_2);
		m_player->SetMaterialDiffuse(0, color);
		m_player->CalculationVtx();
	}

	m_stage = LoadAll("data/FILE/stage.json");
	m_stage->SetStart(false);
	m_stageNext = LoadAll("data/FILE/stage.json",D3DXVECTOR3(0.0f,-3600.0f,0.0f));

	// マウスの位置ロック
	CInput::GetKey()->GetMouse()->UseSetPosLock(true);

	m_countdown = CCountdown::Create(CApplication::CENTER_POS);

	m_isDeathStop = false;
	return S_OK;
}

//-----------------------------------------------------------------------------
// 終了
//-----------------------------------------------------------------------------
void CGame::Uninit(void)
{
	CInput::GetKey()->GetMouse()->UseSetPosLock(false);	// ロックの解除
	CApplication::GetInstance()->GetSound()->Stop();	// 音楽の停止
	Release();
}

//-----------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------
void CGame::Update(void)
{
	// ステージが始まるまでの間隔
	if (m_countdown != nullptr)
	{
		if (m_countdown->IsEnd())
		{
			m_countdown->SetUpdateStatus(CObject::EUpdateStatus::END);
			m_countdown = nullptr;
			m_stage->SetStart(true);	// ステージをスタートする
			m_player->SetIsMove(true);	// プレイヤーの移動を許可する
		}
	}

	// ポーズ
	if (CInput::GetKey()->Trigger(DIK_P))
	{
		CPause* pause = new CPause;
		pause->Init();
	}

	StageClear();
	BackStaging();
	PlayerDeath();
}

//-----------------------------------------------------------------------------
// 1ステージクリア時
//-----------------------------------------------------------------------------
void CGame::StageClear()
{
	// ステージが終了時
	if (!m_stage->IsEnd())
	{
		return;
	}

	m_section++;			// ステージの回数
	m_stage->Release();		// 終了処理
	m_stage = m_stageNext;	// 次のステージを現在のステージにする
	m_stage->SetStart(false);

	m_countdown = CCountdown::Create(CApplication::CENTER_POS);

	if (m_section > 3)
	{
		m_section = 0;
		m_stageNext = LoadAll("data/FILE/stage.json", D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}
	else
	{
		D3DXVECTOR3 pos = m_stage->GetPos();
		m_stageNext = LoadAll("data/FILE/stage.json", D3DXVECTOR3(0.0f, pos.y - 3600.0f, 0.0f));
	}
}

//-----------------------------------------------------------------------------
// プレイヤー死亡時
//-----------------------------------------------------------------------------
void CGame::PlayerDeath()
{
	if (m_player->IsDied() && !m_isDeathStop)
	{
		D3DXVECTOR3 pos = m_stage->GetPos();
		pos.y += 100.0f;

		CPlayerDied::SetOriginPos(pos);
		pos -= m_player->GetPos();
		pos /= 150.0f;
		m_player->AddMove(pos);
		m_isDeathStop = true;

		// 最初に見つけた指定したタイプのobjectを持ってくる
		CObject* object = CObject::SearchType(CObject::EType::ENEMY, CTaskGroup::EPriority::LEVEL_3D_1);

		while (object != nullptr)
		{
			CObject* next = object->NextSameType();	// 同じタイプのobjectを持ってくる

			object->Release();

			object = next;
		}
	}

	if (m_isDeathStop)
	{
		m_stage->PopReset();

		if (!m_player->IsDied())
		{
			m_isDeathStop = false;
		}
	}
}

//-----------------------------------------------------------------------------
// 後ろの演出
//-----------------------------------------------------------------------------
void CGame::BackStaging()
{
	// 背景の演出
	static int cnt = 0;
	cnt++;
	if (cnt % 90 != 0)
	{
		return;
	}

	cnt = 0;

	{
		CBgBox* box = CBgBox::Create();
		box->SetPos(D3DXVECTOR3(FloatRandam(9000.0f, 1000.0f), -9000.0f, FloatRandam(9000.0f, 1000.0f)));
		box->SetScale(D3DXVECTOR3(40.0f, 40.0f, 40.0f));
	}

	{
		CBgBox* box = CBgBox::Create();
		box->SetPos(D3DXVECTOR3(FloatRandam(-1000.0f, -9000.0f), -9000.0f, FloatRandam(-1000.0f, -9000.0f)));
		box->SetScale(D3DXVECTOR3(40.0f, 40.0f, 40.0f));
	}

	{
		CBgBox* box = CBgBox::Create();
		box->SetPos(D3DXVECTOR3(FloatRandam(9000.0f, 1000.0f), -9000.0f, FloatRandam(-1000.0f, -9000.0f)));
		box->SetScale(D3DXVECTOR3(40.0f, 40.0f, 40.0f));
	}

	{
		CBgBox* box = CBgBox::Create();
		box->SetPos(D3DXVECTOR3(FloatRandam(-1000.0f, -9000.0f), -9000.0f, FloatRandam(9000.0f, 1000.0f)));
		box->SetScale(D3DXVECTOR3(40.0f, 40.0f, 40.0f));
	}
}
