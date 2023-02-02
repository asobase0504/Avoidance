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

	m_nextMode = MODE_NONE;
	m_onPause = false;

	//モード選択時の背景黒くする
	m_bg = CObject2d::Create(CTaskGroup::LEVEL_2D_UI);
	m_bg->SetPouseUpdate(true);
	m_bg->SetSize(CApplication::CENTER_POS);
	m_bg->SetPos(CApplication::CENTER_POS);
	m_bg->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.25f));

	//ゲームの文字
	m_selectText[0] = CObject2d::Create(CTaskGroup::LEVEL_2D_UI);
	m_selectText[0]->SetPouseUpdate(true);
	m_selectText[0]->SetTexture("PAUSEGAME");
	m_selectText[0]->SetSize(D3DXVECTOR3(150.0f, 50.0f, 0.0f));
	m_selectText[0]->SetPos(CApplication::CENTER_POS);
	m_selectText[0]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));

	float y = 100.0f;

	//チュートリアルの文字
	m_selectText[1] = CObject2d::Create(CTaskGroup::LEVEL_2D_UI);
	m_selectText[1]->SetTexture("PAUSERETURN");
	m_selectText[1]->SetSize(D3DXVECTOR3(150.0f, 50.0f, 0.0f));
	m_selectText[1]->SetPos(D3DXVECTOR3(CApplication::CENTER_POS.x, CApplication::CENTER_POS.y + y, 0.0f));
	m_selectText[1]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));

	y += 100.0f;

	//ランキングの文字
	m_selectText[2] = CObject2d::Create(CTaskGroup::LEVEL_2D_UI);
	m_selectText[2]->SetTexture("PAUSETITLE");
	m_selectText[2]->SetSize(D3DXVECTOR3(150.0f, 50.0f, 0.0f));
	m_selectText[2]->SetPos(D3DXVECTOR3(CApplication::CENTER_POS.x, CApplication::CENTER_POS.y + y, 0.0f));
	m_selectText[2]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));

	//ランキングの文字
	m_selectText[3] = CObject2d::Create(CTaskGroup::LEVEL_2D_UI);
	m_selectText[3]->SetTexture("PAUSEMENU");
	m_selectText[3]->SetSize(D3DXVECTOR3(300.0f, 100.0f, 0.0f));
	m_selectText[3]->SetPos(D3DXVECTOR3(CApplication::CENTER_POS.x, CApplication::CENTER_POS.y - y, 0.0f));
	m_selectText[3]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));

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
	m_selectText[0]->Release();
	m_selectText[1]->Release();
	m_selectText[2]->Release();
	m_selectText[3]->Release();
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

	// 遷移
	if (input->Trigger(CMouse::MOUSE_KEY::MOUSE_KEY_LEFT))
	{
		if (input->GetMouse()->GetMouseCursorPos().x > 500.0f)
		{
			CApplication::GetInstance()->GetFade()->NextMode(CApplication::MODE_TITLE);
			SetUpdateStatus(EUpdateStatus::END);
		}
		else
		{
			SetUpdateStatus(EUpdateStatus::END);
		}
	}

	//if (CInputpInput->Trigger(KEY_PAUSE) && *CApplication::GetInstance()->GetFade()->GetFade() == CFade::FADENON)
	//{
	//	if (m_onPause)
	//	{
	//		Set();
	//		m_pBg->SetColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 0.0f));
	//		for (int i = 0; i < 4; i++)
	//		{
	//			m_pObject2d[i]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
	//		}
	//	}
	//	else
	//	{
	//		m_nextMode = 0;
	//		Set();
	//		//背景
	//		m_pBg->SetColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 0.8f));
	//		for (int i = 0; i < 4; i++)
	//		{//全部一回変える
	//			m_pObject2d[i]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f));
	//		}

	//		//選択してるやつ
	//		m_pObject2d[m_nextMode]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	//	}
	//}

	//if (CInputpInput->Trigger(KEY_DECISION))
	//{
	//	if (m_onPause)
	//	{
	//		switch (m_nextMode)
	//		{
	//		case EMode::MODE_GAME:
	//			//モードの設定
	//			CApplication::GetInstance()->GetFade()->NextMode(CApplication::MODE_GAME);
	//			break;
	//		case EMode::MODE_RETURN:
	//			//モードの設定
	//			m_pBg->SetColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 0.0f));
	//			for (int i = 0; i < 4; i++)
	//			{
	//				m_pObject2d[i]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
	//			}
	//			break;
	//		case EMode::MODE_TITLE:
	//			//モードの設定
	//			CApplication::GetInstance()->GetFade()->NextMode(CApplication::MODE_TITLE);
	//			break;
	//		default:
	//			break;
	//		}
	//		m_onPause = false;
	//	}

	//	if (m_onPause)
	//	{
	//		m_pBg->SetColor(D3DXCOLOR(0.3f, 0.3f, 0.30f, 0.5f));
	//		for (int i = 0; i < 3; i++)
	//		{
	//			m_pObject2d[i]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f));
	//		}

	//		m_pObject2d[m_nextMode]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	//	}
	//	else
	//	{
	//		m_pBg->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
	//		for (int i = 0; i < 3; i++)
	//		{
	//			m_pObject2d[i]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
	//		}
	//	}
	//}

	//if (m_onPause)
	//{
	//	if (CInputpInput->Trigger(KEY_UP))
	//	{
	//		//モード選択
	//		m_pObject2d[m_nextMode]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f));

	//		m_nextMode = (EMode)(m_nextMode - 1);


	//		if (m_nextMode < EMode::MODE_GAME)
	//		{
	//			m_nextMode = EMode::MODE_TITLE;
	//		}

	//		m_pObject2d[m_nextMode]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	//	}
	//	if (CInputpInput->Trigger(KEY_DOWN))
	//	{
	//		//モード選択
	//		m_pObject2d[m_nextMode]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f));

	//		m_nextMode = (EMode)(m_nextMode + 1);

	//		if (m_nextMode >= EMode::MODE_END)
	//		{
	//			m_nextMode = EMode::MODE_GAME;
	//		}

	//		m_pObject2d[m_nextMode]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	//	}
	//}
}
