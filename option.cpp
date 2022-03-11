//============================================
//設定
//ACG000
//Author: goto yuuki
//Author:Kishimoto Eiji
//
//============================================
#include "option.h"
#include "fade.h"
#include "input.h"
#include "menu.h"

#include <assert.h>

//***********************************
//マクロ定義
//***********************************
#define MAX_TEXTURE		(4)			//使用するテクスチャの最大数
#define MENU_WIDTH		(400.0f)	//メニューの幅
#define MENU_HEIGHT		(140.0f)	//メニューの高さ

//***********************************
//オプションの列挙型
//***********************************
typedef enum
{
	OPTION_TIMELIMIT = 0,	//制限時間
	OPTION_POINT,			//ポイント数
	OPTION_SETCOUNT,		//セット数
	OPTION_GOTOGAME,		//ゲームへ進む
	OPTION_MAX
}OPTION;

//***********************************
//制限時間の列挙型
//***********************************
typedef enum
{
	TIMELIMIT_15 = 0,		//15秒
	TIMELIMIT_30,			//30秒
	TIMELIMIT_45,			//45秒
	TIMELIMIT_90,			//90秒
	TIMELIMIT_INFINITY,		//無限(制限時間なし)
	TIMELIMIT_MAX
}TIMELIMIT;

//***********************************
//ポイント数の列挙型
//***********************************
typedef enum
{
	POINTCOUNT_12 = 0,	//12点
	POINTCOUNT_15,		//15点
	POINTCOUNT_21,		//21点
	POINTCOUNT_MAX
}POINTCOUNT;

//***********************************
//セット数の列挙型
//***********************************
typedef enum
{
	SETCOUNT_ONE = 0,	//1セット
	SETCOUNT_TWO,		//2セット
	SETCOUNT_THREE,		//3セット
	SETCOUNT_MAX
}SETCOUNT;

//***********************************
//スタティック変数
//***********************************
static LPDIRECT3DTEXTURE9		s_pTexture = NULL;				//テクスチャへのポインタ(背景)
static LPDIRECT3DTEXTURE9		s_apTextureMenu[MAX_TEXTURE];	//テクスチャへのポインタ(メニュー)
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuff = NULL;				//頂点バッファへのポインタ

static int s_nSelectMenu;		//選ばれているメニュー
static int s_nSelectTimeLimit;	//選ばれている制限時間
static int s_nSelectPoint;		//選ばれているポイント数
static int s_nSelectSetCount;	//選ばれているセット数

static int s_nTimeLimit;	//制限時間を保存する
static int s_nPoint;		//ポイント数を保存する
static int s_nSetCount;		//セット数を保存する

//***********************************
//プロトタイプ宣言
//***********************************

static void SelectMenu(void);
static int SelectTimeLimit(void);
static int SelectPoint(void);
static int SelectSetCount(void);

//============================================
//オプションの初期化
//============================================
void InitOption(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

	//テクスチャポインタの初期化
	memset(s_apTextureMenu, NULL, sizeof(s_apTextureMenu));

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
								&s_apTextureMenu[2]);

	//変数の初期化
	s_nSelectMenu = 0;
	s_nSelectTimeLimit = 0;
	s_nSelectPoint = 0;
	s_nSelectSetCount = 0;
	s_nTimeLimit = 0;
	s_nPoint = 0;
	s_nSetCount = 0;

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
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

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
	menu.fTop = SCREEN_HEIGHT * 0.5f;
	menu.fBottom = SCREEN_HEIGHT;
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
}

//============================================
//オプションの更新
//============================================
void UpdateOption(void)
{
	SelectMenu();	//メニュー選択
	UpdateMenu();	//メニュー更新
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
		s_nSelectTimeLimit = SelectTimeLimit();
		break;

	case OPTION_POINT:	//ポイント数
		
		//ポイント数の選択
		s_nSelectPoint = SelectPoint();
		break;

	case OPTION_SETCOUNT:	//セット数

		//セット数の選択
		s_nSelectSetCount = SelectSetCount();
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
		}
		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------
// 制限時間の選択
//--------------------------------------------
static int SelectTimeLimit(void)
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

	int nTimeLimit = 0;

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

	return nTimeLimit;	//設定した値を返す
}

//--------------------------------------------
// ポイント数の選択
//--------------------------------------------
static int SelectPoint(void)
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

	int nPoint = 0; 

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

	default:
		assert(false);
		break;
	}

	return nPoint;	//設定した値を返す
}

//--------------------------------------------
// セット数の選択
//--------------------------------------------
static int SelectSetCount(void)
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

	int nSetCount = 0;

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

	default:
		assert(false);
		break;
	}

	return nSetCount;	//設定した値を返す
}