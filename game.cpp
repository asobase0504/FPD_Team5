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
#include "result.h"
#include "time.h"
#include "ui.h"

//------------------------------------
// 静的変数
//------------------------------------
static int s_nNeedTime;			// 必要タイム数
static int s_nNeedPoint;		// 必要ポイント数
static int s_nNeedSet;			// 必要セット数
static int s_nPlayerSet[2];		// プレイヤーがどれほどセットを取っているか
static bool bIsResult;			// リザルトの表示中か
static bool bIsResetRound;		// リセット中か

//------------------------------------
// プロトタイプ宣言
//------------------------------------

//=========================================
// 初期化
//=========================================
void InitGame(void)
{
	InitDisk();			// ディスク
	InitShadow();		// 影
	InitPlayer();		// プレイヤー
	InitStage();		// ステージ
	InitLandingMark();	// ディスクの落下地点
	InitEffect();		// エフェクト
	InitUI();			// UI
	InitResult();		// リザルト

	// 初期化
	s_nPlayerSet[0] = 0;
	s_nPlayerSet[1] = 0;
	bIsResult = false;
	bIsResetRound = false;
	GetReferee()->bThrow = true;
	SetTime(s_nNeedTime);
}

//=========================================
// 終了
//=========================================
void UninitGame(void)
{
	UninitPlayer();			// プレイヤー
	UninitShadow();			// 影
	UninitDisk();			// ディスク
	UninitStage();			// ステージ
	UninitLandingMark();	// ディスクの落下地点
	UninitEffect();			// エフェクト
	UninitUI();				// UI
	UninitResult();			// リザルト
}

//=========================================
// 更新
//=========================================
void UpdateGame(void)
{
	if (bIsResult)
	{
		UpdateResult();			// リザルト
	}
	else
	{
		if (bIsResetRound)
		{	
			if (ResetPosPlayer())
			{
				bIsResetRound = false;
			}
		}

		UpdateStage();			// ステージ
		UpdatePlayer();			// プレイヤー
		UpdateDisk();			// ディスク
		UpdateLandingMark();	// ディスクの落下地点
		UpdateShadow();			// 影
		UpdateEffect();			// エフェクト
		UpdateUI();				// UI
	}

	// スコア関係の処理
	{
		SCORE* pScore = GetScore();

		// スコアが一定以上か否か
		for (int i = 0; i < 2; i++)
		{
			if (pScore[i].nScore[0] >= s_nNeedPoint)
			{
				// １セットのリセット
				s_nPlayerSet[i]++;	// セット数の取得
				AddScore(-pScore[0].nScore[0], 0);	// スコアのリセット
				AddScore(-pScore[1].nScore[0], 1);	// スコアのリセット
				SetTime(s_nNeedTime);				// タイムリセット
				bIsResetRound = true;
			}
		}

		// セット数が一定以上か否か
		if (s_nPlayerSet[0] >= s_nNeedSet)
		{
			bIsResult = true;
		}
		else if (s_nPlayerSet[1] >= s_nNeedSet)
		{
			bIsResult = true;
		}
	}
}

//=========================================
// 描画
//=========================================
void DrawGame()
{
	DrawStage();		// ステージ
	DrawShadow();		// 影
	DrawLandingMark();	// ディスクの落下地点
	DrawEffect();		// エフェクト
	DrawPlayer();		// プレイヤー
	DrawDisk();			// ディスク
	DrawUI();			// UI

	if (bIsResult)
	{
		DrawResult();		// リザルト
	}
}

//=========================================
// 必要タイムの設定
//=========================================
void SetNeedTime(int nPoint)
{
	s_nNeedTime = nPoint;
}

//=========================================
// 必要ポイントの設定
//=========================================
void SetNeedPoint(int nPoint)
{
	s_nNeedPoint = nPoint;
}

//=========================================
// 必要セット数の設定
//=========================================
void SetNeedSet(int nPoint)
{
	s_nNeedSet = nPoint;
}

//=========================================
// リセット中か否か
//=========================================
bool GetResetRound(void)
{
	return bIsResetRound;
}
