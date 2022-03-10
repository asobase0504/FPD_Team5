//=========================================
// 
// ゴール(ヘッダーファイル)
// Author Tanimoto_Kosuke
//
// Update 22/03/09
// 
//=========================================
#ifndef _GOAL_H_
#define _GOAL_H_

#include "main.h"

//****************************************************************************
//マクロ定義
//****************************************************************************
#define GOAL_WIDTH				(230)	//ゴールの横サイズ
#define STRIKE_GOAL_WIDTH		(60)	//真ん中ゴールの横サイズ
#define GOAL_HEIGHT				(50)	//ゴールの縦サイズ
#define MAX_GOAL				(6)		//ゴールの最大数
#define MAX_IMAGE_GOAL			(2)		//ゴール画像の最大数

//****************************************************************************
//ゴールの種類
//****************************************************************************
typedef enum
{
	GOAL_TYPE_NORMAL = 0,	//3点ゴール
	GOAL_TYPE_STRIKE,		//5点ゴール
	GOAL_TYPE_MAX
}GOAL_TYPE;

typedef enum
{
	GOAL_COLLISION_NONE = 0,
	GOAL_COLLISION_TOP,
	GOAL_COLLISION_BOTTOM,
	GOAL_COLLISION_RIGHT,
	GOAL_COLLISION_LEFT,
	GOAL_COLLISION_MAX
}GOAL_COLLISION;

//****************************************************************************
//構造体の定義
//****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 rot;	//回転
	D3DXVECTOR3 nor;	//法線
	D3DXCOLOR	col;	//色
	GOAL_TYPE type;		//種類
	float fAngle;		//角度(アークタンジェント)
	float fLength;		//長さ(スクウェアルート)
	bool bUse;			//使用判定
}GOAL;

//****************************************************************************
//プロトタイプ宣言
//****************************************************************************
void InitGoal(void);
void UninitGoal(void);
void UpdateGoal(void);
void DrawGoal(void);

void ColisionGoal(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pNor,float fWidth, float fHeight);
GOAL *GetGoal(void);


#endif