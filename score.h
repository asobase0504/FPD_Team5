//------------------------------
//スコア設定のヘッダー
//ACG000
//Author: goto yuuki
//
//------------------------------
#ifndef _SCORE_H_
#define _SCORE_H_

//プロトタイプ宣言
void InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);
void SetScore(int nScore);
void AddScore(int nValse);
int GetScore(void);

#endif
