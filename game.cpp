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
	InitStage();
	InitPlayer();
	InitWall();
	InitDisk();
}

//=========================================
// 終了
//=========================================
void UninitGame(void)
{
	UninitStage();
	UninitPlayer();
	UninitWall();
	UninitDisk();
}

//=========================================
// 更新
//=========================================
void UpdateGame(void)
{
	UpdateStage();
	UpdatePlayer();
	UpdateWall();
	UpdateDisk();
}

//=========================================
// 描画
//=========================================
void DrawGame()
{
	DrawStage();
	DrawPlayer();
	DrawWall();
	DrawDisk();
}