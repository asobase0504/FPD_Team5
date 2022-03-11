//=========================================
// 
// ステージ(ヘッダーファイル)
// Author Tanimoto_Kosuke
//
// Update 22/03/10
// 
//=========================================
#ifndef _STAGE_H_
#define _STAGE_H_

#include "main.h"

//****************************************************************************
//マクロ定義
//****************************************************************************
#define STAGE_WIDTH			(SCREEN_WIDTH - 50)		//ステージの横サイズ
#define STAGE_HEIGHT		(SCREEN_HEIGHT - 200)	//ステージの縦サイズ
#define STAGE_NET_WIDTH		(10)					//網の横サイズ
#define STAGE_NET_HEIGHT	(SCREEN_HEIGHT - 200)	//網の縦サイズ
#define MAX_STAGE			(5)						//ステージオブジェクトの最大数
#define MAX_IMAGE_STAGE		(4)						//ステージ画像の最大数
#define DISK_DELETE	(30)							//落ちたディスクが消える時間

//****************************************************************************
//ステージの種類
//****************************************************************************
typedef enum
{
	STAGE_TYPE_FRONT = 0,	//ステージ前側
	STAGE_TYPE_BACK,		//ステージ後ろ側
	STAGE_TYPE_NET,		//網
	STAGE_TYPE_GOALNET,		//網(ゴール)
	STAGE_TYPE_MAX
}STAGE_TYPE;

//****************************************************************************
//構造体の定義
//****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 rot;	//回転
	D3DXVECTOR3 nor;	//法線
	D3DXCOLOR	col;	//色
	STAGE_TYPE type;	//種類
	float fAngle;		//角度(アークタンジェント)
	float fLength;		//長さ(スクウェアルート)
}STAGE;

//****************************************************************************
//プロトタイプ宣言
//****************************************************************************
void InitStage(void);
void UninitStage(void);
void UpdateStage(void);
void DrawStage(void);

#endif