////=============================================================================
////
//// タイトルロゴ
//// Author : Yuda Kaito
////
////=============================================================================
////-----------------------------------------------------------------------------
//// include
////-----------------------------------------------------------------------------
//#include "application.h"
//#include "title_menu.h"
//#include "utility.h"
//
////-----------------------------------------------------------------------------
//// コンストラクタ
////-----------------------------------------------------------------------------
//CTitleMenu::CTitleMenu(CTaskGroup::EPriority list) :
//	CObject2d(list)
//{
//}
//
////-----------------------------------------------------------------------------
//// デストラクタ
////-----------------------------------------------------------------------------
//CTitleMenu::~CTitleMenu()
//{
//}
//
////-----------------------------------------------------------------------------
//// 初期化
////-----------------------------------------------------------------------------
//HRESULT CTitleMenu::Init()
//{
//	CObject2d::Init();
//	D3DXVECTOR3 pos = CApplication::CENTER_POS;
//	SetPos(pos);
//	SetTexture("TITLE_LOGO");
//	m_count = 0;
//	m_rateSizeX = 0.01f;
//	m_rateSizeY = 0.01f;
//
//	return S_OK;
//}
//
////-----------------------------------------------------------------------------
//// 終了
////-----------------------------------------------------------------------------
//void CTitleMenu::Uninit()
//{
//}
//
////-----------------------------------------------------------------------------
//// 出現中更新
////-----------------------------------------------------------------------------
//void CTitleMenu::PopUpdate()
//{
//	if (m_count < 45)
//	{
//		m_count++;
//		return;
//	}
//
//	m_rateSizeX = ease::OutSine(m_rateSizeX / 0.25f) * 0.25f;
//
//	if (m_rateSizeY < 0.25f)
//	{
//		m_rateSizeY += ease::OutSine(0.075f) * 0.025f;
//	}
//
//	SetSize(D3DXVECTOR3(1376 * m_rateSizeX, 256 * m_rateSizeY, 0.0f));
//
//	if (m_rateSizeY <= 0.25f && m_rateSizeY >= 0.25f)
//	{
//		m_updateStatus = EUpdateStatus::NORMAL;
//	}
//
//	CObject2d::NormalUpdate();
//}
//
////-----------------------------------------------------------------------------
//// 通常中更新
////-----------------------------------------------------------------------------
//void CTitleMenu::NormalUpdate()
//{
//	CObject2d::NormalUpdate();
//}
//
////-----------------------------------------------------------------------------
//// 終了中更新
////-----------------------------------------------------------------------------
//void CTitleMenu::EndUpdate()
//{
//	Release();
//}
