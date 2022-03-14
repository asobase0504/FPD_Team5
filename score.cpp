//------------------------------
//スコア設定
//ACG000
//Author: goto yuuki
//
//------------------------------
#include"main.h"
#include"score.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureScore = NULL;				//テクスチャへのポインタ
SCORE	g_nScore[2];									//スコアの構造体

//------------------------------
//スコアの初期化処理
//------------------------------
void InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスへのポインタ

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/gotou/TEXTURE/number000.png",
		&g_pTextureScore);

	VERTEX_2D*pVtx;		//頂点情報へのポインタ

	int nScoreCnt;
	for (nScoreCnt = 0; nScoreCnt < NUM_SCORE; nScoreCnt++)
	{
		g_nScore[nScoreCnt].nScore = 30;															//値を初期化する
		g_nScore[nScoreCnt].nCnt = 0;																//値を初期化する
		g_nScore[nScoreCnt].pos = D3DXVECTOR3(500.0f, 50.0f, 0.0f);			//位置を初期化する


	//頂点バッファの生成
		pDevice->CreateVertexBuffer(
			(sizeof(VERTEX_2D) * 4 * 2 * NUM_SCORE),		//4つで画像一個分
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&g_nScore[nScoreCnt].pVtxBuff,
			NULL);

		//頂点バッファをロックし、頂点情報へのポインタを取得
		g_nScore[nScoreCnt].pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	}
	for (nScoreCnt = 0; nScoreCnt < 2; nScoreCnt++)
	{
		//スコアの情報の初期化
		for (int nPosCnt = 0; nPosCnt < NUM_SCORE; nPosCnt++)
		{
			//頂点座標
			pVtx[0].pos = D3DXVECTOR3(g_nScore[nScoreCnt].pos.x - 30.0f + nPosCnt * 70.0f, g_nScore[nScoreCnt].pos.y - 30, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_nScore[nScoreCnt].pos.x + 30.0f + nPosCnt * 70.0f, g_nScore[nScoreCnt].pos.y - 30, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_nScore[nScoreCnt].pos.x - 30.0f + nPosCnt * 70.0f, g_nScore[nScoreCnt].pos.y + 30, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_nScore[nScoreCnt].pos.x + 30.0f + nPosCnt * 70.0f, g_nScore[nScoreCnt].pos.y + 30, 0.0f);

			if (nPosCnt == 1 || nPosCnt == 2)
			{
				g_nScore[nScoreCnt].pos += D3DXVECTOR3(60.0f, 0.0f, 0.0f);
				if (nPosCnt == 2)
				{
					g_nScore[nScoreCnt].pos -= D3DXVECTOR3(60.0f, 0.0f, 0.0f);
				}
			}

			//RHWの設定
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//テクスチャの座標設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

			pVtx += 4; //頂点ポイントを四つ進む
		}

	}
	//頂点バッファをアンロックする
	g_nScore[nScoreCnt].pVtxBuff->Unlock();
}

//------------------------------
//スコアの終了処理
//------------------------------
void UninitScore(void)
{
	//テクスチャの破棄
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}

	for (int nScoreCnt = 0; nScoreCnt < NUM_SCORE; nScoreCnt++)
	{
		//頂点バッファの破棄
		if (g_nScore[nScoreCnt].pVtxBuff != NULL)
		{
			g_nScore[nScoreCnt].pVtxBuff->Release();
			g_nScore[nScoreCnt].pVtxBuff = NULL;
		}
	}
}

//------------------------------
//スコアの更新処理
//------------------------------
void UpdateScore(void)
{
	SetScore(g_nScore[0].nScore);
}

//------------------------------
//スコアの描画処理
//------------------------------
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntScore;

	for (int nScoreCnt = 0; nScoreCnt < NUM_SCORE; nScoreCnt++)
	{
		//デバイスの取得
		pDevice = GetDevice();

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_nScore[nScoreCnt].pVtxBuff, 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		for (nCntScore = 0; nCntScore < NUM_SCORE; nCntScore++)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureScore);

			//ポリゴンの描画
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,		//プリミティブの種類
				nCntScore * 4,				//描画する最初の頂点インデックス
				2);							//プリミティブ（ポリゴン）の数
		}
	}
}

