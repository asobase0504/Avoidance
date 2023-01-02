//=============================================================================
//
// �I�u�W�F�N�gX���� [objectX.cpp]
// Author : KADO TAKUMA
//
//=============================================================================

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "objectX.h"
#include "objectX_group.h"
#include "object_polygon3d.h"
#include "application.h"
#include "renderer.h"
#include "main.h"
#include "texture.h"
#include "utility.h"
#include <assert.h>

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CObjectX::CObjectX(CTaskGroup::EPriority nPriority) :
	CObject(nPriority),
	m_pParent(nullptr),
	m_scale(1.0f,1.0f,1.0f)
{
	//�I�u�W�F�N�g�̃^�C�v�Z�b�g����
	CObject::SetType(CObject::MODEL);
	D3DXMatrixIdentity(&m_mtxRot);
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CObjectX::~CObjectX()
{
}

//=============================================================================
// �I�u�W�F�N�g�̏�����
// Author : Hamada Ryuuga
// �T�v : ���������s��
//=============================================================================
HRESULT CObjectX::Init()
{
	return S_OK;
}

//=============================================================================
// �I�u�W�F�N�g�̏I��
// Author : Hamada Ryuuga
// �T�v : �I�����s��
//=============================================================================
void CObjectX::Uninit()
{
	//�C���X�^���X�̉������
	Release();
}

//=============================================================================
// �I�u�W�F�N�g�̍X�V
// Author : Yuda Kaito
// �T�v : �X�V���s��
//=============================================================================
void CObjectX::NormalUpdate()
{
}

//=============================================================================
// �`��
// Author : Hamada Ryuga
// �T�v : �`����s��
//=============================================================================
void CObjectX::Draw()
{
	GiftMtx(&m_mtxWorld, m_pos, m_rot);	// �}�g���b�N�X�̐ݒ�

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

	if (m_pParent != nullptr)
	{
		D3DXMATRIX mtxParent = m_pParent->GetMtxWorld();

		// �s��|���Z�֐�
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);
	}

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	// ���[���h�}�g���b�N�X�̐ݒ�i���[���h���W�s��̐ݒ�j
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���݂̃}�e���A����ێ�
	D3DMATERIAL9 matDef;
	pDevice->GetMaterial(&matDef);

	if (m_pBuffMat != nullptr)
	{
		// �}�e���A���f�[�^�ւ̃|�C���^���擾
		D3DXMATERIAL* pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();
		D3DXCOLOR diffuse;

		for (int nCntMat = 0; nCntMat < (int)m_NumMat; nCntMat++)
		{
			diffuse = pMat[nCntMat].MatD3D.Diffuse;
			if (m_materialDiffuse.count(nCntMat) != 0)
			{
				pMat[nCntMat].MatD3D.Diffuse = m_materialDiffuse[nCntMat];
			}

			// �}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// ���f���p�[�c�̕`��
			m_pMesh->DrawSubset(nCntMat);

			pMat[nCntMat].MatD3D.Diffuse = diffuse;
		}
	}

	// �ێ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
// �N�I�[�^�j�I�����g�p�����`��
// Author : Yuda Kaito
// �T�v : �`����s��
//=============================================================================
void CObjectX::Draw(const D3DXQUATERNION& quaternion)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetInstance()->GetRenderer()->GetDevice();

	D3DXMATRIX mtxTrans, mtxParent;		// �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �N�H�[�^�j�I���̎g�p�����p���̐ݒ�
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxRot);		// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);	// (���s��ړ��֐�(��1������x,y,z�����̈ړ��s����쐬))
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	if (m_pParent != nullptr)
	{
		mtxParent = m_pParent->GetMtxWorld();

		// �s��|���Z�֐�
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);
	}

	// ���[���h�}�g���b�N�X�̐ݒ�i���[���h���W�s��̐ݒ�j
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���݂̃}�e���A����ێ�
	D3DMATERIAL9 matDef;
	pDevice->GetMaterial(&matDef);

	if (m_pBuffMat != nullptr)
	{
		// �}�e���A���f�[�^�ւ̃|�C���^���擾
		D3DXMATERIAL* pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();
		D3DXCOLOR diffuse;

		for (int nCntMat = 0; nCntMat < (int)m_NumMat; nCntMat++)
		{
			diffuse = pMat[nCntMat].MatD3D.Diffuse;
			if (m_materialDiffuse.count(nCntMat) != 0)
			{
				pMat[nCntMat].MatD3D.Diffuse = m_materialDiffuse[nCntMat];
			}

			// �}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// ���f���p�[�c�̕`��
			m_pMesh->DrawSubset(nCntMat);

			pMat[nCntMat].MatD3D.Diffuse = diffuse;
		}
	}

	// �ێ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
