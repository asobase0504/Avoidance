//=============================================================================
//
// 入力処理 [inputkeydata.h]
// Author : KOZUNA HIROHITO
// 
//=============================================================================

#ifndef _INPUT_KEY_DATA_H_		//このマクロ定義がされなかったら
#define _INPUT_KEY_DATA_H_		//2重インクルード防止のマクロ定義

//----------------------------------------------------------------------------
//列挙型定義
//----------------------------------------------------------------------------

//総合キー一覧
enum STAN_DART_INPUT_KEY
{						//					キーボード						ジョイパッド
	KEY_UP = 0,			//上			 W & UP							スティック上　＆　十字キー上
	KEY_DOWN,			//下			 S  & Down						スティック下　＆　十字キー下
	KEY_LEFT,			//左			 A  &  Left						スティック左　＆　十字キー左
	KEY_RIGHT,			//右			 D  &  Right					スティック右　＆　十字キー右
	KEY_MOVE_UP,		//上(移動)		 W & UP							スティック上　＆　十字キー上
	KEY_MOVE_DOWN,		//下(移動)		 W & UP							スティック上　＆　十字キー上
	KEY_MOVE_LEFT,		//左(移動)		 W & UP							スティック上　＆　十字キー上
	KEY_MOVE_RIGHT,		//右(移動)		 W & UP							スティック上　＆　十字キー上
	KEY_SHOT_UP,		//上(射撃)		 W & UP							スティック上　＆　十字キー上
	KEY_SHOT_DOWN,		//下(射撃)		 W & UP							スティック上　＆　十字キー上
	KEY_SHOT_LEFT,		//左(射撃)		 W & UP							スティック上　＆　十字キー上
	KEY_SHOT_RIGHT,		//右(射撃)		 W & UP							スティック上　＆　十字キー上
	KEY_DECISION,		//決定			 エンターキー								A
	KEY_SHOT,			//スペース		 スペースキー								R1
	KEY_BACK,			//バック		 バックスペースキー & B						B
	KEY_SHIFT,			//シフト		 シフト左右									L1
	KEY_MOVE,			//移動系すべて
	KEY_PAUSE,			//ポーズボタン	 P											START
	KEY_MAX
};

//キー種類(ジョイパッド十字キーのみ)
enum JOYKEY_DIRECT_CROSS
{
	JOYKEY_CROSS_UP = 0,
	JOYKEY_CROSS_UP_RIGHT = 4500,
	JOYKEY_CROSS_RIGHT = 9000,
	JOYKEY_CROSS_DOWN_RIGHT = 13500,
	JOYKEY_CROSS_DOWN = 18000,
	JOYKEY_CROSS_DOWN_LEFT = 22500,
	JOYKEY_CROSS_LEFT = 27000,
	JOYKEY_CROSS_UP_LEFT = 31500,
	JOYKEY_CROSS_MAX
};

//キー種類(ジョイパッド)
enum DirectJoypad
{
	JOYPAD_X = 0,	// Xボタン
	JOYPAD_Y,		// Yボタン
	JOYPAD_A,		// Aボタン
	JOYPAD_B,		// Bボタン
	JOYPAD_L1,		// L1ボタン
	JOYPAD_R1,		// R1ボタン
	JOYPAD_L2,		// L2ボタン
	JOYPAD_R2,		// R2ボタン
	JOYPAD_LT,		// 左スティック押し込みボタン
	JOYPAD_RT,		// 右スティック押し込みボタン
	JOYPAD_BACK,	// バックボタン
	JOYPAD_START,	// スタートボタン
	JOYPAD_HOME,	// ホームボタン
	JOYPAD_UP,		// 上
	JOYPAD_DOWN,	// 下
	JOYPAD_LEFT,	// 左
	JOYPAD_RIGHT,	// 右
	JOYPAD_MAX
};
#endif

