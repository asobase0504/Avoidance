//=============================================================================
//
// エネミーを作成
// Author:Yuda Kaito
//
//=============================================================================
#include "factory_enemy.h"
#include "enemy.h"
#include "enemy_oneway.h"
#include "enemy_planprogress.h"
#include "enemy_laser.h"
#include "enemy_homing.h"
#include "enemy_homing_bomb.h"
#include "enemy_reflection.h"

//-----------------------------------------------------------------------------
// エネミーの作成
//-----------------------------------------------------------------------------
CEnemy* CreateEnemy(EEnemyType type)
{
	CEnemy* enemy = nullptr;

	switch (type)
	{
	case EEnemyType::ONE_WAY:
		enemy = new CEnemyOneWay;
		break;
	case EEnemyType::PLAN_PROGRESS:
		enemy = new CEnemyPlanProgress;
		break;
	case EEnemyType::LASER:
		enemy = new CEnemyLaser;
		break;
	case EEnemyType::HOMING:
		enemy = new CEnemyHoming;
		break;
	case EEnemyType::HOMING_BOMB:
		enemy = new CEnemyHomingBomb;
		break;
	case EEnemyType::REFLECTION:
		enemy = new CEnemyReflection;
		break;
	case EEnemyType::MAX:
	default:
		assert(false);
		break;
	}

	return enemy;
}
