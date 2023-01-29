//=============================================================================
//
// �G�l�~�[���쐬
// Author:Yuda Kaito
//
//=============================================================================
#ifndef _FACTIRY_ENEMY_H_
#define _FACTIRY_ENEMY_H_

//-----------------------------------------------------------------------------
// �񋓌^
//-----------------------------------------------------------------------------
enum class EEnemyType
{
	LASER,
	HOMING,
	HOMING_BOMB,
	PLAN_PROGRESS,
	REFLECTION,
	ONE_WAY,
	MAX
};

//-----------------------------------------------------------------------------
// �O���錾
//-----------------------------------------------------------------------------
#include "enemy.h"

//-----------------------------------------------------------------------------
// �֐��錾
//-----------------------------------------------------------------------------
CEnemy* CreateEnemy(EEnemyType);
#endif
