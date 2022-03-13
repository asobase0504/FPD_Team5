//=========================================
// 
// ゲーム内処理
// Author YudaKaito
// 
//=========================================
//------------------------------------
// include
//------------------------------------
#include "main.h"
#include "game.h"
#include "player.h"
#include "input.h"
#include "sound.h"
#include "stage.h"
#include <stdio.h>
#include "wall.h"
#include "disk.h"
#include "shadow.h"
//#include "effect.h"

//------------------------------------
// マクロ定義
//------------------------------------

//------------------------------------
// スタティック変数
//------------------------------------

//=========================================
// 初期化
//=========================================
void InitGame(void)
{
	InitShadow();	// 影
	InitWall();		// 壁
	InitDisk();		// ディスク
	InitPlayer();	// プレイヤー
	InitStage();	// ステージ
	//InitEffect();
}

//=========================================
// 終了
//=========================================
void UninitGame(void)
{
	UninitShadow();	// 影
	UninitPlayer();	// プレイヤー
	UninitWall();	// 壁
	UninitDisk();	// ディスク
	UninitStage();	// ステージ
//	UninitEffect();
}

//=========================================
// 更新
//=========================================
void UpdateGame(void)
{
	UpdateStage();	// ステージ
	UpdatePlayer();	// プレイヤー
	UpdateWall();	// 壁
	UpdateDisk();	// ディスク
	UpdateShadow();	// 影
//	UpdateEffect();
}

//=========================================
// 描画
//=========================================
void DrawGame()
{
	DrawStage();	// ステージ
	DrawWall();		// 壁
	DrawShadow();	// 影
	DrawPlayer();	// プレイヤー
	DrawDisk();		// ディスク
//	DrawEffect();
}