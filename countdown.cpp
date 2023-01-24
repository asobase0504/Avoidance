#include "countdown.h"
#include "number.h"
#include "object2d.h"

const int CCountdown::START_TIME(60 * 4);

CCountdown::CCountdown()
{
}

CCountdown::~CCountdown()
{
}

HRESULT CCountdown::Init()
{
	m_count = 0;

	{
		m_stopLogo = CObject2d::Create(CTaskGroup::LEVEL_2D_1);
		D3DXVECTOR3 numberPos = m_pos + D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_stopLogo->SetPos(numberPos);
		m_stopLogo->SetSize(D3DXVECTOR3(100.0f, 100.0f, 0.0f));
		m_stopLogo->SetTexture("STOP_LOGO");
	}

	{
		m_side[0] = CObject2d::Create(CTaskGroup::LEVEL_2D_1);
		D3DXVECTOR3 numberPos = m_pos + D3DXVECTOR3(0.0f, 200.0f, 0.0f);
		m_side[0]->SetPos(numberPos);
		m_side[0]->SetSize(D3DXVECTOR3(207.5f, 7.5f, 0.0f));
	}
	{
		m_side[1] = CObject2d::Create(CTaskGroup::LEVEL_2D_1);
		D3DXVECTOR3 numberPos = m_pos + D3DXVECTOR3(200.0f, 0.0f, 0.0f);
		m_side[1]->SetPos(numberPos);
		m_side[1]->SetSize(D3DXVECTOR3(7.5f, 207.5f, 0.0f));
	}
	{
		m_side[2] = CObject2d::Create(CTaskGroup::LEVEL_2D_1);
		D3DXVECTOR3 numberPos = m_pos + D3DXVECTOR3(-200.0f, 0.0f, 0.0f);
		m_side[2]->SetPos(numberPos);
		m_side[2]->SetSize(D3DXVECTOR3(7.5f, 207.5f, 0.0f));
	}
	{
		m_side[3] = CObject2d::Create(CTaskGroup::LEVEL_2D_1);
		D3DXVECTOR3 numberPos = m_pos + D3DXVECTOR3(0.0f, -200.0f, 0.0f);
		m_side[3]->SetPos(numberPos);
		m_side[3]->SetSize(D3DXVECTOR3(207.5f, 7.5f, 0.0f));
	}

	return E_NOTIMPL;
}

void CCountdown::Uninit()
{
}

void CCountdown::NormalUpdate()
{
	m_count++;

	//m_number->SetNumber(m_count / 60);

	if (m_count >= START_TIME - 60)
	{
		m_stopLogo->Release();
	}
}

void CCountdown::Draw()
{
}

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
