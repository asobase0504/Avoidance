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
// 定数
//-----------------------------------------------------------------------------
const int CGame::FALL_TIME = 145;

//-----------------------------------------------------------------------------
// コンストラクタ
// Author : Yuda Kaito
//-----------------------------------------------------------------------------
CGame::CGame():
	m_stage(nullptr),
	m_stageNext(nullptr),
	m_section(0),
	m_fallCount(0)
{
}

//-----------------------------------------------------------------------------
// デストラクタ
// Author : Yuda Kaito
//-----------------------------------------------------------------------------
CGame::~CGame()
{
}

//-----------------------------------------------------------------------------
// 初期化
// Author : Yuda Kaito
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

	// マウスの位置ロック
	CInput::GetKey()->GetMouse()->UseSetPosLock(true);

	m_countdown = CCountdown::Create(CApplication::CENTER_POS);

	m_isDeathStop = false;
	return S_OK;
}

//-----------------------------------------------------------------------------
// 終了
// Author : Yuda Kaito
//-----------------------------------------------------------------------------
void CGame::Uninit(void)
{
	CInput::GetKey()->GetMouse()->UseSetPosLock(false);	// ロックの解除
	CApplication::GetInstance()->GetSound()->Stop();	// 音楽の停止
	Release();
}

//-----------------------------------------------------------------------------
// 更新
// Author : Yuda Kaito
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
// Author : Yuda Kaito
//-----------------------------------------------------------------------------
void CGame::StageClear()
{
	// ステージが終了時
	if (!m_stage->IsEnd())
	{
		return;
	}

	if (m_fallCount == 0)
	{
		// 最初に見つけた指定したタイプのobjectを持ってくる
		CObject* object = CObject::SearchType(CObject::EType::ENEMY, CTaskGroup::EPriority::LEVEL_3D_1);

		while (object != nullptr)
		{
			CObject* next = object->NextSameType();	// 同じタイプのobjectを持ってくる

			object->Release();

			object = next;
		}
	}

	m_fallCount++;

	if (m_fallCount >= FALL_TIME)
	{
		m_fallCount = FALL_TIME;

		m_player->SetMove(D3DXVECTOR3(0.0f,0.0f,0.0f));

		// 次のステージに移行
		if (CInput::GetKey()->Trigger(DIK_RETURN))
		{
			m_player->SetPos(D3DXVECTOR3(0.0f, 1900.0f, 0.0f));
			m_fallCount = 0;
			NextStage();
		}

		// タイトルに移行
		if (CInput::GetKey()->Trigger(DIK_BACK))
		{
			m_fallCount = 0;
			CApplication::GetInstance()->GetFade()->NextMode(CApplication::MODE_TITLE);
		}

		// リトライ
		if (CInput::GetKey()->Trigger(DIK_R))
		{
			m_player->SetPos(D3DXVECTOR3(0.0f, 1900.0f, 0.0f));
			m_fallCount = 0;
			RetryStage();
		}
	}
}

//-----------------------------------------------------------------------------
// 次のステージへ移行
// Author : Yuda Kaito
//-----------------------------------------------------------------------------
void CGame::NextStage()
{
	m_countdown = CCountdown::Create(CApplication::CENTER_POS);

	m_stage->Release();	// 終了処理

	m_stageNext = LoadAll("data/FILE/stage.json");

	m_stage = m_stageNext;		// 次のステージを現在のステージにする
	m_stage->SetStart(false);	// スタートしてない状態にする
	m_player->SetIsGoal(false);	// プレイヤーがゴールしていない状態にする
}

//-----------------------------------------------------------------------------
// 同じステージを繰り返す
// Author : Yuda Kaito
//-----------------------------------------------------------------------------
void CGame::RetryStage()
{
	m_countdown = CCountdown::Create(CApplication::CENTER_POS);

	m_stage->Release();	// 終了処理

	m_stageNext = LoadAll("data/FILE/stage.json");

	m_stage = m_stageNext;		// 次のステージを現在のステージにする
	m_stage->SetStart(false);	// スタートしてない状態にする
	m_player->SetIsGoal(false);	// プレイヤーがゴールしていない状態にする
}

//-----------------------------------------------------------------------------
// プレイヤー死亡時
// Author : Yuda Kaito
//-----------------------------------------------------------------------------
void CGame::PlayerDeath()
{
	if (m_player->IsDied() && !m_isDeathStop)
	{
		D3DXVECTOR3 pos = m_stage->GetPos();
		pos.y += 60.0f;

		CPlayerDied::SetOriginPos(pos);
		pos -= m_player->GetPos();
		pos /= 210.0f;
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
// Author : Yuda Kaito
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
