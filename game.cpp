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
#include "effect.h"
#include "referee.h"
#include "landingPoint.h"
#include "score.h"
#include "ui.h"

//------------------------------------
// 静的変数
//------------------------------------
static int nNeedPoint;

//------------------------------------
// プロトタイプ宣言
//------------------------------------
static void SetNeedPoint(int nPoint);

//=========================================
// 初期化
//=========================================
void InitGame(void)
{
	InitWall();			// 壁
	InitDisk();			// ディスク
	InitShadow();		// 影
	InitPlayer();		// プレイヤー
	InitStage();		// ステージ
	InitLandingMark();	// ディスクの落下地点
	InitEffect();		// エフェクト
	InitReferee();		// レフェリー
	InitUI();			// UI

	GetReferee()->bThrow = true;
}

//=========================================
// 終了
//=========================================
void UninitGame(void)
{
	UninitPlayer();			// プレイヤー
	UninitWall();			// 壁
	UninitShadow();			// 影
	UninitDisk();			// ディスク
	UninitStage();			// ステージ
	UninitLandingMark();	// ディスクの落下地点
	UninitEffect();			// エフェクト
	UninitReferee();		// レフェリー
	UninitUI();				// UI
}

//=========================================
// 更新
//=========================================
void UpdateGame(void)
{
	UpdateStage();			// ステージ
	UpdatePlayer();			// プレイヤー
	UpdateWall();			// 壁
	UpdateDisk();			// ディスク
	UpdateLandingMark();	// ディスクの落下地点
	UpdateShadow();			// 影
	UpdateEffect();			// エフェクト
	UpdateReferee();		// レフェリー
	UpdateUI();				// UI
}

//=========================================
// 描画
//=========================================
void DrawGame()
{
	DrawStage();		// ステージ
	DrawWall();			// 壁
	DrawShadow();		// 影
	DrawLandingMark();	// ディスクの落下地点
	DrawEffect();		// エフェクト
	DrawDisk();			// ディスク
	DrawPlayer();		// プレイヤー
	DrawReferee();		// レフェリー
	DrawUI();			// UI
}

//=========================================
// 必要ポイントの設定
//=========================================
void SetNeedPoint(int nPoint)
{
	nNeedPoint = nPoint;
}
