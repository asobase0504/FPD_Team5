//------------------------------
//設定のヘッダー
//ACG000
//Author: goto yuuki
//
//------------------------------
#ifndef _OPTION_H_		//このマクロが定義されてなかったら
#define _OPTION_H_		//2重インクルード防止のマクロ定義

#include "main.h"
#include "time.h"
#include "math.h"
#include "input.h"

//マクロ定義
#define MAX_OPTION	(4)		//使用するタイトルの画像の枚数

//プロトタイプ宣言
void InitOption(void);
void UninitOption(void);
void UpdateOption(void);
void DrawOption(void);
void SetOption(void);

#endif
