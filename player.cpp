//=========================================
// 
// プレイヤーの処理
// Author YudaKaito
// 
//=========================================

//-----------------------------------------
// include
//-----------------------------------------
#include "main.h"
#include "input.h"
#include "player.h"
#include <assert.h>

//-----------------------------------------
// スタティック変数
//-----------------------------------------
static LPDIRECT3DTEXTURE9 s_pTexture = NULL;		// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = NULL;	// 頂点バッファへのポインタ
static Player s_player[2] = {};

//-----------------------------------------
// プロトタイプ宣言
//-----------------------------------------
static void MovePlayer(void);
static void JumpPlayer(void);

//=========================================
// プレイヤーの初期化処理
//=========================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポイント
	Player *pPlayer = s_player;

	// テクスチャの読込
	D3DXCreateTextureFromFile(pDevice,PLAYER_TEX,&s_pTexture);

	pPlayer->pos = D3DXVECTOR3(600.0f, SCREEN_HEIGHT - 50.0f, 0.0f);	// 位置を初期化
	pPlayer->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						// 移動量を初期化
	pPlayer->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						// 向きを初期化
	pPlayer->fWidth = PLAYER_WIDTH;										// プレイヤーの幅
	pPlayer->fHeigth = PLAYER_HEIGTH;									// プレイヤーの高さ
	pPlayer->bUse = true;												// プレイヤーの表示の有無
	pPlayer->state = PLAYERSTATE_APPEAR;								// プレイヤーのステータス
	pPlayer->RevivalInterval = 0;										// 復活のインターバル

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;		// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(pPlayer->pos.x - pPlayer->fWidth, pPlayer->pos.y - pPlayer->fHeigth, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pPlayer->pos.x + pPlayer->fWidth, pPlayer->pos.y - pPlayer->fHeigth, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pPlayer->pos.x - pPlayer->fWidth, pPlayer->pos.y + pPlayer->fHeigth, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pPlayer->pos.x + pPlayer->fWidth, pPlayer->pos.y + pPlayer->fHeigth, 0.0f);

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(0.4f, 0.71f, 0.63f, 1.0f);
	pVtx[1].col = D3DXCOLOR(0.4f, 0.71f, 0.63f, 1.0f);
	pVtx[2].col = D3DXCOLOR(0.4f, 0.71f, 0.63f, 1.0f);
	pVtx[3].col = D3DXCOLOR(0.4f, 0.71f, 0.63f, 1.0f);

	// rhwの設定
	pVtx[0].rhw	= 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//=========================================
// プレイヤーの終了処理
//=========================================
void UninitPlayer(void)
{
	// テクスチャの破棄
	if (s_pTexture != NULL)
	{
		s_pTexture->Release();
		s_pTexture = NULL;
	}

	// 頂点バッファの破棄
	if (s_pVtxBuff != NULL)
	{
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//=========================================
// プレイヤーの更新処理
//=========================================
void UpdatePlayer(void)
{
	VERTEX_2D *pVtx;		// 頂点情報へのポインタ
	Player *pPlayer = s_player;

	switch (pPlayer->state)
	{
	case PLAYERSTATE_APPEAR:	// プレイヤーが出現中
		pPlayer->state = PLAYERSTATE_NORMAL;
		pPlayer->bUse = true;
		pPlayer->move.y = 0.0f;
		pPlayer->rot.z = 0.0f;
		pPlayer->pos = D3DXVECTOR3(600.0f, SCREEN_HEIGHT - 50.0f, 0.0f);	// 位置を初期化
		break;
	case PLAYERSTATE_NORMAL:	// プレイヤーが活動中
		// 重力の加算
		pPlayer->move.y += WOARD_GRAVITY;

		// 前回の座標を更新
		pPlayer->posOld = pPlayer->pos;

		JumpPlayer();
		MovePlayer();

		break;
	default:
		assert(false);
		break;
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定

	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//=========================================
// プレイヤーの描画処理
//=========================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポイント
	Player *pPlayer = GetPlayer();

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリーム設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (pPlayer->bUse)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, s_pTexture);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//=========================================
// プレイヤーの移動処理
//=========================================
void MovePlayer(void)
{
	Player *pPlayer = s_player;
}

//=========================================
// プレイヤーのジャンプ処理
//=========================================
void JumpPlayer(void)
{
	Player *pPlayer = s_player;
}

//=========================================
// プレイヤーの取得処理
//=========================================
Player* GetPlayer(void)
{
	return s_player;
}
