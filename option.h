//============================================
//設定のヘッダー
//ACG000
//Author: goto yuuki
//Author:Kishimoto Eiji
//
//============================================
#ifndef _OPTION_H_		//このマクロが定義されてなかったら
#define _OPTION_H_		//2重インクルード防止のマクロ定義

#include "main.h"

//============================================
//プロトタイプ宣言
//============================================

void InitOption(void);
void UninitOption(void);
void UpdateOption(void);
void DrawOption(void);
void SetOption(void);

#endif
