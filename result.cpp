//=============================================================================
//
// リザルト画面
// Author:Hamada Ryuuga
//
//=============================================================================
//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "result.h"
#include "input.h"
#include "application.h"
#include "fade.h"
#include "sound.h"
#include "player.h"
#include "motion.h"
#include "bg.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CResult::CResult()
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CResult::~CResult()
{
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CResult::Init(void)
{
	CObject2d* test = CObject2d::Create();
	test->Init();
	test->SetColor(D3DXCOLOR(0.5f, 0.0f, 0.5f, 1.0f));
	test->SetPos(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));
	test->SetSize(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));
	return S_OK;
}

//=============================================================================
// 終了
//=============================================================================
void CResult::Uninit(void)
{
	CApplication::GetInstance()->GetSound()->Stop();
}

//=============================================================================
// 更新
//=============================================================================
void CResult::Update(void)
{
	if (CInput::GetKey()->Trigger(CInput::KEY_DOWN))
	{
		CApplication::GetInstance()->GetFade()->NextMode(CApplication::MODE_TITLE);
	}
}

//=============================================================================
// 描画
//=============================================================================
void CResult::Draw(void)
{

}