//=========================================
// 
// ポップアップ(ヘッダーファイル)
// Author YudaKaito
// 
//=========================================
#ifndef _POINT_H_
#define _POINT_H_

#include "main.h"

//------------------------------------
//マクロ定義
//------------------------------------

//------------------------------------
//ポイントの構造体
//------------------------------------
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;	//頂点バッファへのポインタ
	D3DXVECTOR3 pos;					//現在の位置
	int nScore;							//スコアの値
	bool bUse;
}Point;

//------------------------------------
//プロトタイプ宣言
//------------------------------------
void InitPoint(void);
void UninitPoint(void);
void UpdatePoint(void);
void DrawPoint(void);
void SetPoint(void);

#endif