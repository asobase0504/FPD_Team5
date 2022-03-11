//------------------------------
//スコア設定のヘッダー
//ACG000
//Author: goto yuuki
//
//------------------------------
#ifndef _SCORE_H_
#define _SCORE_H_

//マクロ定義
#define NUM_SCORE		(2)								//桁数
#define MAX_CNT_SCORE	(2)								//最大カウンター数

//スコアの構造体
typedef struct
{
	D3DXVECTOR3 posScore[MAX_CNT_SCORE];				//現在の位置
	int nScore;											//スコアの値
	int nCnt;										//スコアのカウンター
}SCORE;

typedef struct
{
	D3DXVECTOR3 c_posScore[MAX_CNT_SCORE];				//現在の位置
	int cScore;											//スコアの値
	int cCnt;										//スコアのカウンター
}SCORE2;

//プロトタイプ宣言
void InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);
void SetScore(int nScore,int cScore);
void AddScore(int nValse,int cValse);
SCORE GetScore(void);

#endif
