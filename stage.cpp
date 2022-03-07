//=========================================
// 
// ステージ処理
// Author Tanimoto_Kosuke
//
// Update 22/03/07
// 
//=========================================
//------------------------------------
// include
//------------------------------------
#include "stage.h"
#include "main.h"
#include <stdio.h>

//------------------------------------
// スタティック変数
//------------------------------------
static LPDIRECT3DTEXTURE9		s_pTextureStage = NULL;	//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffStage = NULL;	//頂点バッファへのポインタ
static STAGE s_aStage;									//ステージの情報
static bool s_bPause;	// ポーズ中かどうか

//=========================================
// ステージの初期化処理
//=========================================
void InitStage(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ

	//テクスチャーの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\block004.jpg",	//テクスチャのファイル名
		&s_pTextureStage
	);

	//構造体の初期化処理
	s_aStage.pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
	s_aStage.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			
	s_aStage.nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_aStage.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	s_aStage.fAngle = atan2f(STAGE_WIDTH, STAGE_HEIGHT);
	s_aStage.fLength = sqrtf((STAGE_WIDTH * STAGE_WIDTH) + (STAGE_HEIGHT * STAGE_HEIGHT)) / 2.0f;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffStage,
		NULL
	);

	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffStage->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定 = (配置位置 ± 正弦(対角線の角度 ± 向き) * 対角線の長さ)
	pVtx[0].pos.x = s_aStage.pos.x - sinf(s_aStage.fAngle + s_aStage.rot.x) * s_aStage.fLength;
	pVtx[0].pos.y = s_aStage.pos.y - cosf(s_aStage.fAngle + s_aStage.rot.y) * s_aStage.fLength;
	pVtx[0].pos.z = s_aStage.pos.z;

	pVtx[1].pos.x = s_aStage.pos.x + sinf(s_aStage.fAngle - s_aStage.rot.x) * s_aStage.fLength;
	pVtx[1].pos.y = s_aStage.pos.y - cosf(s_aStage.fAngle - s_aStage.rot.y) * s_aStage.fLength;
	pVtx[1].pos.z = s_aStage.pos.z;

	pVtx[2].pos.x = s_aStage.pos.x - sinf(s_aStage.fAngle - s_aStage.rot.x) * s_aStage.fLength;
	pVtx[2].pos.y = s_aStage.pos.y + cosf(s_aStage.fAngle - s_aStage.rot.y) * s_aStage.fLength;
	pVtx[2].pos.z = s_aStage.pos.z;

	pVtx[3].pos.x = s_aStage.pos.x + sinf(s_aStage.fAngle + s_aStage.rot.x) * s_aStage.fLength;
	pVtx[3].pos.y = s_aStage.pos.y + cosf(s_aStage.fAngle + s_aStage.rot.y) * s_aStage.fLength;
	pVtx[3].pos.z = s_aStage.pos.z;

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャ座標の設定(0.0f ~ (1 / xパターン数)f)
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	s_pVtxBuffStage->Unlock();
}

//=========================================
// ステージの終了処理
//=========================================
void UninitStage(void)
{
	//テクスチャの破棄
	if (s_pTextureStage != NULL)
	{
		s_pTextureStage->Release();
		s_pTextureStage = NULL;
	}

	//頂点バッファの破棄
	if (s_pVtxBuffStage != NULL)
	{
		s_pVtxBuffStage->Release();
		s_pVtxBuffStage = NULL;
	}
}

//=========================================
// ステージの初期化処理
//=========================================
void UpdateStage(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ
	
	VERTEX_2D *pVtx;				//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffStage->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定 = (配置位置 ± 正弦(対角線の角度 ± 向き) * 対角線の長さ)
	pVtx[0].pos.x = s_aStage.pos.x - sinf(s_aStage.fAngle + s_aStage.rot.x) * s_aStage.fLength;
	pVtx[0].pos.y = s_aStage.pos.y - cosf(s_aStage.fAngle + s_aStage.rot.y) * s_aStage.fLength;
	pVtx[0].pos.z = s_aStage.pos.z;

	pVtx[1].pos.x = s_aStage.pos.x + sinf(s_aStage.fAngle - s_aStage.rot.x) * s_aStage.fLength;
	pVtx[1].pos.y = s_aStage.pos.y - cosf(s_aStage.fAngle - s_aStage.rot.y) * s_aStage.fLength;
	pVtx[1].pos.z = s_aStage.pos.z;

	pVtx[2].pos.x = s_aStage.pos.x - sinf(s_aStage.fAngle - s_aStage.rot.x) * s_aStage.fLength;
	pVtx[2].pos.y = s_aStage.pos.y + cosf(s_aStage.fAngle - s_aStage.rot.y) * s_aStage.fLength;
	pVtx[2].pos.z = s_aStage.pos.z;

	pVtx[3].pos.x = s_aStage.pos.x + sinf(s_aStage.fAngle + s_aStage.rot.x) * s_aStage.fLength;
	pVtx[3].pos.y = s_aStage.pos.y + cosf(s_aStage.fAngle + s_aStage.rot.y) * s_aStage.fLength;
	pVtx[3].pos.z = s_aStage.pos.z;

	//頂点バッファをアンロックする
	s_pVtxBuffStage->Unlock();
}

