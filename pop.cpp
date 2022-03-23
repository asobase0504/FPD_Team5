//=========================================
// 
// ゴール処理
// Author Tanimoto_Kosuke
//
// Update 22/03/22
// 
//=========================================
//------------------------------------
// include
//------------------------------------
#include "pop.h"
#include <stdio.h>
#include "disk.h"
#include "stage.h"
#include "score.h"
#include "goal.h"

//------------------------------------
// スタティック変数
//------------------------------------
static LPDIRECT3DTEXTURE9		s_pTexturePop[MAX_IMAGE_POP] = {};	//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffPop = NULL;				//頂点バッファへのポインタ
static POP s_aPop[MAX_POP];											//ゴールの情報

//=========================================
// ポップの初期化処理
//=========================================
void InitPop(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ
										
	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\goal\\Arrow.png",	//テクスチャのファイル名
		&s_pTexturePop[POP_TYPE_NORMAL]
	);

	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\goal\\Arrow.png",	//テクスチャのファイル名
		&s_pTexturePop[POP_TYPE_STRIKE]
	);

	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\pop\\life000.png",	//テクスチャのファイル名
		&s_pTexturePop[POP_TYPE_FELL]
	);

	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\gotou\\TEXTURE\\number000.png",	//テクスチャのファイル名
		&s_pTexturePop[POP_TYPE_SCORE]
	);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_POP,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffPop,
		NULL
	);

	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffPop->Lock(0, 0, (void**)&pVtx, 0);

	//構造体の初期化処理
	for (int nCntPop = 0; nCntPop < MAX_POP; nCntPop++, pVtx += 4)
	{
		s_aPop[nCntPop].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		s_aPop[nCntPop].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		s_aPop[nCntPop].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		s_aPop[nCntPop].fPopCounter = 0;
		s_aPop[nCntPop].bUse = false;
		s_aPop[nCntPop].bSide = false;

		//頂点座標の設定 = (配置位置 ± 正弦(対角線の角度 ± 向き) * 対角線の長さ)
		pVtx[0].pos.x = s_aPop[nCntPop].pos.x - sinf(s_aPop[nCntPop].fAngle + s_aPop[nCntPop].rot.x) * s_aPop[nCntPop].fLength;
		pVtx[0].pos.y = s_aPop[nCntPop].pos.y - cosf(s_aPop[nCntPop].fAngle + s_aPop[nCntPop].rot.y) * s_aPop[nCntPop].fLength;
		pVtx[0].pos.z = s_aPop[nCntPop].pos.z;

		pVtx[1].pos.x = s_aPop[nCntPop].pos.x + sinf(s_aPop[nCntPop].fAngle - s_aPop[nCntPop].rot.x) * s_aPop[nCntPop].fLength;
		pVtx[1].pos.y = s_aPop[nCntPop].pos.y - cosf(s_aPop[nCntPop].fAngle - s_aPop[nCntPop].rot.y) * s_aPop[nCntPop].fLength;
		pVtx[1].pos.z = s_aPop[nCntPop].pos.z;

		pVtx[2].pos.x = s_aPop[nCntPop].pos.x - sinf(s_aPop[nCntPop].fAngle - s_aPop[nCntPop].rot.x) * s_aPop[nCntPop].fLength;
		pVtx[2].pos.y = s_aPop[nCntPop].pos.y + cosf(s_aPop[nCntPop].fAngle - s_aPop[nCntPop].rot.y) * s_aPop[nCntPop].fLength;
		pVtx[2].pos.z = s_aPop[nCntPop].pos.z;

		pVtx[3].pos.x = s_aPop[nCntPop].pos.x + sinf(s_aPop[nCntPop].fAngle + s_aPop[nCntPop].rot.x) * s_aPop[nCntPop].fLength;
		pVtx[3].pos.y = s_aPop[nCntPop].pos.y + cosf(s_aPop[nCntPop].fAngle + s_aPop[nCntPop].rot.y) * s_aPop[nCntPop].fLength;
		pVtx[3].pos.z = s_aPop[nCntPop].pos.z;

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//テクスチャ座標の設定(0.0f ~ (1 / xパターン数)f)
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	//頂点バッファをアンロックする
	s_pVtxBuffPop->Unlock();
}

