//=========================================
// 
// パーティクルエミッタ
// Author YudaKaito
// Author HamadaRyuuga
//=========================================
#ifndef _PARTICLE_EMITTER_H_
#define _PARTICLE_EMITTER_H_

//=========================================
// include
//=========================================
#include <d3dx9.h>
#include "particle.h"
#include "object.h"

//=========================================
// 前方宣言
//=========================================
class CParticle;

//=========================================
// パーティクルエミッタ
//=========================================
class CParticleEmitter : public CObject
{
public: // 列挙型
	enum EObjectType
	{
		POLIGON2D = 0,
		POLIGON3D,
		BILLBOARD,
		MODEL
	};

public: // 構造体
	struct SInfo
	{
		D3DXVECTOR3 maxPopPos;		// 発生位置(最大)
		D3DXVECTOR3 minPopPos;		// 発生位置(最小)
		float fAngle;				// 発射角度
		float fAddAngle;			// 発射角度を生成時に変化させる量
		int popNumber;				// 一度の出現数
	};
public: // 静的メンバー関数
public: // メンバー関数
	CParticleEmitter();
	~CParticleEmitter();

	HRESULT Init();
	void Uninit();
	void NormalUpdate();

	void PopParticle(void);
	void SetParticle(const CParticle::SInfo& inParticle);
	void SetEmitter(const SInfo& inEmitter) { m_info = inEmitter; }
	CParticle::SInfo* GetParticle() { return &m_particleInfo; }

	void SetObjectType(const EObjectType inType) { m_objectType = inType; }

private: // メンバー変数
	SInfo m_info;					// エミッターが管理する情報一覧
	CParticle::SInfo m_particleInfo;	// このエミッターから出るパーティクルのデータ
	EObjectType m_objectType;		// 出す物体の形式
};

#endif // !_PARTICLE_EMITTER_H_
