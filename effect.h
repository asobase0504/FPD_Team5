//================================//
//								  //
//       エフェクトのヘッダー	  //
//       Author: Ricci Alex		  //
//								  //
//================================//

#ifndef _EFFECT_H_
#define _EFFECT_H_

//インクルードファイル
#include "main.h"

//マクロ定義
#define MAX_EFFECT				(2400)					//エフェクトの最大数

//エフェクトの種類
typedef enum
{
	EFFECT_TYPE_WALL_IMPACT = 0,						//壁と当たるエフェクト
	EFFECT_TYPE_MAX
}EFFECT_TYPE;

//エフェクトの構造体
typedef struct
{
	D3DXVECTOR3 pos;			//エフェクトの位置
	D3DXVECTOR3 move;			//エフェクトの移動量
	D3DXVECTOR3 acceleration;	//エフェクトの加速
	D3DXVECTOR3 fSize;			//エフェクトのサイズ
	D3DXVECTOR3 fDeltaSize;		//エフェクトのサイズの減数
	D3DXCOLOR col;				//エフェクトのカラー
	D3DXCOLOR fDeltaCol;		//エフェクトのカラーの減数
	int nLife;					//エフェクトのライフ
	EFFECT_TYPE type;			//エフェクトの種類
	bool bUse;					//使用されているかどうか
}Effect;

//プロトタイプ宣言
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 acceleration,
	D3DXVECTOR3 Size, D3DXVECTOR3 DeltaSize, D3DXCOLOR col, D3DXCOLOR Delta, int Life, EFFECT_TYPE Type);

#endif // !_EFFECT_H_