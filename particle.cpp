// ＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// effect.cpp
// ＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
#include "particle.h"
#include "main.h"

#include "utility.h"
#include "renderer.h"
#include "application.h"
#include <assert.h>
#include "object.h"

//==================================================
// 静的メンバー変数
//==================================================

//--------------------------------------------------
// コンストラクタ
//--------------------------------------------------
CParticle::CParticle() : 
	m_data({})
{

}

//--------------------------------------------------
// デストラクタ
//--------------------------------------------------
CParticle::~CParticle()
{
}

//--------------------------------------------------
// 初期化
//--------------------------------------------------
HRESULT CParticle::Init()
{
	return S_OK;
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void CParticle::Uninit()
{
	CTask::Uninit();
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void CParticle::Update()
{
	CTask::Update();

	/* ↓使用しているなら↓ */

	// エフェクトの移動

	m_object->AddPos(m_data.move);

	// 推移
	m_data.nLife--;							// 体力の減少
	m_data.move.y += m_data.fWeight;			// 重力
	m_data.move *= m_data.fAttenuation;			// 移動量の推移
	m_data.fWeight += m_data.fWeightTransition;	// 重さの推移
	m_data.move.x -= 0.1f;

	if (m_data.color.bColTransition)
	{// 色の推移
		if (m_data.color.nEndTime >= m_data.color.nCntTransitionTime)
		{
			m_data.color.nCntTransitionTime++;
			m_object->AddColor(m_data.color.colTransition);
			//myColor.a += m_data.color.colTransition.a;
		}
	}

	m_object->AddColorAlpha(-(1.0f / m_data.nMaxLife));
	m_object->SetSize(D3DXVECTOR3(m_data.fWidth, m_data.fWidth,0.0f));

	if (m_data.nLife <= 0)
	{// エフェクトの寿命
		Release();
		m_object->Release();
	}
}

//--------------------------------------------------
// 生成
//--------------------------------------------------
CParticle* CParticle::Create(CObject* inObject,const SInfo& inParticle, const D3DXVECTOR3& inPos)
{
	CParticle* particle = nullptr;
	if (particle == nullptr)
	{
		particle = new CParticle;
		particle->Init();
		
		particle->m_object = inObject;
		particle->m_object->Init();
		particle->m_object->SetSize(D3DXVECTOR3(inParticle.fScale, inParticle.fScale,0.0f));
		particle->m_object->SetPos(inPos);
		particle->m_data = inParticle;
		//particle->SetTexture(particle->m_data.nIdxTex);
		particle->m_object->SetColor(D3DXCOLOR(particle->m_data.color.colBigin.r, particle->m_data.color.colBigin.g, particle->m_data.color.colBigin.b, particle->m_data.color.colBigin.a));
		
		particle->m_object->SetTexture("SMOKE");

		return particle;
	}
	return nullptr;
}

//--------------------------------------------------
// テクスチャの読込み
//--------------------------------------------------
void CParticle::LoadTex()
{

}

//--------------------------------------------------
// float を DWORD に変換
//--------------------------------------------------
DWORD CParticle::FloattoDword(float fVal)
{
	return *((DWORD*)&fVal);
}
