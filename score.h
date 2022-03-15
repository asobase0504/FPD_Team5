//------------------------------
//スコア設定のヘッダー
//ACG000
//Author: goto yuuki
//
//------------------------------
#ifndef _SCORE_H_
#define _SCORE_H_

//マクロ定義
#define NUM_SCORE		(4)								//桁数
#define MAX_CNT_SCORE	(2)								//最大カウンター数

//スコアの構造体
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;			//頂点バッファへのポインタ
	D3DXVECTOR3 pos;				//現在の位置
	D3DXVECTOR3 aPosTexU[MAX_CNT_SCORE];
	int nScore[2];											//スコアの値
	int nCnt;										//スコアのカウンター
}SCORE;

//プロトタイプ宣言
void InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);
void SetScore(int nScore);//,int nPlayerIdx);
void AddScore(int nValse,int nPlayerIdx);
SCORE GetScore(void);

#endif