// �e�q�֌W�̂���`��
// Author : �������l
// �T�v : �`����s��
//=============================================================================
void CObjectX::Draw(D3DXMATRIX mtxParent)
{
	// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxTrans, mtxScaling;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �����̔��f
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxRot);			// �s��|���Z�֐� 

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);		// �s��ړ��֐�
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);			// �s��|���Z�֐�

	// �s��|���Z�֐�
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetInstance()->GetRenderer()->GetDevice();

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���݂̃}�e���A����ێ�
	D3DMATERIAL9 matDef;
	pDevice->GetMaterial(&matDef);

	// �e�N�X�`���|�C���^�̎擾
	CTexture *pTexture = CApplication::GetInstance()->GetTexture();

	if (m_pBuffMat != nullptr)
	{
		// �}�e���A���f�[�^�ւ̃|�C���^���擾
		D3DXMATERIAL* pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();
		D3DXCOLOR diffuse;

		for (int nCntMat = 0; nCntMat < (int)m_NumMat; nCntMat++)
		{// �}�e���A���̐ݒ�

			diffuse = pMat[nCntMat].MatD3D.Diffuse;

			if (m_materialDiffuse.count(nCntMat) != 0)
			{
				pMat[nCntMat].MatD3D.Diffuse = m_materialDiffuse[nCntMat];
			}

			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, pTexture->GetTexture("NULL"));

			// ���f���p�[�c�̕`��
			m_pMesh->DrawSubset(nCntMat);

			pMat[nCntMat].MatD3D.Diffuse = diffuse;

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, nullptr);
		}
	}

	// �ێ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

void CObjectX::SetScale(const D3DXVECTOR3& inScale)
{
	m_scale = inScale;

	m_MinVtx.x *= m_scale.x;
	m_MinVtx.y *= m_scale.y;
	m_MinVtx.z *= m_scale.z;

	m_MaxVtx.x *= m_scale.x;
	m_MaxVtx.y *= m_scale.y;
	m_MaxVtx.z *= m_scale.z;

	MulSize(m_scale);
}

//=============================================================================
// �����̐ݒ�
//=============================================================================
void CObjectX::SetRot(const D3DXVECTOR3 & inRot)
{
	if (m_rot != inRot)
	{
		SetMtxRot(inRot);
	}
	m_rot = inRot;
}

//=============================================================================
// ���_�ő召�l�̌v�Z����
//=============================================================================
void CObjectX::SetMtxRot(const D3DXVECTOR3 & inRot)
{
	D3DXQUATERNION quaternion;
	D3DXQuaternionRotationYawPitchRoll(&quaternion, inRot.y, inRot.x, inRot.z);
	SetMtxQuaternion(quaternion);
}

//=============================================================================
// ���_�ő召�l�̌v�Z����
//=============================================================================
void CObjectX::CalculationVtx()
{
	D3DXMATRIX mtxTrans, mtxWorld;

	D3DXMatrixIdentity(&mtxWorld);

	// �����̔��f
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &m_mtxRot);					// �s��|���Z�֐�

	D3DXVec3TransformCoord(&m_MaxVtx, &m_MaxVtx, &mtxWorld);
	D3DXVec3TransformCoord(&m_MinVtx, &m_MinVtx, &mtxWorld);

	if (m_MaxVtx.x < m_MinVtx.x)
	{
		float change = m_MaxVtx.x;
		m_MaxVtx.x = m_MinVtx.x;
		m_MinVtx.x = change;
	}

	if (m_MaxVtx.y < m_MinVtx.y)
	{
		float change = m_MaxVtx.y;
		m_MaxVtx.y = m_MinVtx.y;
		m_MinVtx.y = change;
	}

	if (m_MaxVtx.z < m_MinVtx.z)
	{
		float change = m_MaxVtx.z;
		m_MaxVtx.z = m_MinVtx.z;
		m_MinVtx.z = change;
	}

	SetScale(m_scale);
}

//=============================================================================
// ��������
//=============================================================================
CObjectX * CObjectX::Create(D3DXVECTOR3 pos, CTaskGroup::EPriority nPriority)
{
	// �|�C���^�錾
	CObjectX *pObjectX = nullptr;

	// �C���X�^���X����
	pObjectX = new CObjectX(nPriority);

	if (pObjectX != nullptr)
	{// �|�C���^�����݂�������s
		pObjectX->Init();
		pObjectX->SetPos(pos);
		pObjectX->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
	else
	{// �|�C���^����������������s
		assert(false);
	}

	// �|�C���^��Ԃ�
	return pObjectX;
}

//=============================================================================
// ���f���̓ǂݍ���
//=============================================================================
void CObjectX::LoadModel(const char *aFileName)
{
	CObjectXGroup *xGroup = CApplication::GetInstance()->GetObjectXGroup();
	m_pBuffMat = xGroup->GetBuffMat(aFileName);
	m_MaxVtx = xGroup->GetMaxVtx(aFileName);
	m_pMesh = xGroup->GetMesh(aFileName);
	m_MinVtx = xGroup->GetMinVtx(aFileName);
	m_NumMat = xGroup->GetNumMat(aFileName);
	m_size = xGroup->GetSize(aFileName);
}

//=============================================================================
// ���s���e����
//=============================================================================
void CObjectX::Projection(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetInstance()->GetRenderer()->GetDevice();

	D3DXMATRIX mtxTrans;	// �v�Z�p�}�g���b�N�X

	// �ϐ��錾
	D3DXMATRIX mtxShadow;
	D3DXPLANE planeField;
	D3DXVECTOR4 vecLight;
	D3DXVECTOR3 pos, normal;
	D3DMATERIAL9 Material;

	// �V���h�E�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxShadow);

	vecLight = -D3DXVECTOR4(0.2f, -0.5f, 0.3f, 0.0f);

	if (m_pos.y >= -20.0f)
	{
		pos = D3DXVECTOR3(0.0f, 0.1f, 0.0f);
	}

	normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXPlaneFromPointNormal(&planeField, &pos, &normal);
	D3DXMatrixShadow(&mtxShadow,&vecLight, &planeField);

	// ���[���h�}�g���b�N�X�Ɗ|�����킹��
	D3DXMatrixMultiply(&mtxShadow, &m_mtxWorld, &mtxShadow);

	// ���[���h�}�g���b�N�X�̐ݒ�i���[���h���W�s��̐ݒ�j
	pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

	// ���݂̃}�e���A����ێ�
	D3DMATERIAL9 matDef;
	pDevice->GetMaterial(&matDef);

	if (m_pBuffMat != nullptr)
	{
		// �}�e���A���f�[�^�ւ̃|�C���^���擾
		D3DXMATERIAL* pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)m_NumMat; nCntMat++)
		{
			// �}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			Material = pMat[nCntMat].MatD3D;
			Material.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			Material.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

			// �}�e���A���̐ݒ�
			pDevice->SetMaterial(&Material);

			// ���f���p�[�c�̕`��
			m_pMesh->DrawSubset(nCntMat);
		}
	}

	// �ێ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
