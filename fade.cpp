//=============================================================================
//
// フェード設定
// Author : Hamada Ryuuga
//
//=============================================================================
//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <assert.h>
#include "fade.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CFade::CFade() : CObject2d(CTaskGroup::LEVEL_FADE)
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CFade::~CFade()
{
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CFade::Init(void)
{
	AttachProtect();
	// 現在のモーション番号の保管
	CObject2d::Init();
	m_fadeSp = 0.07f;
	m_fadeSet = 0.0f;
	return S_OK;
}

//=============================================================================
// 終了
//=============================================================================
void CFade::Uninit(void)
{
	// 現在のモーション番号の保管
	 CObject2d::Uninit();
}

//=============================================================================
// 更新
//=============================================================================
void CFade::NormalUpdate(void)
{
	CObject2d::NormalUpdate();
	if (m_fade != FADENON)
	{
		// 現在のモーション番号の保管
		if (m_fade == FADEOUT)
		{
			AddColorAlpha(-m_fadeSp);
		}

		if (m_fade == FADEIN)
		{
			AddColorAlpha(m_fadeSp);
		}

		if (GetColorAlpha() >= 1.0f)
		{
			m_fade = FADEOUT;
			SetColorAlpha(1.0f);
			CApplication::GetInstance()->SetMode(m_nextMode);

		}
		if (GetColorAlpha() <= 0.0f)
		{
			m_fade = FADENON;
			SetColorAlpha(0.0f);
		}
	}
}

//=============================================================================
// 描画
//=============================================================================
void CFade::Draw(void)
{
	 CObject2d::Draw();
}

//=============================================================================
// create
//=============================================================================
CFade* CFade::Create()
{
	CFade * pObject = new CFade;

	if (pObject != nullptr)
	{
		pObject->Init();
		pObject->m_nextMode = CApplication::MODE_TITLE;
		pObject->SetPos(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));
		pObject->SetSize(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));
		pObject->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
		pObject->SetType(CObject::EType::MODE);
		pObject->m_fade = FADENON;
	}
	return pObject;
}


//=============================================================================
// 次のモードに移行
//=============================================================================
void CFade::NextMode(CApplication::MODE nextMode)
{
	if (m_fade != FADENON)
	{
		return;
	}

	/* ↓フェードを移行する場合↓ */

	Init();
	m_nextMode = nextMode;
	SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
	m_fade = FADEIN;
}
