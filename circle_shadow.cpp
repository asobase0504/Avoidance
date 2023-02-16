#//=============================================================================
//
// �ۉe
// Author:Yuda Kaito
//
//=============================================================================
//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "circle_shadow.h"
#include "task_group.h"
#include "objectX.h"

//-----------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------
CCircleShadow::CCircleShadow()
{
}

//-----------------------------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------------------------
CCircleShadow::~CCircleShadow()
{
}

//-----------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------
HRESULT CCircleShadow::Init()
{
	CObjectPolygon3D::Init();

	SetTexture("CIRCLE_SHADOW");
	SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	SetSize(D3DXVECTOR3(15.0f, 15.0f, 0.0f));
	SetRot(D3DXVECTOR3(D3DX_PI * 0.5f, 0.0f, 0.0f));
	return S_OK;
}

//-----------------------------------------------------------------------------
// �I��
//-----------------------------------------------------------------------------
void CCircleShadow::Uninit()
{
	CObjectPolygon3D::Uninit();
}

//-----------------------------------------------------------------------------
// �ʏ�X�V
//-----------------------------------------------------------------------------
void CCircleShadow::NormalUpdate()
{
	CObjectPolygon3D::NormalUpdate();

	// Y�����Œ�ɂ���
	SetPos(D3DXVECTOR3(0.0f, -FLT_MAX, 0.0f));

	// �S�Ă̒n�ʂƓ����蔻����s��
	CObject::TypeAllFunc(EType::PLAIN, CTaskGroup::EPriority::LEVEL_3D_1, [this](CObject* inObject)
	{
		CObjectX* plain = (CObjectX*)inObject;

		D3DXVECTOR3 outPos;

		if (plain->RayAndAABB(m_parent->GetPos(), D3DXVECTOR3(0.0f, -1.0f, 0.0f), &outPos))
		{
			outPos.y += 5.0f;

			bool isParentThanOn = m_parent->GetPos().y > outPos.y;	// �e����
			bool isThisThanDown = GetPos().y < outPos.y;			// �\�肳��Ă�ʒu����

			if (isParentThanOn && isThisThanDown)
			{
				SetPos(outPos);
			}
		}
	});
}

//-----------------------------------------------------------------------------
// �쐬
//-----------------------------------------------------------------------------
CCircleShadow * CCircleShadow::Create(CObject* inObject)
{
	CCircleShadow* shadow = new CCircleShadow;

	if (shadow != nullptr)
	{
		shadow->Init();
		shadow->m_parent = inObject;
	}

	return shadow;
}
