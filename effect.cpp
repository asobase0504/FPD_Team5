//================================//
//								  //
//       effect.cpp				  //
//       Author: Ricci Alex		  //
//								  //
//================================//

//====================================
//インクルードファイル
//====================================
#include "effect.h"
#include "input.h"

//====================================
//グローバル変数
//====================================
static Effect g_aEffect[MAX_EFFECT];							//エフェクト型の配列
static LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;			//頂点バッファへのポインタ
static LPDIRECT3DTEXTURE9 g_apTexEffect = NULL;					//テクスチャへのポインタ

//====================================
//エフェクトの初期化処理
//====================================
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイス情報の取得
	VERTEX_2D*pVtx;									//頂点情報へのポインタ

													//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\shadow000.jpg",
		&g_apTexEffect);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EFFECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		ZeroMemory(&g_aEffect[nCntEffect], sizeof(Effect));

		//頂点座標の設定
		pVtx[(nCntEffect * 4) + 0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[(nCntEffect * 4) + 1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[(nCntEffect * 4) + 2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[(nCntEffect * 4) + 3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//rhwの設定
		pVtx[(nCntEffect * 4) + 0].rhw = 1.0f;
		pVtx[(nCntEffect * 4) + 1].rhw = 1.0f;
		pVtx[(nCntEffect * 4) + 2].rhw = 1.0f;
		pVtx[(nCntEffect * 4) + 3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[(nCntEffect * 4) + 0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[(nCntEffect * 4) + 1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[(nCntEffect * 4) + 2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[(nCntEffect * 4) + 3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//テクスチャ座標の設定
		pVtx[(nCntEffect * 4) + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[(nCntEffect * 4) + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[(nCntEffect * 4) + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[(nCntEffect * 4) + 3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//頂点バッファをアンロックする
	g_pVtxBuffEffect->Unlock();
}

//====================================
//エフェクトの終了処理
//====================================
void UninitEffect(void)
{
	//頂点バッファの破棄
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}

	//テクスチャポインタの破棄
	if (g_apTexEffect != NULL)
	{
		g_apTexEffect->Release();
		g_apTexEffect = NULL;
	}
}

//====================================
//エフェクトの更新処理
//====================================
void UpdateEffect(void)
{
	VERTEX_2D *pVtx = NULL;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{
			g_aEffect[nCntEffect].pos += g_aEffect[nCntEffect].move;
			g_aEffect[nCntEffect].move += g_aEffect[nCntEffect].acceleration;
			g_aEffect[nCntEffect].fSize -= g_aEffect[nCntEffect].fDeltaSize;
			g_aEffect[nCntEffect].col -= g_aEffect[nCntEffect].fDeltaCol;
			g_aEffect[nCntEffect].nLife--;

			if (g_aEffect[nCntEffect].nLife <= 0)
			{
				g_aEffect[nCntEffect].bUse = false;
			}
			if (g_aEffect[nCntEffect].fSize.x <= 0.0f || g_aEffect[nCntEffect].fSize.y <= 0.0f)
			{
				g_aEffect[nCntEffect].bUse = false;
			}

			pVtx[(nCntEffect * 4) + 0].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fSize.x, g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fSize.y, 0.0f);
			pVtx[(nCntEffect * 4) + 1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fSize.x, g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fSize.y, 0.0f);
			pVtx[(nCntEffect * 4) + 2].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fSize.x, g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fSize.y, 0.0f);
			pVtx[(nCntEffect * 4) + 3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fSize.x, g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fSize.y, 0.0f);

			pVtx[(nCntEffect * 4) + 0].col = g_aEffect[nCntEffect].col;
			pVtx[(nCntEffect * 4) + 1].col = g_aEffect[nCntEffect].col;
			pVtx[(nCntEffect * 4) + 2].col = g_aEffect[nCntEffect].col;
			pVtx[(nCntEffect * 4) + 3].col = g_aEffect[nCntEffect].col;
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffEffect->Unlock();
}

//====================================
//エフェクトの描画処理
//====================================
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//加算合成の設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_apTexEffect);

			//ディスクを描画する
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * 4, 2);
		}
	}

	//設定を元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pDevice->SetTexture(0, NULL);
}

//====================================
//エフェクトの設定処理
//====================================
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 acceleration,
	D3DXVECTOR3 Size, D3DXVECTOR3 DeltaSize, D3DXCOLOR col, D3DXCOLOR Delta, int Life, EFFECT_TYPE Type)
{
	VERTEX_2D *pVtx = NULL;
	
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == false)
		{
			g_aEffect[nCntEffect].pos = pos;
			g_aEffect[nCntEffect].move = move;
			g_aEffect[nCntEffect].acceleration = acceleration;
			g_aEffect[nCntEffect].fSize = Size;
			g_aEffect[nCntEffect].fDeltaSize = DeltaSize;
			g_aEffect[nCntEffect].col = col;
			g_aEffect[nCntEffect].fDeltaCol = Delta;
			g_aEffect[nCntEffect].nLife = Life;
			g_aEffect[nCntEffect].type = Type;
			g_aEffect[nCntEffect].bUse = true;

			pVtx[(nCntEffect * 4) + 0].pos = D3DXVECTOR3(pos.x - Size.x, pos.y - Size.y, 0.0f);
			pVtx[(nCntEffect * 4) + 1].pos = D3DXVECTOR3(pos.x + Size.x, pos.y - Size.y, 0.0f);
			pVtx[(nCntEffect * 4) + 2].pos = D3DXVECTOR3(pos.x - Size.x, pos.y + Size.y, 0.0f);
			pVtx[(nCntEffect * 4) + 3].pos = D3DXVECTOR3(pos.x + Size.x, pos.y + Size.y, 0.0f);

			pVtx[(nCntEffect * 4) + 0].col = col;
			pVtx[(nCntEffect * 4) + 1].col = col;
			pVtx[(nCntEffect * 4) + 2].col = col;
			pVtx[(nCntEffect * 4) + 3].col = col;

			switch (Type)
			{
			case EFFECT_TYPE_WALL_IMPACT:

				pVtx[(nCntEffect * 4) + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[(nCntEffect * 4) + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[(nCntEffect * 4) + 2].tex = D3DXVECTOR2(0.0f, 0.5f);
				pVtx[(nCntEffect * 4) + 3].tex = D3DXVECTOR2(1.0f, 0.5f);

				break;
			}

			break;
		}
	}
	//頂点バッファをアンロックする
	g_pVtxBuffEffect->Unlock();
}