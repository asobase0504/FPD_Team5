//=========================================
// 
// ポイントの表示
// Author YudaKaito
// 
//=========================================
//------------------------------------
// include
//------------------------------------
#include "point.h"
#include "score.h"

//------------------------------------
// マクロ定義
//------------------------------------
#define NUM_POINT	(2)
#define DIGIT_POINT	(2)

//------------------------------------
// 静的変数
//------------------------------------
static LPDIRECT3DTEXTURE9 g_pTextureNumber = NULL;			//テクスチャへのポインタ
static Point s_aPoint[NUM_POINT];
static bool s_bPointUse;
static int s_nUseTime;

//------------------------------------
// プロトタイプ宣言
//------------------------------------
static void DestroyPoint(void);

//=========================================
// 初期化
//=========================================
void InitPoint(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number.png", &g_pTextureNumber);

	Point* pPoint = s_aPoint;

	for (int nCntScore = 0; nCntScore < NUM_POINT; nCntScore++, pPoint++)
	{
		//頂点バッファの生成
		pDevice->CreateVertexBuffer(
			(sizeof(VERTEX_2D) * 4 * 2),		//4つで画像一個分
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&pPoint->pVtxBuff,
			NULL);

		pPoint->pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f - 400.0f + 700.0f * nCntScore, SCREEN_HEIGHT * 0.5f, 0.0f);
		pPoint->bUse = false;
		s_bPointUse = false;
		s_nUseTime = 0;

		VERTEX_2D*pVtx;		//頂点情報へのポインタ

		//頂点バッファをロックし、頂点情報へのポインタを取得
		pPoint->pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntDigit = 0; nCntDigit < DIGIT_POINT; nCntDigit++ , pVtx += 4)
		{

			pVtx[0].pos = D3DXVECTOR3(pPoint->pos.x - 55.0f + nCntDigit * 115.0f, pPoint->pos.y - 60.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pPoint->pos.x + 55.0f + nCntDigit * 115.0f, pPoint->pos.y - 60.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pPoint->pos.x - 55.0f + nCntDigit * 115.0f, pPoint->pos.y + 60.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pPoint->pos.x + 55.0f + nCntDigit * 115.0f, pPoint->pos.y + 60.0f, 0.0f);

			//RHWの設定
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 0.5f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 0.5f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 0.5f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 0.5f, 1.0f);

			//テクスチャの座標設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
		}

		//頂点バッファをアンロックする
		pPoint->pVtxBuff->Unlock();
	}
}

//=========================================
// 終了
//=========================================
void UninitPoint(void)
{
}

//=========================================
// 更新
//=========================================
void UpdatePoint(void)
{
	if (s_bPointUse)
	{
		s_nUseTime++;

		if (s_nUseTime >= 120)
		{
			s_bPointUse = false;
		}
	}
}

//=========================================
// 描画
//=========================================
void DrawPoint(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ

	Point* pPoint = s_aPoint;

	for (int nCntPoint = 0; nCntPoint < NUM_POINT; nCntPoint++, pPoint++)
	{
		if (!s_bPointUse)
		{
			continue;
		}

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, pPoint->pVtxBuff, 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureNumber);

		for (int nCntDigit = 0; nCntDigit < DIGIT_POINT; nCntDigit++)
		{
			//ポリゴンの描画
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,		//プリミティブの種類
				nCntDigit * 4,				//描画する最初の頂点インデックス
				2);							//プリミティブ（ポリゴン）の数
		}
	}
}

//=========================================
// 設定
//=========================================
void SetPoint(void)
{
	SCORE* pScore = GetScore();
	Point* pPoint = s_aPoint;
	s_bPointUse = true;

	for (int i = 0; i < NUM_POINT; i++, pScore++, pPoint++)
	{
		pPoint->nScore = pScore->nScore[0];

		int aNumDigit = pPoint->nScore;

		VERTEX_2D*pVtx;		//頂点情報へのポインタ

		//頂点バッファをロックし、頂点情報へのポインタを取得
		pPoint->pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		//テクスチャの座標設定
		int aData = aNumDigit * 0.1f;
		pVtx[0].tex = D3DXVECTOR2(aData % 10 * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2((aData + 1) % 10 * 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(aData % 10 * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2((aData + 1) % 10 * 0.1f, 1.0f);

		pVtx[4].tex = D3DXVECTOR2(aNumDigit % 10 * 0.1f, 0.0f);
		pVtx[5].tex = D3DXVECTOR2((aNumDigit + 1) % 10 * 0.1f, 0.0f);
		pVtx[6].tex = D3DXVECTOR2(aNumDigit % 10 * 0.1f, 1.0f);
		pVtx[7].tex = D3DXVECTOR2((aNumDigit + 1) % 10 * 0.1f, 1.0f);

		//頂点バッファをアンロックする
		pPoint->pVtxBuff->Unlock();
	}
}

//=========================================
// 破壊
//=========================================
void DestroyPoint(void)
{
	Point* pPoint = s_aPoint;
}
