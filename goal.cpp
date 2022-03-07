//=========================================
// 
// ゴール処理
// Author Tanimoto_Kosuke
//
// Update 22/03/07
// 
//=========================================
//------------------------------------
// include
//------------------------------------
#include "goal.h"
#include "main.h"
#include <stdio.h>

//------------------------------------
// スタティック変数
//------------------------------------
static LPDIRECT3DTEXTURE9		s_pTextureGoal[MAX_IMAGE_GOAL] = {};	//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffGoal = NULL;	//頂点バッファへのポインタ
static GOAL s_aGoal[MAX_GOAL];							//ゴールの情報
static bool s_bPause;	// ポーズ中かどうか

//=========================================
// ゴールの初期化処理
//=========================================
void InitGoal(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ

										//テクスチャーの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\block000.jpg",	//テクスチャのファイル名
		&s_pTextureGoal[GOAL_TYPE_NORMAL]
	);

	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\block001.jpg",	//テクスチャのファイル名
		&s_pTextureGoal[GOAL_TYPE_STRIKE]
	);

	s_aGoal[0].pos = D3DXVECTOR3(125.0f, 215.0f, 0.0f);
	s_aGoal[0].rot = D3DXVECTOR3(D3DX_PI / 2, D3DX_PI / 2, 0.0f);
	s_aGoal[0].fAngle = atan2f(GOAL_WIDTH, GOAL_HEIGHT);
	s_aGoal[0].fLength = sqrtf((GOAL_WIDTH * GOAL_WIDTH) + (GOAL_HEIGHT * GOAL_HEIGHT)) / 2.0f;
	s_aGoal[0].type = GOAL_TYPE_NORMAL;

	s_aGoal[1].pos = D3DXVECTOR3(125.0f, SCREEN_HEIGHT / 2, 0.0f);
	s_aGoal[1].rot = D3DXVECTOR3(D3DX_PI / 2, D3DX_PI / 2, 0.0f);
	s_aGoal[1].fAngle = atan2f(STRIKE_GOAL_WIDTH, GOAL_HEIGHT);
	s_aGoal[1].fLength = sqrtf((STRIKE_GOAL_WIDTH * STRIKE_GOAL_WIDTH) + (GOAL_HEIGHT * GOAL_HEIGHT)) / 2.0f;
	s_aGoal[1].type = GOAL_TYPE_STRIKE;

	s_aGoal[2].pos = D3DXVECTOR3(125.0f, SCREEN_HEIGHT - 215.0f, 0.0f);
	s_aGoal[2].rot = D3DXVECTOR3(D3DX_PI / 2, D3DX_PI / 2, 0.0f);
	s_aGoal[2].fAngle = atan2f(GOAL_WIDTH, GOAL_HEIGHT);
	s_aGoal[2].fLength = sqrtf((GOAL_WIDTH * GOAL_WIDTH) + (GOAL_HEIGHT * GOAL_HEIGHT)) / 2.0f;
	s_aGoal[2].type = GOAL_TYPE_NORMAL;

	s_aGoal[3].pos = D3DXVECTOR3(SCREEN_WIDTH - 125.0f, 215.0f, 0.0f);
	s_aGoal[3].rot = D3DXVECTOR3(-D3DX_PI / 2, -D3DX_PI / 2, 0.0f);
	s_aGoal[3].fAngle = atan2f(GOAL_WIDTH, GOAL_HEIGHT);
	s_aGoal[3].fLength = sqrtf((GOAL_WIDTH * GOAL_WIDTH) + (GOAL_HEIGHT * GOAL_HEIGHT)) / 2.0f;
	s_aGoal[3].type = GOAL_TYPE_NORMAL;

	s_aGoal[4].pos = D3DXVECTOR3(SCREEN_WIDTH - 125.0f, SCREEN_HEIGHT / 2, 0.0f);
	s_aGoal[4].rot = D3DXVECTOR3(-D3DX_PI / 2, -D3DX_PI / 2, 0.0f);
	s_aGoal[4].fAngle = atan2f(STRIKE_GOAL_WIDTH, GOAL_HEIGHT);
	s_aGoal[4].fLength = sqrtf((STRIKE_GOAL_WIDTH * STRIKE_GOAL_WIDTH) + (GOAL_HEIGHT * GOAL_HEIGHT)) / 2.0f;
	s_aGoal[4].type = GOAL_TYPE_STRIKE;

	s_aGoal[5].pos = D3DXVECTOR3(SCREEN_WIDTH - 125.0f, SCREEN_HEIGHT - 215.0f, 0.0f);
	s_aGoal[5].rot = D3DXVECTOR3(-D3DX_PI / 2, -D3DX_PI / 2, 0.0f);
	s_aGoal[5].fAngle = atan2f(GOAL_WIDTH, GOAL_HEIGHT);
	s_aGoal[5].fLength = sqrtf((GOAL_WIDTH * GOAL_WIDTH) + (GOAL_HEIGHT * GOAL_HEIGHT)) / 2.0f;
	s_aGoal[5].type = GOAL_TYPE_NORMAL;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_GOAL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffGoal,
		NULL
	);

	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffGoal->Lock(0, 0, (void**)&pVtx, 0);

	//構造体の初期化処理
	for (int nCntGoal = 0; nCntGoal < MAX_GOAL; nCntGoal++, pVtx += 4)
	{
		s_aGoal[nCntGoal].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		s_aGoal[nCntGoal].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

		//頂点座標の設定 = (配置位置 ± 正弦(対角線の角度 ± 向き) * 対角線の長さ)
		pVtx[0].pos.x = s_aGoal[nCntGoal].pos.x - sinf(s_aGoal[nCntGoal].fAngle + s_aGoal[nCntGoal].rot.x) * s_aGoal[nCntGoal].fLength;
		pVtx[0].pos.y = s_aGoal[nCntGoal].pos.y - cosf(s_aGoal[nCntGoal].fAngle + s_aGoal[nCntGoal].rot.y) * s_aGoal[nCntGoal].fLength;
		pVtx[0].pos.z = s_aGoal[nCntGoal].pos.z;

		pVtx[1].pos.x = s_aGoal[nCntGoal].pos.x + sinf(s_aGoal[nCntGoal].fAngle - s_aGoal[nCntGoal].rot.x) * s_aGoal[nCntGoal].fLength;
		pVtx[1].pos.y = s_aGoal[nCntGoal].pos.y - cosf(s_aGoal[nCntGoal].fAngle - s_aGoal[nCntGoal].rot.y) * s_aGoal[nCntGoal].fLength;
		pVtx[1].pos.z = s_aGoal[nCntGoal].pos.z;

		pVtx[2].pos.x = s_aGoal[nCntGoal].pos.x - sinf(s_aGoal[nCntGoal].fAngle - s_aGoal[nCntGoal].rot.x) * s_aGoal[nCntGoal].fLength;
		pVtx[2].pos.y = s_aGoal[nCntGoal].pos.y + cosf(s_aGoal[nCntGoal].fAngle - s_aGoal[nCntGoal].rot.y) * s_aGoal[nCntGoal].fLength;
		pVtx[2].pos.z = s_aGoal[nCntGoal].pos.z;

		pVtx[3].pos.x = s_aGoal[nCntGoal].pos.x + sinf(s_aGoal[nCntGoal].fAngle + s_aGoal[nCntGoal].rot.x) * s_aGoal[nCntGoal].fLength;
		pVtx[3].pos.y = s_aGoal[nCntGoal].pos.y + cosf(s_aGoal[nCntGoal].fAngle + s_aGoal[nCntGoal].rot.y) * s_aGoal[nCntGoal].fLength;
		pVtx[3].pos.z = s_aGoal[nCntGoal].pos.z;

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
	s_pVtxBuffGoal->Unlock();
}

