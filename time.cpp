//------------------------------
//時間設定
//
//Author: goto yuuki
//
//------------------------------
#include "time.h"
#include "math.h"								//∞を生み出す変数

//グローバル変数
static LPDIRECT3DTEXTURE9 g_pTexture = NULL;			//テクスチャヘノポインタ
static LPDIRECT3DVERTEXBUFFER9 g_pVtxBuff = NULL;		//頂点バッファへのポインタ
static TIME g_Time;										//タイム構造体

//タイムの初期化処理
void InitTime(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/number000.png",
		&g_pTexture);

	bool bTimeFlag = false;									//タイトルの移動のフラグ
	g_Time.nTime =30;										//タイムの値
	g_Time.nCntTime = 0;									//タイムカウンター値
	g_Time.nMinusTime =0;										//タイムを引く数

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_CNT_TIME,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuff,
		NULL);

	VERTEX_2D*pVtx;									//頂点情報へのポインタ

	//タイムをずらして描画する設定

	for (g_Time.nCntTime = 0; g_Time.nCntTime < MAX_CNT_TIME; g_Time.nCntTime++)
	{
		g_Time.pos[g_Time.nCntTime] = D3DXVECTOR3(594.0f + (20.0f + (20.0f * 2) * g_Time.nCntTime), 0.0f + 60.0f, 0.0f);
	}

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (g_Time.nCntTime = 0; g_Time.nCntTime < MAX_CNT_TIME; g_Time.nCntTime++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_Time.pos[g_Time.nCntTime].x - 20.0f, g_Time.pos[g_Time.nCntTime].y - 35.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Time.pos[g_Time.nCntTime].x + 20.0f, g_Time.pos[g_Time.nCntTime].y - 35.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Time.pos[g_Time.nCntTime].x - 20.0f, g_Time.pos[g_Time.nCntTime].y + 35.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Time.pos[g_Time.nCntTime].x + 20.0f, g_Time.pos[g_Time.nCntTime].y + 35.0f, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;		//頂点データのポインタを4つ分進める
	}
	//頂点バッファをアンロック
	g_pVtxBuff->Unlock();
}
//タイムの終了処理
void UninitTime(void)
{
	//テクスチャの破棄
	if (g_pTexture != NULL)
	{
		g_pTexture->Release();
		g_pTexture = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuff != NULL)
	{
		g_pVtxBuff->Release();
		g_pVtxBuff = NULL;
	}
}

//タイムの更新処理
void UpdateTime(void)
{
	int aPosTexU[2];			//各桁の数字を格納
	g_Time.nMinusTime++;				//nCntTimeが引かれていく
	if (g_Time.nTime != 0)
	{

		if (g_Time.nMinusTime % 60 == 0)		//nCntTimeが0より小さくなったら
		{
			g_Time.nTime--;
			//g_nTime[s_nSelectTime]--;				//時間が一秒ずつ減っていく
		}
	}
	aPosTexU[0] = g_Time.nTime % 100 / 10;
	aPosTexU[1] = g_Time.nTime % 10;
	//aPosTexU[2] = g_Time.nTime % 10;

	//aPosTexU[0] = g_nTime[s_nSelectTime] % 1000 / 100;
	//aPosTexU[1] = g_nTime[s_nSelectTime] % 100 / 10;
	//aPosTexU[2] = g_nTime[s_nSelectTime] % 10;

	VERTEX_2D*pVtx;												//頂点情報へのポインタ

																//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (g_Time.nCntTime = 0; g_Time.nCntTime < MAX_CNT_TIME; g_Time.nCntTime++)
	{
		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f + (aPosTexU[g_Time.nCntTime] * 0.1f), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (aPosTexU[g_Time.nCntTime] * 0.1f), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (aPosTexU[g_Time.nCntTime] * 0.1f), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (aPosTexU[g_Time.nCntTime] * 0.1f), 1.0f);

		pVtx += 4;
	}
	//頂点バッファをアンロック
	g_pVtxBuff->Unlock();

	/*if (g_nTime <= 0)
	{
	if (GetFade() == FADE_NONE)
	{
	ChangeMode(MODE_RESULT);
	}
	}*/

}

//タイムの描画処理
void DrawTime(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (g_Time.nCntTime = 0; g_Time.nCntTime < MAX_CNT_TIME; g_Time.nCntTime++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTexture);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			4 * g_Time.nCntTime,
			2);
	}
}

//
// 設定
//
void SetTime(int nTime)
{
	g_Time.nTime = nTime;
}

TIME *GetTime(void)
{
	return &g_Time;		//「&」を使うのはポインタを使用してるから
}