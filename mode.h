//==================================================
// 
// FPG制作 ( mode.h )
// Author  : katsuki mizuki
// 
//==================================================
#ifndef _MODE_H_		// このマクロが定義されてなかったら
#define _MODE_H_		// 2重インクルード防止のマクロ定義

//--------------------------------------------------
// 列挙型
//--------------------------------------------------
typedef enum
{
	MODE_TITLE = 0,	// タイトル画面
	MODE_GAME,		// ゲーム画面
	MODE_RESULT,	// リザルト画面
	MODE_MAX,
	MODE_NONE
}MODE;

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
void UninitMode(void);
void UpdateMode(void);
void DrawMode(void);
void SetMode(void);
MODE GetMode(void);
void ChangeMode(MODE mode);

#endif // !_MODE_H_
