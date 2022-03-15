//============================================
//
//チーム制作<フライングパワーディスク>[select.cpp]
//Author:Kishimoto Eiji
//
//============================================

#include "select.h"
#include "option.h"
#include "input.h"

#include <assert.h>

//***********************************
//マクロ定義
//***********************************
#define MAX_TEXTURE		(3)			//使用するテクスチャの最大数
#define MAX_TIMELIMIT	(5)			//制限時間の選択肢の最大数
#define MAX_POINT		(4)			//ポイント数の選択肢の最大数
#define MAX_SETCOUNT	(4)			//セット数の選択肢の最大数

#define TEXTURE_TIMELIMIT	("data/TEXTURE/制限時間")		//制限時間のテクスチャ
#define TEXTURE_POINT		("data/TEXTURE/ポイント数")		//ポイント数のテクスチャ
#define TEXTURE_SETCOUNT	("data/TEXTURE/セット数")		//セット数のテクスチャ

//***********************************
//スタティック変数
//***********************************
static LPDIRECT3DTEXTURE9		s_apTexture[MAX_TEXTURE];	//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuff = NULL;			//頂点バッファへのポインタ

static int s_nSelectTimeLimit;	//選ばれている制限時間
static int s_nSelectPoint;		//選ばれているポイント数
static int s_nSelectSetCount;	//選ばれているセット数

static int s_nTimeLimit;	//制限時間を保存する
static int s_nPoint;		//ポイント数を保存する
static int s_nSetCount;		//セット数を保存する

//***********************************
//プロトタイプ宣言
//***********************************

static void ChangeTexture(int nSelectMenu,int nSelectOption);

//============================================
//セレクトの初期化
//============================================
void InitSelect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
								TEXTURE_TIMELIMIT,
								&s_apTexture[0]);	//制限時間

	D3DXCreateTextureFromFile(pDevice,
								TEXTURE_POINT,
								&s_apTexture[1]);	//ポイント数

	D3DXCreateTextureFromFile(pDevice,
								TEXTURE_SETCOUNT,
								&s_apTexture[2]);	//セット数

	//変数の初期化
	s_nSelectTimeLimit = 0;
	s_nSelectPoint = 0;
	s_nSelectSetCount = 0;
	s_nTimeLimit = 0;
	s_nPoint = 0;
	s_nSetCount = 0;

	VERTEX_2D * pVtx = NULL;	//頂点情報へのポインタ

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TEXTURE,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&s_pVtxBuff,
								NULL);

	//頂点バッファをロックし、頂点データへのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i > MAX_TEXTURE; i++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

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

		//テクスチャ座標の設定------>後で変える
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;		//4つ分進める
	}

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//============================================
//セレクトの終了
//============================================
void UninitSelect(void)
{
	for (int i = 0; i > MAX_TEXTURE; i++)
	{
		if (s_apTexture[i] != NULL)
		{//テクスチャの破棄
			s_apTexture[i]->Release();
			s_apTexture[i] = NULL;
		}
	}

	if (s_pVtxBuff != NULL)
	{//頂点バッファの破棄
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//============================================
//セレクトの更新
//============================================
void UpdateSelect(void)
{
	
}

//============================================
//セレクトの描画
//============================================
void DrawSelect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i > MAX_TEXTURE; i++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, s_apTexture[i]);

		//ポリゴンの描画       
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//ポリゴンの形
								i * 4,				//頂点の開始場所
								2);					//プリミティブの数
	}
}

//============================================
// 制限時間の選択
// int nSelectMenu ---> 選択しているメニュー
//============================================
int SelectTimeLimit(int nSelectMenu)
{
	if (GetKeyboardTrigger(DIK_A) || GetJoypadTrigger(JOYKEY_LEFT, 0))
	{// 左を入力( Aキー or 十字キー左 )

		//選択肢を一つ前(左)にする
		s_nSelectTimeLimit = ((s_nSelectTimeLimit - 1) + TIMELIMIT_MAX) % TIMELIMIT_MAX;
	}
	else if (GetKeyboardTrigger(DIK_D) || GetJoypadTrigger(JOYKEY_RIGHT, 0))
	{// 右を入力( Dキー or 十字キー右 )
		
		//選択肢を一つ後(右)にする
		s_nSelectTimeLimit = ((s_nSelectTimeLimit + 1) + TIMELIMIT_MAX) % TIMELIMIT_MAX;
	}

	int nTimeLimit = 0;		//値を返す用

	switch (s_nSelectTimeLimit)
	{
	case TIMELIMIT_15:		//15秒
		nTimeLimit = 15;	//一時保存
		break;

	case TIMELIMIT_30:		//30秒
		nTimeLimit = 30;	//一時保存
		break;

	case TIMELIMIT_45:		//45秒
		nTimeLimit = 45;	//一時保存
		break;

	case TIMELIMIT_90:		//90秒
		nTimeLimit = 90;	//一時保存
		break;

	case TIMELIMIT_INFINITY:	//無限(制限時間なし)
		nTimeLimit = 99;		//一時保存(仮で送る値)
		break;

	default:
		assert(false);
		break;
	}

	//テクスチャ位置の切り替え
	ChangeTexture(nSelectMenu, s_nSelectTimeLimit);

	return nTimeLimit;	//設定した値を返す
}

