//============================================
//
//チーム制作<フライングパワーディスク>[option.cpp]
//Author: goto yuuki
//Author:Kishimoto Eiji
//
//============================================
#include "option.h"
#include "select.h"
#include "fade.h"
#include "input.h"
#include "menu.h"
#include "mode.h"

#include <assert.h>

//***********************************
//マクロ定義
//***********************************
#define MAX_TEXTURE		(4)			//使用するテクスチャの最大数
#define MENU_WIDTH		(850.0f)	//メニューの幅
#define MENU_HEIGHT		(85.0f)		//メニューの高さ

//***********************************
//スタティック変数
//***********************************
static LPDIRECT3DTEXTURE9		s_pTexture = NULL;				//テクスチャへのポインタ(背景)
static LPDIRECT3DTEXTURE9		s_apTextureMenu[MAX_TEXTURE];	//テクスチャへのポインタ(メニュー)
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuff = NULL;				//頂点バッファへのポインタ

static int s_nSelectMenu;		//選ばれているメニュー
static int s_nSelectTimeLimit;	//選択した制限時間
static int s_nSelectPoint;		//選択したポイント数
static int s_nSelectSetCount;	//選択したセット数

//***********************************
//プロトタイプ宣言
//***********************************

static void SelectMenu(void);

//============================================
//オプションの初期化
//============================================
void InitOption(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/背景",
								&s_pTexture);

	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/タイマー",
								&s_apTextureMenu[0]);

	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/ポイント数",
								&s_apTextureMenu[1]);

	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/セット数",
								&s_apTextureMenu[2]);

	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/タイトルへ戻る",
								&s_apTextureMenu[3]);

	//変数の初期化
	s_nSelectMenu = 0;
	
	VERTEX_2D * pVtx = NULL;	//頂点情報へのポインタ

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&s_pVtxBuff,
								NULL);

	//頂点バッファをロックし、頂点データへのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
	pVtx[1].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
	pVtx[2].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
	pVtx[3].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();

	/*********** メニューのセット **********/

	//メニューの引数の情報
	MenuArgument menu;
	menu.nNumUse = OPTION_MAX;
	menu.fLeft = 0.0f;
	menu.fRight = SCREEN_WIDTH;
	menu.fTop = SCREEN_HEIGHT * 0.2f;
	menu.fBottom = SCREEN_HEIGHT * 0.8f;
	menu.fWidth = MENU_WIDTH;
	menu.fHeight = MENU_HEIGHT;

	for (int i = 0; i < OPTION_MAX; i++)
	{
		menu.pTexture[i] = s_apTextureMenu[i];
	}

	//枠の引数の情報
	FrameArgument frame;
	frame.pTexture = NULL;
	frame.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	frame.bUse = false;

	//メニューのセット
	InitMenu();
	SetMenu(menu, frame);

	InitSelect();
}

//============================================
//オプションの終了
//============================================
void UninitOption(void)
{
	for (int i = 0; i < MAX_TEXTURE; i++)
	{
		if (s_apTextureMenu[i] != NULL)
		{//テクスチャの破棄
			s_apTextureMenu[i]->Release();
			s_apTextureMenu[i] = NULL;
		}
	}

	if (s_pVtxBuff != NULL)
	{//頂点バッファの破棄
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}

	UninitSelect();
}

//============================================
//オプションの更新
//============================================
void UpdateOption(void)
{
	SelectMenu();	//メニュー選択
	UpdateMenu();	//メニュー更新
	UpdateSelect();
}

//============================================
//オプションの描画
//============================================
void DrawOption(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, s_pTexture);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//ポリゴンの形
							0,					//頂点の開始場所
							2);					//プリミティブの数

	DrawMenu();		//メニュー描画
	DrawSelect();
}

//--------------------------------------------
// メニューの選択
//--------------------------------------------
static void SelectMenu(void)
{
	if (GetFade() != FADE_NONE)
	{// フェードしている
		return;
	}

	if (GetKeyboardTrigger(DIK_W) || GetJoypadTrigger(JOYKEY_UP, 0))
	{// 上を入力( Wキー or 十字キー上 )
		InitColorOption();		//選択肢の色を初期化

		//選択肢を上にずらす
		s_nSelectMenu = ((s_nSelectMenu - 1) + OPTION_MAX) % OPTION_MAX;

		ChangeOption(s_nSelectMenu);	//選択肢を変更
	}
	else if (GetKeyboardTrigger(DIK_S) || GetJoypadTrigger(JOYKEY_DOWN, 0))
	{// 下を入力( Sキー or 十字キー下 )
		InitColorOption();		//選択肢の色を初期化

		//選択肢を下にずらす
		s_nSelectMenu = ((s_nSelectMenu + 1) + OPTION_MAX) % OPTION_MAX;

		ChangeOption(s_nSelectMenu);	//選択肢を変更
	}

	switch (s_nSelectMenu)
	{
	case OPTION_TIMELIMIT:	//制限時間

		//制限時間の選択
		s_nSelectTimeLimit = SelectTimeLimit(s_nSelectMenu);
		break;

	case OPTION_POINT:	//ポイント数
		
		//ポイント数の選択
		s_nSelectPoint = SelectPoint(s_nSelectMenu);
		break;

	case OPTION_SETCOUNT:	//セット数

		//セット数の選択
		s_nSelectSetCount = SelectSetCount(s_nSelectMenu);
		break;

	case OPTION_GOTOGAME:	//ゲームへ進む

		if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(JOYKEY_START, 0))
		{//決定キー(ENTERキー)が押されたかどうか
			/* イメージ↓

			SetTime(s_nSelectTimeLimit);	//制限時間
			SetPoint(s_nSelectPoint);		//ポイント数
			SetSetCount(s_nSelectSetCount);	//セット数

			<ChangeModeでゲームへ>
			*/

			ChangeMode(MODE_GAME);

		}
		break;

	default:
		assert(false);
		break;
	}
}