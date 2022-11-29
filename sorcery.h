//============================
//
// 大魔法設定ヘッター
// Author:hamada ryuuga
//
//============================

#ifndef _SORCERY_H_
#define _SORCERY_H_


#include "3dpolygon.h"
#include "player.h"

class CSorcey : public C3dpolygon
{
public:
	CSorcey();
	~CSorcey();

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void SetMove(const D3DXVECTOR3 &move)override;
	static CSorcey *Create(D3DXVECTOR3 pos, CPlayer::NOWMAGIC type);
	void SetAnimation(const int U, const int V);
	void SetSpeed(const int &Speed);
	void PlayAnimation();
	void Move();
	void Collision();
private:

	int m_CounterAnim;
	int m_PatternAnimX;
	int m_PatternAnimY;

	int m_DivisionX;
	int m_DivisionY;
	int m_DivisionMAX;
	int m_AnimationSpeed;
	CPlayer::NOWMAGIC  m_NouPlayer;
	D3DXVECTOR3 m_move;
};

#endif