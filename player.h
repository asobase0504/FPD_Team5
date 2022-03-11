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

//------------------------------------
// プレイヤーの種別
//------------------------------------
typedef enum
{
	PLAYERTYPE_1,
	PLAYERTYPE_2,
	PLAYERTYPE_3,
	PLAYERTYPE_4,
	PLAYERTYPE_MAX,
}PLAYERTYPE;

//------------------------------------
// 跳躍状態
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
	D3DXVECTOR3 pos;					// 位置
	D3DXVECTOR3 move;					// 移動量
	float fSize;						// 大きさ
	float fThrowCurvePower;				// 曲げて投げる力
	float fThrowPower;					// 投げる力
	float fMoveSpeed;					// 移動量
	float fSlidingVolume;				// スライディング量
	float fSlidingRigorCnt;				// スライディングの硬直時間のカウント
	float fSlidingRigorMax;				// スライディングの硬直時間
	float fAttenuationMoveSpead;		// 減衰値
	JUMPSTATE jumpstate;				// 跳躍の状態
	bool bHaveDisk;						// ディスクを所持しているか
	bool bUseSliding;					// スライディング状態
	bool bUse;							// 表示状態
}Player;

//------------------------------------
// プロトタイプ宣言
//------------------------------------
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
void SetPlayer(const D3DXVECTOR3& pos, PLAYERTYPE type);
Player* GetPlayer(void);

#endif // !_PLAYER_H_
