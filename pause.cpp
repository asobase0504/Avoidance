//=============================================================================
//
// 制作 ( タイトル )
// Author : hamada ryuuga
//
//=============================================================================
//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "pause.h"
#include "input.h"
#include "application.h"
#include "Bg.h"
#include "player.h"
#include "fade.h"
#include "sound.h"
#include "ranking.h"
#include "application.h"

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
	m_nextMode = MODE_GAME;
	m_onPause = false;

	//モード選択時の背景黒くするやつ
	m_pBg = CObject2d::Create(CTaskGroup::LEVEL_3D_UI);
	m_pBg->SetTexture("PAUSEBG");
	m_pBg->SetSize(D3DXVECTOR3(450.0f, CApplication::CENTER_POS.y, 0.0f));
	m_pBg->SetPos(CApplication::CENTER_POS);
	m_pBg->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));

	//ゲームの文字
	m_pObject2d[0] = CObject2d::Create(CTaskGroup::LEVEL_3D_UI);
	m_pObject2d[0]->SetTexture("PAUSEGAME");
	m_pObject2d[0]->SetSize(D3DXVECTOR3(150.0f, 50.0f, 0.0f));
	m_pObject2d[0]->SetPos(CApplication::CENTER_POS);
	m_pObject2d[0]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));

	float y = 100.0f;

	//チュートリアルの文字
	m_pObject2d[1] = CObject2d::Create(CTaskGroup::LEVEL_3D_UI);
	m_pObject2d[1]->SetTexture("PAUSERETURN");
	m_pObject2d[1]->SetSize(D3DXVECTOR3(150.0f, 50.0f, 0.0f));
	m_pObject2d[1]->SetPos(D3DXVECTOR3(CApplication::CENTER_POS.x, CApplication::CENTER_POS.y + y, 0.0f));
	m_pObject2d[1]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));

	y += 100.0f;

	//ランキングの文字
	m_pObject2d[2] = CObject2d::Create(CTaskGroup::LEVEL_3D_UI);
	m_pObject2d[2]->SetTexture("PAUSETITLE");
	m_pObject2d[2]->SetSize(D3DXVECTOR3(150.0f, 50.0f, 0.0f));
	m_pObject2d[2]->SetPos(D3DXVECTOR3(CApplication::CENTER_POS.x, CApplication::CENTER_POS.y + y, 0.0f));
	m_pObject2d[2]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));

	//ランキングの文字
	m_pObject2d[3] = CObject2d::Create(CTaskGroup::LEVEL_3D_UI);
	m_pObject2d[3]->SetTexture("PAUSEMENU");
	m_pObject2d[3]->SetSize(D3DXVECTOR3(300.0f, 100.0f, 0.0f));
	m_pObject2d[3]->SetPos(D3DXVECTOR3(CApplication::CENTER_POS.x, CApplication::CENTER_POS.y - y, 0.0f));
	m_pObject2d[3]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));

	return S_OK;
}

//=============================================================================
// 破棄
//=============================================================================
void CPause::Uninit(void)
{
	
}

//=============================================================================
// 更新
//=============================================================================
void CPause::NormalUpdate(void)
{
	CInput *CInputpInput = CInput::GetKey();

	if (CInputpInput->Trigger(CInput::KEY_PAUSE) && *CApplication::GetInstance()->GetFade()->GetFade() == CFade::FADENON)
	{
		if (m_onPause)
		{
			Set();
			m_pBg->SetColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 0.0f));
			for (int i = 0; i < 4; i++)
			{
				m_pObject2d[i]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			}
		}
		else
		{
			m_nextMode = 0;
			Set();
			//背景
			m_pBg->SetColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 0.8f));
			for (int i = 0; i < 4; i++)
			{//全部一回変える
				m_pObject2d[i]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f));
			}

			//選択してるやつ
			m_pObject2d[m_nextMode]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}

	if (CInputpInput->Trigger(CInput::KEY_DECISION))
	{
		if (m_onPause)
		{
			switch (m_nextMode)
			{
			case EMode::MODE_GAME:
				//モードの設定
				CApplication::GetInstance()->GetFade()->NextMode(CApplication::MODE_GAME);
				break;
			case EMode::MODE_RETURN:
				//モードの設定
				m_pBg->SetColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 0.0f));
				for (int i = 0; i < 4; i++)
				{
					m_pObject2d[i]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
				}
				break;
			case EMode::MODE_TITLE:
				//モードの設定
				CApplication::GetInstance()->GetFade()->NextMode(CApplication::MODE_TITLE);
				break;
			default:
				break;
			}
			m_onPause = false;
		}

		if (m_onPause)
		{
			m_pBg->SetColor(D3DXCOLOR(0.3f, 0.3f, 0.30f, 0.5f));
			for (int i = 0; i < 3; i++)
			{
				m_pObject2d[i]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f));
			}

			m_pObject2d[m_nextMode]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		else
		{
			m_pBg->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			for (int i = 0; i < 3; i++)
			{
				m_pObject2d[i]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			}
		}
	}

	if (m_onPause)
	{
		if (CInputpInput->Trigger(CInput::KEY_UP))
		{
			//モード選択
			m_pObject2d[m_nextMode]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f));

			m_nextMode = (EMode)(m_nextMode - 1);


			if (m_nextMode < EMode::MODE_GAME)
			{
				m_nextMode = EMode::MODE_TITLE;
			}

			m_pObject2d[m_nextMode]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		if (CInputpInput->Trigger(CInput::KEY_DOWN))
		{
			//モード選択
			m_pObject2d[m_nextMode]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f));

			m_nextMode = (EMode)(m_nextMode + 1);

			if (m_nextMode >= EMode::MODE_END)
			{
				m_nextMode = EMode::MODE_GAME;
			}

			m_pObject2d[m_nextMode]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CPause::Draw(void)
{

}