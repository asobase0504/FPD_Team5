//------------------------------
//スコア設定
//ACG000
//Author: goto yuuki
//
//------------------------------
#include"main.h"
#include"score.h"

//マクロ定義
#define NUM_SCORE (8)		//桁数

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureScore = NULL;				//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;			//頂点バッファへのポインタ
D3DXVECTOR3 g_posScore;		//スコアの位置
int g_nScore;				//スコアの値

							//------------------------------
							//スコアの初期化処理
							//------------------------------
void InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

									//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"TEXTURE/number000.png",
		&g_pTextureScore);

	g_posScore = D3DXVECTOR3(900.0f, 30.0f, 0.0f);			//位置を初期化する
	g_nScore = 0;											//値を初期化する
	int nCntScore;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		(sizeof(VERTEX_2D) * 4 * NUM_SCORE),		//4つで画像一個分
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL);

	VERTEX_2D*pVtx;		//頂点情報へのポインタ

						//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	//スコアの情報の初期化
	for (nCntScore = 0; nCntScore < NUM_SCORE; nCntScore++)
	{

		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(g_posScore.x - 30, g_posScore.y - 30, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posScore.x + 30, g_posScore.y - 30, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posScore.x - 30, g_posScore.y + 30, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posScore.x + 30, g_posScore.y + 30, 0.0f);

		g_posScore += D3DXVECTOR3(50.0f, 0.0f, 0.0f);

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

	//頂点バッファをアンロックする
	g_pVtxBuffScore->Unlock();
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

	//頂点バッファの破棄
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}
}

//------------------------------
//スコアの更新処理
//------------------------------
void UpdateScore(void)
{
	SetScore(g_nScore);
}

//------------------------------
//スコアの描画処理
//------------------------------
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntScore;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

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

//------------------------------
//スコアのセット処理
//------------------------------
void SetScore(int nScore)
{
	int nCntScore;
	int aPosTexU[8];	//各桁の数字を格納

	g_nScore = nScore;

	aPosTexU[0] = g_nScore / 10000000;
	aPosTexU[1] = (g_nScore % 10000000) / 1000000;
	aPosTexU[2] = (g_nScore % 1000000) / 100000;
	aPosTexU[3] = (g_nScore % 100000) / 10000;
	aPosTexU[4] = (g_nScore % 10000) / 1000;
	aPosTexU[5] = (g_nScore % 1000) / 100;
	aPosTexU[6] = (g_nScore % 100) / 10;
	aPosTexU[7] = (g_nScore % 10) / 1;

	VERTEX_2D*pVtx; //頂点へのポインタ	

					//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

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
	g_pVtxBuffScore->Unlock();

	//GetRanking(g_nScore);
}

//------------------------------
//スコアの加算処理
//------------------------------
void AddScore(int nValse)
{
	int nCntScore;
	int aPosTexU[8]; //各桁の数字を格納

	g_nScore += nValse;

	aPosTexU[0] = g_nScore / 10000000;
	aPosTexU[1] = (g_nScore % 10000000) / 1000000;
	aPosTexU[2] = (g_nScore % 1000000) / 100000;
	aPosTexU[3] = (g_nScore % 100000) / 10000;
	aPosTexU[4] = (g_nScore % 10000) / 1000;
	aPosTexU[5] = (g_nScore % 1000) / 100;
	aPosTexU[6] = (g_nScore % 100) / 10;
	aPosTexU[7] = (g_nScore % 10) / 1;

	VERTEX_2D*pVtx; //頂点へのポインタ

					//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

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
	g_pVtxBuffScore->Unlock();

}

//スコアの取得
int GetScore(void)
{
	return g_nScore;
}