//=============================================================================
//
// �}�E�X���͏���(mouse.h)
// Author : �������l
// �T�v : �}�E�X�������s��
//
//=============================================================================

//***************************************************************************
// �C���N���[�h
//***************************************************************************
#include <assert.h>
#include <stdio.h>

#include "mouse.h"
#include "renderer.h" // �X�N���[���̑傫�����擾���邽��

//=============================================================================
// �R���X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�������ɍs������
//=============================================================================
CMouse::CMouse()
{
	m_device = nullptr;
	m_isPosLock = false;
	memset(&m_aKeyState, 0, sizeof(m_aKeyState));
	memset(&m_aKeyStateTrigger, 0, sizeof(m_aKeyStateTrigger));
	memset(&m_aKeyStateRelease, 0, sizeof(m_aKeyStateRelease));
}

//=============================================================================
// �f�X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�I�����ɍs������
//=============================================================================
CMouse::~CMouse()
{

}

//=============================================================================
// ������
// Author : �������l
// �T�v : �}�E�X�̃A�N�Z�X�����l���ł����ꍇ�A�������p������
//=============================================================================
HRESULT CMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	// ���̓f�o�C�X�i�}�E�X�j�̐���
	if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_device, NULL)))
	{
		return E_FAIL;
	}

	//�f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(m_device->SetDataFormat(&c_dfDIMouse2)))
	{
		return E_FAIL;
	}

	// �������[�h��ݒ�
	if (FAILED(m_device->SetCooperativeLevel(hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// �E�B���h�E�n���h���̊i�[
	m_hWnd = hWnd;

	// Device�ւ̃A�N�Z�X�����l��
	m_device->Acquire();
	return S_OK;
}

//=============================================================================
// �I��
// Author : Yuda Kaito
// �T�v : �}�E�X�̃A�N�Z�X����������A�f�o�C�X�ƃI�u�W�F�N�g���������
//=============================================================================
void CMouse::Uninit(void)
{
	//���̓f�o�C�X�̕���
	if (m_device != NULL)
	{
		m_device->Unacquire();		// �A�N�Z�X�������
		m_device->Release();
		m_device = NULL;
	}
}

//=============================================================================
// �X�V
// Author : �������l
// �T�v : �}�E�X���͂��ꂽ����ۑ�����
//=============================================================================
void CMouse::Update(void)
{
	// �ϐ��錾
	DIMOUSESTATE2 aKeyState;		// �}�E�X���͏��
	int nCntKey;

	// ���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(m_device->GetDeviceState(sizeof(aKeyState), &aKeyState)))
	{
		for (nCntKey = 0; nCntKey < MAX_MOUSE_KEY; nCntKey++)
		{
			// �}�E�X�g���K�[����ۑ�
			m_aKeyStateTrigger.rgbButtons[nCntKey] = (m_aKeyState.rgbButtons[nCntKey] ^ aKeyState.rgbButtons[nCntKey]) & aKeyState.rgbButtons[nCntKey];

			// �}�E�X�����[�X����ۑ�
			m_aKeyStateRelease.rgbButtons[nCntKey] = (m_aKeyState.rgbButtons[nCntKey] ^ aKeyState.rgbButtons[nCntKey]) & m_aKeyState.rgbButtons[nCntKey];

			// �}�E�X�v���X����ۑ�
			m_aKeyState = aKeyState;
		}
	}
	else
	{
		// �}�E�X�ւ̃A�N�Z�X�����l��
		m_device->Acquire();
	}

	if (m_isPosLock)
	{
		SetCursorPosLock();
	}
}

//=============================================================================
//  �}�E�X�v���X���̎擾
// Author : �������l
// �T�v : �����̃L�[�̏���Ԃ�
//=============================================================================
bool CMouse::GetPress(MOUSE_KEY nKey)
{
	return (m_aKeyState.rgbButtons[nKey] & 0x80) ? true : false;
}

//=============================================================================
// �}�E�X�g���K�[���̎擾
// Author : �������l
// �T�v : �����̃L�[�̏���Ԃ�
//=============================================================================
bool CMouse::GetTrigger(MOUSE_KEY nKey)
{
	return (m_aKeyStateTrigger.rgbButtons[nKey] & 0x80) ? true : false;
}

//=============================================================================
// �}�E�X�����[�X���̎擾
// Author : �������l
// �T�v : �����̃L�[�̏���Ԃ�
//=============================================================================
bool CMouse::GetRelease(MOUSE_KEY nKey)
{
	return (m_aKeyStateRelease.rgbButtons[nKey] & 0x80) ? true : false;
}

//=============================================================================
// �}�E�X�J�[�\���̈ʒu�̎擾
// Author : �������l
// �T�v : �}�E�X�J�[�\���̈ʒu���擾���A���l��Ԃ�
//=============================================================================
D3DXVECTOR3 CMouse::GetMouseCursorPos()
{
	// �X�N���[�����W��̃}�E�X�J�[�\���̈ʒu�̎擾
	GetCursorPos(&m_mouseCursor);

	// �E�B���h�E��̃}�E�X�J�[�\���̈ʒu�̎擾
	ScreenToClient(m_hWnd, &m_mouseCursor);

	return D3DXVECTOR3((float)m_mouseCursor.x, (float)m_mouseCursor.y, 0.0f);
}

//=============================================================================
// �}�E�X�z�C�[���̈ړ��ʎ擾
// Author : �������l
// �T�v : �}�E�X�z�C�[���̈ړ��ʂ̐��l��Ԃ�
//=============================================================================
int CMouse::GetWheelPower(void)
{
	return (int)m_aKeyState.lZ;
}

//=============================================================================
// �}�E�X�̈ړ��ʎ擾
// Author : �������l
// �T�v : �}�E�X�̈ړ��ʂ̐��l��Ԃ�
//=============================================================================
D3DXVECTOR3 CMouse::GetMouseCursorMove(void)
{
	return D3DXVECTOR3((float)(m_aKeyState.lX), (float)(m_aKeyState.lY), (float)(m_aKeyState.lZ));
}

//=============================================================================
// �J�[�\���̃��b�N
// Author : YudaKaito
// �T�v : �J�[�\���̈ʒu�����b�N����B
//=============================================================================
void CMouse::SetCursorPosLock()
{
	D3DXVECTOR2 pos;

	pos.x = SCREEN_WIDTH / 2;
	pos.y = SCREEN_HEIGHT / 2;

	WINDOWINFO windowInfo;

	//�E�B���h�E�̈ʒu���擾
	windowInfo.cbSize = sizeof(WINDOWINFO);
	GetWindowInfo(m_hWnd, &windowInfo);

	//�}�E�X�̈ړ���̐�΍��W�i���j�^�[���ォ��̍��W�j
	pos.x += windowInfo.rcWindow.left;
	pos.y += windowInfo.rcWindow.top + 35; //�E�B���h�E�̃^�C�g���o�[�̕��i35px�j���v���X

	SetCursorPos((int)pos.x, (int)pos.y);
}
