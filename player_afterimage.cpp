//=============================================================================
//
// プレイヤーの残像設定
// Author:Yuda Kaito
//
//=============================================================================
#include <assert.h>
#include "player_afterimage.h"
#include "utility.h"

const int CPlayerAfterimage::MAX_LIFE = 120;
const float CPlayerAfterimage::ALPHA_COLOR = 0.45f;

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
CPlayerAfterimage::CPlayerAfterimage()
{
}

//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
CPlayerAfterimage::~CPlayerAfterimage()
{
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
HRESULT CPlayerAfterimage::Init()
{
	// 現在のモーション番号の保管
	CObjectX::Init();
	LoadModel("BOX");

	m_life = MAX_LIFE;
	SetColorAlpha(ALPHA_COLOR);

	return S_OK;
}

//-----------------------------------------------------------------------------
// 終了
//-----------------------------------------------------------------------------
void CPlayerAfterimage::Uninit()
{
	CObjectX::Uninit();
}

//-----------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------
void CPlayerAfterimage::NormalUpdate()
{
	m_life--;

	float scale = ease::InSine((float)m_life / (float)MAX_LIFE);

	SetScale(D3DXVECTOR3(scale, scale, scale));
	if (m_life <= 0)
	{
		SetUpdateStatus(CObject::EUpdateStatus::END);
	}
}

//-----------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------
void CPlayerAfterimage::EndUpdate()
{
	Release();
}

//-----------------------------------------------------------------------------
// 描画
//-----------------------------------------------------------------------------
void CPlayerAfterimage::Draw()
{
	CObjectX::Draw();
}

//-----------------------------------------------------------------------------
// 生成
//-----------------------------------------------------------------------------
CPlayerAfterimage* CPlayerAfterimage::Create(const D3DXVECTOR3& inPos)
{
	CPlayerAfterimage* objectX = new CPlayerAfterimage;

	if (objectX != nullptr)
	{
		objectX->Init();
		objectX->SetPos(inPos);
	}

	return objectX;
}
