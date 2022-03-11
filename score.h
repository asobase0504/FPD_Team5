//------------------------------
//スコア設定のヘッダー
//ACG000
//Author: goto yuuki
//
//------------------------------
#ifndef _SCORE_H_
#define _SCORE_H_

//スコアの構造体
typedef struct
{
	D3DXVECTOR3 pos;		//現在の位置
	int nScore;				//スコアの値
}SCORE;

//プロトタイプ宣言
void InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);
void SetScore(int nScore);
void AddScore(int nValse);
SCORE GetScore(void);

#endif
