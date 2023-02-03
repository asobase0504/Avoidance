//=============================================================================
//
// プレイヤーの残像設定
// Author:Yuda Kaito
//
//=============================================================================
#include <assert.h>
#include "player_afterimage_fall.h"
#include "utility.h"

const int CPlayerAfterimageFall::MAX_LIFE = 10;
const float CPlayerAfterimageFall::ALPHA_COLOR = 0.45f;

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
CPlayerAfterimageFall::CPlayerAfterimageFall()
{
}

//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
CPlayerAfterimageFall::~CPlayerAfterimageFall()
{
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
HRESULT CPlayerAfterimageFall::Init()
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
void CPlayerAfterimageFall::Uninit()
{
	CObjectX::Uninit();
}

//-----------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------
void CPlayerAfterimageFall::NormalUpdate()
{
	m_life--;

	float scale = ease::OutSine((float)m_life / (float)MAX_LIFE) * 0.75f;

	SetScale(D3DXVECTOR3(scale, scale, scale));

	if (m_life <= 0)
	{
		SetUpdateStatus(CObject::EUpdateStatus::END);
	}
}

//-----------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------
void CPlayerAfterimageFall::EndUpdate()
{
	Release();
}

//-----------------------------------------------------------------------------
// 描画
//-----------------------------------------------------------------------------
void CPlayerAfterimageFall::Draw()
{
	CObjectX::Draw();
}

//-----------------------------------------------------------------------------
// 生成
//-----------------------------------------------------------------------------
CPlayerAfterimageFall* CPlayerAfterimageFall::Create(const D3DXVECTOR3& inPos)
{
	CPlayerAfterimageFall* objectX = new CPlayerAfterimageFall;

	if (objectX != nullptr)
	{
		objectX->Init();
		objectX->SetPos(inPos);
		objectX->SetMove(D3DXVECTOR3(0.0f,FloatRandam(25.0f,35.5f),0.0f));
	}

	return objectX;
}
