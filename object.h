//=============================================================================
//
// オブジェクト
// Author : Hamada Ryuuga
// AUthor : Yuda Kaito
//
//=============================================================================
#ifndef _OBJECT_H_			// このマクロ定義がされてなかったら
#define _OBJECT_H_			// 二重インクルード防止のマクロ定義

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "renderer.h"
#include "task.h"

//=============================================================================
// オブジェクトクラス
//=============================================================================
class CObject : public CTask
{
public:
	enum EType
	{
		ENEMY = 0,
		PLAYER,
		GOAL,
		MODE,
		MODEL,
		PAUSE,
		MAX,
		NONE
	};

	// 更新状態の列挙
	enum class EUpdateStatus
	{
		POP = 0,
		NORMAL,
		END,
		MAX
	};

private:
	void Update() override final;
public:
	CObject(CTaskGroup::EPriority inPriority = CTaskGroup::LEVEL_2D_BG, CTaskGroup::EPushMethod inMethod = CTaskGroup::EPushMethod::PUSH_CURRENT);
	virtual ~CObject() override;

	// 更新
	void SetUpdateStatus(const EUpdateStatus inStatus) { m_updateStatus = inStatus; }

	/* 種類系 */
	void SetType(const EType inType) { m_type = inType; }
	EType GetType() { return m_type; }
	static CObject* SearchType(EType inType, int inPriority);
	CObject* NextSameType();

	/* 位置系 */
	virtual void SetPos(const D3DXVECTOR3 &inPos) { m_pos = inPos; }
	void AddPos(const D3DXVECTOR3 &inPos) { SetPos(m_pos + inPos); }
	void MulPos(const float inRatio) { SetPos(m_pos * inRatio); }
	const D3DXVECTOR3 &GetPos() const { return m_pos; }

	/* 移動系 */
	virtual void SetMove(const D3DXVECTOR3 &inMove) { m_move = inMove; }
	void AddMove(const D3DXVECTOR3 &inMove) { SetMove(m_move + inMove); }
	void MulMove(const float &inRatio) { SetMove(m_move * inRatio); }
	const D3DXVECTOR3 &GetMove() const { return m_move; }

	/* 向き系 */
	virtual void SetRot(const D3DXVECTOR3 &inRot) { m_rot = inRot; }
	void AddRot(const D3DXVECTOR3 &inRot) { SetRot(m_rot + inRot); }
	void MulRot(const float &inRatio) { SetRot(m_rot * inRatio); }
	const D3DXVECTOR3 &GetRot() const { return m_rot; }

	/* 回転系 */
	virtual void SetMoveRot(const D3DXVECTOR3 &inMove) { m_moveRot = inMove; }
	void AddMoveRot(const D3DXVECTOR3 &inMove) { SetMove(m_moveRot + inMove); }
	void MulMoveRot(const float &inRatio) { SetMove(m_moveRot * inRatio); }
	const D3DXVECTOR3 &GetMoveRot() const { return m_moveRot; }

	/* 大きさ系 */
	virtual void SetSize(const D3DXVECTOR3 &inSize) { m_size = inSize; }
	void AddSize(const D3DXVECTOR3 &inSize) { SetSize(m_size + inSize); }
	void MulSize(const float &inRatio) { SetSize(m_size * inRatio); }
	const D3DXVECTOR3 &GetSize() const { return m_size; }

	/* 色系 */
	virtual void SetColor(const D3DXCOLOR &inColor) { m_color = inColor; }
	void AddColor(const D3DXCOLOR &inColor) { SetColor(m_color + inColor); }
	void MulColor(const float &inRatio) { SetColor(m_color * inRatio); }
	const D3DXCOLOR &GetColor() const { return m_color; }
	// 透明色のみ
	virtual void SetColorAlpha(float inAlpha) { SetColor(D3DXCOLOR(m_color.r,m_color.g,m_color.b,inAlpha)); }
	void AddColorAlpha(float inAlpha) { SetColorAlpha(m_color.a + inAlpha); }
	void MulColorAlpha(float inRatio) { SetColorAlpha(m_color.a * inRatio); }
	float GetColorAlpha() const { return m_color.a; }

	/* テスクチャ系 */
	void SetTexture(std::string inKey) { m_textureKey = inKey; }
	std::string GetTexture() { return m_textureKey; }

protected:
	virtual void PopUpdate();
	virtual void NormalUpdate();
	virtual void EndUpdate();

protected:
	D3DXVECTOR3 m_pos;		// 位置
	D3DXVECTOR3 m_move;		// 移動量
	D3DXVECTOR3 m_rot;		// 向き
	D3DXVECTOR3 m_moveRot;	// 回転量
	D3DXVECTOR3 m_size;		// 大きさ
	D3DXCOLOR m_color;		// 色

	EType m_type;	// 種別
	std::string m_textureKey;	// テクスチャにアクセスするキー
	
	EUpdateStatus m_updateStatus;	// 更新状態
	bool m_isPopInNormalUpdate;		// 出現中に通常更新を行うか
	bool m_isEndInNormalUpdate;		// 終了中に通常更新を行うか
};
#endif