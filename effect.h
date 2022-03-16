//================================//
//								  //
//       エフェクトのヘッダー	  //
//       Author: Ricci Alex		  //
//								  //
//================================//

#ifndef _EFFECT_H_
#define _EFFECT_H_

// インクルードファイル
#include "main.h"

// エフェクトの種類
typedef enum
{
	EFFECT_TYPE_SLIDING_IMPACT_1 = 0,	// スライディングエフェクト1
	EFFECT_TYPE_SLIDING_IMPACT_2,		// スライディングエフェクト2
	EFFECT_TYPE_SLIDING_IMPACT_3,		// スライディングエフェクト3
	EFFECT_TYPE_WALL_IMPACT,			// 壁と当たるエフェクト
	EFFECT_TYPE_MAX
}EFFECT_TYPE;

// エフェクトの構造体
typedef struct
{
	D3DXVECTOR3 pos;			// エフェクトの位置
	D3DXVECTOR3 move;			// エフェクトの移動量
	float rot;			// エフェクトの移動量
	D3DXVECTOR3 acceleration;	// エフェクトの加速
	D3DXVECTOR3 fSize;			// エフェクトのサイズ
	D3DXVECTOR3 fDeltaSize;		// エフェクトのサイズの減数
	D3DXCOLOR col;				// エフェクトのカラー
	D3DXCOLOR fDeltaCol;		// エフェクトのカラーの減数
	int nLife;					// エフェクトのライフ
	EFFECT_TYPE type;			// エフェクトの種類
	float fAngle;				// 対角線の角度
	float fLenght;				// 対角線の長さ
	bool bUse;					// 使用されているかどうか
}Effect;

// プロトタイプ宣言
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEffect(D3DXVECTOR3 pos, EFFECT_TYPE Type);

#endif // !_EFFECT_H_