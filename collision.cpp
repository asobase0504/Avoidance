//=============================================================================
// 
// 当たり判定
// Author  : Yuda Kaito
// 
//=============================================================================

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "collision.h"
#include "utility.h"

namespace Collision
{
//-----------------------------------------------------------------------------
// 矩形の線分と矩形の線分の当たり判定(全体)
//-----------------------------------------------------------------------------
bool RectangleSegment(const RECTANGLE_DIRECTION direction, const CObject * object1, const CObject * object2, D3DXVECTOR3 * outPos, float * t1, float * t2)
{
	switch (direction)
	{
	case RECTANGLE_DIRECTION::TOP:
		return SegmentAndRectangleTop(object1, object2, outPos, t1, t2);
		break;
	case RECTANGLE_DIRECTION::DOWN:
		return SegmentAndRectangleDown(object1, object2, outPos, t1, t2);
		break;
	case RECTANGLE_DIRECTION::LEFT:
		return SegmentAndRectangleLeft(object1, object2, outPos, t1, t2);
		break;
	case RECTANGLE_DIRECTION::RIGHT:
		return SegmentAndRectangleRight(object1, object2, outPos, t1, t2);
		break;
	default:
		break;
	}
	return false;
}

//-----------------------------------------------------------------------------
// 矩形の線分と矩形の線分の当たり判定(全体)
//-----------------------------------------------------------------------------
bool RectangleSegment(const RECTANGLE_DIRECTION direction, const D3DXVECTOR3 & pos1, const D3DXVECTOR3 & size1, const D3DXVECTOR3 & pos2, const D3DXVECTOR3 & size2, D3DXVECTOR3* outPos, float* t1, float* t2)
{
	switch (direction)
	{
	case RECTANGLE_DIRECTION::TOP:
		return SegmentAndRectangleTop(pos1, size1, pos2, size2, outPos, t1, t2);
		break;
	case RECTANGLE_DIRECTION::DOWN:
		return SegmentAndRectangleDown(pos1, size1, pos2, size2, outPos, t1, t2);
		break;
	case RECTANGLE_DIRECTION::LEFT:
		return SegmentAndRectangleLeft(pos1, size1, pos2, size2, outPos, t1, t2);
		break;
	case RECTANGLE_DIRECTION::RIGHT:
		return SegmentAndRectangleRight(pos1, size1, pos2, size2, outPos, t1, t2);
		break;
	default:
		break;
	}
	return false;
}

//-----------------------------------------------------------------------------
// 矩形と矩形の当たり判定(全体)
//-----------------------------------------------------------------------------
bool RectangleAndRectangle(const D3DXVECTOR3 & pos1, const D3DXVECTOR3 & size1, const D3DXVECTOR3 & pos2, const D3DXVECTOR3 & size2)
{
	/* 自身の判定用 */
	float fLeft = pos1.x - (size1.x * 0.5f);			// 左端
	float fRight = pos1.x + (size1.x * 0.5f);			// 右端
	float fTop = pos1.y + (size1.y * 0.5f);				// 上端
	float fBottom = pos1.y - (size1.y * 0.5f);			// 下端

	/* 対象の判定用 */
	float fLeftTarget = pos2.x - (size2.x * 0.5f);		// 左端
	float fRightTarget = pos2.x + (size2.x * 0.5f);		// 右端
	float fTopTarget = pos2.y + (size2.y * 0.5f);		// 上端
	float fBottomTarget = pos2.y - (size2.y * 0.5f);	// 下端

	// 当たり判定
	bool isHitLeft = fLeft < fRightTarget;
	bool isHitRigth = fRight > fLeftTarget;
	bool isHitTop = fTop > fBottomTarget;
	bool isHitBottom = fBottom < fTopTarget;

	if (isHitLeft || isHitRigth || isHitTop || isHitBottom)
	{//「自身」が「対象の範囲内」に来た場合
		return true;	//「当たった」を返す
	}

	return false;	//「当たっていない」を返す
}

//-----------------------------------------------------------------------------
// 矩形の上側と矩形の当たり判定
//-----------------------------------------------------------------------------
bool SegmentAndRectangleTop(const CObject* object1, const CObject* object2, D3DXVECTOR3 * outPos, float * t1, float * t2)
{
	D3DXVECTOR3 start1 = object1->GetPos();
	D3DXVECTOR3 size1 = object1->GetSize();
	start1.x += -size1.x;
	start1.y += -size1.y;
	D3DXVECTOR3 vec1 = object1->GetPos();
	vec1.x += size1.x;
	vec1.y += -size1.y;
	vec1 -= start1;

	{
		D3DXVECTOR3 start2 = object2->GetPos();
		D3DXVECTOR3 size2 = object2->GetSize();
		start2.x += size2.x;
		start2.y += -size2.y;
		D3DXVECTOR3 vec2 = object2->GetPos();
		vec2.x += size2.x;
		vec2.y += size2.y;
		vec2 -= start2;

		if (SegmentAndSegment(start1, vec1, start2, vec2, outPos, t1, t2))
		{
			return true;
		}
	}

	{
		D3DXVECTOR3 start2 = object2->GetPos();
		D3DXVECTOR3 size2 = object2->GetSize();
		start2.x += -size2.x;
		start2.y += -size2.y;
		D3DXVECTOR3 vec2 = object2->GetPos();
		vec2.x += -size2.x;
		vec2.y += size2.y;
		vec2 -= start2;

		if (SegmentAndSegment(start1, vec1, start2, vec2, outPos, t1, t2))
		{
			return true;
		}
	}

	return false;
}

//-----------------------------------------------------------------------------
// 矩形の上側と矩形の当たり判定
//-----------------------------------------------------------------------------
bool SegmentAndRectangleTop(const D3DXVECTOR3& pos1, const D3DXVECTOR3& size1, const D3DXVECTOR3& pos2, const D3DXVECTOR3& size2, D3DXVECTOR3* outPos, float* t1, float* t2)
{
	D3DXVECTOR3 start1 = pos1;
	start1.x += -size1.x;
	start1.y += -size1.y;
	D3DXVECTOR3 vec1 = pos1;
	vec1.x += size1.x;
	vec1.y += -size1.y;
	vec1 -= start1;

	{
		D3DXVECTOR3 start2 = pos2;
		start2.x += size2.x;
		start2.y += -size2.y;
		D3DXVECTOR3 vec2 = pos2;
		vec2.x += size2.x;
		vec2.y += size2.y;
		vec2 -= start2;

		if (SegmentAndSegment(start1, vec1, start2, vec2, outPos, t1, t2))
		{
			return true;
		}
	}

	{
		D3DXVECTOR3 start2 = pos2;
		start2.x += -size2.x;
		start2.y += -size2.y;
		D3DXVECTOR3 vec2 = pos2;
		vec2.x += -size2.x;
		vec2.y += size2.y;
		vec2 -= start2;

		if (SegmentAndSegment(start1, vec1, start2, vec2, outPos, t1, t2))
		{
			return true;
		}
	}

	return false;
}

//-----------------------------------------------------------------------------
// 矩形の下側と矩形の当たり判定
//-----------------------------------------------------------------------------
bool SegmentAndRectangleDown(const CObject * object1, const CObject * object2, D3DXVECTOR3 * outPos, float * t1, float * t2)
{
	D3DXVECTOR3 start1 = object1->GetPos();
	D3DXVECTOR3 size1 = object1->GetSize();
	start1.x += -size1.x;
	start1.y += size1.y;
	D3DXVECTOR3 vec1 = object1->GetPos();
	vec1.x += size1.x;
	vec1.y += size1.y;
	vec1 -= start1;

	{
		D3DXVECTOR3 start2 = object2->GetPos();
		D3DXVECTOR3 size2 = object2->GetSize();
		start2.x += size2.x;
		start2.y += -size2.y;
		D3DXVECTOR3 vec2 = object2->GetPos();
		vec2.x += size2.x;
		vec2.y += size2.y;
		vec2 -= start2;

		if (SegmentAndSegment(start1, vec1, start2, vec2, outPos, t1, t2))
		{
			return true;
		}
	}

	{
		D3DXVECTOR3 start2 = object2->GetPos();
		D3DXVECTOR3 size2 = object2->GetSize();
		start2.x += -size2.x;
		start2.y += -size2.y;
		D3DXVECTOR3 vec2 = object2->GetPos();
		vec2.x += -size2.x;
		vec2.y += size2.y;
		vec2 -= start2;

		if (SegmentAndSegment(start1, vec1, start2, vec2, outPos, t1, t2))
		{
			return true;
		}
	}

	return false;
}

//-----------------------------------------------------------------------------
// 矩形の下側と矩形の当たり判定
//-----------------------------------------------------------------------------
bool SegmentAndRectangleDown(const D3DXVECTOR3& pos1, const D3DXVECTOR3& size1, const D3DXVECTOR3& pos2, const D3DXVECTOR3& size2, D3DXVECTOR3* outPos, float* t1, float* t2)
{
	D3DXVECTOR3 start1 = pos1;
	start1.x += -size1.x;
	start1.y += size1.y;
	D3DXVECTOR3 vec1 = pos1;
	vec1.x += size1.x;
	vec1.y += size1.y;
	vec1 -= start1;

	{
		D3DXVECTOR3 start2 = pos2;
		start2.x += size2.x;
		start2.y += -size2.y;
		D3DXVECTOR3 vec2 = pos2;
		vec2.x += size2.x;
		vec2.y += size2.y;
		vec2 -= start2;

		if (SegmentAndSegment(start1, vec1, start2, vec2, outPos, t1, t2))
		{
			return true;
		}
	}

	{
		D3DXVECTOR3 start2 = pos2;
		start2.x += -size2.x;
		start2.y += -size2.y;
		D3DXVECTOR3 vec2 = pos2;
		vec2.x += -size2.x;
		vec2.y += size2.y;
		vec2 -= start2;

		if (SegmentAndSegment(start1, vec1, start2, vec2, outPos, t1, t2))
		{
			return true;
		}
	}

	return false;
}

//-----------------------------------------------------------------------------
// 矩形の左側と矩形の当たり判定
//-----------------------------------------------------------------------------
bool SegmentAndRectangleLeft(const CObject * object1, const CObject * object2, D3DXVECTOR3 * outPos, float * t1, float * t2)
{
	D3DXVECTOR3 start1 = object1->GetPos();
	D3DXVECTOR3 size1 = object1->GetSize();
	start1.x += -object1->GetSize().x;
	start1.y += -object1->GetSize().y;
	D3DXVECTOR3 vec1 = object1->GetPos();
	vec1.x += -size1.x;
	vec1.y += size1.y;
	vec1 -= start1;

	{
		D3DXVECTOR3 start2 = object2->GetPos();
		D3DXVECTOR3 size2 = object2->GetSize();
		start2.x += -size2.x;
		start2.y += size2.y;
		D3DXVECTOR3 vec2 = object2->GetPos();
		vec2.x += size2.x;
		vec2.y += size2.y;
		vec2 -= start2;

		if (SegmentAndSegment(start1, vec1, start2, vec2, outPos, t1, t2))
		{
			return true;
		}
	}

	{
		D3DXVECTOR3 start2 = object2->GetPos();
		D3DXVECTOR3 size2 = object2->GetSize();
		start2.x += -size2.x;
		start2.y += -size2.y;
		D3DXVECTOR3 vec2 = object2->GetPos();
		vec2.x += size2.x;
		vec2.y += -size2.y;
		vec2 -= start2;

		if (SegmentAndSegment(start1, vec1, start2, vec2, outPos, t1, t2))
		{
			return true;
		}
	}

	return false;
}

//-----------------------------------------------------------------------------
// 矩形の左側と矩形の当たり判定
//-----------------------------------------------------------------------------
bool SegmentAndRectangleLeft(const D3DXVECTOR3& pos1, const D3DXVECTOR3& size1, const D3DXVECTOR3& pos2, const D3DXVECTOR3& size2, D3DXVECTOR3* outPos, float* t1, float* t2)
{
	D3DXVECTOR3 start1 = pos1;
	start1.x += -size1.x;
	start1.y += -size1.y;
	D3DXVECTOR3 vec1 = pos1;
	vec1.x += -size1.x;
	vec1.y += size1.y;
	vec1 -= start1;

	{
		D3DXVECTOR3 start2 = pos2;
		start2.x += -size2.x;
		start2.y += size2.y;
		D3DXVECTOR3 vec2 = pos2;
		vec2.x += size2.x;
		vec2.y += size2.y;
		vec2 -= start2;

		if (SegmentAndSegment(start1, vec1, start2, vec2, outPos, t1, t2))
		{
			return true;
		}
	}

	{
		D3DXVECTOR3 start2 = pos2;
		start2.x += -size2.x;
		start2.y += -size2.y;
		D3DXVECTOR3 vec2 = pos2;
		vec2.x += size2.x;
		vec2.y += -size2.y;
		vec2 -= start2;

		if (SegmentAndSegment(start1, vec1, start2, vec2, outPos, t1, t2))
		{
			return true;
		}
	}

	return false;
}

//-----------------------------------------------------------------------------
// 矩形の右側と矩形の当たり判定
//-----------------------------------------------------------------------------
bool SegmentAndRectangleRight(const CObject * object1, const CObject * object2, D3DXVECTOR3 * outPos, float * t1, float * t2)
{
	D3DXVECTOR3 start1 = object1->GetPos();
	D3DXVECTOR3 size1 = object1->GetSize();
	start1.x += size1.x;
	start1.y += -size1.y;
	D3DXVECTOR3 vec1 = object1->GetPos();
	vec1.x += size1.x;
	vec1.y += size1.y;
	vec1 -= start1;

	{
		D3DXVECTOR3 start2 = object2->GetPos();
		D3DXVECTOR3 size2 = object2->GetSize();
		start2.x += -size2.x;
		start2.y += size2.y;
		D3DXVECTOR3 vec2 = object2->GetPos();
		vec2.x += size2.x;
		vec2.y += size2.y;
		vec2 -= start2;

		if (SegmentAndSegment(start1, vec1, start2, vec2, outPos, t1, t2))
		{
			return true;
		}
	}

	{
		D3DXVECTOR3 start2 = object2->GetPos();
		D3DXVECTOR3 size2 = object2->GetSize();
		start2.x += -size2.x;
		start2.y += -size2.y;
		D3DXVECTOR3 vec2 = object2->GetPos();
		vec2.x += size2.x;
		vec2.y += -size2.y;
		vec2 -= start2;

		if (SegmentAndSegment(start1, vec1, start2, vec2, outPos, t1, t2))
		{
			return true;
		}
	}

	return false;
}

//-----------------------------------------------------------------------------
// 矩形の右側と矩形の当たり判定
//-----------------------------------------------------------------------------
bool SegmentAndRectangleRight(const D3DXVECTOR3& pos1, const D3DXVECTOR3& size1, const D3DXVECTOR3& pos2, const D3DXVECTOR3& size2, D3DXVECTOR3* outPos, float* t1, float* t2)
{
	D3DXVECTOR3 start1 = pos1;
	start1.x += size1.x;
	start1.y += -size1.y;
	D3DXVECTOR3 vec1 = pos1;
	vec1.x += size1.x;
	vec1.y += size1.y;
	vec1 -= start1;

	{
		D3DXVECTOR3 start2 = pos2;
		start2.x += -size2.x;
		start2.y += size2.y;
		D3DXVECTOR3 vec2 = pos2;
		vec2.x += size2.x;
		vec2.y += size2.y;
		vec2 -= start2;

		if (SegmentAndSegment(start1, vec1, start2, vec2, outPos, t1, t2))
		{
			return true;
		}
	}

	{
		D3DXVECTOR3 start2 = pos2;
		start2.x += -size2.x;
		start2.y += -size2.y;
		D3DXVECTOR3 vec2 = pos2;
		vec2.x += size2.x;
		vec2.y += -size2.y;
		vec2 -= start2;

		if (SegmentAndSegment(start1, vec1, start2, vec2, outPos, t1, t2))
		{
			return true;
		}
	}

	return false;
}

//-----------------------------------------------------------------------------
// 点と矩形の当たり判定
//-----------------------------------------------------------------------------
bool PointAndRectangle(const D3DXVECTOR3 & pointPos, const D3DXVECTOR3 & rectanglePos, const D3DXVECTOR3 & rectangleSize)
{
	bool isHitLeft = rectanglePos.x - rectangleSize.x > pointPos.x;
	if (isHitLeft)
	{
		return false;
	}

	bool isHitRight = rectanglePos.x + rectangleSize.x < pointPos.x;
	if (isHitRight)
	{
		return false;
	}

	bool isHitDown = rectanglePos.y - rectangleSize.y > pointPos.y;
	if (isHitDown)
	{
		return false;
	}

	bool isHitTop = rectanglePos.y + rectangleSize.y < pointPos.y;
	if (isHitTop)
	{
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------
// 線分と線分の当たり判定
//-----------------------------------------------------------------------------
bool SegmentAndSegment(const D3DXVECTOR3& start1, const D3DXVECTOR3& vec1, const D3DXVECTOR3& start2, const D3DXVECTOR3& vec2, D3DXVECTOR3* outPos, float* t1, float* t2)
{
	// ベクトルの始点同士の距離。
	D3DXVECTOR3 v = start2 - start1;

	D3DXVECTOR3 segment1 = vec1;
	D3DXVECTOR3 segment2 = vec2;

	// ブロックのベクトルと被対象のベクトルが平行か調べる
	float Bv_Tv = Vec2Cross(&segment1, &segment2);
	if (Bv_Tv == 0.0f)
	{
		return false;	// 並行である。
	}

	float v_Bv = Vec2Cross(&v, &segment1);
	float v_Tv = Vec2Cross(&v, &segment2);

	float hit1 = v_Tv / Bv_Tv;
	float hit2 = v_Bv / Bv_Tv;

	if ((hit1 < 0.0f) || (hit1 > 1.0f) || (hit2 < 0.0f) || (hit2 > 1.0f))
	{
		return false;
	}

	if (t1 != NULL)
	{
		*t1 = hit1;
	}

	if (t2 != NULL)
	{
		*t2 = hit2;
	}

	if (outPos != NULL)
	{
		*outPos = start1 + vec1 * hit1;
	}
	return true;
}

//-----------------------------------------------------------------------------
// 2D状の円同士の当たり判定
//-----------------------------------------------------------------------------
bool Circle2DAndCircle2D(const D3DXVECTOR3& Pos1, float fRadius1, const D3DXVECTOR3& Pos2, float fRadius2)
{
	float fCalculationX = Pos1.x - Pos2.x;	// Xの差分
	float fCalculationY = Pos1.y - Pos2.y;	// Yの差分

	// 現在の２点の距離
	float fLength = sqrtf(fCalculationX * fCalculationX + fCalculationY * fCalculationY);

	// 半径同士の和
	float fDiff = fRadius1 + fRadius2;

	// ２この物体の半径同士の和より現在の２点の距離が小さいかどうか
	if (fDiff >= fLength)
	{
		return true;	//当たった
	}

	return false;	//当たってない
}

}