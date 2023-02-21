//=============================================================================
//
// ステージ編集画面
// Author:Hamada Ryuuga
//
//=============================================================================
#ifndef _STAGE_EDIT_H_		//このマクロが定義されてなかったら
#define _STAGE_EDIT_H_		//2重インクルード防止のマクロ定義

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "mode.h"

//-----------------------------------------------------------------------------
// 前方宣言
//-----------------------------------------------------------------------------
class CStage;
class CStageImgui;
class CEditOperator;

//=============================================================================
// ステージ編集
//=============================================================================
class CStageEdit : public CMode
{
public: // 列挙型
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
	CStage* m_stage;						// 今のステージ
	std::vector<std::string> m_stagePath;	// ファイルパス
	int m_stageSectionID;					// 編集中のステージID;

	CStageImgui* imgui;						// ImGui
	CEditOperator* m_operator;				// 操作者

	EEditType m_type;
};
#endif