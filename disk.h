//================================//
//								  //
//       ディスクのヘッダー		  //
//       Author: Ricci Alex		  //
//								  //
//================================//
#ifndef _DISK_H_		//このマクロが定義されてなかったら
#define _DISK_H_		//2重インクルード防止のマクロ定義

#include "main.h" 

//マクロ定義
#define MAX_DISK			(1)				//ディスクの最大数

//構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;						//ディスクの現在の位置
	D3DXVECTOR3 lastPos;					//ディスクの前回の位置
	D3DXVECTOR3 move;						//ディスクの移動量
	float fSize;							//ディスクの大きさ
	bool bUse;								//使用されているかどうか
}Disk;

//プロトタイプ宣言
void InitDisk(void);
void UninitDisk(void);
void UpdateDisk(void);
void DrawDisk(void);
void SetDisk(D3DXVECTOR3 pos, D3DXVECTOR3 move, float size);

#endif