//============================
//
// �QDpolygon�ݒ�
// Author:hamada ryuuga
//
//============================

#include "2dpolygontemplate.h"
#include "utility.h"
#include "texture.h"

//------------------------------------
// �R���X�g���N�^
//------------------------------------
CTest::CTest() :CObject2d(CTaskGroup::LEVEL_2D_1)
{
}

//------------------------------------
// �f�X�g���N�^
//------------------------------------
CTest::~CTest()
{
}

//------------------------------------
// ������
//------------------------------------
HRESULT CTest::Init()
{
	CObject2d::Init();

	return S_OK;
}

//------------------------------------
// �I��
//------------------------------------
void CTest::Uninit()
{
	CObject2d::Uninit();
}

//------------------------------------
// �X�V
//------------------------------------
void CTest::NormalUpdate()
{
	CObject2d::NormalUpdate();
	//����
	CTest::move();
}

//------------------------------------
// �`��
//------------------------------------
void CTest::Draw()
{
	CObject2d::Draw();
}

//------------------------------------
// create
//------------------------------------
CTest *CTest::Create(D3DXVECTOR3 pos, bool b3D)
{
	CTest * pObject = new CTest;

	if (pObject != nullptr)
	{
		D3DXVECTOR3 Poppos = pos;
		if (b3D)
		{
			Poppos = ScreenCastWorld(Poppos,			// �X�N���[�����W
				D3DXVECTOR3((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, 0.0f));								// �X�N���[���T�C�Y
		}
		pObject->Init();
		pObject->SetPos(Poppos);
		pObject->SetTexture("EXPLOSION");//�e�N�X�`���I��
		pObject->SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));//move�̐ݒ�
		pObject->SetSize(D3DXVECTOR3(300.0f, 300.0f, 0.0f));//�T�C�Y�ݒ�

		//������(1���̖���,2�c�̖���,3Animation���x,�S��{�[�������Ǖ\������܂ł̃^�C�����O,5�����ɃA�j���[�V�����Đ����邩�ǂ���)
		pObject->SetAnimation(7,1,0,0,false);//Animation�摜�������ꍇ���������,�ꖇ�G�Ȃ�����Ȃ��ƃo�O��

		pObject->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));//�F�ݒ�
	
	}
	return pObject;
}


//------------------------------------
// �����n��
//------------------------------------
void CTest::move()
{
	//�������ꂽ���Ƃ��͂�����	SetMove()�ŕς�����

	m_pos += m_move;
}