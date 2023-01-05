//=============================================================================
//
// ���͏��� [input.h]
// Author : KOZUNA HIROHITO
// Author2 : YUDA KAITO
// 
//=============================================================================

#ifndef _INPUT_H_		//���̃}�N����`������Ȃ�������
#define _INPUT_H_		//2�d�C���N���[�h�h�~�̃}�N����`

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "inputkeydata.h"
#include "DirectInput.h"
#include "mouse.h"
#include <vector>

//----------------------------------------------------------------------------
//�O���錾
//----------------------------------------------------------------------------
class CInputKeyboard;
class CInputJoyPad;
class CMouse;

//----------------------------------------------------------------------------
//�N���X��`
//----------------------------------------------------------------------------
class CInput
{
public:
	//*�C���v�b�g���K�v�Ȏ��Ăяo��
	static CInput *GetKey() { return m_pInput; }		//�v���C��g���K�[�Ȃǂ̃A�h���X�̎擾
private:
	static CInput *m_pInput;	// ���̃N���X�̏��
public:

	CInput();
	~CInput();

	//*�A�v���P�[�V���������ɏ������
	static CInput *Create();						// ���͏����n�̃N���G�C�g�AInit�̑O�ɏ���
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);	// ���͏����S���̏�����
	void Uninit();									// ���͏����S���̏I������
	void Update();									// ���͏����S���̍X�V����

	// �S�Ẵf�o�C�X
	bool Press(STAN_DART_INPUT_KEY key) { return KeyChackAll(key, 1); }	// �����v���X
	bool Trigger(STAN_DART_INPUT_KEY key) { return KeyChackAll(key, 2); }	// �����g���K�[
	bool Release(STAN_DART_INPUT_KEY key) { return KeyChackAll(key, 3); }	// ���������[�X

	// ���͂��Ă���f�o�C�X���w��
	bool Press(STAN_DART_INPUT_KEY key, int nNum) { return KeyChackNum(key, 1, nNum); }		// �����v���X
	bool Trigger(STAN_DART_INPUT_KEY key, int nNum) { return KeyChackNum(key, 2, nNum); }	// �����g���K�[
	bool Release(STAN_DART_INPUT_KEY key, int nNum) { return KeyChackNum(key, 3, nNum); }	// ���������[�X

	// ���͂��Ă���f�o�C�X��Ԃ�
	std::vector<int> PressDevice(STAN_DART_INPUT_KEY key);		// �����v���X
	std::vector<int> TriggerDevice(STAN_DART_INPUT_KEY key);	// �����g���K�[
	std::vector<int> ReleaseDevice(STAN_DART_INPUT_KEY key);	// ���������[�X

	/* Keyboard */
	bool Press(int nKey);	// �v���X
	bool Trigger(int nkey);	// �g���K�[
	bool Release(int nkey);	// �����[�X

	/* JoyPad */
	bool Press(DirectJoypad key);				// �v���X
	bool Press(DirectJoypad key, int nNum);		// �v���X
	bool Trigger(DirectJoypad key);				// �g���K�[
	bool Trigger(DirectJoypad key, int nNum);	// �g���K�[
	bool Release(DirectJoypad key);				// �����[�X
	bool Release(DirectJoypad key, int nNum);	// �����[�X

	D3DXVECTOR3 VectorMoveKey();	// �\���L�[���̃x�N�g���擾
	D3DXVECTOR3 VectorMoveJoyStick(bool bleftandright,int nNum = 0);	// �W���C�X�e�B�b�N�̃x�N�g���擾

	// �q�����Ă�JoyPad�̐�
	int GetAcceptJoyPadCount();

	/* Mouse */
	CMouse* GetMouse() { return m_mouse; }	// �}�E�X�S�̂̎擾
	bool Press(CMouse::MOUSE_KEY key) { return m_mouse->GetPress(key); }		// �v���X
	bool Trigger(CMouse::MOUSE_KEY key) { return m_mouse->GetTrigger(key); }	// �g���K�[
	bool Release(CMouse::MOUSE_KEY key) { return m_mouse->GetRelease(key); }	// �����[�X

private:
	bool KeyChackAll(STAN_DART_INPUT_KEY key, int type);			// �S�f�o�C�X�̓��͂��m�F
	bool KeyChackNum(STAN_DART_INPUT_KEY key, int type, int nNum);	// �w�肵���f�o�C�X�̓��͂��m�F

private:

	CInputKeyboard* m_pKeyboard;	// �L�[�{�[�h�̏��
	CInputJoyPad* m_pJoyPad;		// �W���C�p�b�h�̏��
	CMouse* m_mouse;				// �}�E�X
};

#endif