// �F���i�g�U���ˌ��j�̐ݒ�
//=============================================================================
void CObjectX::SetMaterialDiffuse(unsigned int index, const D3DXCOLOR & inColor)
{
	// �ύX�\��̃}�e���A�����Ȃ��ꍇ
	if (index >= m_NumMat)
	{
		assert(false);
		return;
	}

	if (m_materialDiffuse.count(index) == 0)
	{
		m_materialDiffuse.emplace(index, inColor);
	}
	else
	{
		m_materialDiffuse[index] = inColor;
	}
}

//=============================================================================
// �����蔻�� (���E,��,��O)
//=============================================================================
bool CObjectX::Collision(const D3DXVECTOR3& pPos, const D3DXVECTOR3& pPosOld, const D3DXVECTOR3& pSize)
{
	if (!m_isCollision)
	{
		return false;
	}

	D3DXVECTOR3 pos = GetPos();

	// �ϐ��錾
	bool bIsLanding = false;

	// ���f���̍��������蔻��
	if ((pPos.z - pSize.z * 0.5f < pos.z + m_MaxVtx.z) &&
		(pPos.z + pSize.z * 0.5f > pos.z + m_MinVtx.z) &&
		(pPosOld.x + pSize.x * 0.5f <= pos.x + m_MinVtx.x ) &&
		(pPos.x + pSize.x * 0.5f > pos.x + m_MinVtx.x * 0.5f) &&
		(pPos.y + pSize.y > pos.y - m_MaxVtx.y * 0.5f) &&
		(pPos.y < pos.y + m_MaxVtx.y * 0.5f))
	{
		bIsLanding = true;
	}
	// ���f���̉E�������蔻��
	if ((pPos.z - pSize.z * 0.5f < pos.z + m_MaxVtx.z) &&
		(pPos.z + pSize.z * 0.5f > pos.z + m_MinVtx.z) &&
		(pPosOld.x - pSize.x * 0.5f >= pos.x + m_MaxVtx.x) &&
		(pPos.x - pSize.x * 0.5f < pos.x + m_MaxVtx.x * 0.5f) &&
		(pPos.y + pSize.y > pos.y - m_MaxVtx.y * 0.5f) &&
		(pPos.y < pos.y + m_MaxVtx.y * 0.5f))
	{
		bIsLanding = true;
	}
	// ���f���̉��������蔻��
	if ((pPos.x - pSize.x * 0.5f < pos.x + m_MaxVtx.x) &&
		(pPos.x + pSize.x * 0.5f > pos.x + m_MinVtx.x) &&
		(pPosOld.z - pSize.z * 0.5f >= pos.z + m_MaxVtx.z) &&
		(pPos.z - pSize.z * 0.5f < pos.z + m_MaxVtx.z * 0.5f) &&
		(pPos.y + pSize.y > pos.y - m_MaxVtx.y * 0.5f) &&
		(pPos.y < pos.y + m_MaxVtx.y * 0.5f))
	{
		bIsLanding = true;
	}
	// ���f���̎�O�������蔻��
	if ((pPos.x - pSize.x * 0.5f < pos.x + m_MaxVtx.x) &&
		(pPos.x + pSize.x * 0.5f > pos.x + m_MinVtx.x) &&
		(pPosOld.z + pSize.z * 0.5f <= pos.z + m_MinVtx.z) &&
		(pPos.z + pSize.z * 0.5f > pos.z + m_MinVtx.z * 0.5f) &&
		(pPos.y + pSize.y > pos.y - m_MaxVtx.y * 0.5f) &&
		(pPos.y < pos.y + m_MaxVtx.y * 0.5f))
	{
		bIsLanding = true;
	}

	// �l��Ԃ�
	return bIsLanding;
}

