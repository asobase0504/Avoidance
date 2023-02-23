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
#include "delay_process.h"

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
#include "mouse_object.h"
#include "select.h"
#include "clear.h"

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
	m_stageSection(0),
	m_player(nullptr),
	m_countdown(nullptr),
	m_section(0),
	m_fallCount(0),
	m_isDeathStop(false),
	m_nextText(nullptr),
	m_retryText(nullptr),
	m_backText(nullptr),
	m_mouseCursor(nullptr)
{
	m_stagePath.clear();
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

	m_stagePath = LoadPath("data/FILE/stageGroup.json");

	m_stage = LoadAll(m_stagePath[m_stageSection]);
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
	// ステージが始まるまでのカウントダウン
	if (m_countdown != nullptr)
	{
		m_player->SetIsMove(false);		// プレイヤーの移動を無効にする

		if (m_countdown->IsEnd())
		{
			m_countdown->SetUpdateStatus(CObject::EUpdateStatus::END);
			m_countdown = nullptr;
			m_stage->SetStart(true);	// ステージをスタートする
			m_player->SetIsMove(true);	// プレイヤーの移動を許可する
		}
	}

	// ステージがクリアされてなくて、カウントダウン中でもなく、プレイヤーが死んでいない場合
	if ((CInput::GetKey()->Trigger(DIK_P) || CInput::GetKey()->Trigger(JOYPAD_START, 0)) && !m_stage->IsEnd() && m_countdown == nullptr && !m_player->IsDied())
	{
		CPause* pause = new CPause;
		pause->Init();
	}

	StageClear();
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
		CClear::Create(CApplication::CENTER_POS);

		/* ステージのリセット */
		m_stage->SetStart(false);	// スタートしてない状態にする
		m_stage->PopReset();		// 敵が湧かないようにする

		// Enemy全てにリリース処理をかける
		CObject::TypeAllFunc(CObject::EType::ENEMY, CTaskGroup::EPriority::LEVEL_3D_1, [](CObject* inObject)
		{
			inObject->Release();
		});

		// UIを並べる場所を設定
		D3DXVECTOR3 pos = CApplication::CENTER_POS;
		pos.x += 375.0f;
		pos.y += 180.0f;

		{// 次へUIの作成
			pos.y -= 70.0f;
			m_nextText = CSelect::Create(pos);
			m_nextText->SetSize(D3DXVECTOR3(100.0f, 17.5f, 0.0f));
			m_nextText->SetTexture("TEXT_NEXT");
			m_nextText->SetSelect(true);

			// 選択していたらこの処理を行う
			m_nextText->SetFunctionSelection([this](CSelect* inSelect)
			{
				inSelect->SetSize(D3DXVECTOR3(100.0f, 17.5f, 0.0f) * 1.5f);

				if (CInput::GetKey()->Trigger(JOYPAD_DOWN, 0))
				{
					m_retryText->SetSelect(true);
					inSelect->SetSelect(false);
				}

				if (CInput::GetKey()->Trigger(JOYPAD_UP, 0))
				{
					m_backText->SetSelect(true);
					inSelect->SetSelect(false);
				}
			});
			// 選択されていなかったらこの処理を行う
			m_nextText->SetFunctionNoSelection([](CSelect* inSelect)
			{
				inSelect->SetSize(D3DXVECTOR3(100.0f, 17.5f, 0.0f));

			});
			// クリックしたらこの処理を行う
			m_nextText->SetFunctionClick([this](CSelect* inSelect)
			{
				// マウスの位置ロック
				CInput::GetKey()->GetMouse()->UseSetPosLock(true);
				m_mouseCursor->SetUpdateStatus(CObject::EUpdateStatus::END);

				m_player->SetPos(D3DXVECTOR3(0.0f, 1900.0f, 0.0f));
				m_fallCount = 0;
				NextStage();

				m_nextText->Release();
				m_retryText->Release();
				m_backText->Release();
			});
		}

		{// もう一度UIの作成
			pos.y += 70.0f;
			m_retryText = CSelect::Create(pos);
			m_retryText->SetSize(D3DXVECTOR3(100.0f, 17.5f, 0.0f));
			m_retryText->SetTexture("TEXT_RETRY");

			// 選択していたらこの処理を行う
			m_retryText->SetFunctionSelection([this](CSelect* inSelect)
			{
				inSelect->SetSize(D3DXVECTOR3(100.0f, 17.5f, 0.0f) * 1.5f);

				if (CInput::GetKey()->Trigger(JOYPAD_DOWN, 0))
				{
					
					m_backText->SetSelect(true);
					inSelect->SetSelect(false);
				}

				if (CInput::GetKey()->Trigger(JOYPAD_UP, 0))
				{
					m_nextText->SetSelect(true);
					inSelect->SetSelect(false);
				}
			});
			// 選択されていなかったらこの処理を行う
			m_retryText->SetFunctionNoSelection([](CSelect* inSelect)
			{
				inSelect->SetSize(D3DXVECTOR3(100.0f, 17.5f, 0.0f));

			});
			// クリックしたらこの処理を行う
			m_retryText->SetFunctionClick([this](CSelect* inSelect)
			{
				// マウスの位置ロック
				CInput::GetKey()->GetMouse()->UseSetPosLock(true);
				m_mouseCursor->SetUpdateStatus(CObject::EUpdateStatus::END);

				RetryStage();

				m_nextText->Release();
				m_retryText->Release();
				m_backText->Release();
			});
		}

		{// タイトルに戻るUIの作成
			pos.y += 70.0f;
			m_backText = CSelect::Create(pos);
			m_backText->SetSize(D3DXVECTOR3(100.0f, 17.5f, 0.0f));
			m_backText->SetTexture("TEXT_TITLE");

			// 選択していたらこの処理を行う
			m_backText->SetFunctionSelection([this](CSelect* inSelect)
			{
				inSelect->SetSize(D3DXVECTOR3(100.0f, 17.5f, 0.0f) * 1.5f);

				if (CInput::GetKey()->Trigger(JOYPAD_DOWN, 0))
				{

					m_nextText->SetSelect(true);
					inSelect->SetSelect(false);
				}

				if (CInput::GetKey()->Trigger(JOYPAD_UP, 0))
				{
					m_retryText->SetSelect(true);
					inSelect->SetSelect(false);
				}
			});
			// 選択されていなかったらこの処理を行う
			m_backText->SetFunctionNoSelection([](CSelect* inSelect)
			{
				inSelect->SetSize(D3DXVECTOR3(100.0f, 17.5f, 0.0f));

			});
			// クリックしたらこの処理を行う
			m_backText->SetFunctionClick([this](CSelect* inSelect)
			{
				// マウスの位置ロック
				CInput::GetKey()->GetMouse()->UseSetPosLock(true);
				inSelect->SetUpdateStatus(CObject::EUpdateStatus::END);

				m_fallCount = 0;
				CApplication::GetInstance()->GetFade()->NextMode(CApplication::MODE_TITLE);
			});
		}

		// マウスの見た目の作成(位置の固定化を解除)
		m_mouseCursor = CMouseObject::Create();
	}

	m_fallCount++;

	if (m_fallCount > FALL_TIME)
	{
		m_fallCount = FALL_TIME + 1;

		m_player->SetMove(D3DXVECTOR3(0.0f,0.0f,0.0f));
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

	m_stageSection++;

	m_stageNext = LoadAll(m_stagePath[m_stageSection]);

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
	m_player->SetPos(D3DXVECTOR3(0.0f, 100.0f, 0.0f));
	m_fallCount = 0;

	m_countdown = CCountdown::Create(CApplication::CENTER_POS);

	// Enemyに全てリリース処理をかける
	CObject::TypeAllFunc(CObject::EType::ENEMY, CTaskGroup::EPriority::LEVEL_3D_1, [](CObject* inObject)
	{
		inObject->Release();
	});

	m_stage->Release();	// 終了処理

	m_stage = LoadAll(m_stagePath[m_stageSection]);		// 現在のステージをもう一度読み込む

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
		// プレイヤーの位置をステージの中心のちょっと上に変更する
		CDelayProcess::DelayProcess(CPlayerDied::MAX_LIFE - CPlayerDied::AGGREGATE_TIME, this, [this]()
		{
			D3DXVECTOR3 pos = m_stage->GetPos();
			pos.y += 50.0f;

			CPlayerDied::SetOriginPos(pos);

			pos -= m_player->GetPos();

			pos /= (float)CPlayerDied::AGGREGATE_TIME;
			m_player->AddMove(pos);
		});

		m_isDeathStop = true;

		// Enemyに全てリリース処理をかける
		CObject::TypeAllFunc(CObject::EType::ENEMY, CTaskGroup::EPriority::LEVEL_3D_1, [](CObject* inObject)
		{
			inObject->Release();
		});
	}

	if (m_isDeathStop)
	{
		m_stage->PopReset();

		if (!m_player->IsDied())
		{
			m_stage->ResetGoal();
			m_isDeathStop = false;
		}
	}
}
