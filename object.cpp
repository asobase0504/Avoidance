//=============================================================================
//
// オブジェクト
// Author : Hamada Ryuuga
// Author : Yuda Kaito
//
//=============================================================================
//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "object.h"
#include "application.h"
#include <assert.h>

//=============================================================================
// コンストラクタ
//=============================================================================
CObject::CObject(CTaskGroup::EPriority inPriority, CTaskGroup::EPushMethod inMethod) :
	CTask(inPriority, inMethod),
	m_updateStatus(EUpdateStatus::POP),
	m_pos(D3DXVECTOR3(0.0f,0.0f,0.0f)),			// 位置
	m_move(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),		// 移動量
	m_rot(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),		// 向き
	m_moveRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),	// 回転量
	m_size(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),		// 大きさ
	m_color(D3DXCOLOR(0.0f,0.0f,0.0f,1.0f))		// 色
{
	SetRole(ROLE_OBJECT);
}

//=============================================================================
// デストラクタ
//=============================================================================
CObject::~CObject()
{
}

//=============================================================================
// 更新
//=============================================================================
void CObject::Update()
{
	m_posOld = m_pos;

	// 移動
	AddPos(GetMove());
	AddRot(GetMoveRot());

	switch (m_updateStatus)
	{
	case EUpdateStatus::POP:
		PopUpdate();
		break;
	case EUpdateStatus::NORMAL:
		NormalUpdate();
		break;
	case EUpdateStatus::END:
		EndUpdate();
		break;
	default:
		assert(false);
		break;
	}
}

//=============================================================================
// 指定された種別を探す
//=============================================================================
CObject* CObject::SearchType(EType inType, int inPriority)
{
	CTaskGroup* taskGroup = CApplication::GetInstance()->GetTaskGroup();

	// 指定したpriority中にある先頭CObjectを取得
	CObject* now = (CObject*)taskGroup->SearchRoleTop(CTask::ERole::ROLE_OBJECT, inPriority);

	while (now != nullptr)
	{
		CObject* next = (CObject*)taskGroup->SearchSameRoleNext(now);	// nowから次のCObjectを探す。
		if (now->GetType() == inType)
		{// 予定したタイプかチェック
			return now;	// 合っていたら返す。
		}
		now = next;
	}
	return nullptr;
}

//=============================================================================
// 自身と同じ種別のobjectを探す
//=============================================================================
CObject * CObject::NextSameType()
{
	CTaskGroup* taskGroup = CApplication::GetInstance()->GetTaskGroup();
	CObject* now = (CObject*)taskGroup->SearchSameRoleNext(this);
	EType type = this->GetType();

	while (now != nullptr)
	{
		CObject* next = (CObject*)taskGroup->SearchSameRoleNext(now);
		if (now->GetType() == type)
		{
			return now;
		}
		now = next;
	}
	return nullptr;
}

void CObject::MulSize(const D3DXVECTOR3 & inRatio)
{
	D3DXVECTOR3 size;
	size.x = m_size.x * inRatio.x;
	size.y = m_size.y * inRatio.y;
	size.z = m_size.z * inRatio.z;
	SetSize(size);
}

//=============================================================================
// 出現中の更新
//=============================================================================
void CObject::PopUpdate()
{
	m_updateStatus = EUpdateStatus::NORMAL;
}

//=============================================================================
// 通常中の更新
//=============================================================================
void CObject::NormalUpdate()
{
}

//=============================================================================
// 終了中の更新
//=============================================================================
void CObject::EndUpdate()
{
	Release();
}