//=============================================================================
// �����蔻�� (���E,��,��O)
//=============================================================================
bool CObjectX::Collision(D3DXVECTOR3 * pPos, D3DXVECTOR3 * pPosOld, D3DXVECTOR3 * inMaxVtx, D3DXVECTOR3 * inMinVtx)
{
	if (!m_isCollision)
	{
		return false;
	}

	D3DXVECTOR3 pos = GetPos();

	// �ϐ��錾
	bool bIsLanding = false;

	// ���f���̏��グ����
	if ((pPos->y + 20.0f > pos.y + m_MaxVtx.y))
	{
		return bIsLanding;
	}

	// ���f���̍��������蔻��
	if ((pPos->z + inMinVtx->z < pos.z + m_MaxVtx.z) &&
		(pPos->z + inMaxVtx->z > pos.z + m_MinVtx.z) &&
		(pPosOld->x + inMaxVtx->x  <= pos.x + m_MinVtx.x) &&
		(pPos->x + inMaxVtx->x > pos.x + m_MinVtx.x) &&
		(pPos->y + inMaxVtx->y > pos.y + m_MinVtx.y) &&
		(pPos->y < pos.y + m_MaxVtx.y))
	{
		bIsLanding = true;
		pPos->x = pos.x + m_MinVtx.x + inMinVtx->x - 5.0f;
	}

	// ���f���̉E�������蔻��
	if ((pPos->z + inMinVtx->z < pos.z + m_MaxVtx.z) &&
		(pPos->z + inMaxVtx->z > pos.z + m_MinVtx.z) &&
		(pPosOld->x + inMinVtx->x >= pos.x + m_MaxVtx.x) &&
		(pPos->x + inMinVtx->x < pos.x + m_MaxVtx.x) &&
		(pPos->y + inMaxVtx->y > pos.y + m_MinVtx.y) &&
		(pPos->y < pos.y + m_MaxVtx.y))
	{
		bIsLanding = true;
		pPos->x = pos.x + m_MaxVtx.x + inMaxVtx->x;
		pPos->x -= 0.1f;
	}

	// ���f���̉��������蔻��
	if ((pPos->x + inMinVtx->x < pos.x + m_MaxVtx.x) &&
		(pPos->x + inMaxVtx->x > pos.x + m_MinVtx.x) &&
		(pPosOld->z + inMinVtx->z >= pos.z + m_MaxVtx.z) &&
		(pPos->z + inMinVtx->z < pos.z + m_MaxVtx.z) &&
		(pPos->y + inMaxVtx->y > pos.y + m_MinVtx.y) &&
		(pPos->y < pos.y + m_MaxVtx.y))
	{
		bIsLanding = true;
		pPos->z = pos.z + m_MaxVtx.z + inMaxVtx->z;
	}

	// ���f���̎�O�������蔻��
	if ((pPos->x + inMinVtx->x < pos.x + m_MaxVtx.x) &&
		(pPos->x + inMaxVtx->x > pos.x + m_MinVtx.x) &&
		(pPosOld->z + inMaxVtx->z <= pos.z + m_MinVtx.z) &&
		(pPos->z + inMaxVtx->z > pos.z + m_MinVtx.z) &&
		(pPos->y + inMaxVtx->y > pos.y + m_MinVtx.y) &&
		(pPos->y < pos.y + m_MaxVtx.y))
	{
		bIsLanding = true;
		pPos->z = pos.z + m_MinVtx.z + inMinVtx->z - 5.0f;
		pPos->z -= 0.1f;
	}

	// �l��Ԃ�
	return bIsLanding;
}

//=============================================================================
// �����蔻�� (�㑤)
//=============================================================================
bool CObjectX::UpCollision(D3DXVECTOR3 * pPos, D3DXVECTOR3 * pPosOld, D3DXVECTOR3 * pSize, D3DXVECTOR3 * pMove)
{
	if (!m_isCollision)
	{
		return false;
	}

	// �ϐ��錾
	bool bIsLanding = false;

	// ���f���̏㑤�����蔻��
	if ((pPos->z - pSize->z * 0.5f < m_pos.z + m_MaxVtx.z) &&
		(pPos->z + pSize->z * 0.5f > m_pos.z + m_MinVtx.z) &&
		(pPos->x - pSize->x * 0.5f < m_pos.x + m_MaxVtx.x) &&
		(pPos->x + pSize->x * 0.5f > m_pos.x + m_MinVtx.x) &&
		(pPos->y <= m_pos.y + m_MaxVtx.y))
	{
		bIsLanding = true;
		pPos->y = m_pos.y + m_MaxVtx.y;
		if (pPos->y == pPosOld->y)
		{
			pMove->y = 0.0f;
		}
	}

	// �l��Ԃ�
	return bIsLanding;
}