//============================================
// ポイント数の選択
// int nSelectMenu ---> 選択しているメニュー
//============================================
int SelectPoint(int nSelectMenu)
{
	if (GetKeyboardTrigger(DIK_A) || GetJoypadTrigger(JOYKEY_LEFT, 0))
	{// 左を入力( Aキー or 十字キー左 )

		//選択肢を一つ前(左)にする
		s_nSelectPoint = ((s_nSelectPoint - 1) + POINTCOUNT_MAX) % POINTCOUNT_MAX;
	}
	else if (GetKeyboardTrigger(DIK_D) || GetJoypadTrigger(JOYKEY_RIGHT, 0))
	{// 右を入力( Dキー or 十字キー右 )
		
		//選択肢を一つ後(右)にする
		s_nSelectPoint = ((s_nSelectPoint + 1) + POINTCOUNT_MAX) % POINTCOUNT_MAX;
	}

	int nPoint = 0; 	//値を返す用

	switch (s_nSelectPoint)
	{
	case POINTCOUNT_12:	//12点
		nPoint = 12;	//一時保存
		break;

	case POINTCOUNT_15:	//15点
		nPoint = 15;	//一時保存
		break;

	case POINTCOUNT_21:	//21点
		nPoint = 21;	//一時保存
		break;

	case POINTCOUNT_25:	//25点
		nPoint = 25;	//一時保存
		break;

	default:
		assert(false);
		break;
	}

	//テクスチャ位置の切り替え
	ChangeTexture(nSelectMenu, s_nSelectPoint);

	return nPoint;	//設定した値を返す
}

//============================================
// セット数の選択
// int nSelectMenu ---> 選択しているメニュー
//============================================
int SelectSetCount(int nSelectMenu)
{
	if (GetKeyboardTrigger(DIK_A) || GetJoypadTrigger(JOYKEY_LEFT, 0))
	{// 左を入力( Aキー or 十字キー左 )

		//選択肢を一つ前(左)にする
		s_nSelectSetCount = ((s_nSelectSetCount - 1) + SETCOUNT_MAX) % SETCOUNT_MAX;
	}
	else if (GetKeyboardTrigger(DIK_D) || GetJoypadTrigger(JOYKEY_RIGHT, 0))
	{// 右を入力( Dキー or 十字キー右 )
		
		//選択肢を一つ後(右)にする
		s_nSelectSetCount = ((s_nSelectSetCount + 1) + SETCOUNT_MAX) % SETCOUNT_MAX;
	}

	int nSetCount = 0;	//値を返す用

	switch (s_nSelectSetCount)
	{
	case SETCOUNT_ONE:		//1セット
		nSetCount = 1;		//一時保存
		break;

	case SETCOUNT_TWO:		//2セット
		nSetCount = 2;		//一時保存
		break;

	case SETCOUNT_THREE:	//3セット
		nSetCount = 3;		//一時保存
		break;

	case SETCOUNT_FOUR:		//4セット
		nSetCount = 4;		//一時保存
		break;

	default:
		assert(false);
		break;
	}

	//テクスチャ位置の切り替え
	ChangeTexture(nSelectMenu, s_nSelectSetCount);

	return nSetCount;	//設定した値を返す
}

//--------------------------------------------
// テクスチャを切り替える
// int nSelectMenu -----> 選択しているメニュー
// int nSelectOption ---> 選択しているオプション
//--------------------------------------------
static void ChangeTexture(int nSelectMenu, int nSelectOption)
{
	VERTEX_2D * pVtx = NULL;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (nSelectMenu * 4);		//指定の位置まで進める

	float fDivid,fTexV;		//テクスチャ座標設定用

	switch (nSelectMenu)
	{
	case OPTION_TIMELIMIT:		//制限時間

		fDivid = (1.0f / MAX_TIMELIMIT);	//テクスチャに合わせて分割
		fTexV = fDivid * nSelectOption;		//選択肢に応じたテクスチャ位置にする
		break;

	case OPTION_POINT:		//ポイント数

		fDivid = (1.0f / MAX_POINT);		//テクスチャに合わせて分割
		fTexV = fDivid * nSelectOption;		//選択肢に応じたテクスチャ位置にする
		break;

	case OPTION_SETCOUNT:	//セット数

		fDivid = (1.0f / MAX_SETCOUNT);		//テクスチャに合わせて分割
		fTexV = fDivid * nSelectOption;		//選択肢に応じたテクスチャ位置にする
		break;

	default:
		assert(false);
		break;
	}

	//テクスチャ座標の設定------>後で変える
	pVtx[0].tex = D3DXVECTOR2(0.0f, fTexV);
	pVtx[1].tex = D3DXVECTOR2(1.0f, fTexV);
	pVtx[2].tex = D3DXVECTOR2(0.0f, fTexV + fDivid);
	pVtx[3].tex = D3DXVECTOR2(1.0f, fTexV + fDivid);

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}