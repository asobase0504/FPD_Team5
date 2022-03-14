//==================================================
//
// タイトル画面 ( title.cpp )
// Author : Yuda Kaito
// Author:Kishimoto Eiji
//
//==================================================
#include "title.h"
#include "menu.h"
#include "fade.h"
#include "input.h"
#include "mode.h"

#include <assert.h>

//**************************************************
// マクロ定義
//**************************************************
#define MAX_TEXTURE		(3)			//使用するテクスチャの最大数
#define MENU_WIDTH		(400.0f)	//メニューの幅
#define MENU_HEIGHT		(140.0f)	//メニューの高さ

//**************************************************
//タイトルメニューの列挙型
//**************************************************
typedef enum
{
	MENU_GAMESTART = 0,	//ゲーム開始
	MENU_OPTION,		//設定
	MENU_EXIT,			//終了
	MENU_MAX
}MENU;

//**************************************************
//スタティック変数
//**************************************************
static LPDIRECT3DTEXTURE9		s_pTexture = NULL;				//テクスチャへのポインタ(タイトル背景)
static LPDIRECT3DTEXTURE9		s_apTextureMenu[MAX_TEXTURE];	//テクスチャへのポインタ(タイトルメニュー)
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuff = NULL;				//頂点バッファへのポインタ
static int						s_nSelectMenu;					//選ばれているメニュー

//**************************************************
//プロトタイプ宣言
//**************************************************
static void SelectMenu(void);

//==================================================
// 初期化
//==================================================
void InitTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	VERTEX_2D *pVtx;							//頂点情報へのポインタ

	//テクスチャポインタの初期化
	memset(s_apTextureMenu, NULL, sizeof(s_apTextureMenu));

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/仮タイトル.png",
								&s_pTexture);

	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/GameStart.png",
								&s_apTextureMenu[MENU_GAMESTART]);

	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/Tutorial.png",
								&s_apTextureMenu[MENU_OPTION]);

	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/GameEnd.png",
								&s_apTextureMenu[MENU_EXIT]);

	//変数の初期化
	s_nSelectMenu = 0;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&s_pVtxBuff,
								NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();

	/*********** メニューのセット **********/

	//メニューの引数の情報
	MenuArgument menu;
	menu.nNumUse	= MENU_MAX;
	menu.fLeft		= 0.0f;
	menu.fRight		= SCREEN_WIDTH;
	menu.fTop		= SCREEN_HEIGHT * 0.5f;
	menu.fBottom	= SCREEN_HEIGHT;
	menu.fWidth		= MENU_WIDTH;
	menu.fHeight	= MENU_HEIGHT;

	for (int i = 0; i < MENU_MAX; i++)
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
}

//==================================================
// 終了
//==================================================
void UninitTitle(void)
{
	UninitMenu();	//メニュー

	if (s_pTexture != NULL)
	{//テクスチャの破棄
		s_pTexture->Release();
		s_pTexture = NULL;
	}

	if (s_pVtxBuff != NULL)
	{//頂点バッファの破棄
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//==================================================
// 更新
//==================================================
void UpdateTitle(void)
{
	SelectMenu();	//メニュー選択
	UpdateMenu();	//メニュー更新
}

//==================================================
// 描画
//==================================================
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, s_pTexture);

	//チュートリアルの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
							0,						//描画する最初の頂点インデックス
							2);						//描画するプリミティブ数

	DrawMenu();		//メニュー描画
}

//--------------------------------------------------
// メニューの選択
//--------------------------------------------------
static void SelectMenu(void)
{
	if (GetFade() != FADE_NONE)
	{// フェードしている
		return;
	}

	if (GetKeyboardTrigger(DIK_W) || GetJoypadTrigger(JOYKEY_UP, 0))
	{// Wキーが押されたかどうか
		InitColorOption();

		s_nSelectMenu = ((s_nSelectMenu - 1) + MENU_MAX) % MENU_MAX;

		ChangeOption(s_nSelectMenu);
	}
	else if (GetKeyboardTrigger(DIK_S) || GetJoypadTrigger(JOYKEY_DOWN, 0))
	{// Sキーが押されたかどうか
		InitColorOption();

		s_nSelectMenu = ((s_nSelectMenu + 1) + MENU_MAX) % MENU_MAX;

		ChangeOption(s_nSelectMenu);
	}

	if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(JOYKEY_START, 0))
	{//決定キー(ENTERキー)が押されたかどうか
		switch (s_nSelectMenu)
		{
		case MENU_GAMESTART:	//ゲーム開始

			ChangeMode(MODE_GAME);

			break;

		case MENU_OPTION:	//設定

			//SetFade(MODE_RANKING);	//設定画面へ

			break;

		case MENU_EXIT:		//終了

			break;

		default:
			assert(false);
			break;
		}
	}
}