//=============================================================================
// OBB��OBB�̓����蔻��
//=============================================================================
bool CObjectX::OBBAndOBB(CObjectX* inObjectX)
{
	if (!inObjectX->IsCollision())
	{
		return false;
	}

	// ��]�s��
	D3DXMATRIX mtxThisRot = m_mtxRot;
	mtxThisRot._11 = m_mtxRot._11;
	mtxThisRot._12 = m_mtxRot._21;
	mtxThisRot._13 = m_mtxRot._31;
	mtxThisRot._21 = m_mtxRot._12;
	mtxThisRot._22 = m_mtxRot._22;
	mtxThisRot._23 = m_mtxRot._32;
	mtxThisRot._31 = m_mtxRot._13;
	mtxThisRot._32 = m_mtxRot._23;
	mtxThisRot._33 = m_mtxRot._33;

	D3DXMATRIX mtx = mtxThisRot * inObjectX->GetMatRot();
	D3DXMATRIX mtxAds;
	mtxAds._11 = fabs(mtx._11);
	mtxAds._12 = fabs(mtx._21);
	mtxAds._13 = fabs(mtx._31);
	mtxAds._21 = fabs(mtx._12);
	mtxAds._22 = fabs(mtx._22);
	mtxAds._23 = fabs(mtx._32);
	mtxAds._31 = fabs(mtx._13);
	mtxAds._32 = fabs(mtx._23);
	mtxAds._33 = fabs(mtx._33);

	const float etc = 1.0e-6;	// �߂����Ꮼ���Ȑ���(100������1)
	bool parallel = false;

	if (mtxAds._11 + etc >= 1.0f ||
		mtxAds._12 + etc >= 1.0f || 
		mtxAds._13 + etc >= 1.0f || 
		mtxAds._21 + etc >= 1.0f || 
		mtxAds._22 + etc >= 1.0f || 
		mtxAds._23 + etc >= 1.0f || 
		mtxAds._31 + etc >= 1.0f || 
		mtxAds._32 + etc >= 1.0f || 
		mtxAds._33 + etc >= 1.0f)
	{
		parallel = true;
	}

	D3DXVECTOR3 interval;
	D3DXVECTOR3 pos = (GetPos() - inObjectX->GetPos());
	D3DXVec3TransformCoord(&interval, &pos, &m_mtxRot);

	float s;

	D3DXVECTOR3 thisScale = m_scale;
	D3DXVECTOR3 targetScale = inObjectX->GetScale();
	float aMax = -FLT_MAX;
	float bMax = -FLT_MAX;
	float eMax = -FLT_MAX;
	int aAxis = ~0;
	int bAxis = ~0;
	int eAxis = ~0;
	D3DXVECTOR3 nA;
	D3DXVECTOR3 nB;
	D3DXVECTOR3 nE;

	auto TrackFaceAxis = [](int* axis,int nCnt,float s,float* max,const D3DXVECTOR3& normal, D3DXVECTOR3* axisNormal)
	{
		if (s > 0.0f)
		{
			return true;
		}

		if (s > *max)
		{
			*max = s;
			*axis = nCnt;
			*axisNormal = normal;
		}

		return false;
	};

	//A.e1
	s = fabs(interval.x) - (thisScale.x + D3DXVec3Dot(&D3DXVECTOR3(mtxAds._11, mtxAds._12, mtxAds._13), &targetScale));
	if (TrackFaceAxis(&aAxis, 0, s, &aMax, D3DXVECTOR3(m_mtxRot._11, m_mtxRot._12, m_mtxRot._13), &nA))
	{
		return false;
	}

	//A.e2
	s = fabs(interval.y) - (thisScale.y + D3DXVec3Dot(&D3DXVECTOR3(mtxAds._21, mtxAds._22, mtxAds._23), &targetScale));
	if (TrackFaceAxis(&aAxis, 1, s, &aMax, D3DXVECTOR3(m_mtxRot._21, m_mtxRot._22, m_mtxRot._23), &nA))
	{
		return false;
	}

	//A.e3
	s = fabs(interval.z) - (thisScale.z + D3DXVec3Dot(&D3DXVECTOR3(mtxAds._31, mtxAds._32, mtxAds._33), &targetScale));
	if (TrackFaceAxis(&aAxis, 2, s, &aMax, D3DXVECTOR3(m_mtxRot._31, m_mtxRot._32, m_mtxRot._33), &nA))
	{
		return false;
	}

	D3DXMATRIX targetMtxRot = inObjectX->GetMatRot();

	//B.e1
	s = fabs(D3DXVec3Dot(&interval, &D3DXVECTOR3(mtx._11, mtx._12, mtx._13))) - (targetScale.x + D3DXVec3Dot(&D3DXVECTOR3(mtxAds._11, mtxAds._12, mtxAds._13), &thisScale));
	if (TrackFaceAxis(&bAxis, 3, s, &bMax, D3DXVECTOR3(targetMtxRot._11, targetMtxRot._12, targetMtxRot._13), &nB))
	{
		return false;
	}

	//B.e2
	s = fabs(D3DXVec3Dot(&interval, &D3DXVECTOR3(mtx._21, mtx._22, mtx._23))) - (targetScale.y + D3DXVec3Dot(&D3DXVECTOR3(mtxAds._21, mtxAds._22, mtxAds._23), &thisScale));
	if (TrackFaceAxis(&bAxis, 4, s, &bMax, D3DXVECTOR3(targetMtxRot._21, targetMtxRot._22, targetMtxRot._23), &nB))
	{
		return false;
	}

	//B.e3
	s = fabs(D3DXVec3Dot(&interval, &D3DXVECTOR3(mtx._31, mtx._32, mtx._33))) - (targetScale.z + D3DXVec3Dot(&D3DXVECTOR3(mtxAds._31, mtxAds._32, mtxAds._33),&thisScale));
	if (TrackFaceAxis(&bAxis, 5, s, &bMax, D3DXVECTOR3(targetMtxRot._31, targetMtxRot._32, targetMtxRot._33), &nB))
	{
		return false;
	}

	if (!parallel)
	{
		// Edge axis checks
		float thisRadius;
		float targetRadius;

		auto TrackEdgeAxis = [](int* axis, int nCnt, float s, float* max, const D3DXVECTOR3& normal, D3DXVECTOR3* axisNormal)
		{
			if (s > 0.0f)
			{
				return true;
			}

			float i = 1.0f / D3DXVec3Length(&normal);
			s *= i;

			if (s > *max)
			{
				*max = s;
				*axis = nCnt;
				*axisNormal = normal * i;
			}

			return false;
		};

		//C11
		thisRadius = thisScale.y * mtxAds._13 + thisScale.z * mtxAds._12;
		targetRadius = targetScale.y * mtxAds._31 + targetScale.z * mtxAds._21;
		s = fabs(interval.z * mtx._12 - interval.y * mtx._13) - (thisRadius + targetRadius);
		if (TrackEdgeAxis(&eAxis, 6, s, &eMax, D3DXVECTOR3(0.0f, -mtx._13, mtx._12), &nE))
			return false;

		//C12
		thisRadius = thisScale.y * mtxAds._23 + thisScale.z * mtxAds._22;
		targetRadius = targetScale.x * mtxAds._31 + targetScale.z * mtxAds._11;
		s = fabs(interval.z * mtx._22 - interval.y * mtx._23) - (thisRadius + targetRadius);
		if (TrackEdgeAxis(&eAxis, 7, s, &eMax, D3DXVECTOR3(0.0f, -mtx._23, mtx._22), &nE))
			return false;

		//C13
		thisRadius = thisScale.y * mtxAds._33 + thisScale.z * mtxAds._32;
		targetRadius = targetScale.x * mtxAds._21 + targetScale.y * mtxAds._11;
		s = fabs(interval.z * mtx._32 - interval.y * mtx._33) - (thisRadius + targetRadius);
		if (TrackEdgeAxis(&eAxis, 8, s, &eMax, D3DXVECTOR3(0.0f, -mtx._33, mtx._32), &nE))
			return false;

		//C21
		thisRadius = thisScale.x * mtxAds._13 + thisScale.z * mtxAds._11;
		targetRadius = targetScale.y * mtxAds._31 + targetScale.z * mtxAds._22;
		s = fabs(interval.x * mtx._13 - interval.z * mtx._11) - (thisRadius + targetRadius);
		if (TrackEdgeAxis(&eAxis, 9, s, &eMax, D3DXVECTOR3(mtx._13, 0.0f, -mtx._11), &nE))
			return false;

		//C22
		thisRadius = thisScale.x * mtxAds._23 + thisScale.z * mtxAds._21;
		targetRadius = targetScale.x * mtxAds._32 + targetScale.z * mtxAds._12;
		s = fabs(interval.x * mtx._23 - interval.z * mtx._21) - (thisRadius + targetRadius);
		if (TrackEdgeAxis(&eAxis, 10, s, &eMax, D3DXVECTOR3(mtx._23, 0.0f, -mtx._21), &nE))
			return false;

		//C23
		thisRadius = thisScale.x * mtxAds._33 + thisScale.z * mtxAds._31;
		targetRadius = targetScale.x * mtxAds._22 + targetScale.y * mtxAds._12;
		s = fabs(interval.x * mtx._33 - interval.z * mtx._31) - (thisRadius + targetRadius);
		if (TrackEdgeAxis(&eAxis, 11, s, &eMax, D3DXVECTOR3(mtx._33, 0.0f, -mtx._31), &nE))
			return false;

		//C31
		thisRadius = thisScale.x * mtxAds._12 + thisScale.y * mtxAds._11;
		targetRadius = targetScale.y * mtxAds._33 + targetScale.z * mtxAds._23;
		s = fabs(interval.y * mtx._11 - interval.x * mtx._12) - (thisRadius + targetRadius);
		if (TrackEdgeAxis(&eAxis, 12, s, &eMax, D3DXVECTOR3(-mtx._13, mtx._12, 0.0f), &nE))
			return false;

		//C32
		thisRadius = thisScale.x * mtxAds._22 + thisScale.y * mtxAds._21;
		targetRadius = targetScale.x * mtxAds._33 + targetScale.z * mtxAds._13;
		s = fabs(interval.y * mtx._21 - interval.x * mtx._22) - (thisRadius + targetRadius);
		if (TrackEdgeAxis(&eAxis, 13, s, &eMax, D3DXVECTOR3(-mtx._22, mtx._21, 0.0f), &nE))
			return false;

		//C33
		thisRadius = thisScale.x * mtxAds._32 + thisScale.y * mtxAds._31;
		targetRadius = targetScale.x * mtxAds._23 + targetScale.y * mtxAds._13;
		s = fabs(interval.y * mtx._31 - interval.x * mtx._32) - (thisRadius + targetRadius);
		if (TrackEdgeAxis(&eAxis, 14, s, &eMax, D3DXVECTOR3(-mtx._32, mtx._31, 0.0f), &nE))
			return false;
	}
	return true;
}

