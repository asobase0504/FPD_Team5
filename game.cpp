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
#include"score.h"

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
	InitPlayer();
	InitWall();
	InitShadow();
	InitDisk();
	InitStage();
	InitScore();
}

//=========================================
// 終了
//=========================================
void UninitGame(void)
{
	UninitPlayer();
	UninitWall();
	UninitShadow();
	UninitDisk();
	UninitStage();
	UninitScore();
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
	UpdateShadow();
	UpdateScore();
}

//=========================================
// 描画
//=========================================
void DrawGame()
{
	DrawStage();
	DrawPlayer();
	DrawWall();
	DrawShadow();
	DrawDisk();
	DrawScore();
}