//=============================================================================
//
// カウントダウンUI
// Author:Yuda Kaito
//
//=============================================================================
#include "countdown.h"
#include "number.h"
#include "object2d.h"
#include "utility.h"
#include "delay_process.h"

//-----------------------------------------------------------------------------
// 定数
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
const int CCountdown::START_TIME(30 * 8);

//-----------------------------------------------------------------------------
// コンストラクタ
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
CCountdown::CCountdown()
{
}

//-----------------------------------------------------------------------------
// デストラクタ
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
CCountdown::~CCountdown()
{
}

//-----------------------------------------------------------------------------
// 初期化
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
HRESULT CCountdown::Init()
{
	m_count = 0;
	m_end = false;
	return S_OK;
}

//-----------------------------------------------------------------------------
// 終了
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
void CCountdown::Uninit()
{
}

//-----------------------------------------------------------------------------
// 出現更新
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
void CCountdown::PopUpdate()
{
	if (true)
	{
		{
			D3DXVECTOR3 pos = m_pos + D3DXVECTOR3(0.0f, -150.0f, 0.0f);
			m_sideTrace[0] = CObject2d::Create(CTaskGroup::LEVEL_2D_1);
			m_sideTrace[0]->SetPos(pos);
			m_sideTrace[0]->SetSize(D3DXVECTOR3(157.5f, 7.5f, 0.0f));
			m_sideTrace[0]->SetColor(D3DXCOLOR(0.55f, 0.55f, 0.5f, 1.0f));
		}
		{
			D3DXVECTOR3 pos = m_pos + D3DXVECTOR3(150.0f, 0.0f, 0.0f);
			m_sideTrace[1] = CObject2d::Create(CTaskGroup::LEVEL_2D_1);
			m_sideTrace[1]->SetPos(pos);
			m_sideTrace[1]->SetSize(D3DXVECTOR3(7.5f, 157.5f, 0.0f));
			m_sideTrace[1]->SetColor(D3DXCOLOR(0.55f, 0.55f, 0.5f, 1.0f));
		}
		{
			D3DXVECTOR3 pos = m_pos + D3DXVECTOR3(0.0f, 150.0f, 0.0f);
			m_sideTrace[2] = CObject2d::Create(CTaskGroup::LEVEL_2D_1);
			m_sideTrace[2]->SetPos(pos);
			m_sideTrace[2]->SetSize(D3DXVECTOR3(157.5f, 7.5f, 0.0f));
			m_sideTrace[2]->SetColor(D3DXCOLOR(0.55f, 0.55f, 0.5f, 1.0f));
		}
		{
			D3DXVECTOR3 pos = m_pos + D3DXVECTOR3(-150.0f, 0.0f, 0.0f);
			m_sideTrace[3] = CObject2d::Create(CTaskGroup::LEVEL_2D_1);
			m_sideTrace[3]->SetPos(pos);
			m_sideTrace[3]->SetSize(D3DXVECTOR3(7.5f, 157.5f, 0.0f));
			m_sideTrace[3]->SetColor(D3DXCOLOR(0.55f, 0.55f, 0.5f, 1.0f));
		}

		{
			m_stopLogo = CObject2d::Create(CTaskGroup::LEVEL_2D_1);
			D3DXVECTOR3 numberPos = m_pos + D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			m_stopLogo->SetPos(numberPos);
			m_stopLogo->SetSize(D3DXVECTOR3(100.0f, 100.0f, 0.0f));
			m_stopLogo->SetTexture("STOP_LOGO");
		}

		{
			D3DXVECTOR3 pos = m_pos + D3DXVECTOR3(0.0f, -150.0f, 0.0f);
			m_side[0] = CObject2d::Create(CTaskGroup::LEVEL_2D_1);
			m_side[0]->SetPos(pos);
			m_side[0]->SetSize(D3DXVECTOR3(157.5f, 7.5f, 0.0f));
		}
		{
			D3DXVECTOR3 pos = m_pos + D3DXVECTOR3(150.0f, 0.0f, 0.0f);
			m_side[1] = CObject2d::Create(CTaskGroup::LEVEL_2D_1);
			m_side[1]->SetPos(pos);
			m_side[1]->SetSize(D3DXVECTOR3(7.5f, 157.5f, 0.0f));
		}
		{
			D3DXVECTOR3 pos = m_pos + D3DXVECTOR3(0.0f, 150.0f, 0.0f);
			m_side[2] = CObject2d::Create(CTaskGroup::LEVEL_2D_1);
			m_side[2]->SetPos(pos);
			m_side[2]->SetSize(D3DXVECTOR3(157.5f, 7.5f, 0.0f));
		}
		{
			D3DXVECTOR3 pos = m_pos + D3DXVECTOR3(-150.0f, 0.0f, 0.0f);
			m_side[3] = CObject2d::Create(CTaskGroup::LEVEL_2D_1);
			m_side[3]->SetPos(pos);
			m_side[3]->SetSize(D3DXVECTOR3(7.5f, 157.5f, 0.0f));
		}

		// 一定時間で消去するようにする
		CDelayProcess::DelayProcess(START_TIME - 60, this,[this]()
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

				if (m_sideTrace[i] != nullptr)
				{
					m_sideTrace[i]->Release();
					m_sideTrace[i] = nullptr;
				}
			}

			SetUpdateStatus(CObject::EUpdateStatus::END);
			m_end = true;
		});

		SetUpdateStatus(CObject::EUpdateStatus::NORMAL);
	}
}

