//================================//
//								  //
//		effect.cpp				  //
//		Author: Ricci Alex		  //
//		Author: Yuda Kaito		  //
//								  //
//================================//

//====================================
// インクルードファイル
//====================================
#include "effect.h"
#include "input.h"

//====================================
// マクロ定義
//====================================
#define MAX_EFFECT	(2400)		// エフェクトの最大数

//====================================
// グローバル変数
//====================================
static Effect s_aEffect[MAX_EFFECT];						// エフェクト型の配列
static Effect s_aEffectType[EFFECT_TYPE_MAX];				// エフェクトのタイプ
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = NULL;			// 頂点バッファへのポインタ
static LPDIRECT3DTEXTURE9 s_pTexture[EFFECT_TYPE_MAX] = {};	// テクスチャへのポインタ

//====================================
// エフェクトの初期化処理
//====================================
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// デバイス情報の取得
	VERTEX_2D*pVtx;									// 頂点情報へのポインタ

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Effect\\effect105.png",
		&s_pTexture[EFFECT_TYPE_SLIDING_IMPACT_1]);
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Effect\\effect105.png",
		&s_pTexture[EFFECT_TYPE_SLIDING_IMPACT_2]);
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Effect\\effect106.png",
		&s_pTexture[EFFECT_TYPE_SLIDING_IMPACT_3]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Effect\\ImpactEffect.png",
		&s_pTexture[EFFECT_TYPE_WALL_IMPACT]);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EFFECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	ZeroMemory(&s_aEffect, sizeof(s_aEffect));

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		int nCntIdxVtx = nCntEffect * 4;
		// 頂点座標の設定
		pVtx[nCntIdxVtx + 0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[nCntIdxVtx + 1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[nCntIdxVtx + 2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[nCntIdxVtx + 3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// rhwの設定
		pVtx[nCntIdxVtx + 0].rhw = 1.0f;
		pVtx[nCntIdxVtx + 1].rhw = 1.0f;
		pVtx[nCntIdxVtx + 2].rhw = 1.0f;
		pVtx[nCntIdxVtx + 3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[nCntIdxVtx + 0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[nCntIdxVtx + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[nCntIdxVtx + 2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[nCntIdxVtx + 3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		pVtx[nCntIdxVtx + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[nCntIdxVtx + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[nCntIdxVtx + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[nCntIdxVtx + 3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//====================================
// エフェクトの終了処理
//====================================
void UninitEffect(void)
{
	// 頂点バッファの破棄
	if (s_pVtxBuff != NULL)
	{
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}

	for (int i = 0; i < EFFECT_TYPE_MAX; i++)
	{
		// テクスチャポインタの破棄
		if (s_pTexture[i] != NULL)
		{
			s_pTexture[i]->Release();
			s_pTexture[i] = NULL;
		}
	}
}

//====================================
//エフェクトの更新処理
//====================================
void UpdateEffect(void)
{
	VERTEX_2D *pVtx = NULL;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	Effect* pEffect = s_aEffect;
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pEffect++)
	{
		if (!pEffect->bUse)
		{
			continue;
		}

		pEffect->pos += pEffect->move;
		pEffect->move += pEffect->acceleration;
		pEffect->fSize -= pEffect->fDeltaSize;
		pEffect->col -= pEffect->fDeltaCol;
		pEffect->nLife--;

		if (pEffect->nLife <= 0)
		{
			pEffect->bUse = false;
		}
		if (pEffect->fSize.x <= 0.0f || s_aEffect[nCntEffect].fSize.y <= 0.0f)
		{
			pEffect->bUse = false;
		}

		switch (pEffect->type)
		{
		case EFFECT_TYPE_SLIDING_IMPACT_1:

			break;
		case EFFECT_TYPE_SLIDING_IMPACT_2:
			pEffect->rot += (rand() / (float)RAND_MAX) * (1.5f - 0.0f) + 0.0f;
			break;
		case EFFECT_TYPE_SLIDING_IMPACT_3:
			pEffect->rot += (rand() / (float)RAND_MAX) * (D3DX_PI - -D3DX_PI) + -D3DX_PI;
			break;
		case EFFECT_TYPE_WALL_IMPACT:

			if (pEffect->fSize.x >= 35.0f)
			{
				pEffect->fDeltaSize.x *= -0.5f;
				pEffect->fDeltaSize.y *= -0.5f;
			}

			break;
		}

		pEffect->fLenght = sqrtf((pEffect->fSize.x * pEffect->fSize.x) + (pEffect->fSize.y * pEffect->fSize.y)) * 0.5f;
		pEffect->fAngle = atan2f(pEffect->fSize.x * 0.5f, pEffect->fSize.y * 0.5f);

		int nCntIdxVtx = nCntEffect * 4;
		pVtx[nCntIdxVtx + 0].pos.x = pEffect->pos.x + sinf(pEffect->rot + pEffect->fAngle + (D3DX_PI * 0.75f)) * pEffect->fLenght;
		pVtx[nCntIdxVtx + 0].pos.y = pEffect->pos.y + cosf(pEffect->rot + pEffect->fAngle + (D3DX_PI * 0.75f)) * pEffect->fLenght;
		pVtx[nCntIdxVtx + 0].pos.z = 0.0f;

		pVtx[nCntIdxVtx + 1].pos.x = pEffect->pos.x + sinf(pEffect->rot + pEffect->fAngle + (D3DX_PI * 0.25f)) * pEffect->fLenght;
		pVtx[nCntIdxVtx + 1].pos.y = pEffect->pos.y + cosf(pEffect->rot + pEffect->fAngle + (D3DX_PI * 0.25f)) * pEffect->fLenght;
		pVtx[nCntIdxVtx + 1].pos.z = 0.0f;

		pVtx[nCntIdxVtx + 2].pos.x = pEffect->pos.x + sinf(pEffect->rot + pEffect->fAngle + (-D3DX_PI * 0.75f)) * pEffect->fLenght;
		pVtx[nCntIdxVtx + 2].pos.y = pEffect->pos.y + cosf(pEffect->rot + pEffect->fAngle + (-D3DX_PI * 0.75f)) * pEffect->fLenght;
		pVtx[nCntIdxVtx + 2].pos.z = 0.0f;

		pVtx[nCntIdxVtx + 3].pos.x = pEffect->pos.x + sinf(pEffect->rot + pEffect->fAngle + (-D3DX_PI * 0.25f)) * pEffect->fLenght;
		pVtx[nCntIdxVtx + 3].pos.y = pEffect->pos.y + cosf(pEffect->rot + pEffect->fAngle + (-D3DX_PI * 0.25f)) * pEffect->fLenght;
		pVtx[nCntIdxVtx + 3].pos.z = 0.0f;

		pVtx[nCntIdxVtx + 0].col = pEffect->col;
		pVtx[nCntIdxVtx + 1].col = pEffect->col;
		pVtx[nCntIdxVtx + 2].col = pEffect->col;
		pVtx[nCntIdxVtx + 3].col = pEffect->col;
	}

	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//====================================
//エフェクトの描画処理
//====================================
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//加算合成の設定
	//pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	Effect* pEffect = s_aEffect;
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pEffect++)
	{
		if (!pEffect->bUse)
		{
			continue;
		}

		// テクスチャの設定
		pDevice->SetTexture(0, s_pTexture[pEffect->type]);

		// 描画する
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * 4, 2);
	}

	// 設定を元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	pDevice->SetTexture(0, NULL);
}

//====================================
// エフェクトの設定処理
//====================================
void SetEffect(D3DXVECTOR3 pos, float rot, EFFECT_TYPE Type)
{
	VERTEX_2D *pVtx = NULL;
	
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	Effect* pEffect = s_aEffect;
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pEffect++)
	{
		if (pEffect->bUse)
		{
			continue;
		}

		ZeroMemory(pEffect, sizeof(Effect));

		pEffect->pos = pos;
		pEffect->type = Type;
		pEffect->bUse = true;

		pVtx[(nCntEffect * 4) + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[(nCntEffect * 4) + 3].tex = D3DXVECTOR2(1.0f, 1.0f);

		switch (pEffect->type)
		{
		case EFFECT_TYPE_SLIDING_IMPACT_1:
			pEffect->fSize = D3DXVECTOR3(30.0f, 30.0f, 0.0f);
			pEffect->fDeltaSize = D3DXVECTOR3(2.0f, 2.0f, 0.0f);
			pEffect->nLife = 10;

			break;
		case EFFECT_TYPE_SLIDING_IMPACT_2:
			pEffect->pos.x += (rand() / (float)RAND_MAX) * (40.0f - -40.0f) + -40.0f;
			pEffect->pos.y += (rand() / (float)RAND_MAX) * (40.0f - -40.0f) + -40.0f;
			pEffect->fSize = D3DXVECTOR3(40.0f, 40.0f, 0.0f);
			pEffect->fDeltaSize = D3DXVECTOR3(2.0f, 2.0f, 0.0f);
			pEffect->col = D3DXCOLOR(0.7f, 0.4f, 0.15f, 0.8f);
			pEffect->nLife = 15;
			pEffect->rot = (rand() / (float)RAND_MAX) * (1.5f - 0.0f) + 0.0f;
			break;
		case EFFECT_TYPE_SLIDING_IMPACT_3:
			pEffect->pos.x += (rand() / (float)RAND_MAX) * (40.0f - -40.0f) + -40.0f;
			pEffect->pos.y += (rand() / (float)RAND_MAX) * (40.0f - -40.0f) + -40.0f;
			pEffect->fSize = D3DXVECTOR3(40.0f, 40.0f, 0.0f);
			pEffect->fDeltaSize = D3DXVECTOR3(4.0f, 4.0f, 0.0f);
			pEffect->col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			pEffect->nLife = 15;
			pEffect->rot = (rand() / (float)RAND_MAX) * (D3DX_PI - -D3DX_PI) + -D3DX_PI;
			break;
		case EFFECT_TYPE_WALL_IMPACT:
			pEffect->pos.x = pos.x;
			pEffect->pos.y = pos.y;
			pEffect->move = D3DXVECTOR3(sinf(rot), cosf(rot), 0.0f);
			pEffect->fSize = D3DXVECTOR3(10.0f, 10.0f, 0.0f);
			pEffect->fDeltaSize = D3DXVECTOR3(-0.5f, -0.5f, 0.0f);
			pEffect->col = D3DXCOLOR(0.25f, 0.75f, 0.75f, 1.0f);
			pEffect->fDeltaCol = D3DXCOLOR(-0.02f, 0.0f, 0.01f, 0.0025f);
			pEffect->nLife = 30;

			if (pos.y < SCREEN_HEIGHT * 0.5f)
			{
				pEffect->rot = rot - D3DX_PI * 0.25f;
			}
			else
			{
				pEffect->rot = rot - D3DX_PI * 1.25f;
			}

			break;
		}

		int nCntIdxVtx = nCntEffect * 4;
		pEffect->fLenght = sqrtf((pEffect->fSize.x * pEffect->fSize.x) + (pEffect->fSize.y * pEffect->fSize.y)) * 0.5f;
		pEffect->fAngle = atan2f(pEffect->fSize.x * 0.5f, pEffect->fSize.y * 0.5f);

		pVtx[nCntIdxVtx + 0].pos.x = pEffect->pos.x + sinf(pEffect->rot + pEffect->fAngle + (D3DX_PI * 0.75f)) * pEffect->fLenght;
		pVtx[nCntIdxVtx + 0].pos.y = pEffect->pos.y + cosf(pEffect->rot + pEffect->fAngle + (D3DX_PI * 0.75f)) * pEffect->fLenght;
		pVtx[nCntIdxVtx + 0].pos.z = 0.0f;

		pVtx[nCntIdxVtx + 1].pos.x = pEffect->pos.x + sinf(pEffect->rot + pEffect->fAngle + (D3DX_PI * 0.25f)) * pEffect->fLenght;
		pVtx[nCntIdxVtx + 1].pos.y = pEffect->pos.y + cosf(pEffect->rot + pEffect->fAngle + (D3DX_PI * 0.25f)) * pEffect->fLenght;
		pVtx[nCntIdxVtx + 1].pos.z = 0.0f;

		pVtx[nCntIdxVtx + 2].pos.x = pEffect->pos.x + sinf(pEffect->rot + pEffect->fAngle + (-D3DX_PI * 0.75f)) * pEffect->fLenght;
		pVtx[nCntIdxVtx + 2].pos.y = pEffect->pos.y + cosf(pEffect->rot + pEffect->fAngle + (-D3DX_PI * 0.75f)) * pEffect->fLenght;
		pVtx[nCntIdxVtx + 2].pos.z = 0.0f;

		pVtx[nCntIdxVtx + 3].pos.x = pEffect->pos.x + sinf(pEffect->rot + pEffect->fAngle + (-D3DX_PI * 0.25f)) * pEffect->fLenght;
		pVtx[nCntIdxVtx + 3].pos.y = pEffect->pos.y + cosf(pEffect->rot + pEffect->fAngle + (-D3DX_PI * 0.25f)) * pEffect->fLenght;
		pVtx[nCntIdxVtx + 3].pos.z = 0.0f;

		break;
	}
	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}