//=============================================================================
// OBB��Box�̓����蔻��
//=============================================================================
bool CObjectX::OBBAndBoxTop(CObjectX * inObjectX, float * outLength)
{
	if (!(inObjectX->GetPos().x + inObjectX->GetMaxVtx().x > m_pos.x ) || !(m_pos.x > inObjectX->GetPos().x + inObjectX->GetMinVtx().x))
	{
		return false;
	}
	
	if (!(inObjectX->GetPos().z + inObjectX->GetMaxVtx().z > m_pos.z) || !(m_pos.z > inObjectX->GetPos().z + inObjectX->GetMinVtx().z))
	{
		return false;
	}
	D3DXVECTOR3 PlanePos = inObjectX->GetPos();
	PlanePos.y += inObjectX->GetMaxVtx().y;

	return OBBAndPolygon(PlanePos, outLength);
}

//=============================================================================
// OBB��Box�̓����蔻��
//=============================================================================
bool CObjectX::OBBAndBoxDown(CObjectX * inObjectX, float * outLength)
{
	if (!(inObjectX->GetPos().x + inObjectX->GetMaxVtx().x > m_pos.x) || !(m_pos.x > inObjectX->GetPos().x + inObjectX->GetMinVtx().x))
	{
		return false;
	}

	if (!(inObjectX->GetPos().z + inObjectX->GetMaxVtx().z > m_pos.z) || !(m_pos.z > inObjectX->GetPos().z + inObjectX->GetMinVtx().z))
	{
		return false;
	}
	D3DXVECTOR3 PlanePos = inObjectX->GetPos();
	PlanePos.y += inObjectX->GetMinVtx().y;

	return OBBAndPolygon(PlanePos, outLength);
}

