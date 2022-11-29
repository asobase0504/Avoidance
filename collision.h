//=============================================================================
//
// 当たり判定
// Author : Yuda Katito
//
//=============================================================================
#ifndef _COLLISION_H_
#define _COLLISION_H_

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "main.h"
#include "object.h"

namespace Collision
{

enum class RECTANGLE_DIRECTION
{
	TOP = 0,
	DOWN,
	LEFT,
	RIGHT
};

// 矩形と矩形
bool RectangleAndRectangle(const D3DXVECTOR3& pos1, const D3DXVECTOR3& size1, const D3DXVECTOR3& pos2, const D3DXVECTOR3& size2);

// 矩形の線分同士
bool RectangleSegment(const RECTANGLE_DIRECTION direction, const CObject* object1, const CObject* object2, D3DXVECTOR3* outPos, float* t1, float* t2);
bool RectangleSegment(const RECTANGLE_DIRECTION direction, const D3DXVECTOR3& pos1, const D3DXVECTOR3& size1, const D3DXVECTOR3& pos2, const D3DXVECTOR3& size2, D3DXVECTOR3* outPos, float* t1, float* t2);

// 線分同士の当たり判定
bool SegmentAndSegment(const D3DXVECTOR3& start1, const D3DXVECTOR3& vec1, const D3DXVECTOR3& start2, const D3DXVECTOR3& vec2, D3DXVECTOR3* OutPos, float* t1, float* t2);

// 矩形上側の線分と矩形
bool SegmentAndRectangleTop(const CObject* object1, const CObject* object2, D3DXVECTOR3* outPos, float* t1, float* t2);
bool SegmentAndRectangleTop(const D3DXVECTOR3& pos1, const D3DXVECTOR3& size1, const D3DXVECTOR3& pos2, const D3DXVECTOR3& size2, D3DXVECTOR3* outPos, float* t1, float* t2);

// 矩形下側の線分と矩形
bool SegmentAndRectangleDown(const CObject* object1, const CObject* object2, D3DXVECTOR3* outPos, float* t1, float* t2);
bool SegmentAndRectangleDown(const D3DXVECTOR3& pos1, const D3DXVECTOR3& size1, const D3DXVECTOR3& pos2, const D3DXVECTOR3& size2, D3DXVECTOR3* outPos, float* t1, float* t2);

// 矩形左側の線分と矩形
bool SegmentAndRectangleLeft(const CObject* object1, const CObject* object2, D3DXVECTOR3* outPos, float* t1, float* t2);
bool SegmentAndRectangleLeft(const D3DXVECTOR3& pos1, const D3DXVECTOR3& size1, const D3DXVECTOR3& pos2, const D3DXVECTOR3& size2, D3DXVECTOR3* outPos, float* t1, float* t2);

// 矩形右側の線分と矩形
bool SegmentAndRectangleRight(const CObject* object1, const CObject* object2, D3DXVECTOR3* outPos, float* t1, float* t2);
bool SegmentAndRectangleRight(const D3DXVECTOR3& pos1, const D3DXVECTOR3& size1, const D3DXVECTOR3& pos2, const D3DXVECTOR3& size2, D3DXVECTOR3* outPos, float* t1, float* t2);

bool PointAndRectangle(const D3DXVECTOR3& pointPos, const D3DXVECTOR3& rectanglePos, const D3DXVECTOR3& rectangleSize);	// 点と矩形
bool Circle2DAndCircle2D(const D3DXVECTOR3& Pos1, float fRadius1, const D3DXVECTOR3& Pos2, float fRadius2);	// 円同士の当たり判定

};

#endif
