//=============================================================================
//
// �X�e�[�W�ҏW���
// Author:Hamada Ryuuga
//
//=============================================================================
#ifndef _STAGE_EDIT_H_		//���̃}�N������`����ĂȂ�������
#define _STAGE_EDIT_H_		//2�d�C���N���[�h�h�~�̃}�N����`

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "mode.h"

//-----------------------------------------------------------------------------
// �O���錾
//-----------------------------------------------------------------------------
class CStage;
class CStageImgui;
class CEditOperator;

//=============================================================================
// �X�e�[�W�ҏW
//=============================================================================
class CStageEdit : public CMode
{
public: // �񋓌^
	enum EEditType
	{
		NEW_ENEMY = 0,
		NEW_BLOCK,
		RE_GOAL,
		RE_ENEMY,
		RE_BLOCK,
		FREE
	};

public:
	CStageEdit();
	~CStageEdit();

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;

	EEditType GetType() { return m_type; }

private:

private:
	CStage* m_stage;						// ���̃X�e�[�W
	std::vector<std::string> m_stagePath;	// �t�@�C���p�X
	int m_stageSectionID;					// �ҏW���̃X�e�[�WID;

	CStageImgui* imgui;						// ImGui
	CEditOperator* m_operator;				// �����

	EEditType m_type;
};
#endif