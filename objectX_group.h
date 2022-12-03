//=============================================================================
//
// �I�u�W�F�N�gX��ǂݍ���ŕۑ����鏈�� [objectX_group.h]
// Author : Yuda Kaito
//
//=============================================================================
#ifndef _OBJECTX_GROUP_H_
#define _OBJECTX_GROUP_H_

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "main.h"
#include <string>
#include <vector>
#include <map>

//=============================================================================
// �N���X�̒�`
//=============================================================================
class CObjectXGroup
{
private: /* �v���C�x�[�g�萔 */
	static const wchar_t* filePath;

public:

	// ���f���f�[�^
	struct SModelData
	{
		LPD3DXMESH pMesh;
		LPD3DXBUFFER pBuffMat;
		DWORD numMat;
		D3DXVECTOR3 minVtx;
		D3DXVECTOR3 maxVtx;
		D3DXVECTOR3 size;
	};
public:
	//-------------------------------------------------------------------------
	// �R���X�g���N�^�ƃf�X�g���N�^
	//-------------------------------------------------------------------------
	CObjectXGroup();
	~CObjectXGroup();

	//-------------------------------------------------------------------------
	// �����o�[�֐�
	//-------------------------------------------------------------------------
	void LoadAll();		// �S�Ă̓ǂݍ���
	void UnloadAll();	// �S�Ă̔j��
	void Load(std::string inKey, std::string inFileName);	// �w��̓ǂݍ���
	void Load(std::vector<std::string> inModel) { Load(inModel[0], inModel[1]); }			// �w��̓ǂݍ���
	void Unload(std::string inKey);				// �w��̔j��

	// Getter
	LPD3DXMESH GetMesh(std::string inKey);		// ���b�V���̎擾
	LPD3DXBUFFER GetBuffMat(std::string inKey);	// �o�b�t�@�[�̎擾
	DWORD GetNumMat(std::string inKey);			// �}�e���A�����̎擾
	D3DXVECTOR3 GetMinVtx(std::string inKey);	// �ŏ����_�̎擾
	D3DXVECTOR3 GetMaxVtx(std::string inKey);	// �ő咸�_�̎擾
	D3DXVECTOR3 GetSize(std::string inKey);		// �傫���̎擾

private: /* �v���C�x�[�g�֐� */
	bool ExistsKey(std::string inKey) { return m_model.count(inKey) != 0; }	// Map���Ɏw�肳�ꂽKey�����݂��邩�ۂ�

private:
	std::map<std::string, SModelData, std::less<>> m_model;	// ���f���̏��
};
#endif