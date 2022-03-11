//------------------------------
//設定
//ACG000
//Author: goto yuuki
//
//------------------------------

#include "option.h"
#include "sound.h"
#include "fade.h"

//------------------------------
//グローバル変数
//------------------------------

LPDIRECT3DTEXTURE9 g_pTextureOption[MAX_OPTION] = {};		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffOption = NULL;			//頂点バッファへのポインタ
D3DXVECTOR3 g_OptionPos;									//オプションの座標
static int s_nSelectTime;
static int *p_nSelectTime;
void InitOption(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

									//デバイスの取得
	pDevice = GetDevice();

	s_nSelectTime = 0;
	*p_nSelectTime;
	p_nSelectTime = &s_nSelectTime;

	//グローバル変数の初期化
	g_OptionPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"TEXTURE/背景",
		&g_pTextureOption[0]);

	D3DXCreateTextureFromFile(pDevice,
		"TEXTURE/タイマー",
		&g_pTextureOption[1]);

	D3DXCreateTextureFromFile(pDevice,
		"TEXTURE/ポイント数",
		&g_pTextureOption[2]);

	D3DXCreateTextureFromFile(pDevice,
		"TEXTURE/セット数",
		&g_pTextureOption[3]);

	VERTEX_2D * pVtx;		//頂点情報へのポインタ

							//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 16,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffOption,
		NULL);

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffOption->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	pVtx[4].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 - 500.0f, 50.0f, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 + 500.0f, 50.0f, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 - 500.0f, 50.0f + 190.0f, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 + 500.0f, 50.0f + 190.0f, 0.0f);

	pVtx[8].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 - 500.0f, 240.0f, 0.0f);
	pVtx[9].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 + 500.0f, 240.0f, 0.0f);
	pVtx[10].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 - 500.0f, 240.0f + 190.0f, 0.0f);
	pVtx[11].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 + 500.0f, 240.0f + 190.0f, 0.0f);

	pVtx[12].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 - 500.0f, 430.0f, 0.0f);
	pVtx[13].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 + 500.0f, 430.0f, 0.0f);
	pVtx[14].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 - 500.0f, 620.0f, 0.0f);
	pVtx[15].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 + 500.0f, 620.0f, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[4].rhw = 1.0f;
	pVtx[5].rhw = 1.0f;
	pVtx[6].rhw = 1.0f;
	pVtx[7].rhw = 1.0f;

	pVtx[8].rhw = 1.0f;
	pVtx[9].rhw = 1.0f;
	pVtx[10].rhw = 1.0f;
	pVtx[11].rhw = 1.0f;

	pVtx[12].rhw = 1.0f;
	pVtx[13].rhw = 1.0f;
	pVtx[14].rhw = 1.0f;
	pVtx[15].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	pVtx[8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[9].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[10].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[11].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	pVtx[12].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[13].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[14].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[15].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx[8].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[9].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[10].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[11].tex = D3DXVECTOR2(1.0f, 1.0f);


	pVtx[12].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[13].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[14].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[15].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffOption->Unlock();

	//サウンドの再生
	//PlaySound(SOUND_LABEL_BGM000);
}
void UninitOption(void)
{
	//サウンドの停止
	//StopSound();

	for (int nCntOption = 0; nCntOption < MAX_OPTION; nCntOption++)
	{//テクスチャの破棄
		if (g_pTextureOption[nCntOption] != NULL)
		{
			g_pTextureOption[nCntOption]->Release();
			g_pTextureOption[nCntOption] = NULL;
		}
	}

	if (g_pVtxBuffOption != NULL)
	{//頂点バッファの破棄
		g_pVtxBuffOption->Release();
		g_pVtxBuffOption = NULL;
	}
}
void UpdateOption(void)
{
	if (GetKeyboardTrigger(DIK_A)|| GetKeyboardTrigger(DIK_LEFT)
	&& (s_nSelectTime > 0 && s_nSelectTime <= 5))
	{
	s_nSelectTime--;
	&s_nSelectTime;
	}
	if (GetKeyboardTrigger(DIK_D) || GetKeyboardTrigger(DIK_RIGHT)
	&& s_nSelectTime <= 0 && s_nSelectTime < 5)
	{
	s_nSelectTime++;
	&s_nSelectTime;
	}
}
void DrawOption(void)
{

	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

									//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffOption, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureOption[0]);

	//ポリゴンの描画       
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,	//ポリゴンの形
		0,						//頂点の開始場所
		2);						//プリミティブの数

								//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureOption[1]);

	//ポリゴンの描画       
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,	//ポリゴンの形
		4,						//頂点の開始場所
		2);						//プリミティブの数

								//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureOption[2]);

	//ポリゴンの描画       
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,	//ポリゴンの形
		8,						//頂点の開始場所
		2);						//プリミティブの数

								//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureOption[3]);

	//ポリゴンの描画       
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,	//ポリゴンの形
		12,						//頂点の開始場所
		2);						//プリミティブの数


}
void SetOption(void)
{

}