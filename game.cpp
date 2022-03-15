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
#include"score.h"
#include "time.h"
#include "referee.h"

//=========================================
// 初期化
//=========================================
void InitGame(void)
{
	InitWall();		// 壁
	InitDisk();		// ディスク
	InitShadow();	// 影
	InitPlayer();	// プレイヤー
	InitStage();	// ステージ
	//InitEffect();
	InitScore();
	InitTime();
	InitRef();
}

//=========================================
// 終了
//=========================================
void UninitGame(void)
{
	UninitPlayer();	// プレイヤー
	UninitWall();	// 壁
	UninitShadow();	// 影
	UninitDisk();	// ディスク
	UninitStage();	// ステージ
//	UninitEffect();
	UninitScore();
	UninitTime();
	UninitRef();
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
	UpdateScore();
	UpdateTime();
	UpdateRef();
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
	DrawScore();
	DrawTime();
	DrawRef();
}