//=============================================================================
//
// �v���C���[�ݒ�w�b�^�[
// Author:Yuda Kaito
//
//=============================================================================
#ifndef _EDIT_OPERATOR_H_
#define _EDIT_OPERATOR_H_

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "motion.h"
#include "objectX.h"
#include "stage_edit.h"

class CEditOperator : public CObjectX
{
private: // �񋓌^
private: // �萔
	static const float MAX_SPEED;		// �ő�ړ���
	static const float SPEED;			// �ړ���
	static const float ATTENUATION;		// �����W��
	static const float JUMPING_POWER;	// ������
	static const float GRAVITY;			// �d��

public: // �p�u���b�N�֐�
	CEditOperator();
	~CEditOperator();

	HRESULT Init() override;		// ������
	void Uninit() override;			// �j��
	void NormalUpdate() override;	// �X�V
	void Draw() override;			// �`��

	static CEditOperator* Create();	// ����

	void SetEditType(CStageEdit::EEditType inType) { m_editType = inType; }

private: // �v���C�x�[�g�֐�
	void Move();		// �ړ�
private: // �����o�[�ϐ�
	bool m_isEditEnemy;
	bool m_isEditBlock;

	CStageEdit::EEditType m_editType;

};
#endif