//-----------------------------------------------------------------------------
// 通常更新
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
void CCountdown::NormalUpdate()
{
	m_count++;

	D3DXVECTOR3 addSize(sinf(m_count * 0.15f), sinf(m_count * 0.15f), sinf(m_count * 0.15f));

	static bool onoff = true;

	if ((onoff && (m_count % 30 == 0)) || (!onoff && (m_count % 10 == 0)))
	{
		if (onoff)
		{
			addSize = D3DXVECTOR3(5.0f, 5.0f, 5.0f);
		}
		else
		{
			addSize = D3DXVECTOR3(-5.0f, -5.0f, -5.0f);
		}

		onoff = !onoff;

		m_stopLogo->AddSize(addSize);
	}

	for (int i = 0; i < m_count / 30; i++)
	{
		switch (i)
		{
		case 0:
			if (m_side[i] != nullptr)
			{
				m_side[i]->SetMove(D3DXVECTOR3(0.0f, -7.0f, 0.0f));
				m_side[i]->AddSize(D3DXVECTOR3(2.5f, -0.45f, 0.0f));
				m_side[i]->AddColorAlpha(-0.01f);
			}
			break;
		case 1:
			if (m_side[i] != nullptr)
			{
				m_side[i]->SetMove(D3DXVECTOR3(7.0f, 0.0f, 0.0f));
				m_side[i]->AddSize(D3DXVECTOR3(-0.45f, 2.5f, 0.0f));
				m_side[i]->AddColorAlpha(-0.01f);
			}
			break;
		case 2:
			if (m_side[i] != nullptr)
			{
				m_side[i]->SetMove(D3DXVECTOR3(0.0f, 7.0f, 0.0f));
				m_side[i]->AddSize(D3DXVECTOR3(2.5f, -0.45f, 0.0f));
				m_side[i]->AddColorAlpha(-0.01f);
			}
			break;
		case 3:
			if (m_side[i] != nullptr)
			{
				m_side[i]->SetMove(D3DXVECTOR3(-7.0f, 0.0f, 0.0f));
				m_side[i]->AddSize(D3DXVECTOR3(-0.45f, 2.5f, 0.0f));
				m_side[i]->AddColorAlpha(-0.01f);
			}
			break;
		case 4:
			{
				m_sideTrace[0]->SetMove(D3DXVECTOR3(0.0f, -3.5f, 0.0f));
				m_sideTrace[1]->SetMove(D3DXVECTOR3(3.5f, 0.0f, 0.0f));
				m_sideTrace[2]->SetMove(D3DXVECTOR3(0.0f, 3.5f, 0.0f));
				m_sideTrace[3]->SetMove(D3DXVECTOR3(-3.5f, 0.0f, 0.0f));
				m_sideTrace[0]->AddSize(D3DXVECTOR3(3.5f, 0.0f, 0.0f));
				m_sideTrace[1]->AddSize(D3DXVECTOR3(0.0f, 3.5f, 0.0f));
				m_sideTrace[2]->AddSize(D3DXVECTOR3(3.5f, 0.0f, 0.0f));
				m_sideTrace[3]->AddSize(D3DXVECTOR3(0.0f, 3.5f, 0.0f));
				m_sideTrace[0]->AddColorAlpha(-0.01f);
				m_sideTrace[1]->AddColorAlpha(-0.01f);
				m_sideTrace[2]->AddColorAlpha(-0.01f);
				m_sideTrace[3]->AddColorAlpha(-0.01f);
		}
			if (m_stopLogo != nullptr)
			{
				m_stopLogo->AddSize(D3DXVECTOR3(3.5f, 3.5f, 0.0f));
				m_stopLogo->AddColorAlpha(-0.01f);
			}
			break;
		default:
			break;
		}
	}
}

//-----------------------------------------------------------------------------
// 作成
// Author:Yuda Kaito
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
