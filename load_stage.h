//=============================================================================
//
// �I�u�W�F�N�gX��ǂݍ���ŕۑ����鏈�� [objectX_group.h]
// Author : Yuda Kaito
//
//=============================================================================
#ifndef _LOAD_STAGE_H_
#define _LOAD_STAGE_H_

//-----------------------------------------------------------------------------
// �O���錾
//-----------------------------------------------------------------------------
class CStage;

std::vector<std::string> LoadPath(const char* filename);
CStage* LoadAll(const char* filename, const D3DXVECTOR3& inPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f));
CStage* LoadAll(std::string filename, const D3DXVECTOR3& inPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f));
void SaveAll(CStage* inStage, const std::string& filename);

#endif