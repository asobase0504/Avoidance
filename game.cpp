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
#include "goal.h"
#include "pause.h"
#include "bg_box.h"

/* Enemy系統 */
#include "enemy_oneway.h"

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
		bg->SetColor(D3DXCOLOR(0.0f,0.0f,0.0f,1.0f));
	}

	{// プレイヤー
		CPlayer* player = CPlayer::Create();
		player->LoadModel("BOX");
		player->SetPos(D3DXVECTOR3(0.0f, 60.0f, 0.0f));
		D3DXCOLOR color = CApplication::GetInstance()->GetColor()->GetColor(CColor::COLOR_2);
		player->SetMaterialDiffuse(0, color);
		player->CalculationVtx();
	}

	m_stage = LoadAll(L"data/FILE/stage.json");
	m_stage->SetStart(true);
	//m_stageNext = LoadAll(L"data/FILE/stage.json",D3DXVECTOR3(0.0f,-1200.0f,0.0f));

	// マウスの位置ロック
	CInput::GetKey()->GetMouse()->UseSetPosLock(true);

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
	// ステージが終了時
	if (m_stage->IsEnd())
	{
		m_section++;				// ステージの回数
		m_stage->Release();			// 終了処理
		m_stage = m_stageNext;		// 次のステージを現在のステージにする
		m_stage->SetStart(true);	// ステージをスタートする

		if (m_section > 3)
		{
			m_section = 0;
			m_stageNext = LoadAll(L"data/FILE/stage.json", D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}
		else
		{
			D3DXVECTOR3 pos = m_stage->GetPos();
			m_stageNext = LoadAll(L"data/FILE/stage.json", D3DXVECTOR3(0.0f, pos.y - 1200.0f, 0.0f));
		}
	}

	if (CInput::GetKey()->Trigger(CMouse::MOUSE_KEY::MOUSE_KEY_LEFT))
	{
		CApplication::GetInstance()->GetFade()->NextMode(CApplication::MODE_TITLE);
	}

	static int cnt = 0;
	cnt++;
	if(cnt % 45 == 0)
	{
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
}

//-----------------------------------------------------------------------------
// 描画
//-----------------------------------------------------------------------------
void CGame::Draw(void)
{

}
