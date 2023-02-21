//=============================================================================
//
// プレイヤー設定
// Author:Yuda Kaito
//
//=============================================================================
//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include <assert.h>
#include "enemy.h"
#include "application.h"
#include "color.h"
#include "sound.h"

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
CEnemy::CEnemy()
{
}

//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
CEnemy::~CEnemy()
{
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
HRESULT CEnemy::Init()
{
	LoadModel("BOX");

	// 現在のモーション番号の保管
	CObjectX::Init();
	SetType(CObject::ENEMY);
	SetMaterialDiffuse(0, m_color);

	AttachOutLine();
	AttachShadow();

	SetColor(1.0f, 1.0f, 1.0f, 0.0f);
	SetColorAlpha(0.0f);

	SetMaterialDiffuse(0, CApplication::GetInstance()->GetColor()->GetColor(CColor::COLOR_3));
	SetCollisionFlag(false);
	return S_OK;
}

//-----------------------------------------------------------------------------
// 終了
//-----------------------------------------------------------------------------
void CEnemy::Uninit()
{
	CObjectX::Uninit();
}

//-----------------------------------------------------------------------------
// 出現更新
//-----------------------------------------------------------------------------
void CEnemy::PopUpdate()
{
	SetColorAlpha(GetColorAlpha() + 0.01f);
	SetColor(D3DXCOLOR(GetColor().r, GetColor().g - 0.01f, GetColor().b - 0.01f, GetColor().a));
	SetMaterialDiffuse(0,GetColor());

	if (GetColorAlpha() >= 1.0f)
	{
		SetColorAlpha(1.0f);
		SetMaterialDiffuse(0, CApplication::GetInstance()->GetColor()->GetColor(CColor::COLOR_3));
		SetUpdateStatus(CObject::EUpdateStatus::NORMAL);
		SetCollisionFlag(true);
	}
}

//-----------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------
void CEnemy::NormalUpdate()
{
}

//-----------------------------------------------------------------------------
// 終了更新
//-----------------------------------------------------------------------------
void CEnemy::EndUpdate()
{
	CApplication::GetInstance()->GetSound()->Play(CSound::LABEL_SE_ENEMY_HIT);
	Release();
}

//-----------------------------------------------------------------------------
// 生成
//-----------------------------------------------------------------------------
CEnemy* CEnemy::Create()
{
	CEnemy* enemy = new CEnemy;

	if (enemy != nullptr)
	{
		enemy->Init();
	}

	return enemy;
}

//-----------------------------------------------------------------------------
// 床との当たり判定
//-----------------------------------------------------------------------------
bool CEnemy::OnHitPlain()
{
	// 最初に見つけた指定したタイプのobjectを持ってくる
	CObject* object = SearchType(CObject::EType::PLAIN, CTaskGroup::EPriority::LEVEL_3D_1);

	while (object != nullptr)
	{
		CObject* next = object->NextSameType();	// 同じタイプのobjectを持ってくる

		if (OBBAndOBB((CObjectX*)object))
		{
			return true;
		}

		object = next;
	}
	return false;
}
