//================================//
//								  //
//       ディスクのヘッダー		  //
//       Author: Ricci Alex		  //
//       Author: tanimoto		  //
//								  //
//================================//
#ifndef _DISK_H_		//このマクロが定義されてなかったら
#define _DISK_H_		//2重インクルード防止のマクロ定義

#include "main.h" 

//マクロ定義
#define MAX_DISK			(1)				//ディスクの最大数
#define LOB_STARTING_SPEED	(5.0f)			//上投げのはじめの上下移動量

//ディスクの種類
typedef enum
{
	DISK_TYPE_NORMAL = 0,			//普通
	DISK_TYPE_LOB,					//上投げ
	DISK_TYPE_SPECIAL_0,
	DISK_TYPE_SPECIAL_1,
	DISK_TYPE_MAX
}DISK_TYPE;

//構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;						//ディスクの現在の位置
	D3DXVECTOR3 lastPos;					//ディスクの前回の位置
	D3DXVECTOR3 move;						//ディスクの移動量
	D3DXVECTOR3 acc;						//ディスクの加速
	DISK_TYPE type;							//ディスクの種類
	float fSize;							//ディスクの大きさ
	float fHeight;							//上投げ用の高さ
	float fVerticalSpeed;					//上投げの移動量
	int nCntPhase;							//必殺技用のカウンター
	int nPlayer;							//ディスクを投げたプレイヤーのインデックス
	bool bBounce;							//壁に当たったら、反射するかどうか
	bool bUse;								//使用されているかどうか
}Disk;

//プロトタイプ宣言
void InitDisk(void);
void UninitDisk(void);
void UpdateDisk(void);
void DrawDisk(void);
void SetDisk(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 acc, DISK_TYPE type, int nPlayer, float size);
void UpdateSpecialDisk(int nCntDisk, int nPlayer);

Disk *GetDisk(void);

#endif