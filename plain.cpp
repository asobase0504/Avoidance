//=============================================================================
//
// �v���C���[�ݒ�
// Author:Yuda Kaito
//
//=============================================================================
#include <assert.h>
#include "plain.h"
#include "utility.h"

#include "application.h"
#include "task_group.h"
#include "camera.h"
#include "color.h"
#include "line.h"

//-----------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------
CPlain::CPlain()
{
}

//-----------------------------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------------------------
CPlain::~CPlain()
{
}

//-----------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------
HRESULT CPlain::Init()
{
	begin = false;
	m_display = true;
	m_endCnt = 0;
	// ���݂̃��[�V�����ԍ��̕ۊ�
	CObjectX::Init();
	LoadModel("BOX");
	SetType(EType::PLAIN);
	return S_OK;
}

//-----------------------------------------------------------------------------
// �I��
//-----------------------------------------------------------------------------
void CPlain::Uninit()
{
	CObjectX::Uninit();
}

//-----------------------------------------------------------------------------
// �X�V
//-----------------------------------------------------------------------------
void CPlain::NormalUpdate()
{
	CObject* player = SearchType(CObject::EType::PLAYER, CTaskGroup::EPriority::LEVEL_3D_1);
	CCamera* camera = (CCamera*)CApplication::GetInstance()->GetTaskGroup()->SearchRoleTop(CTask::ERole::ROLE_CAMERA, CTaskGroup::EPriority::LEVEL_3D_1);

	m_display = !SegmentAndAABB(player->GetPos(), camera->GetPos());
}

//-----------------------------------------------------------------------------
// �X�V
//-----------------------------------------------------------------------------
void CPlain::EndUpdate()
{
	if(!begin)
	{
		SetMove(D3DXVECTOR3(FloatRandam(5.0f, -5.0f), 0.0f, FloatRandam(5.0f, -5.0f)));
		SetMoveRot(D3DXVECTOR3(FloatRandam(0.015f, -0.015f), 0.0f, FloatRandam(0.0125f, -0.015f)));
		begin = true;
	}

	m_endCnt++;

	if (m_endCnt > 320)
	{
		Release();
	}
}

//-----------------------------------------------------------------------------
// �`��
//-----------------------------------------------------------------------------
void CPlain::Draw()
{
	if (m_display)
	{
		CObjectX::Draw();
	}
}

//-----------------------------------------------------------------------------
// ����
//-----------------------------------------------------------------------------
CPlain* CPlain::Create()
{
	CPlain* plain = new CPlain;

	if (plain != nullptr)
	{
		plain->Init();
	}

	return plain;
}