//=========================================
// ポップの終了処理
//=========================================
void UninitPop(void)
{
	for (int nCntImgPop = 0; nCntImgPop < MAX_IMAGE_POP; nCntImgPop++)
	{
		//テクスチャの破棄
		if (s_pTexturePop[nCntImgPop] != NULL)
		{
			s_pTexturePop[nCntImgPop]->Release();
			s_pTexturePop[nCntImgPop] = NULL;
		}
		//頂点バッファの破棄
		if (s_pVtxBuffPop != NULL)
		{
			s_pVtxBuffPop->Release();
			s_pVtxBuffPop = NULL;
		}
	}
}
//=========================================
// ポップの更新処理
//=========================================
void UpdatePop(void)
{
	SCORE *pScore = GetScore();

	int aPosTexU[4];	//各桁の数字を格納
	int nCntScore = 0;

	aPosTexU[0] = (pScore[0].nScore[0] % 100) / 10;
	aPosTexU[1] = (pScore[0].nScore[1] % 10) / 1;
	aPosTexU[2] = (pScore[1].nScore[0] % 100) / 10;
	aPosTexU[3] = (pScore[1].nScore[1] % 10) / 1;

	VERTEX_2D *pVtx;				//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffPop->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPop = 0; nCntPop < MAX_POP; nCntPop++, pVtx += 4)
	{
		if (s_aPop[nCntPop].bUse == true)
		{
			//ポップ秒数処理
			PopCounter(nCntPop);

			//移動量を位置に変換
			s_aPop[nCntPop].pos += s_aPop[nCntPop].move;

			//移動推移
			s_aPop[nCntPop].move *= 0.9f;

			//頂点座標の設定 = (配置位置 ± 正弦(対角線の角度 ± 向き) * 対角線の長さ)
			pVtx[0].pos.x = s_aPop[nCntPop].pos.x - sinf(s_aPop[nCntPop].fAngle + s_aPop[nCntPop].rot.x) * s_aPop[nCntPop].fLength;
			pVtx[0].pos.y = s_aPop[nCntPop].pos.y - cosf(s_aPop[nCntPop].fAngle + s_aPop[nCntPop].rot.y) * s_aPop[nCntPop].fLength;
			pVtx[0].pos.z = s_aPop[nCntPop].pos.z;

			pVtx[1].pos.x = s_aPop[nCntPop].pos.x + sinf(s_aPop[nCntPop].fAngle - s_aPop[nCntPop].rot.x) * s_aPop[nCntPop].fLength;
			pVtx[1].pos.y = s_aPop[nCntPop].pos.y - cosf(s_aPop[nCntPop].fAngle - s_aPop[nCntPop].rot.y) * s_aPop[nCntPop].fLength;
			pVtx[1].pos.z = s_aPop[nCntPop].pos.z;

			pVtx[2].pos.x = s_aPop[nCntPop].pos.x - sinf(s_aPop[nCntPop].fAngle - s_aPop[nCntPop].rot.x) * s_aPop[nCntPop].fLength;
			pVtx[2].pos.y = s_aPop[nCntPop].pos.y + cosf(s_aPop[nCntPop].fAngle - s_aPop[nCntPop].rot.y) * s_aPop[nCntPop].fLength;
			pVtx[2].pos.z = s_aPop[nCntPop].pos.z;

			pVtx[3].pos.x = s_aPop[nCntPop].pos.x + sinf(s_aPop[nCntPop].fAngle + s_aPop[nCntPop].rot.x) * s_aPop[nCntPop].fLength;
			pVtx[3].pos.y = s_aPop[nCntPop].pos.y + cosf(s_aPop[nCntPop].fAngle + s_aPop[nCntPop].rot.y) * s_aPop[nCntPop].fLength;
			pVtx[3].pos.z = s_aPop[nCntPop].pos.z;

			if (s_aPop[nCntPop].type == POP_TYPE_NORMAL)
			{
				//頂点カラーの設定
				pVtx[0].col = D3DCOLOR_RGBA(0, 255, 255, 255);
				pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			}
			else if (s_aPop[nCntPop].type == POP_TYPE_STRIKE)
			{
				//頂点カラーの設定
				pVtx[0].col = D3DCOLOR_RGBA(255, 0, 255, 255);
				pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			}
			else if (s_aPop[nCntPop].type == POP_TYPE_SCORE)
			{
				//テクスチャ座標の設定(0.0f ~ (1 / xパターン数)f)
				pVtx[0].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntScore], 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntScore] + 0.1f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntScore], 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntScore] + 0.1f, 1.0f);
				nCntScore++;
			}

			if (s_aPop[nCntPop].bSide == true)
			{//右側のテクスチャを逆向き
				pVtx[0].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.0f, 1.0f);
			}
		}
	}
	//頂点バッファをアンロックする
	s_pVtxBuffPop->Unlock();
}

