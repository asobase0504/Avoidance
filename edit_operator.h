//=============================================================================
//
// プレイヤー設定ヘッター
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
private: // 列挙型
private: // 定数
	static const float MAX_SPEED;		// 最大移動量
	static const float SPEED;			// 移動量
	static const float ATTENUATION;		// 減衰係数
	static const float JUMPING_POWER;	// 跳躍力
	static const float GRAVITY;			// 重力

public: // パブリック関数
	CEditOperator();
	~CEditOperator();

	HRESULT Init() override;		// 初期化
	void Uninit() override;			// 破棄
	void NormalUpdate() override;	// 更新
	void Draw() override;			// 描画

	static CEditOperator* Create();	// 生成

	void SetEditType(CStageEdit::EEditType inType) { m_editType = inType; }

private: // プライベート関数
	void Move();		// 移動
private: // メンバー変数
	bool m_isEditEnemy;
	bool m_isEditBlock;

	CStageEdit::EEditType m_editType;

};
#endif