//=============================================================================
// OBB��Box�̓����蔻��
//=============================================================================
bool CObjectX::OBBAndBoxLeft(CObjectX * inObjectX, float * outLength)
{
	if (!(inObjectX->GetPos().y + inObjectX->GetMaxVtx().y > m_pos.x) || !(m_pos.y > inObjectX->GetPos().y + inObjectX->GetMinVtx().y))
	{
		return false;
	}

	if (!(inObjectX->GetPos().z + inObjectX->GetMaxVtx().z > m_pos.z) || !(m_pos.z > inObjectX->GetPos().z + inObjectX->GetMinVtx().z))
	{
		return false;
	}
	D3DXVECTOR3 PlanePos = inObjectX->GetPos();
	PlanePos.y += inObjectX->GetMinVtx().x;

	return OBBAndPolygon(PlanePos, outLength);
}

//=============================================================================
// OBB��Box�̓����蔻��
//=============================================================================
bool CObjectX::OBBAndBoxRight(CObjectX * inObjectX, float * outLength)
{
	if (!(inObjectX->GetPos().y + inObjectX->GetMaxVtx().y > m_pos.x) || !(m_pos.y > inObjectX->GetPos().y + inObjectX->GetMinVtx().y))
	{
		return false;
	}

	if (!(inObjectX->GetPos().z + inObjectX->GetMaxVtx().z > m_pos.z) || !(m_pos.z > inObjectX->GetPos().z + inObjectX->GetMinVtx().z))
	{
		return false;
	}
	D3DXVECTOR3 PlanePos = inObjectX->GetPos();
	PlanePos.x += inObjectX->GetMaxVtx().x;

	return OBBAndPolygon(PlanePos, outLength);
}

//=============================================================================
// OBB��Box�̓����蔻��
//=============================================================================
bool CObjectX::OBBAndBoxFront(CObjectX * inObjectX, float * outLength)
{
	if (!(inObjectX->GetPos().x + inObjectX->GetMaxVtx().x > m_pos.x) || !(m_pos.x > inObjectX->GetPos().x + inObjectX->GetMinVtx().x))
	{
		return false;
	}

	if (!(inObjectX->GetPos().y + inObjectX->GetMaxVtx().y > m_pos.z) || !(m_pos.y > inObjectX->GetPos().y + inObjectX->GetMinVtx().y))
	{
		return false;
	}

	D3DXVECTOR3 PlanePos = inObjectX->GetPos();
	PlanePos.z += inObjectX->GetMaxVtx().z;

	return OBBAndPolygon(PlanePos, outLength);
}

//=============================================================================
// OBB��Box�̓����蔻��
//=============================================================================
bool CObjectX::OBBAndBoxBack(CObjectX * inObjectX, float * outLength)
{
	if (!(inObjectX->GetPos().x + inObjectX->GetMaxVtx().x > m_pos.x) || !(m_pos.x > inObjectX->GetPos().x + inObjectX->GetMinVtx().x))
	{
		return false;
	}

	if (!(inObjectX->GetPos().y + inObjectX->GetMaxVtx().y > m_pos.z) || !(m_pos.y > inObjectX->GetPos().y + inObjectX->GetMinVtx().y))
	{
		return false;
	}
	D3DXVECTOR3 PlanePos = inObjectX->GetPos();
	PlanePos.z += inObjectX->GetMinVtx().z;

	return OBBAndPolygon(PlanePos, outLength);
}

//=============================================================================
// OBB��3DPolygon�̓����蔻��
//=============================================================================
bool CObjectX::OBBAndPolygon(const CObjectPolygon3D * inObjectPolgon, float* outLength)
{
	// ���ʂ̖@���ɑ΂���OBB�̎ˉe���̒������Z�o
	float r = 0.0f;		// �ߐڋ���
	D3DXVECTOR3 polygonNormal = inObjectPolgon->GetNormal();	// ���ʂ̖@���x�N�g��

	// X��
	D3DXVECTOR3 thisNormalizeVecX = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	D3DXVec3TransformCoord(&thisNormalizeVecX, &thisNormalizeVecX, &m_mtxRot);
	D3DXVec3Normalize(&thisNormalizeVecX, &thisNormalizeVecX);
	r += fabs(D3DXVec3Dot(&(thisNormalizeVecX * (this->GetSize().x * 0.5f)), &polygonNormal));

	// Y��
	D3DXVECTOR3 thisNormalizeVecY = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXVec3TransformCoord(&thisNormalizeVecY, &thisNormalizeVecY, &m_mtxRot);
	D3DXVec3Normalize(&thisNormalizeVecY, &thisNormalizeVecY);
	r += fabs(D3DXVec3Dot(&(thisNormalizeVecY * (this->GetSize().y * 0.5f)), &polygonNormal));

	// Z��
	D3DXVECTOR3 thisNormalizeVecZ = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	D3DXVec3TransformCoord(&thisNormalizeVecZ, &thisNormalizeVecZ, &m_mtxRot);
	D3DXVec3Normalize(&thisNormalizeVecZ, &thisNormalizeVecZ);
	r += fabs(D3DXVec3Dot(&(thisNormalizeVecZ * (this->GetSize().z * 0.5f)), &polygonNormal));

	// ���ʂ�OBB�̋������Z�o
	D3DXVECTOR3 ObbPos = this->GetPos();
	D3DXVECTOR3 PlanePos = inObjectPolgon->GetPos();

	float dist = D3DXVec3Dot(&(ObbPos - PlanePos), &polygonNormal);

	// �߂��������Z�o
	if (outLength != nullptr)
	{
		if (dist > 0)
		{
			*outLength = r - fabs(dist);
		}
		else
		{
			*outLength = r + fabs(dist);
		}
	}

	// �Փ˔���
	if (fabs(dist) - r < 0.0f)
	{
		return true; // �Փ˂��Ă���
	}

	return false; // �Փ˂��Ă��Ȃ�
}

