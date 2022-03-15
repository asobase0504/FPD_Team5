//=========================================
// 
// レフェリー処理
// Author Tanimoto_Kosuke
//
// Update 22/03/14
// 
//=========================================
//------------------------------------
// include
//------------------------------------
#include "referee.h"
#include "main.h"
#include <stdio.h>
#include "disk.h"
#include "player.h"

//------------------------------------
// スタティック変数
//------------------------------------
static LPDIRECT3DTEXTURE9		s_pTextureRef = NULL;	//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffRef = NULL;	//頂点バッファへのポインタ
static REFEREE s_aRef;									//レフェリーの情報
static bool s_bPause;	// ポーズ中かどうか

//=========================================
// レフェリーの初期化処理
//=========================================
void InitRef(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ

												//テクスチャーの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\referee\\field000.jpg",	//テクスチャのファイル名
		&s_pTextureRef
	);

	//ゴールの位置
	s_aRef.pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - REFEREE_HEIGHT * 0.75, 0.0f);
	s_aRef.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_aRef.nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_aRef.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	s_aRef.fAngle = atan2f(REFEREE_WIDTH, REFEREE_HEIGHT);
	s_aRef.fLength = sqrtf((REFEREE_WIDTH * REFEREE_WIDTH) + (REFEREE_HEIGHT * REFEREE_HEIGHT)) / 2.0f;
	s_aRef.bUse = true;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffRef,
		NULL
	);

	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffRef->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定 = (配置位置 ± 正弦(対角線の角度 ± 向き) * 対角線の長さ)
	pVtx[0].pos.x = s_aRef.pos.x - sinf(s_aRef.fAngle + s_aRef.rot.x) * s_aRef.fLength;
	pVtx[0].pos.y = s_aRef.pos.y - cosf(s_aRef.fAngle + s_aRef.rot.y) * s_aRef.fLength;
	pVtx[0].pos.z = s_aRef.pos.z;

	pVtx[1].pos.x = s_aRef.pos.x + sinf(s_aRef.fAngle - s_aRef.rot.x) * s_aRef.fLength;
	pVtx[1].pos.y = s_aRef.pos.y - cosf(s_aRef.fAngle - s_aRef.rot.y) * s_aRef.fLength;
	pVtx[1].pos.z = s_aRef.pos.z;

	pVtx[2].pos.x = s_aRef.pos.x - sinf(s_aRef.fAngle - s_aRef.rot.x) * s_aRef.fLength;
	pVtx[2].pos.y = s_aRef.pos.y + cosf(s_aRef.fAngle - s_aRef.rot.y) * s_aRef.fLength;
	pVtx[2].pos.z = s_aRef.pos.z;

	pVtx[3].pos.x = s_aRef.pos.x + sinf(s_aRef.fAngle + s_aRef.rot.x) * s_aRef.fLength;
	pVtx[3].pos.y = s_aRef.pos.y + cosf(s_aRef.fAngle + s_aRef.rot.y) * s_aRef.fLength;
	pVtx[3].pos.z = s_aRef.pos.z;

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
	s_pVtxBuffRef->Unlock();
}

//=========================================
// レフェリーの終了処理
//=========================================
void UninitRef(void)
{
	//テクスチャの破棄
	if (s_pTextureRef != NULL)
	{
		s_pTextureRef->Release();
		s_pTextureRef = NULL;
	}
	//頂点バッファの破棄
	if (s_pVtxBuffRef != NULL)
	{
		s_pVtxBuffRef->Release();
		s_pVtxBuffRef = NULL;
	}
}

//=========================================
// レフェリーの更新処理
//=========================================
void UpdateRef(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ

	VERTEX_2D *pVtx;				//頂点情報へのポインタ

	Player *pPlayer = GetPlayer();
	Disk *pDisk = GetDisk();

	if (pPlayer->bHaveDisk == false && pDisk->bUse == false)
	{
		//pos, move, acc, type, nPlayer, size
		//SetDisk(s_aRef.pos + D3DXVECTOR3(0.0f, -50.0f, 0.0f), D3DXVECTOR3(-10.0f, -1.0f, 0.0f) , D3DXVECTOR3(0.0f, 0.0f, 0.0f), DISK_TYPE_NORMAL, 0, 40.0f);
	}

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffRef->Lock(0, 0, (void**)&pVtx, 0);

	if (s_aRef.bUse == true)
	{
		//頂点座標の設定 = (配置位置 ± 正弦(対角線の角度 ± 向き) * 対角線の長さ)
		pVtx[0].pos.x = s_aRef.pos.x - sinf(s_aRef.fAngle + s_aRef.rot.x) * s_aRef.fLength;
		pVtx[0].pos.y = s_aRef.pos.y - cosf(s_aRef.fAngle + s_aRef.rot.y) * s_aRef.fLength;
		pVtx[0].pos.z = s_aRef.pos.z;

		pVtx[1].pos.x = s_aRef.pos.x + sinf(s_aRef.fAngle - s_aRef.rot.x) * s_aRef.fLength;
		pVtx[1].pos.y = s_aRef.pos.y - cosf(s_aRef.fAngle - s_aRef.rot.y) * s_aRef.fLength;
		pVtx[1].pos.z = s_aRef.pos.z;

		pVtx[2].pos.x = s_aRef.pos.x - sinf(s_aRef.fAngle - s_aRef.rot.x) * s_aRef.fLength;
		pVtx[2].pos.y = s_aRef.pos.y + cosf(s_aRef.fAngle - s_aRef.rot.y) * s_aRef.fLength;
		pVtx[2].pos.z = s_aRef.pos.z;

		pVtx[3].pos.x = s_aRef.pos.x + sinf(s_aRef.fAngle + s_aRef.rot.x) * s_aRef.fLength;
		pVtx[3].pos.y = s_aRef.pos.y + cosf(s_aRef.fAngle + s_aRef.rot.y) * s_aRef.fLength;
		pVtx[3].pos.z = s_aRef.pos.z;
	}

	//頂点バッファをアンロックする
	s_pVtxBuffRef->Unlock();
}

//=========================================
// レフェリーの描画処理
//=========================================
void DrawRef()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ

												//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource
	(
		0,
		s_pVtxBuffRef,		//頂点バッファへのポインタ
		0,
		sizeof(VERTEX_2D)		//頂点情報構造体のサイズ
	);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (s_aRef.bUse == true)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, s_pTextureRef);

		//ポリゴンの描画
		pDevice->DrawPrimitive
		(
			D3DPT_TRIANGLESTRIP,	//プリミティブの種類
			0,						//描画する最初の頂点インデックス
			2						//プリミティブアイコンの個数
		);
	}
}

//============================================================================
//レフェリーの取得処理
//============================================================================
REFEREE *GetRef(void)
{
	return &s_aRef;	//ゴール情報の先頭アドレスを返す
}