//=========================================
// ゴールの終了処理
//=========================================
void UninitGoal(void)
{
	for (int nCntImgGoal = 0; nCntImgGoal < MAX_IMAGE_GOAL; nCntImgGoal++)
	{
		//テクスチャの破棄
		if (s_pTextureGoal[nCntImgGoal] != NULL)
		{
			s_pTextureGoal[nCntImgGoal]->Release();
			s_pTextureGoal[nCntImgGoal] = NULL;
		}
		//頂点バッファの破棄
		if (s_pVtxBuffGoal != NULL)
		{
			s_pVtxBuffGoal->Release();
			s_pVtxBuffGoal = NULL;
		}
	}
}

//=========================================
// ゴールの初期化処理
//=========================================
void UpdateGoal(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ

	VERTEX_2D *pVtx;				//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffGoal->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntGoal = 0; nCntGoal < MAX_GOAL; nCntGoal++, pVtx += 4)
	{
		//頂点座標の設定 = (配置位置 ± 正弦(対角線の角度 ± 向き) * 対角線の長さ)
		pVtx[0].pos.x = s_aGoal[nCntGoal].pos.x - sinf(s_aGoal[nCntGoal].fAngle + s_aGoal[nCntGoal].rot.x) * s_aGoal[nCntGoal].fLength;
		pVtx[0].pos.y = s_aGoal[nCntGoal].pos.y - cosf(s_aGoal[nCntGoal].fAngle + s_aGoal[nCntGoal].rot.y) * s_aGoal[nCntGoal].fLength;
		pVtx[0].pos.z = s_aGoal[nCntGoal].pos.z;

		pVtx[1].pos.x = s_aGoal[nCntGoal].pos.x + sinf(s_aGoal[nCntGoal].fAngle - s_aGoal[nCntGoal].rot.x) * s_aGoal[nCntGoal].fLength;
		pVtx[1].pos.y = s_aGoal[nCntGoal].pos.y - cosf(s_aGoal[nCntGoal].fAngle - s_aGoal[nCntGoal].rot.y) * s_aGoal[nCntGoal].fLength;
		pVtx[1].pos.z = s_aGoal[nCntGoal].pos.z;

		pVtx[2].pos.x = s_aGoal[nCntGoal].pos.x - sinf(s_aGoal[nCntGoal].fAngle - s_aGoal[nCntGoal].rot.x) * s_aGoal[nCntGoal].fLength;
		pVtx[2].pos.y = s_aGoal[nCntGoal].pos.y + cosf(s_aGoal[nCntGoal].fAngle - s_aGoal[nCntGoal].rot.y) * s_aGoal[nCntGoal].fLength;
		pVtx[2].pos.z = s_aGoal[nCntGoal].pos.z;

		pVtx[3].pos.x = s_aGoal[nCntGoal].pos.x + sinf(s_aGoal[nCntGoal].fAngle + s_aGoal[nCntGoal].rot.x) * s_aGoal[nCntGoal].fLength;
		pVtx[3].pos.y = s_aGoal[nCntGoal].pos.y + cosf(s_aGoal[nCntGoal].fAngle + s_aGoal[nCntGoal].rot.y) * s_aGoal[nCntGoal].fLength;
		pVtx[3].pos.z = s_aGoal[nCntGoal].pos.z;
	}

	//頂点バッファをアンロックする
	s_pVtxBuffGoal->Unlock();
}

//=========================================
// ゴールの初期化処理
//=========================================
void DrawGoal()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource
	(
		0,
		s_pVtxBuffGoal,		//頂点バッファへのポインタ
		0,
		sizeof(VERTEX_2D)		//頂点情報構造体のサイズ
	);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntGoal = 0; nCntGoal < MAX_GOAL; nCntGoal++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, s_pTextureGoal[s_aGoal[nCntGoal].type]);

		//ポリゴンの描画
		pDevice->DrawPrimitive
		(
			D3DPT_TRIANGLESTRIP,	//プリミティブの種類
			nCntGoal * 4,			//描画する最初の頂点インデックス
			2						//プリミティブアイコンの個数
		);
	}
}