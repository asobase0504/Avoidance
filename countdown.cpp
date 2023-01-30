#include "countdown.h"
#include "number.h"
#include "object2d.h"
#include "utility.h"

//-----------------------------------------------------------------------------
// 定数
//-----------------------------------------------------------------------------
const int CCountdown::START_TIME(30 * 8);

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
CCountdown::CCountdown()
{
}

//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
CCountdown::~CCountdown()
{
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
HRESULT CCountdown::Init()
{
	m_count = 0;
	m_end = false;

	{
		m_stopLogo = CObject2d::Create(CTaskGroup::LEVEL_2D_1);
		D3DXVECTOR3 numberPos = m_pos + D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_stopLogo->SetPos(numberPos);
		m_stopLogo->SetSize(D3DXVECTOR3(100.0f, 100.0f, 0.0f));
		m_stopLogo->SetTexture("STOP_LOGO");
	}

	{
		D3DXVECTOR3 numberPos = m_pos + D3DXVECTOR3(0.0f, -200.0f, 0.0f);
		m_sideTrace[0] = CObject2d::Create(CTaskGroup::LEVEL_2D_1);
		m_sideTrace[0]->SetPos(numberPos);
		m_sideTrace[0]->SetSize(D3DXVECTOR3(207.5f, 7.5f, 0.0f));
		m_sideTrace[0]->SetColorAlpha(0.25f);

		m_side[0] = CObject2d::Create(CTaskGroup::LEVEL_2D_1);
		m_side[0]->SetPos(numberPos);
		m_side[0]->SetSize(D3DXVECTOR3(207.5f, 7.5f, 0.0f));
	}
	{
		D3DXVECTOR3 numberPos = m_pos + D3DXVECTOR3(200.0f, 0.0f, 0.0f);

		m_sideTrace[1] = CObject2d::Create(CTaskGroup::LEVEL_2D_1);
		m_sideTrace[1]->SetPos(numberPos);
		m_sideTrace[1]->SetSize(D3DXVECTOR3(7.5f, 207.5f, 0.0f));
		m_sideTrace[1]->SetColorAlpha(0.25f);

		m_side[1] = CObject2d::Create(CTaskGroup::LEVEL_2D_1);
		m_side[1]->SetPos(numberPos);
		m_side[1]->SetSize(D3DXVECTOR3(7.5f, 207.5f, 0.0f));
	}
	{
		D3DXVECTOR3 numberPos = m_pos + D3DXVECTOR3(0.0f, 200.0f, 0.0f);

		m_sideTrace[2] = CObject2d::Create(CTaskGroup::LEVEL_2D_1);
		m_sideTrace[2]->SetPos(numberPos);
		m_sideTrace[2]->SetSize(D3DXVECTOR3(207.5f, 7.5f, 0.0f));
		m_sideTrace[2]->SetColorAlpha(0.25f);

		m_side[2] = CObject2d::Create(CTaskGroup::LEVEL_2D_1);
		m_side[2]->SetPos(numberPos);
		m_side[2]->SetSize(D3DXVECTOR3(207.5f, 7.5f, 0.0f));
	}
	{
		D3DXVECTOR3 numberPos = m_pos + D3DXVECTOR3(-200.0f, 0.0f, 0.0f);
		
		m_sideTrace[3] = CObject2d::Create(CTaskGroup::LEVEL_2D_1);
		m_sideTrace[3]->SetPos(numberPos);
		m_sideTrace[3]->SetSize(D3DXVECTOR3(7.5f, 207.5f, 0.0f));
		m_sideTrace[3]->SetColorAlpha(0.25f);

		m_side[3] = CObject2d::Create(CTaskGroup::LEVEL_2D_1);
		m_side[3]->SetPos(numberPos);
		m_side[3]->SetSize(D3DXVECTOR3(7.5f, 207.5f, 0.0f));
	}

	return E_NOTIMPL;
}

//-----------------------------------------------------------------------------
// 終了
//-----------------------------------------------------------------------------
void CCountdown::Uninit()
{
}

//-----------------------------------------------------------------------------
// 通常更新
//-----------------------------------------------------------------------------
void CCountdown::NormalUpdate()
{
	m_count++;

	//m_number->SetNumber(m_count / 60);

	float size;
	float size2;

	for (int i = 0; i < m_count / 30; i++)
	{
		switch (i)
		{
		case 0:
			if (m_side[i] != nullptr)
			{
				m_side[i]->SetMove(D3DXVECTOR3(0.0f, -3.0f, 0.0f));
				size = m_side[i]->GetSize().x + 2.5f;
				size2 = m_side[i]->GetSize().y - 0.75f;
				m_side[i]->SetSize(D3DXVECTOR3(size, size2, 0.0f));
			}
			break;
		case 1:
			if (m_side[i] != nullptr)
			{
				m_side[i]->SetMove(D3DXVECTOR3(3.0f, 0.0f, 0.0f));
				size = m_side[i]->GetSize().y + 2.5f;
				size2 = m_side[i]->GetSize().x - 0.75f;
				m_side[i]->SetSize(D3DXVECTOR3(size2, size, 0.0f));
			}
			break;
		case 2:
			if (m_side[i] != nullptr)
			{
				m_side[i]->SetMove(D3DXVECTOR3(0.0f, 3.0f, 0.0f));
				size = m_side[i]->GetSize().x + 2.5f;
				size2 = m_side[i]->GetSize().y - 0.75f;
				m_side[i]->SetSize(D3DXVECTOR3(size, size2, 0.0f));
			}
			break;
		case 3:
			if (m_side[i] != nullptr)
			{
				m_side[i]->SetMove(D3DXVECTOR3(-3.0f, 0.0f, 0.0f));
				size = m_side[i]->GetSize().y + 2.5f;
				size2 = m_side[i]->GetSize().x - 0.75f;
				m_side[i]->SetSize(D3DXVECTOR3(size2, size, 0.0f));
			}
			break;
		case 4:
			if (m_stopLogo != nullptr)
			{
				size = m_stopLogo->GetSize().y - 4.25f;
				size2 = m_stopLogo->GetSize().x + 4.25f;
				m_stopLogo->SetSize(D3DXVECTOR3(size2, size, 0.0f));
			}
			break;
		default:
			break;
		}
	}

	if (m_count >= START_TIME - 60)
	{
		m_stopLogo->Release();
		m_stopLogo = nullptr;

		for (int i = 0; i < 4; i++)
		{
			if (m_side[i] != nullptr)
			{
				m_side[i]->Release();
				m_side[i] = nullptr;
			}

			if (m_sideTrace[i] != nullptr)
			{
				m_sideTrace[i]->Release();
				m_sideTrace[i] = nullptr;
			}
		}

		SetUpdateStatus(CObject::EUpdateStatus::END);
		m_end = true;
	}
}

//-----------------------------------------------------------------------------
// 作成
//-----------------------------------------------------------------------------
CCountdown* CCountdown::Create(const D3DXVECTOR3 & inPos)
{
	CCountdown* object = new CCountdown;

	if (object != nullptr)
	{
		object->SetPos(inPos);
		object->Init();
	}

	return object;
}
