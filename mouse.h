//=============================================================================
//
// �}�E�X���͏���(mouse.h)
// Author : �������l
// �T�v : �}�E�X�������s��
//
//=============================================================================
#ifndef _MOUSE_H_		// ���̃}�N����`������ĂȂ�������
#define _MOUSE_H_		// ��d�C���N���[�h�h�~�̃}�N����`

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "DirectInput.h"

//=============================================================================
// �}�E�X���͏���
// Author : �������l
// �T�v : �}�E�X�������s��
//=============================================================================
class CMouse : public CDirectInput
{
public:
	//--------------------------------------------------------------------
	// �}�E�X�̃L�[���
	// Author : �������l
	// �T�v : �}�E�X�̃L�[���
	//--------------------------------------------------------------------
	enum MOUSE_KEY
	{
		MOUSE_KEY_LEFT = 0,		// �}�E�X���{�^��
		MOUSE_KEY_RIGHT,		// �}�E�X�E�{�^��
		MOUSE_KEY_MIDDLE,		// �}�E�X�����{�^��
		MOUSE_KEY_SIDE_0,		// �}�E�X�T�C�h�{�^��0
		MOUSE_KEY_SIDE_1,		// �}�E�X�T�C�h�{�^��1
		MOUSE_KEY_SIDE_2,		// �}�E�X�T�C�h�{�^��2
		MOUSE_KEY_SIDE_3,		// �}�E�X�T�C�h�{�^��3
		MOUSE_KEY_SIDE_4,		// �}�E�X�T�C�h�{�^��4
		MAX_MOUSE_KEY,			// �}�E�X�̃{�^���̍ő吔
	};

	//--------------------------------------------------------------------
	// �R���X�g���N�^�ƃf�X�g���N�^
	//--------------------------------------------------------------------
	CMouse();
	~CMouse() override;

	//--------------------------------------------------------------------
	// �����o�֐�
	//--------------------------------------------------------------------
	HRESULT Init(HINSTANCE hInstance, HWND hWnd) override;	// ������
	void Uninit(void) override;								// �I��
	void Update(void) override;								// �X�V
	bool GetPress(MOUSE_KEY nKey);							// �v���X���̎擾
	bool GetTrigger(MOUSE_KEY nKey);						// �g���K�[���̎擾
	bool GetRelease(MOUSE_KEY nKey);						// �����[�X���̎擾
	int GetWheelPower(void);								// �}�E�X�̃z�C�[���̓�������

	// �J�[�\��
	D3DXVECTOR3 GetMouseCursorPos(void);					// �X�N���[�����W�̎擾
	D3DXVECTOR3 GetMouseCursorMove(void);					// �ړ��ʂ��o�͏���
	void SetShowCursor(bool bShow) { ShowCursor(bShow); }	// �\���̗L��
	void UseSetPosLock(bool use) { m_isPosLock = use; }
	void SetCursorPosLock();

private:
	//--------------------------------------------------------------------
	// �����o�ϐ�
	//--------------------------------------------------------------------
	LPDIRECTINPUTDEVICE8 m_device;		// ���̓f�o�C�X�ւ̃|�C���^
	DIMOUSESTATE2 m_aKeyState;			// �v���X���
	DIMOUSESTATE2 m_aKeyStateTrigger;	// �g���K�[���
	DIMOUSESTATE2 m_aKeyStateRelease;	// �����[�X���
	POINT m_mouseCursor;				// �}�E�X�J�[�\���̈ʒu
	bool m_isPosLock;
	HWND m_hWnd;						// �E�B���h�E�n���h���i�[����ϐ�
};

#endif
