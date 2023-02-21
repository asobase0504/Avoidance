//=============================================================================
//
// ポーズ画面
// Author : Yuda Kaito
//
//=============================================================================
//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "pause.h"
#include "application.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "mouse_object.h"
#include "select.h"
#include "task_group.h"
#include "game.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CPause::CPause()
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CPause::~CPause()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPause::Init(void)
{
	CInput::GetKey()->GetMouse()->UseSetPosLock(false);
	SetPouseUpdate(true);

	m_onPause = false;

	//モード選択時の背景黒くする
	m_bg = CObject2d::Create(CTaskGroup::LEVEL_2D_UI);
	m_bg->SetPouseUpdate(true);
	m_bg->SetSize(CApplication::CENTER_POS);
	m_bg->SetPos(CApplication::CENTER_POS);
	m_bg->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.25f));

	float y = 100.0f;

	{// 戻るUIの作成
		D3DXVECTOR3 pos(CApplication::CENTER_POS);
		pos.y -= y;

		m_back = CSelect::Create(pos);
		m_back->SetPouseUpdate(true);
		m_back->SetSize(D3DXVECTOR3(200.0f, 35.0f, 0.0f));
		m_back->SetTexture("TEXT_BACK");
		m_back->SetFunctionSelection([](CSelect* inSelect)
		{
			inSelect->SetSize(D3DXVECTOR3(200.0f, 35.0f, 0.0f) * 1.5f);
		});
		m_back->SetFunctionNoSelection([](CSelect* inSelect)
		{
			inSelect->SetSize(D3DXVECTOR3(200.0f, 35.0f, 0.0f));

		});
		m_back->SetFunctionClick([this](CSelect* inSelect)
		{
			SetUpdateStatus(EUpdateStatus::END);
		});
	}

	y -= 100.0f;

	{// リトライUIの作成
		D3DXVECTOR3 pos(CApplication::CENTER_POS);
		pos.y -= y;

		m_retry = CSelect::Create(pos);
		m_retry->SetPouseUpdate(true);
		m_retry->SetSize(D3DXVECTOR3(200.0f, 35.0f, 0.0f));
		m_retry->SetTexture("TEXT_RETRY");
		m_retry->SetFunctionSelection([](CSelect* inSelect)
		{
			inSelect->SetSize(D3DXVECTOR3(200.0f, 35.0f, 0.0f) * 1.5f);
		});
		m_retry->SetFunctionNoSelection([](CSelect* inSelect)
		{
			inSelect->SetSize(D3DXVECTOR3(200.0f, 35.0f, 0.0f));

		});
		m_retry->SetFunctionClick([this](CSelect* inSelect)
		{
			CGame* mode = (CGame*)CApplication::GetInstance()->GetTaskGroup()->SearchRoleTop(ROLE_MODE, CTaskGroup::LEVEL_SYSTEM);
			mode->RetryStage();
			SetUpdateStatus(EUpdateStatus::END);
		});
	}

	y -= 100.0f;

	{// タイトルに戻るUIの作成
		D3DXVECTOR3 pos(CApplication::CENTER_POS);
		pos.y -= y;

		m_titleBack = CSelect::Create(pos);
		m_titleBack->SetPouseUpdate(true);
		m_titleBack->SetSize(D3DXVECTOR3(200.0f, 35.0f, 0.0f));
		m_titleBack->SetTexture("TEXT_TITLE");
		m_titleBack->SetFunctionSelection([](CSelect* inSelect)
		{
			inSelect->SetSize(D3DXVECTOR3(200.0f, 35.0f, 0.0f) * 1.5f);
		});
		m_titleBack->SetFunctionNoSelection([](CSelect* inSelect)
		{
			inSelect->SetSize(D3DXVECTOR3(200.0f, 35.0f, 0.0f));

		});
		m_titleBack->SetFunctionClick([this](CSelect* inSelect)
		{
			CApplication::GetInstance()->GetFade()->NextMode(CApplication::MODE_TITLE);
			SetUpdateStatus(EUpdateStatus::END);
		});
	}

	m_mouseCursor = CMouseObject::Create();
	m_mouseCursor->SetPouseUpdate(true);

	CApplication::GetInstance()->GetTaskGroup()->Pause(true);
	return S_OK;
}

//=============================================================================
// 破棄
//=============================================================================
void CPause::Uninit(void)
{
	CInput::GetKey()->GetMouse()->UseSetPosLock(true);
	CApplication::GetInstance()->GetTaskGroup()->Pause(false);
	m_bg->Release();
	m_back->Release();
	m_retry->Release();
	m_titleBack->Release();
	m_mouseCursor->Release();
}

//=============================================================================
// 更新
//=============================================================================
void CPause::NormalUpdate(void)
{
	CInput* input = CInput::GetKey();

	if (input->Trigger(DIK_P))
	{
		SetUpdateStatus(EUpdateStatus::END);
	}
}
