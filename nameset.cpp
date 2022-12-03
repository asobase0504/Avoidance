//=============================================================================
//
// 制作 ( 名前 )
// Author : Hamada Ryuuga
//
//=============================================================================
//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "nameset.h"
#include "input.h"
#include "application.h"
#include "object2d.h"
#include "ranking.h"
#include "fade.h"

//-----------------------------------------------------------------------------
// 静的メンバー変数の宣言
//-----------------------------------------------------------------------------
std::string  CNameSet::m_PlayName;

//=============================================================================
// コンストラクタ
//=============================================================================
CNameSet::CNameSet()
{
	if (m_object2d[0] != nullptr)
	{
		m_object2d[0] = nullptr;
	}

	for (int i = 0; i < 3; i++)
	{
		if (m_ListName[i] != nullptr)
		{
			m_ListName[i] = nullptr;
		}
	}

	for (int i = 0; i < 10; i++)
	{
		if (m_PlayNameSet[i] != nullptr)
		{
			m_PlayNameSet[i] = nullptr;
		}
	}
}

//=============================================================================
// デストラクタ
//=============================================================================
CNameSet::~CNameSet()
{
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CNameSet::Init(void)
{
	m_PlayName = "";
	m_NowPlay = 0;
	m_NemePos = D3DXVECTOR3(CApplication::CENTER_POS.x-300.0f, 100.0f, 0.0f);

	m_object2d[0] = CObject2d::Create();
	m_object2d[0]->SetTexture("TITLEBG");
	m_object2d[0]->SetSize(CApplication::CENTER_POS);
	m_object2d[0]->SetPos(CApplication::CENTER_POS);

	m_object2d[1] = CObject2d::Create();
	m_object2d[1]->SetTexture("NAMETITLE");
	m_object2d[1]->SetSize(D3DXVECTOR3(500.0f, 175.0f, 0.0f));
	m_object2d[1]->SetPos(D3DXVECTOR3(CApplication::CENTER_POS.x, 575.0f, 0.0f));

	m_ListName[0] = CName::Create();
	m_ListName[0]->SetSize(D3DXVECTOR3(50.0f, 50.0f, 0.0f));
	m_ListName[0]->SetPos(D3DXVECTOR3(CApplication::CENTER_POS.x -125.0f, 375.0f, 0.0f));
	m_ListName[0]->SetAlphabet(CName::MAX);

	m_ListName[1] = CName::Create();
	m_ListName[1]->SetSize(D3DXVECTOR3(75.0f, 75.0f, 0.0f));
	m_ListName[1]->SetPos(D3DXVECTOR3(CApplication::CENTER_POS.x, 350.0f, 0.0f));

	m_ListName[2] = CName::Create();
	m_ListName[2]->SetSize(D3DXVECTOR3(50.0f, 50.0f, 0.0f));
	m_ListName[2]->SetPos(D3DXVECTOR3(CApplication::CENTER_POS.x + 125.0f, 375.0f, 0.0f));
	m_ListName[2]->SetAlphabet(CName::B);

	return S_OK;
}

//=============================================================================
// 破棄
//=============================================================================
void CNameSet::Uninit(void)
{
}

//=============================================================================
// 更新
//=============================================================================
void CNameSet::Update(void)
{
	CInput *CInputpInput = CInput::GetKey();
	if (CInputpInput->Trigger(DIK_0))
	{
		if (m_NowPlay > 0)
		{
			if (m_PlayNameSet[m_NowPlay - 1] != nullptr)
			{
				m_NowPlay--;
				m_PlayNameSet[m_NowPlay]->Release();
				m_PlayName = m_PlayName.substr(0, m_PlayName.length() - 1);
				m_NemePos.x -= 120.0f;
			}
		}
	}

	if (CInputpInput->Trigger(KEY_DECISION))
	{
		if (m_NowPlay >= 7)
		{
			////モードの設定
			//CRanking::SetPlayNeme(m_PlayName);

			//CApplication::GetInstance()->GetFade()->NextMode(CApplication::MODE_RANKING);
			return;
		}

		CNameSet::RankingNeme();
	}	
}
//=============================================================================
// 描画
//=============================================================================
void CNameSet::Draw(void)
{
}

//=============================================================================
// ランキングに名前をねじ込む
//=============================================================================
void CNameSet::RankingNeme()
{
	CName::EAlphabet Type = m_ListName[1]->GetAlphabet();

	if (Type >= CName::A && Type <= CName::Z)
	{
		m_PlayName += ('A' + (char)Type);
	}
	else if (Type == CName::MAX)
	{
		//モードの設定
		if (m_PlayName != "")
		{
			//CRanking::SetPlayNeme(m_PlayName);

			//CApplication::GetInstance()->SetMode(CApplication::MODE_RANKING);
			return;
		}
		else
		{
			return;
		}
	}
	else
	{
		return;
	}

	m_PlayNameSet[m_NowPlay] = CName::Create();
	m_PlayNameSet[m_NowPlay]->SetSize(D3DXVECTOR3(60.0f, 60.0f, 0.0f));
	m_PlayNameSet[m_NowPlay]->SetPos(m_NemePos);
	m_PlayNameSet[m_NowPlay]->SetAlphabet(Type);
	m_PlayNameSet[m_NowPlay]->SetMoveSwitch(false);

	m_NemePos.x += 120.0f;
	m_NowPlay++;
}