//=============================================================================
// OBB�ƕ��s�̓����蔻��
//=============================================================================
bool CObjectX::OBBAndPolygon(const D3DXVECTOR3 & inPos, float * outLength)
{
	// ���ʂ̖@���ɑ΂���OBB�̎ˉe���̒������Z�o
	float r = 0.0f;		// �ߐڋ���
	D3DXVECTOR3 polygonNormal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	// ���ʂ̖@���x�N�g��

	// X��
	D3DXVECTOR3 thisNormalizeVecX = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	D3DXVec3TransformCoord(&thisNormalizeVecX, &thisNormalizeVecX, &m_mtxRot);
	D3DXVec3Normalize(&thisNormalizeVecX, &thisNormalizeVecX);
	r += fabs(D3DXVec3Dot(&(thisNormalizeVecX * (this->GetSize().x * 0.5f)), &polygonNormal));

	// Y��
	D3DXVECTOR3 thisNormalizeVecY = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXVec3TransformCoord(&thisNormalizeVecY, &thisNormalizeVecY, &m_mtxRot);
	D3DXVec3Normalize(&thisNormalizeVecY, &thisNormalizeVecY);
	r += fabs(D3DXVec3Dot(&(thisNormalizeVecY * (this->GetSize().y * 0.5f)), &polygonNormal));

	// Z��
	D3DXVECTOR3 thisNormalizeVecZ = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	D3DXVec3TransformCoord(&thisNormalizeVecZ, &thisNormalizeVecZ, &m_mtxRot);
	D3DXVec3Normalize(&thisNormalizeVecZ, &thisNormalizeVecZ);
	r += fabs(D3DXVec3Dot(&(thisNormalizeVecZ * (this->GetSize().z * 0.5f)), &polygonNormal));

	// ���ʂ�OBB�̋������Z�o
	D3DXVECTOR3 ObbPos = this->GetPos();

	float dist = D3DXVec3Dot(&(ObbPos - inPos), &polygonNormal);

	// �߂��������Z�o
	if (outLength != nullptr)
	{
		if (dist > 0)
		{
			*outLength = r - fabs(dist);
		}
		else
		{
			*outLength = r + fabs(dist);
		}
	}

	// �Փ˔���
	if (fabs(dist) - r < 0.0f)
	{
		return true; // �Փ˂��Ă���
	}

	return false; // �Փ˂��Ă��Ȃ�
}

//=============================================================================
// 3�̓��ς̐�Βl�̘a�œ��e���������v�Z
// �����F������Sep�͕W��������Ă��邱��
//=============================================================================
float CObjectX::LenSegOnSeparateAxis(D3DXVECTOR3 * Sep, D3DXVECTOR3 * e1, D3DXVECTOR3 * e2, D3DXVECTOR3 * e3)
{
	float r1 = fabs(D3DXVec3Dot(Sep, e1));
	float r2 = fabs(D3DXVec3Dot(Sep, e2));
	float r3 = e3 ? (fabs(D3DXVec3Dot(Sep, e3))) : 0.0f;
	return r1 + r2 + r3;
}

bool CObjectX::UpCollision(D3DXVECTOR3 * pPos, D3DXVECTOR3 * pPosOld, D3DXVECTOR3 * inMaxVtx, D3DXVECTOR3 * inMinVtx, D3DXVECTOR3 * pMove)
{
	if (!m_isCollision)
	{
		return false;
	}

	// �ϐ��錾
	bool bIsLanding = false;

	// ���f���̏㑤�����蔻��
	if ((pPos->z + inMinVtx->z < m_pos.z + m_MaxVtx.z) &&
		(pPos->z + inMaxVtx->z > m_pos.z + m_MinVtx.z) &&
		(pPos->x + inMinVtx->x < m_pos.x + m_MaxVtx.x) &&
		(pPos->x + inMaxVtx->x > m_pos.x + m_MinVtx.x) &&
		(pPos->y <= m_pos.y + m_MaxVtx.y))
	{
		bIsLanding = true;
		pPos->y = m_pos.y + m_MaxVtx.y;
		if (pPos->y == pPosOld->y)
		{
			pMove->y = 0.0f;
		}
	}

	// �l��Ԃ�
	return bIsLanding;
}
