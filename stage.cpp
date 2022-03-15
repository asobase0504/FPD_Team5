//=========================================
// 
// ステージ処理
// Author Tanimoto_Kosuke
//
// Update 22/03/14
// 
//=========================================
//------------------------------------
// include
//------------------------------------
#include "stage.h"
#include "main.h"
#include <stdio.h>
#include "goal.h"
#include "wall.h"
#include "disk.h"
#include "shadow.h"

//------------------------------------
// スタティック変数
//------------------------------------
static LPDIRECT3DTEXTURE9		s_pTextureStage[MAX_IMAGE_STAGE] = {};	//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffStage = NULL;					//頂点バッファへのポインタ
static STAGE s_aStage[MAX_STAGE];										//ステージの情報
static bool s_bFell;			//落ちた判定
static float s_fFellCounter;	//落ちた時間
static STAGE_LENGTH s_p1;		//プレイヤー1ステージ長さ
static STAGE_LENGTH s_p2;		//プレイヤー2ステージ長さ

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
		"data\\TEXTURE\\stage\\block004.jpg",	//テクスチャのファイル名
		&s_pTextureStage[STAGE_TYPE_FRONT]
	);

	//テクスチャーの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\stage\\block005.jpg",	//テクスチャのファイル名
		&s_pTextureStage[STAGE_TYPE_BACK]
	);

	//テクスチャーの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\stage\\block005.jpg",	//テクスチャのファイル名
		&s_pTextureStage[STAGE_TYPE_NET]
	);


	//テクスチャーの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\tanimoto\\TEXTURE\\block004.jpg",	//テクスチャのファイル名
		&s_pTextureStage[STAGE_TYPE_GOALNET]
	);

	//p1のステージ長さ
	s_p1.min = D3DXVECTOR3(MIN_WIDTH, MIN_HEIGHT, 0.0f);
	s_p1.max = D3DXVECTOR3((SCREEN_WIDTH / 2) - (STAGE_NET_WIDTH / 2), MAX_HEIGHT, 0.0f);

	//p2のステージ長さ
	s_p2.min = D3DXVECTOR3((SCREEN_WIDTH / 2) + (STAGE_NET_WIDTH / 2), MIN_HEIGHT, 0.0f);
	s_p2.max = D3DXVECTOR3(MAX_WIDTH, MAX_HEIGHT, 0.0f);

	//構造体の初期化処理
	s_aStage[0].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.3f);
	s_aStage[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_aStage[0].fAngle = atan2f(STAGE_WIDTH, STAGE_HEIGHT);
	s_aStage[0].fLength = sqrtf((STAGE_WIDTH * STAGE_WIDTH) + (STAGE_HEIGHT * STAGE_HEIGHT)) / 2.0f;
	s_aStage[0].type = STAGE_TYPE_FRONT;

	s_aStage[1].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.4f);
	s_aStage[1].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_aStage[1].fAngle = atan2f(SCREEN_WIDTH, SCREEN_HEIGHT);
	s_aStage[1].fLength = sqrtf((SCREEN_WIDTH * SCREEN_WIDTH) + (SCREEN_HEIGHT * SCREEN_HEIGHT)) / 2.0f;
	s_aStage[1].type = STAGE_TYPE_BACK;

	s_aStage[2].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.1f);
	s_aStage[2].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_aStage[2].fAngle = atan2f(STAGE_NET_WIDTH, STAGE_NET_HEIGHT);
	s_aStage[2].fLength = sqrtf((STAGE_NET_WIDTH * STAGE_NET_WIDTH) + (STAGE_NET_HEIGHT * STAGE_NET_HEIGHT)) / 2.0f;
	s_aStage[2].type = STAGE_TYPE_NET;

	s_aStage[3].pos = D3DXVECTOR3(MIN_WIDTH, SCREEN_HEIGHT / 2, 0.0f);
	s_aStage[3].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_aStage[3].fAngle = atan2f(STAGE_NET_WIDTH, STAGE_NET_HEIGHT);
	s_aStage[3].fLength = sqrtf((STAGE_NET_WIDTH * STAGE_NET_WIDTH) + (STAGE_NET_HEIGHT * STAGE_NET_HEIGHT)) / 2.0f;
	s_aStage[3].type = STAGE_TYPE_GOALNET;

	s_aStage[4].pos = D3DXVECTOR3(MAX_WIDTH, SCREEN_HEIGHT / 2, 0.0f);
	s_aStage[4].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_aStage[4].fAngle = atan2f(STAGE_NET_WIDTH, STAGE_NET_HEIGHT);
	s_aStage[4].fLength = sqrtf((STAGE_NET_WIDTH * STAGE_NET_WIDTH) + (STAGE_NET_HEIGHT * STAGE_NET_HEIGHT)) / 2.0f;
	s_aStage[4].type = STAGE_TYPE_GOALNET;
	
	//頂点バッファの生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_STAGE,
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

	for (int nCntStage = 0; nCntStage < MAX_STAGE; nCntStage++,pVtx += 4)
	{
		//頂点座標の設定 = (配置位置 ± 正弦(対角線の角度 ± 向き) * 対角線の長さ)
		pVtx[0].pos.x = s_aStage[nCntStage].pos.x - sinf(s_aStage[nCntStage].fAngle + s_aStage[nCntStage].rot.x) * s_aStage[nCntStage].fLength;
		pVtx[0].pos.y = s_aStage[nCntStage].pos.y - cosf(s_aStage[nCntStage].fAngle + s_aStage[nCntStage].rot.y) * s_aStage[nCntStage].fLength;
		pVtx[0].pos.z = s_aStage[nCntStage].pos.z;

		pVtx[1].pos.x = s_aStage[nCntStage].pos.x + sinf(s_aStage[nCntStage].fAngle - s_aStage[nCntStage].rot.x) * s_aStage[nCntStage].fLength;
		pVtx[1].pos.y = s_aStage[nCntStage].pos.y - cosf(s_aStage[nCntStage].fAngle - s_aStage[nCntStage].rot.y) * s_aStage[nCntStage].fLength;
		pVtx[1].pos.z = s_aStage[nCntStage].pos.z;

		pVtx[2].pos.x = s_aStage[nCntStage].pos.x - sinf(s_aStage[nCntStage].fAngle - s_aStage[nCntStage].rot.x) * s_aStage[nCntStage].fLength;
		pVtx[2].pos.y = s_aStage[nCntStage].pos.y + cosf(s_aStage[nCntStage].fAngle - s_aStage[nCntStage].rot.y) * s_aStage[nCntStage].fLength;
		pVtx[2].pos.z = s_aStage[nCntStage].pos.z;

		pVtx[3].pos.x = s_aStage[nCntStage].pos.x + sinf(s_aStage[nCntStage].fAngle + s_aStage[nCntStage].rot.x) * s_aStage[nCntStage].fLength;
		pVtx[3].pos.y = s_aStage[nCntStage].pos.y + cosf(s_aStage[nCntStage].fAngle + s_aStage[nCntStage].rot.y) * s_aStage[nCntStage].fLength;
		pVtx[3].pos.z = s_aStage[nCntStage].pos.z;

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
	}

	//頂点バッファをアンロックする
	s_pVtxBuffStage->Unlock();

	SetWall(D3DXVECTOR3(SCREEN_WIDTH / 2, MIN_HEIGHT, 0.0f), STAGE_WIDTH, 10.0f, 0.0f);		//壁(上側)
	SetWall(D3DXVECTOR3(SCREEN_WIDTH / 2, MAX_HEIGHT, 0.0f), STAGE_WIDTH, 10.0f, D3DX_PI);	//壁(下側)

	InitGoal();
}

