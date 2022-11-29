//=============================================================================
//
// LIFE
// Author : 浜田琉雅
//
//=============================================================================


#ifndef _LIFE_H_			// このマクロ定義がされてなかったら
#define _LIFE_H_			// 二重インクルード防止のマクロ定義

#include "renderer.h"
#include "object2d.h"
#include "texture.h"

class CLife : public CObject2d
{
private:


public:
	static CLife *Create(const D3DXVECTOR3 & pos, float Life);

	CLife(const int list);
	~CLife() override;
	HRESULT Init() override;
	void Uninit() override;
	void NormalUpdate() override;
	void Draw() override;
	void SetPos(const D3DXVECTOR3 &pos) override;
	void SetMove(const D3DXVECTOR3 &move)override;
	
	void SetDamage(const int Damage);
	static bool GetMaxLife() { return m_MaxHp; };

private:
	float m_Life;
	float m_MaxLife;
	static bool m_MaxHp;
	D3DXVECTOR3 m_SetUpPos;
	CObject2d *m_object2d[1];

};

#endif

