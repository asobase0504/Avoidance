//=============================================================================
//
// リザルト画面
// Author:hamada ryuuga
//
//=============================================================================
//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "tutorial.h"
#include "input.h"
#include "application.h"
#include "fade.h"
#include "sound.h"

#include "player.h"
#include "motion.h"
#include "bg.h"

#include "particle_manager.h"
#include "score.h"
#include "life.h"

#include "text.h"

#include "multiply.h"

//-----------------------------------------------------------------------------
// 静的メンバー変数の宣言
//-----------------------------------------------------------------------------
CMagicBox* CTutorial::m_pMagicBox;
CParticleManager* CTutorial::m_pPaticleManager;
CPlayer* CTutorial::m_pPlayer;

//=============================================================================
// コンストラクタ
//=============================================================================
CTutorial::CTutorial()
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CTutorial::~CTutorial()
{
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CTutorial::Init(void)
{
	m_isMoveClear = false;
	m_isAttackClear = false;
	m_isMagicClear = false;
	m_nextTaskCount = 0;
	D3DXVECTOR3 BGPos;
	BGPos.x = 0.0f;
	BGPos.y = 0.0f;
	BGPos.z -= 0.0f;
	m_magic = 2;

	m_pBg[0] = CBg::Create();
	m_pBg[0]->SetTexture("TUTORIAL");
	m_pBg[0]->SetSize(CApplication::CENTER_POS);

	m_pBg[0]->SetCollar(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	m_pPlayer = nullptr;
	m_pPlayer = CPlayer::Create();
	m_pPlayer->SetType(CObject::PLAYER);


	m_pPaticleManager = new CParticleManager;
	// パーティクル
	if (FAILED(m_pPaticleManager->Init()))
	{
		return E_FAIL;
	}

	m_pScore = CScore::Create(D3DXVECTOR3(500.0f, 30.0f, 0.0f));
	m_pScore->Set(0);

	CText::Create(CText::GON, 300,10, "まずはイドウをしてみよう！");

	return S_OK;

}

//=============================================================================
// 終了
//=============================================================================
void CTutorial::Uninit(void)
{
	CApplication::GetInstance()->GetSound()->Stop();
	
	if (m_pBg[0] != nullptr)
	{
		m_pBg[0]->Uninit();
	}
	



	if (m_pPaticleManager != nullptr)
	{
		m_pPaticleManager->Uninit();
		delete m_pPaticleManager;
		m_pPaticleManager = nullptr;

	}

	CApplication::GetInstance()->GetSound()->Stop();
	CApplication::GetInstance()->GetTaskGroup()->AllRelease();
}

//=============================================================================
// 更新
//=============================================================================
void CTutorial::Update(void)
{
	m_nextTaskCount++;
	m_pPaticleManager->Update();

	CInput *CInputpInput = CInput::GetKey();
	if (CInputpInput->Trigger(CInput::KEY_DELETE))
	{
		//モードの設定
		CApplication::GetInstance()->GetFade()->NextMode(CApplication::MODE_TITLE);
	}

	if (CInputpInput->Trigger(CInput::KEY_RELOAD))
	{
		if (m_pPaticleManager->GetEmitter().size() == 0)
		{
			/*	m_pMagicBox->CMagicBox::Magicplay((CTexture::TEXTURE)m_magic);
			m_magic++; 
			if (m_magic >= 6)
			{
				m_magic = 2;
			}*/
		}
	}

	if (m_nextTaskCount >= 300)
	{
		if (!m_isMoveClear
			&& (CInputpInput->Press(CInput::KEY_UP)
				|| CInputpInput->Press(CInput::KEY_DOWN)
				|| CInputpInput->Press(CInput::KEY_RIGHT)
				|| CInputpInput->Press(CInput::KEY_LEFT))
			)
		{
			CText::Create(CText::GON, 300,10, "ナイス！！うまいのじゃ！\nタマをうってみるのじゃ！");
			m_isMoveClear = true;
			m_nextTaskCount = 0;
		}
		else if (!m_isAttackClear && CInputpInput->Press(CInput::KEY_SHOT)&& m_isMoveClear)
		{
			CText::Create(CText::GON, 300,10, "ナイス！！うまいのじゃ！\nつぎはマホウをハツドウしてみのじゃ！");
			m_isAttackClear = true;
			m_nextTaskCount = 0;
		}
		else if (!m_isMagicClear && CInputpInput->Trigger(CInput::KEY_DECISION)&& m_isAttackClear)
		{
			CText::Create(CText::GON, 500,10, "ナイス！！うまいのじゃ！\nさあこれでチュートリアルはおわりじゃ！\nぶっとばしにいくのじゃ！");
			m_isMagicClear = true;
			m_nextTaskCount = 0;
		}
	}
}

//=============================================================================
// 描画
//=============================================================================
void CTutorial::Draw(void)
{

}
