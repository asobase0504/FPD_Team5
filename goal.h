//=========================================
// 
// ゴール(ヘッダーファイル)
// Author Tanimoto_Kosuke
//
// Update 22/03/14
// 
//=========================================
#ifndef _GOAL_H_
#define _GOAL_H_

#include "main.h"

//****************************************************************************
//マクロ定義
//****************************************************************************
#define GOAL_WIDTH				(100)	//ゴールの横サイズ
#define GOAL_HEIGHT				(230)	//ゴールの縦サイズ
#define GOAL_POP_WIDTH			(150)	//ゴールの横サイズ
#define GOAL_POP_HEIGHT			(50)	//ゴールの縦サイズ
#define STRIKE_GOAL_HEIGHT		(60)	//真ん中ゴールの横サイズ
#define MAX_GOAL				(12)	//ゴールの最大数
#define MAX_IMAGE_GOAL			(4)		//ゴール画像の最大数

//****************************************************************************
//ゴールの種類
//****************************************************************************
typedef enum
{
	GOAL_TYPE_NORMAL = 0,	//3点ゴール
	GOAL_TYPE_STRIKE,		//5点ゴール
	GOAL_TYPE_NORMAL_POP,	//3点ゴールポップ
	GOAL_TYPE_STRIKE_POP,	//5点ゴールポップ
	GOAL_TYPE_MAX
}GOAL_TYPE;

//****************************************************************************
//構造体の定義
//****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 move;	//移動量
	D3DXVECTOR3 rot;	//回転
	D3DXVECTOR3 nor;	//法線
	D3DXCOLOR	col;	//色
	GOAL_TYPE type;		//種類
	float fAngle;		//角度(アークタンジェント)
	float fLength;		//長さ(スクウェアルート)
	bool bUse;			//使用判定
	bool bGoal;			//ゴール判定
}GOAL;

//****************************************************************************
//プロトタイプ宣言
//****************************************************************************
void InitGoal(void);
void UninitGoal(void);
void UpdateGoal(void);
void DrawGoal(void);
void SetGoalPop(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nIdxGoal);

void ColisionGoal(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pLastPos, float fWidth, float fHeight);
bool ColSegmentsGoal(D3DXVECTOR3 &seg1Start, D3DXVECTOR3 &seg1Vec, D3DXVECTOR3 &seg2Start, D3DXVECTOR3 &seg2Vec);
float Vec3CrossGoal(D3DXVECTOR3* vec1, D3DXVECTOR3* vec2);

GOAL *GetGoal(void);

#endif