//=========================================
// ステージの初期化処理
//=========================================
void DrawStage()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource
	(
		0,
		s_pVtxBuffStage,		//頂点バッファへのポインタ
		0,
		sizeof(VERTEX_2D)		//頂点情報構造体のサイズ
	);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, s_pTextureStage);

	//ポリゴンの描画
	pDevice->DrawPrimitive
	(
		D3DPT_TRIANGLESTRIP,	//プリミティブの種類
		0,						//描画する最初の頂点インデックス
		2						//プリミティブアイコンの個数
	);
}

////=========================================
//// ステージの当たり判定処理
////=========================================
//STAGE_COLLISION ColisionStage(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pNor, float fWidth, float fHeight)
//{
//	VERTEX_2D *pVtx;			//頂点情報へのポインタ
//	STAGE_COLLISION stageCollision = STAGE_COLLISION_NONE;
//
//	//頂点バッファをロックし、頂点情報へのポインタを取得
//	s_pVtxBuffStage->Lock(0, 0, (void**)&pVtx, 0);
//
//	//ブロック上側の処理
//	if (pPosOld->y <= -STAGE_HEIGHT
//		&& pPos->y >= -STAGE_HEIGHT
//		&& -STAGE_WIDTH< pPos->x + fWidth / 2
//		&& STAGE_WIDTH> pPos->x - fWidth / 2
//		)
//	{//プレイヤー前回位置の足が、ブロックの頭より高い位置にある場合(P前回位0置.y <= B現在位置.y)
//	 //プレイヤー現在位置の足がブロック内にめりこんだ場合
//		stageCollision = STAGE_COLLISION_TOP;
//	}
//	//ブロック下側の処理
//	else if (pPosOld->y - fHeight >= STAGE_HEIGHT
//		&& pPos->y - fHeight <= STAGE_HEIGHT
//		&& -STAGE_WIDTH < pPos->x + fWidth / 2
//		&& STAGE_WIDTH > pPos->x - fWidth / 2
//		)
//	{//プレイヤー前回位置の頭が、ブロック現在位置の下より低位置の場合(P前回位置.y >= B現在位置.y)
//	 //プレイヤーの頭が、ブロック内にめりこんだ場合
//		stageCollision = STAGE_COLLISION_BOTTOM;
//	}
//	//ブロック左側の処理
//	else if (pPosOld->x + fWidth / 2 <= -STAGE_WIDTH
//		&& pPos->x + fWidth / 2 >= -STAGE_WIDTH
//		&& pPos->y > -STAGE_HEIGHT
//		&& pPos->y - fHeight < STAGE_HEIGHT
//		)
//	{//プレイヤー前回位置右側が、ブロック現在位置より左にある場合(P前回位置.x <= B現在位置.x)
//	 //プレイヤー現在位置右側が、ブロック内にめりこんだ場合
//		stageCollision = STAGE_COLLISION_LEFT;
//	}
//	//ブロック右側の処理
//	else if (pPosOld->x - fWidth / 2 >= STAGE_WIDTH
//		&& pPos->x - fWidth / 2 <= STAGE_WIDTH
//		&& pPos->y > -STAGE_HEIGHT
//		&& pPos->y - fHeight < STAGE_HEIGHT
//		)
//	{//プレイヤー前回位置の左側が、ブロックの右側より左にある場合(P前回位置.x >= B現在位置.x)
//	 //プレイヤー左側が、ブロック内にめりこんだ場合
//		stageCollision = STAGE_COLLISION_RIGHT;
//	}
//
//	//頂点バッファをアンロックする
//	s_pVtxBuffStage->Unlock();
//	return stageCollision;
//}