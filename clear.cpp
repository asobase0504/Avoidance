//=============================================================================
//
// �J�E���g�_�E��UI
// Author:Yuda Kaito
//
//=============================================================================
#include "clear.h"
#include "number.h"
#include "object2d.h"
#include "utility.h"
#include "delay_process.h"

//-----------------------------------------------------------------------------
// �萔
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
const int CClear::START_TIME(120);

//-----------------------------------------------------------------------------
// �R���X�g���N�^
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
CClear::CClear()
{
}

//-----------------------------------------------------------------------------
// �f�X�g���N�^
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
CClear::~CClear()
{
}

//-----------------------------------------------------------------------------
// ������
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
HRESULT CClear::Init()
{
	m_count = 0;
	m_end = false;

	{
		m_stopLogo = CObject2d::Create(CTaskGroup::LEVEL_2D_1);
		D3DXVECTOR3 numberPos = m_pos + D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_stopLogo->SetPos(numberPos);
		m_stopLogo->SetSize(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		m_stopLogo->SetTexture("CHECK");
		m_stopLogo->SetColor(D3DXCOLOR(1.0f, 0.2f, 0.0f, 1.0f));
	}


	for(int i = 0; i < 4;i++)
	{
		D3DXVECTOR3 pos = m_pos + D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_side[i] = CObject2d::Create(CTaskGroup::LEVEL_2D_1);
		m_side[i]->SetPos(pos);
		m_side[i]->SetSize(D3DXVECTOR3(7.5f, 7.5f, 0.0f));
		m_side[i]->SetColor(D3DXCOLOR(1.0f, 0.2f, 0.0f, 1.0f));
	}

	// ��莞�Ԃŏ�������悤�ɂ���
	CDelayProcess::DelayProcess(START_TIME, this, [this]()
	{
		SetUpdateStatus(CObject::EUpdateStatus::END);
		m_end = true;
	});

	return S_OK;
}

//-----------------------------------------------------------------------------
// �I��
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
void CClear::Uninit()
{
}

//-----------------------------------------------------------------------------
// �o���X�V
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
void CClear::PopUpdate()
{
	m_stopLogo->AddSize(D3DXVECTOR3(3.5f,3.5f, 0.0f));
	m_side[0]->AddPos(D3DXVECTOR3(0.0f, -5.0f, 0.0f));
	m_side[0]->AddSize(D3DXVECTOR3(5.0f, 0.0f, 0.0f));
	m_side[1]->AddPos(D3DXVECTOR3(5.0f, 0.0f, 0.0f));
	m_side[1]->AddSize(D3DXVECTOR3(0.0f, 5.0f, 0.0f));
	m_side[2]->AddPos(D3DXVECTOR3(0.0f, 5.0f, 0.0f));
	m_side[2]->AddSize(D3DXVECTOR3(5.0f, 0.0f, 0.0f));
	m_side[3]->AddPos(D3DXVECTOR3(-5.0f, 0.0f, 0.0f));
	m_side[3]->AddSize(D3DXVECTOR3(0.0f, 5.0f, 0.0f));

	if (m_side[0]->GetSize().x >= 157.5f)
	{
		SetUpdateStatus(CObject::EUpdateStatus::NORMAL);
	}
}

//-----------------------------------------------------------------------------
// �ʏ�X�V
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
void CClear::NormalUpdate()
{
}

//-----------------------------------------------------------------------------
// �I���X�V
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
void CClear::EndUpdate()
{
	m_stopLogo->AddSize(D3DXVECTOR3(-7.0f, -7.0f, 0.0f));
	m_stopLogo->AddColorAlpha(-0.07f);
	m_side[0]->AddPos(D3DXVECTOR3(0.0f, 10.0f, 0.0f));
	m_side[0]->AddSize(D3DXVECTOR3(-10.0f, 0.0f, 0.0f));
	m_side[0]->AddColorAlpha(-0.07f);
	m_side[1]->AddPos(D3DXVECTOR3(-10.0f, 0.0f, 0.0f));
	m_side[1]->AddSize(D3DXVECTOR3(0.0f, -10.0f, 0.0f));
	m_side[1]->AddColorAlpha(-0.07f);
	m_side[2]->AddPos(D3DXVECTOR3(0.0f, -10.0f, 0.0f));
	m_side[2]->AddSize(D3DXVECTOR3(-10.0f, 0.0f, 0.0f));
	m_side[2]->AddColorAlpha(-0.07f);
	m_side[3]->AddPos(D3DXVECTOR3(10.0f, 0.0f, 0.0f));
	m_side[3]->AddSize(D3DXVECTOR3(0.0f, -10.0f, 0.0f));
	m_side[3]->AddColorAlpha(-0.07f);

	if (m_side[0]->GetSize().x <= 7.5f)
	{
		if (m_stopLogo != nullptr)
		{
			m_stopLogo->Release();
			m_stopLogo = nullptr;
		}

		for (int i = 0; i < 4; i++)
		{
			if (m_side[i] != nullptr)
			{
				m_side[i]->Release();
				m_side[i] = nullptr;
			}
		}
		Release();
	}
}

//-----------------------------------------------------------------------------
// �쐬
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
CClear* CClear::Create(const D3DXVECTOR3 & inPos)
{
	CClear* object = new CClear;

	if (object != nullptr)
	{
		object->SetPos(inPos);
		object->Init();
	}

	return object;
}
