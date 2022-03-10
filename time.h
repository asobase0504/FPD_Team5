//------------------------------
//時間設定のヘッダー
//ACG000
//Author: goto yuuki
//
//------------------------------
#ifndef _TIME_H_		//このマクロ定義がされてなかったら
#define _TIME_H_		//２重インクルード防止のマクロ定義

#include "main.h"
#include "fade.h"

//マクロ定義
#define MAX_TIME	(5)	//最大タイムカウンター数

//プロトタイプ宣言
void InitTime(void);
void UninitTime(void);
void UpdateTime(void);
void DrawTime(void);
void SetTime(void);
#endif