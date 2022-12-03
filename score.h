//============================
//
// �X�R�A�ݒ�w�b�^�[
// Author:hamada ryuuga
//
//============================

#ifndef _SCORE_H_
#define _SCORE_H_

#include "object.h"

#define	MAX_SCORE	(8)

class CNumber;

//�v���g�^�C�v�錾
class CScore : public CObject
{
public:
	CScore();
	~CScore();
	HRESULT Init() override;
	void Uninit() override;
	void NormalUpdate() override;
	void Draw() override;
	void Set(int nScore);
	void Add(int nValue);

	static CScore*Create(const D3DXVECTOR3 pos);
	void SetPos(const D3DXVECTOR3 &pos) { m_posScore = pos; };
	const D3DXVECTOR3 *GetPos() const { return &m_posScore; };
	static int& GetScore();

private:

	 CNumber* m_pNumber[MAX_SCORE];
	 static int m_nScore;
	 D3DXVECTOR3 m_posScore; //�X�R�A�̈ʒu
};




#endif 