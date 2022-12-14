//=============================================================================
//
// Q[ζΚ
// Author : Yuda Kaito
//
//=============================================================================
//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "game.h"
/* Sestemn */
#include "application.h"
#include "fade.h"
#include "sound.h"
#include "input.h"
#include "color.h"

/* 3Dn */
#include "camera_game.h"
#include "light.h"
#include "stage.h"
#include "load_stage.h"

/* Objectn */
#include "object_polygon3d.h"
#include "player.h"
#include "goal.h"
#include "pause.h"

/* Enemyn */
#include "enemy_oneway.h"

//-----------------------------------------------------------------------------
// ΓIoΟΜιΎ
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// RXgN^
//-----------------------------------------------------------------------------
CGame::CGame():
	m_stage(nullptr),
	m_stageNext(nullptr),
	m_section(0)
{
}

//-----------------------------------------------------------------------------
// fXgN^
//-----------------------------------------------------------------------------
CGame::~CGame()
{
}

//-----------------------------------------------------------------------------
// ϊ»
//-----------------------------------------------------------------------------
HRESULT CGame::Init(void)
{
	m_section = 0;

	// Cg
	CLight* light = new CLight;
	light->Init();

	// J
	CCamera* camera = new CCameraGame;
	camera->Init();

	// Ή
	CApplication::GetInstance()->GetSound()->Play(CSound::LABEL_BGM_GAME);

	// wi
	{
		CObject2d* bg = CObject2d::Create();
		bg->SetPos(CApplication::CENTER_POS);
		bg->SetSize(CApplication::CENTER_POS);
		bg->SetColor(CApplication::GetInstance()->GetColor()->GetColor(CColor::COLOR_0));
	}

	{// vC[
		CPlayer* player = CPlayer::Create();
		player->LoadModel("BOX");
		player->SetPos(D3DXVECTOR3(0.0f, 30.0f, 0.0f));
		D3DXCOLOR color = CApplication::GetInstance()->GetColor()->GetColor(CColor::COLOR_2);
		player->SetMaterialDiffuse(0, color);
		player->CalculationVtx();
	}

	m_stage = LoadAll(L"data/FILE/stage.json");
	m_stage->SetStart(true);
	m_stageNext = LoadAll(L"data/FILE/stage.json",D3DXVECTOR3(0.0f,-1200.0f,0.0f));

	// }EXΜΚubN
	CInput::GetKey()->GetMouse()->UseSetPosLock(true);

	return S_OK;
}

//-----------------------------------------------------------------------------
// IΉ
//-----------------------------------------------------------------------------
void CGame::Uninit(void)
{
	CApplication::GetInstance()->GetSound()->Stop();
	Release();
}

//-----------------------------------------------------------------------------
// XV
//-----------------------------------------------------------------------------
void CGame::Update(void)
{
	// Xe[WͺIΉ
	if (m_stage->IsEnd())
	{
		m_section++;				// Xe[WΜρ
		m_stage->Release();			// IΉ
		m_stage = m_stageNext;		// ΜXe[Wπ»έΜXe[WΙ·ι
		m_stage->SetStart(true);	// Xe[WπX^[g·ι

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
}

//-----------------------------------------------------------------------------
// `ζ
//-----------------------------------------------------------------------------
void CGame::Draw(void)
{

}
