//=============================================================================
//
// オブジェクトXを読み込んで保存する処理 [objectX_group.h]
// Author : Yuda Kaito
//
//=============================================================================
#ifndef _LOAD_STAGE_H_
#define _LOAD_STAGE_H_

//-----------------------------------------------------------------------------
// 前方宣言
//-----------------------------------------------------------------------------
class CStage;

CStage* LoadAll(const char* filename, const D3DXVECTOR3& inPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f));

#endif