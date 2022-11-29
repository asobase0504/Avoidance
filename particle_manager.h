//=========================================
// 
// パーティクルエミッタ
// Author YudaKaito
// Author HamadaRyuuga
//=========================================
#ifndef _PARTICLE_MANAGER_H_
#define _PARTICLE_MANAGER_H_

//=========================================
// include
//=========================================
#include <vector>
#include "particle.h"
#include "particle_emitter.h"
#include "task.h"

//=========================================
// 前方宣言
//=========================================
class CParticleEmitter;

//=========================================
// パーティクルの管理
//=========================================
class CParticleManager : public CTask
{
public:	// 列挙型
public:	// 構造体
	struct BundledData
	{
		CParticle::SInfo particleData;
		CParticleEmitter::SInfo emitterData;
	};

public:
	CParticleManager();
	~CParticleManager();

	HRESULT Init();
	void Uninit();
	void Update();
	CParticleEmitter* Create(CParticleEmitter::EObjectType inObject,const D3DXVECTOR3& pos, const int& idx);
	void SetBundledData(const BundledData&inData);
	void ChangeBundledData(const int idx, const BundledData&inData);

	// Getter
	std::vector<BundledData>& GetBundledData() { return m_bundledData; }
	std::list<CParticleEmitter*> GetEmitter() { return m_particleEmitter; }

	// 外部読込み
	void LoadBundledData(const wchar_t* cUrl);

private:
	std::vector<BundledData> m_bundledData;	// 情報体
	std::list<CParticleEmitter*> m_particleEmitter;	// エミッタ―情報
	int m_Index;
};

#endif // !_PARTICLE_FACTORY_H_