//------------------------------
//スコアのセット処理
//------------------------------
void SetScore(int nScore)
{
	int nCntScore;
	int aPosTexU[4];	//各桁の数字を格納

	g_nScore[2].nScore = nScore;

	aPosTexU[0] = (g_nScore[0].nScore % 100) / 10;
	aPosTexU[1] = (g_nScore[1].nScore % 10) / 1;
	aPosTexU[2] = (g_nScore[0].nScore % 100) / 10;
	aPosTexU[3] = (g_nScore[1].nScore % 10) / 1;

	//aPosTexU[0] = (g_nScore[2].nScore % 100) / 10;
	//aPosTexU[0] = (g_nScore[3].nScore % 10) / 1;

	for (int nScoreCnt = 0; nScoreCnt < NUM_SCORE; nScoreCnt++)
	{
		//aPosTexU[0] = g_nScore.nScore / 10000000;
		//aPosTexU[1] = (g_nScore.nScore % 10000000) / 1000000;
		//aPosTexU[2] = (g_nScore.nScore % 1000000) / 100000;
		//aPosTexU[3] = (g_nScore.nScore % 100000) / 10000;
		//aPosTexU[4] = (g_nScore.nScore % 10000) / 1000;
		//aPosTexU[5] = (g_nScore.nScore % 1000) / 100;
		//aPosTexU[6] = (g_nScore.nScore % 100) / 10;
		//aPosTexU[7] = (g_nScore.nScore % 10) / 1;

		VERTEX_2D*pVtx; //頂点へのポインタ	

						//頂点バッファをロックし頂点情報へのポインタを取得
		g_nScore[nScoreCnt].pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (nCntScore = 0; nCntScore < NUM_SCORE; nCntScore++)
		{
			//テクスチャの座標設定
			pVtx[0].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntScore], 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntScore] + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntScore], 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntScore] + 0.1f, 1.0f);
			pVtx += 4; //頂点ポイントを四つ進む
		}
		//頂点バッファをアンロック
		g_nScore[nScoreCnt].pVtxBuff->Unlock();

		//GetRanking(g_nScore);
	}
}

//------------------------------
//スコアの加算処理
//------------------------------
void AddScore(int nValse)
{
	int nCntScore;
	int aPosTexU[4]; //各桁の数字を格納

	g_nScore[2].nScore += nValse;

	aPosTexU[0] = (g_nScore[0].nScore % 100) / 10;
	aPosTexU[1] = (g_nScore[1].nScore % 10) / 1;
	aPosTexU[2] = (g_nScore[2].nScore % 100) / 10;
	aPosTexU[3] = (g_nScore[3].nScore % 10) / 1;

	for (int nScoreCnt = 0; nScoreCnt < NUM_SCORE; nScoreCnt++)
	{
		//aPosTexU[0] = g_nScore.nScore / 10000000;
		//aPosTexU[1] = (g_nScore.nScore % 10000000) / 1000000;
		//aPosTexU[2] = (g_nScore.nScore % 1000000) / 100000;
		//aPosTexU[3] = (g_nScore.nScore % 100000) / 10000;
		//aPosTexU[4] = (g_nScore.nScore % 10000) / 1000;
		//aPosTexU[5] = (g_nScore.nScore % 1000) / 100;
		//aPosTexU[6] = (g_nScore.nScore % 100) / 10;
		//aPosTexU[7] = (g_nScore.nScore % 10) / 1;

		VERTEX_2D*pVtx; //頂点へのポインタ

						//頂点バッファをロックし頂点情報へのポインタを取得
		g_nScore[nScoreCnt].pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (nCntScore = 0; nCntScore < NUM_SCORE; nCntScore++)
		{

			//テクスチャの座標設定
			pVtx[0].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntScore], 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntScore] + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntScore], 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntScore] + 0.1f, 1.0f);
			pVtx += 4; //頂点ポイントを四つ進む

		}

		//頂点バッファをアンロック
		g_nScore[nScoreCnt].pVtxBuff->Unlock();
	}
}

//スコアの取得
SCORE GetScore(void)
{
	return g_nScore[0];
}

//SCORE2 GetScore2(void)
//{
//	return g_cScore;
//}