//=========================================
// ポップの描画処理
//=========================================
void DrawPop()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ

	pDevice->SetStreamSource
	(
		0,
		s_pVtxBuffPop,		//頂点バッファへのポインタ
		0,
		sizeof(VERTEX_2D)		//頂点情報構造体のサイズ
	);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntPop = 0; nCntPop < MAX_POP; nCntPop++)
	{
		if (s_aPop[nCntPop].bUse == true)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, s_pTexturePop[s_aPop[nCntPop].type]);

			//ポリゴンの描画
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,	//プリミティブの種類
				nCntPop * 4,			//描画する最初の頂点インデックス
				2						//プリミティブアイコンの個数
			);
		}
	}
}

//============================================================================
//ポップの設定処理
//============================================================================
void SetPop(D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool side, POP_TYPE type, int nIdxGoal)
{
	s_aPop[nIdxGoal].pos = pos;
	s_aPop[nIdxGoal].rot = rot;
	s_aPop[nIdxGoal].bSide = side;
	s_aPop[nIdxGoal].type = type;
	s_aPop[nIdxGoal].bUse = true;

	if (s_aPop[nIdxGoal].type == POP_TYPE_NORMAL || s_aPop[nIdxGoal].type == POP_TYPE_STRIKE)
	{
		s_aPop[nIdxGoal].fAngle = atan2f(GOAL_POP_WIDTH, GOAL_POP_HEIGHT);
		s_aPop[nIdxGoal].fLength = sqrtf((GOAL_POP_WIDTH * GOAL_POP_WIDTH) + (GOAL_POP_HEIGHT * GOAL_POP_HEIGHT)) / 2.0f;
	}
	else if (s_aPop[nIdxGoal].type == POP_TYPE_FELL)
	{
		s_aPop[nIdxGoal].fAngle = atan2f(FELL_POP_WIDTH, FELL_POP_HEIGHT);
		s_aPop[nIdxGoal].fLength = sqrtf((FELL_POP_WIDTH * FELL_POP_WIDTH) + (FELL_POP_HEIGHT * FELL_POP_HEIGHT)) / 2.0f;
	}
	else if (s_aPop[nIdxGoal].type == POP_TYPE_SCORE)
	{
		s_aPop[nIdxGoal].fAngle = atan2f(SCORE_POP_WIDTH, SCORE_POP_HEIGHT);
		s_aPop[nIdxGoal].fLength = sqrtf((SCORE_POP_WIDTH * SCORE_POP_WIDTH) + (SCORE_POP_HEIGHT * SCORE_POP_HEIGHT)) / 2.0f;
	}
}

//============================================================================
//ポップの秒数処理
//============================================================================
void PopCounter(int nIdxPop)
{
	if (s_aPop[nIdxPop].type == POP_TYPE_NORMAL || s_aPop[nIdxPop].type == POP_TYPE_STRIKE)
	{
		//ポップ時間
		if (s_aPop[nIdxPop].fPopCounter >= 80)
		{
			if (s_aPop[nIdxPop].bSide == 0)
			{
				s_aPop[nIdxPop].move.x = -POP_SPEAD;
			}
			else
			{
				s_aPop[nIdxPop].move.x = POP_SPEAD;
			}
		}
		if (s_aPop[nIdxPop].fPopCounter >= 120)
		{
			s_aPop[nIdxPop].bUse = false;
			s_aPop[nIdxPop].fPopCounter = 0;
			s_aPop[nIdxPop].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}
		else
		{
			s_aPop[nIdxPop].fPopCounter++;
		}
	}
	else if (s_aPop[nIdxPop].type == POP_TYPE_FELL || s_aPop[nIdxPop].type == POP_TYPE_SCORE)
	{
		//ポップ時間
		if (s_aPop[nIdxPop].fPopCounter >= 120)
		{
			s_aPop[nIdxPop].bUse = false;
			s_aPop[nIdxPop].fPopCounter = 0;
			s_aPop[nIdxPop].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}
		else
		{
			s_aPop[nIdxPop].fPopCounter++;
		}
	}
}

//============================================================================
//ポップの取得処理
//============================================================================
POP *GetPop(void)
{
	return s_aPop;	//ゴール情報の先頭アドレスを返す
}