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
class CObject2d;
class CSelect;

//=============================================================================
// �Q�[���N���X
//=============================================================================
class CGame : public CMode
{
private:
	static const int FALL_TIME;
public:
	CGame();
	~CGame();

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;

	void RetryStage();

private:
	void StageClear();
	void NextStage();
	void PlayerDeath();

private:
	CStage* m_stage;		// ���̃X�e�[�W
	CStage* m_stageNext;	// ���̃X�e�[�W
	int m_stageSection;
	std::vector<std::string> m_stagePath;

	CPlayer* m_player;
	CCountdown* m_countdown;	// �J�E���g�_�E��
	int m_section;				// 1�ʂ̃X�e�[�W��

	int m_fallCount;			// �����鎞��

	bool m_isDeathStop;

	CSelect* m_nextText;
	CSelect* m_retryText;
	CSelect* m_backText;

	CObject2d* m_mouseCursor;
};
#endif