//=========================================
// ステージの終了処理
//=========================================
void UninitStage(void)
{
	for (int nCntImgStage = 0; nCntImgStage < MAX_IMAGE_STAGE; nCntImgStage++)
	{
		//テクスチャの破棄
		if (s_pTextureStage[nCntImgStage] != NULL)
		{
			s_pTextureStage[nCntImgStage]->Release();
			s_pTextureStage[nCntImgStage] = NULL;
		}
		//頂点バッファの破棄
		if (s_pVtxBuffStage != NULL)
		{
			s_pVtxBuffStage->Release();
			s_pVtxBuffStage = NULL;
		}
	}

	UninitGoal();
}

//=========================================
// ステージの更新処理
//=========================================
void UpdateStage(void)
{	
	VERTEX_2D *pVtx;				//頂点情報へのポインタ

	Disk *pDisk = GetDisk();

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffStage->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntStage = 0; nCntStage < MAX_STAGE; nCntStage++, pVtx += 4)
	{
		//頂点座標の設定 = (配置位置 ± 正弦(対角線の角度 ± 向き) * 対角線の長さ)
		pVtx[0].pos.x = s_aStage[nCntStage].pos.x - sinf(s_aStage[nCntStage].fAngle + s_aStage[nCntStage].rot.x) * s_aStage[nCntStage].fLength;
		pVtx[0].pos.y = s_aStage[nCntStage].pos.y - cosf(s_aStage[nCntStage].fAngle + s_aStage[nCntStage].rot.y) * s_aStage[nCntStage].fLength;
		pVtx[0].pos.z = s_aStage[nCntStage].pos.z;

		pVtx[1].pos.x = s_aStage[nCntStage].pos.x + sinf(s_aStage[nCntStage].fAngle - s_aStage[nCntStage].rot.x) * s_aStage[nCntStage].fLength;
		pVtx[1].pos.y = s_aStage[nCntStage].pos.y - cosf(s_aStage[nCntStage].fAngle - s_aStage[nCntStage].rot.y) * s_aStage[nCntStage].fLength;
		pVtx[1].pos.z = s_aStage[nCntStage].pos.z;

		pVtx[2].pos.x = s_aStage[nCntStage].pos.x - sinf(s_aStage[nCntStage].fAngle - s_aStage[nCntStage].rot.x) * s_aStage[nCntStage].fLength;
		pVtx[2].pos.y = s_aStage[nCntStage].pos.y + cosf(s_aStage[nCntStage].fAngle - s_aStage[nCntStage].rot.y) * s_aStage[nCntStage].fLength;
		pVtx[2].pos.z = s_aStage[nCntStage].pos.z;

		pVtx[3].pos.x = s_aStage[nCntStage].pos.x + sinf(s_aStage[nCntStage].fAngle + s_aStage[nCntStage].rot.x) * s_aStage[nCntStage].fLength;
		pVtx[3].pos.y = s_aStage[nCntStage].pos.y + cosf(s_aStage[nCntStage].fAngle + s_aStage[nCntStage].rot.y) * s_aStage[nCntStage].fLength;
		pVtx[3].pos.z = s_aStage[nCntStage].pos.z;
	}
	//頂点バッファをアンロックする
	s_pVtxBuffStage->Unlock();

	//ディスクが動かない場合
	if (pDisk->move.x == 0.0f && pDisk->move.y == 0.0f)
	{
		s_bFell = true;
	}
	else
	{
		s_bFell = false;
	}

	//ディスクが落ちた場合
	if (s_bFell == true)
	{
		s_fFellCounter++;
	}
	else
	{
		s_fFellCounter = 0;
	}

	//ディスクを消す時間
	if (s_fFellCounter >= DISK_DELETE)
	{
		Shadow* pShadow = GetShadow();

		pDisk->bUse = false;
		pShadow[pDisk->nIdxShadow].bUse = false;
	}

	UpdateGoal();
}

//=========================================
// ステージの描画処理
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

	for (int nCntStage = 0; nCntStage < MAX_STAGE; nCntStage++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, s_pTextureStage[s_aStage[nCntStage].type]);

		//ポリゴンの描画
		pDevice->DrawPrimitive
		(
			D3DPT_TRIANGLESTRIP,	//プリミティブの種類
			nCntStage * 4,			//描画する最初の頂点インデックス
			2						//プリミティブアイコンの個数
		);
	}

	DrawGoal();
}

//============================================================================
//ステージ長さの取得処理
//============================================================================
STAGE_LENGTH *GetP1StgLng(void)
{
	return &s_p1;	//p1ステージ長さの情報の先頭アドレスを返す
}

STAGE_LENGTH *GetP2StgLng(void)
{
	return &s_p2;	//p2ステージ長さの先頭アドレスを返す
}
