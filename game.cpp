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
#include "stgfg.h"
#include "pipe.h"
#include "gear.h"

//------------------------------------
// マクロ定義
//------------------------------------
#define RESET_DELAY	(20)	// 次開始までの遅延

//------------------------------------
// 静的変数
//------------------------------------
static int s_nNeedTime;			// 必要タイム数
static int s_nNeedPoint;		// 必要ポイント数
static int s_nNeedSet;			// 必要セット数
static int s_nPlayerSet[2];		// プレイヤーがどれほどセットを取っているか
static bool bIsResult;			// リザルトの表示中か
static bool bIsResetGame;		// スコアリセット中か
static int s_nResetGamePosCnt;	// 所定の位置に戻ってどれほど経過したか

//------------------------------------
// プロトタイプ宣言
//------------------------------------
static void RoundReset(void);

//=========================================
// 初期化
//=========================================
void InitGame(void)
{
	InitDisk();			// ディスク
	InitShadow();		// 影
	InitPlayer();		// プレイヤー
	InitUI();			// UI
	InitStage();		// ステージ
	InitStgFg();		// ステージ前景
	InitLandingMark();	// ディスクの落下地点
	InitEffect();		// エフェクト
	InitResult();		// リザルト
	InitPipe();			// 配管
	InitGear();			// 歯車

	// 初期化
	s_nPlayerSet[0] = 0;
	s_nPlayerSet[1] = 0;
	s_nResetGamePosCnt = 0;
	bIsResult = false;
	RoundReset();

	SetPipe(D3DXVECTOR3(150.0f, 70.0f, 0.0f), D3DXVECTOR3(25.5f, 70.0f, 0.0f), PIPE_TYPE_VERTICAL);
	SetPipe(D3DXVECTOR3(250.0f, 70.0f, 0.0f), D3DXVECTOR3(25.5f, 70.0f, 0.0f), PIPE_TYPE_VERTICAL);
	SetPipe(D3DXVECTOR3(350.0f, 70.0f, 0.0f), D3DXVECTOR3(25.5f, 70.0f, 0.0f), PIPE_TYPE_VERTICAL);
	SetPipe(D3DXVECTOR3(450.0f, 70.0f, 0.0f), D3DXVECTOR3(25.5f, 70.0f, 0.0f), PIPE_TYPE_VERTICAL);
	SetPipe(D3DXVECTOR3(420.0f, 35.0f, 0.0f), D3DXVECTOR3(70.0f, 21.85f, 0.0f), PIPE_TYPE_STEAM);
	SetPipe(D3DXVECTOR3(420.0f, 90.0f, 0.0f), D3DXVECTOR3(70.0f, 21.85f, 0.0f), PIPE_TYPE_STEAM);
	SetPipe(D3DXVECTOR3(820.0f, 70.0f, 0.0f), D3DXVECTOR3(60.0f, 21.85f, 0.0f), PIPE_TYPE_STEAM);
	SetGear(D3DXVECTOR3(1060.0f, 40.0f, 0.0f), 100.0f, -D3DX_PI * 0.008f, 2);
	SetGear(D3DXVECTOR3(1116.0f, 90.0f, 0.0f), 70.0f, D3DX_PI * 0.008f, 2);
	SetGear(D3DXVECTOR3(1174.0f, 52.0f, 0.0f), 90.0f, -D3DX_PI * 0.008f, 2);
	SetGear(D3DXVECTOR3(1265.0f, 81.0f, 0.0f), 130.0f, D3DX_PI * 0.008f, 2);
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
	UninitStgFg();			// ステージ前景
	UninitLandingMark();	// ディスクの落下地点
	UninitEffect();			// エフェクト
	UninitUI();				// UI
	UninitResult();			// リザルト
	UninitPipe();			// 配管
	UninitGear();			// 歯車
}

//=========================================
// 更新
//=========================================
void UpdateGame(void)
{
	if (bIsResult)
	{ // リザルト中
		UpdateResult();			// リザルト
	}
	else
	{
		if (bIsResetGame && ResetPosPlayer())
		{
			s_nResetGamePosCnt++;
			if (s_nResetGamePosCnt == RESET_DELAY)
			{
				DestroyDisk();
				GetReferee()->bThrow = true;
			}

			if (GetPlayer()[0].bHaveDisk || GetPlayer()[1].bHaveDisk)
			{
				s_nResetGamePosCnt = 0;
				bIsResetGame = false;	// リセットの終了
			}
		}

		UpdateStage();			// ステージ
		UpdateStgFg();			// ステージ前景
		UpdatePlayer();			// プレイヤー
		UpdateDisk();			// ディスク
		UpdateLandingMark();	// ディスクの落下地点
		UpdateShadow();			// 影
		UpdateEffect();			// エフェクト
		UpdateUI();				// UI
		UpdatePipe();			// 配管
		UpdateGear();			// 歯車
	}

	// スコア関係の処理
	{
		SCORE* pScore = GetScore();

		// スコアが一定以上か否か
		for (int i = 0; i < 2; i++)
		{
			if (pScore[i].nScore >= s_nNeedPoint)
			{
				// １セットのリセット
				s_nPlayerSet[i]++;	// セット数の取得
				RoundReset();
			}
		}

		if (GetTime()->nTime <= 0)
		{
			DestroyDisk();	// ディスクの削除
			if (s_nPlayerSet[0] > s_nPlayerSet[1])
			{ // P1の勝ち
				s_nPlayerSet[0]++;	// セット数の取得
				SetThoThrowRefreeIdx(1);	// 投げる方向の選択
				RoundReset();
			}
			else if (s_nPlayerSet[0] < s_nPlayerSet[1])
			{ // P2の勝ち
				s_nPlayerSet[1]++;	// セット数の取得
				SetThoThrowRefreeIdx(0);	// 投げる方向の選択
				RoundReset();
			}
			else
			{ // 引き分け
				RoundReset();
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
	DrawGear();			// 歯車
	DrawShadow();		// 影
	DrawLandingMark();	// ディスクの落下地点
	DrawEffect();		// エフェクト
	DrawPipe();			// 配管

	if (GetDisk()->type == DISK_TYPE_LOB)
	{
		DrawPlayer();		// プレイヤー
		DrawDisk();			// ディスク
	}
	else
	{
		DrawDisk();			// ディスク
		DrawPlayer();		// プレイヤー
	}
	DrawStgFg();		// ステージ前景

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
bool* GetResetScore(void)
{
	return &bIsResetGame;
}

//=========================================
// ラウンドリセット
//=========================================
void RoundReset(void)
{
	SCORE* pScore = GetScore();
	Player * pPlayer = GetPlayer();

	for (int nIdxPlayer = 0; nIdxPlayer < 2; nIdxPlayer++, pPlayer++)
	{
		AddScore(-pScore[nIdxPlayer].nScore, 0);	// スコアのリセット
		pPlayer->bHaveDisk = false;
	}

	SetTime(s_nNeedTime);				// タイムリセット
	bIsResetGame = true;
}
