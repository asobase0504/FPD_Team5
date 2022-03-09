//=========================================
// 
// ステージ(ヘッダーファイル)
// Author Tanimoto_Kosuke
//
// Update 22/03/07
// 
//=========================================
#ifndef _STAGE_H_
#define _STAGE_H_

#include "main.h"

//****************************************************************************
//マクロ定義
//****************************************************************************
#define STAGE_WIDTH		(SCREEN_WIDTH - 200)	//ステージの横サイズ
#define STAGE_HEIGHT	(SCREEN_HEIGHT - 200)	//ステージの縦サイズ

//****************************************************************************
//構造体の定義
//****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 rot;	//回転
	D3DXVECTOR3 nor;	//法線
	D3DXCOLOR	col;	//色
	float fAngle;		//角度(アークタンジェント)
	float fLength;		//長さ(スクウェアルート)
}STAGE;

typedef enum
{
	STAGE_COLLISION_NONE = 0,
	STAGE_COLLISION_TOP,
	STAGE_COLLISION_BOTTOM,
	STAGE_COLLISION_RIGHT,
	STAGE_COLLISION_LEFT,
	STAGE_COLLISION_MAX
}STAGE_COLLISION;

//****************************************************************************
//プロトタイプ宣言
//****************************************************************************
void InitStage(void);
void UninitStage(void);
void UpdateStage(void);
void DrawStage(void);

//STAGE_COLLISION ColisionStage(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pNor,float fWidth, float fHeight);

#endif