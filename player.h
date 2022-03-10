//=========================================
// 
// プレイヤーのヘッダーファイル
// Author YudaKaito
// 
//=========================================
#ifndef _PLAYER_H_			// このマクロ定義がされてなかったら
#define _PLAYER_H_			// 2重インクルード防止のマクロ定義

//------------------------------------
// include
//------------------------------------
#include "main.h"

//------------------------------------
// マクロ定義
//------------------------------------
#define PLAYER_TEX		"data/TEXTURE/player00.png"		// プレイヤーのテクスチャ
#define PLAYER_WIDTH	(35.0f)							// プレイヤーの幅
#define PLAYER_HEIGTH	(35.0f)							// プレイヤーの高さ

//------------------------------------
// プレイヤーの状態
//------------------------------------
typedef enum
{
	PLAYERSTATE_APPEAR = 0,	// 出現状態(点滅)
	PLAYERSTATE_NORMAL,		// 通常状態
	PLAYERSTATE_DEATH,		// 死亡状態
	PLAYERSTATE_REVIVAL,	// 復活状態
	PLAYERSTATE_MAX
}PLAYRSTATE;

//------------------------------------
// プレイヤーの状態
//------------------------------------
typedef enum
{
	JUMP_NONE = 0,
	JUMP_NOW,
	JUMP_MAX
}JUMPSTATE;

//------------------------------------
// プレイヤー構造体の定義
//------------------------------------
typedef struct
{
	LPDIRECT3DTEXTURE9 pTexture;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;	// 頂点バッファへのポインタ
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 posOld;			// 位置
	D3DXVECTOR3 move;			// 移動量
	D3DXVECTOR3 rot;			// 向き
	float fWidth;				// 画像の幅
	float fHeigth;				// 画像の高さ
	float fMove;				// 移動量
	JUMPSTATE jumpstate;		// 跳躍の状態
	int nDirectionMove;			// 向き
	PLAYRSTATE state;			// ステータス
	int RevivalInterval;		// 復活タイミング
	int nCntAnim;				// アニメーションのカウンター
	int nPatternAnim;			// アニメーションのパターン番号
	bool bUse;					// 表示状態
}Player;

//------------------------------------
// プロトタイプ宣言
//------------------------------------
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
void SetPlayer(D3DXVECTOR3* pos);
Player* GetPlayer(void);

#endif // !_PLAYER_H_
