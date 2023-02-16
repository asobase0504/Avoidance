//=============================================================================
//
// モデルを潰す影
// Author:Yuda Kaito
//
//=============================================================================
//-----------------------------------------------------------------------------
// include
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
#include "model_shadow.h"
#include "application.h"

//-----------------------------------------------------------------------------
// コンストラクタ
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
CModelShadow::CModelShadow()
{
}

//-----------------------------------------------------------------------------
// デストラクタ
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
CModelShadow::~CModelShadow()
{
}

//-----------------------------------------------------------------------------
// 初期化
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
HRESULT CModelShadow::Init()
{
	for (int nCntMat = 0; nCntMat < (int)m_NumMat; nCntMat++)
	{
		SetMaterialDiffuse(nCntMat, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	}

	return S_OK;
}

//-----------------------------------------------------------------------------
// 終了
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
void CModelShadow::Uninit()
{
}

//-----------------------------------------------------------------------------
// 描画
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
void CModelShadow::Draw()
{
	// 計算用マトリックス
	D3DXMATRIX mtxTrans;

	// ワールドマトリックスの初期化
	// 行列初期化関数(第1引数の行列を単位行列に初期化)
	D3DXMatrixIdentity(&m_mtxWorld);

	// 大きさを反映
	D3DXMatrixScaling(&mtxTrans, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// 行列掛け算関数(第2引数×第3引数第を１引数に格納)
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// 変数宣言
	D3DXMATRIX mtxShadow;
	D3DXPLANE planeField;
	D3DXVECTOR4 vecLight;
	D3DXVECTOR3 pos, normal;

	// シャドウマトリックスの初期化
	D3DXMatrixIdentity(&mtxShadow);

	vecLight = -D3DXVECTOR4(0.2f, -0.5f, 0.3f, 0.0f);

	normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXPlaneFromPointNormal(&planeField, &m_pos, &normal);
	D3DXMatrixShadow(&mtxShadow, &vecLight, &planeField);

	// ワールドマトリックスと掛け合わせる
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxShadow);
}

//-----------------------------------------------------------------------------
// 通常更新
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
void CModelShadow::NormalUpdate()
{
	// Y軸を最低にする
	SetPos(D3DXVECTOR3(0.0f, -FLT_MAX, 0.0f));

	// 全ての地面と当たり判定を行う
	CObject::TypeAllFunc(EType::PLAIN, CTaskGroup::EPriority::LEVEL_3D_1, [this](CObject* inObject)
	{
		CObjectX* plain = (CObjectX*)inObject;

		D3DXVECTOR3 outPos;

		if (plain->RayAndAABB(m_parent->GetPos(), D3DXVECTOR3(0.0f, -1.0f, 0.0f), &outPos))
		{
			outPos.y += 5.0f;

			bool isParentThanOn = m_parent->GetPos().y > outPos.y;	// 親より上
			bool isThisThanDown = GetPos().y < outPos.y;			// 予定されてる位置より上

			if (isParentThanOn && isThisThanDown)
			{
				SetPos(outPos);
			}
		}
	});
}

//-----------------------------------------------------------------------------
// 作成
// Author:Yuda Kaito
//-----------------------------------------------------------------------------
CModelShadow * CModelShadow::Create(CObjectX * inObject)
{
	CModelShadow* shadow = new CModelShadow;

	if (shadow != nullptr)
	{
		shadow->Init();
		shadow->m_parent = inObject;
		shadow->SetMtxRot(shadow->m_parent->GetMtxRot());
		shadow->SetScale(shadow->m_parent->GetScale());
	}

	return shadow;
}
