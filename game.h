//=============================================================================
//
// �Q�[����ʂ̃w�b�_�[
// Author:Hamada Ryuuga
//
//=============================================================================
#ifndef _GAME_H_		//���̃}�N������`����ĂȂ�������
#define _GAME_H_		//2�d�C���N���[�h�h�~�̃}�N����`

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "mode.h"

//-----------------------------------------------------------------------------
// �O���錾
//-----------------------------------------------------------------------------
class CStage;
class CCountdown;
class CPlayer;

//=============================================================================
// �Q�[���N���X
//=============================================================================
class CGame : public CMode
{
public:
	CGame();
	~CGame();

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;

private:
	void StageClear();
	void PlayerDeath();
	void BackStaging();

private:
	CStage* m_stage;		// ���̃X�e�[�W
	CStage* m_stageNext;	// ���̃X�e�[�W
	CPlayer* m_player;
	CCountdown* m_countdown;// �J�E���g�_�E��
	int m_section;			// 1�ʂ̃X�e�[�W��

	bool m_isDeathStop;
};
#endif