//=============================================================================
//
// ���f����ׂ��e
// Author:Yuda Kaito
//
//=============================================================================
//-----------------------------------------------------------------------------
// include
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
#include "model_shadow.h"
#include "application.h"

//-----------------------------------------------------------------------------
// �R���X�g���N�^
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
CModelShadow::CModelShadow()
{
}

//-----------------------------------------------------------------------------
// �f�X�g���N�^
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
CModelShadow::~CModelShadow()
{
}

//-----------------------------------------------------------------------------
// ������
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
HRESULT CModelShadow::Init()
{
	for (int nCntMat = 0; nCntMat < (int)m_NumMat; nCntMat++)
	{
		SetMaterialDiffuse(nCntMat, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	}

	return S_OK;
}

//-----------------------------------------------------------------------------
// �I��
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
void CModelShadow::Uninit()
{
}

//-----------------------------------------------------------------------------
// �`��
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
void CModelShadow::Draw()
{
	// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxTrans;

	// ���[���h�}�g���b�N�X�̏�����
	// �s�񏉊����֐�(��1�����̍s���P�ʍs��ɏ�����)
	D3DXMatrixIdentity(&m_mtxWorld);

	// �傫���𔽉f
	D3DXMatrixScaling(&mtxTrans, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// �ϐ��錾
	D3DXMATRIX mtxShadow;
	D3DXPLANE planeField;
	D3DXVECTOR4 vecLight;
	D3DXVECTOR3 pos, normal;

	// �V���h�E�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxShadow);

	vecLight = -D3DXVECTOR4(0.2f, -0.5f, 0.3f, 0.0f);

	normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXPlaneFromPointNormal(&planeField, &m_pos, &normal);
	D3DXMatrixShadow(&mtxShadow, &vecLight, &planeField);

	// ���[���h�}�g���b�N�X�Ɗ|�����킹��
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxShadow);
}

//-----------------------------------------------------------------------------
// �ʏ�X�V
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
void CModelShadow::NormalUpdate()
{
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
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
CModelShadow * CModelShadow::Create(CObjectX * inObject)
{
	CModelShadow* shadow = new CModelShadow;

	if (shadow != nullptr)
	{
		shadow->Init();
		shadow->m_parent = inObject;
		shadow->SetMtxRot(shadow->m_parent->GetMtxRot());
		shadow->SetScale(shadow->m_parent->GetScale());
	}

	return shadow;
}
