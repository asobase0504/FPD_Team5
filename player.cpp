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
#include "disk.h"
#include <assert.h>

//-----------------------------------------
// マクロ定義
//-----------------------------------------
#define NUM_PLAYER	(2)

//-----------------------------------------
// スタティック変数
//-----------------------------------------
static Player s_player[NUM_PLAYER] = {};

//-----------------------------------------
// プロトタイプ宣言
//-----------------------------------------
static void MovePlayer(int nIdxPlayer);
static void JumpPlayer(int nIdxPlayer);
static void ThrowPlayer(int nIdxPlayer);

//=========================================
// プレイヤーの初期化処理
//=========================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポイント
	Player *pPlayer = s_player;

	for (int i = 0; i < NUM_PLAYER; i++, pPlayer++)
	{
		// テクスチャの読込
		D3DXCreateTextureFromFile(pDevice, PLAYER_TEX, &pPlayer->pTexture);

		pPlayer->pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f + i * 200.0f, SCREEN_HEIGHT * 0.5f, 0.0f);	// 位置を初期化
		pPlayer->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						// 移動量を初期化
		pPlayer->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						// 向きを初期化
		pPlayer->fWidth = PLAYER_WIDTH;										// プレイヤーの幅
		pPlayer->fHeigth = PLAYER_HEIGTH;									// プレイヤーの高さ
		pPlayer->bUse = true;												// プレイヤーの表示の有無
		pPlayer->RevivalInterval = 0;										// 復活のインターバル

		// 頂点バッファの生成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&pPlayer->pVtxBuff,
			NULL);

		VERTEX_2D *pVtx;		// 頂点情報へのポインタ

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		pPlayer->pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

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
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点バッファをアンロックする
		pPlayer->pVtxBuff->Unlock();
	}
}

//=========================================
// プレイヤーの終了処理
//=========================================
void UninitPlayer(void)
{
	Player *pPlayer = s_player;

	for (int i = 0; i < NUM_PLAYER; i++, pPlayer++)
	{
		// テクスチャの破棄
		if (pPlayer->pTexture != NULL)
		{
			pPlayer->pTexture->Release();
			pPlayer->pTexture = NULL;
		}

		// 頂点バッファの破棄
		if (pPlayer->pVtxBuff != NULL)
		{
			pPlayer->pVtxBuff->Release();
			pPlayer->pVtxBuff = NULL;
		}
	}
}

//=========================================
// プレイヤーの更新処理
//=========================================
void UpdatePlayer(void)
{
	Player *pPlayer = s_player;

	for (int nIdxPlayer = 0; nIdxPlayer < NUM_PLAYER; nIdxPlayer++, pPlayer++)
	{
		pPlayer->pos += pPlayer->move;

		// 前回の座標を更新
		pPlayer->posOld = pPlayer->pos;

		JumpPlayer(nIdxPlayer);		// 跳躍
		MovePlayer(nIdxPlayer);		// 移動
		ThrowPlayer(nIdxPlayer);	// 投げる

		VERTEX_2D *pVtx;		// 頂点情報へのポインタ

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		pPlayer->pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(pPlayer->pos.x - pPlayer->fWidth, pPlayer->pos.y - pPlayer->fHeigth, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pPlayer->pos.x + pPlayer->fWidth, pPlayer->pos.y - pPlayer->fHeigth, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pPlayer->pos.x - pPlayer->fWidth, pPlayer->pos.y + pPlayer->fHeigth, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pPlayer->pos.x + pPlayer->fWidth, pPlayer->pos.y + pPlayer->fHeigth, 0.0f);

		// 頂点バッファをアンロックする
		pPlayer->pVtxBuff->Unlock();
	}
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

	for (int i = 0; i < NUM_PLAYER; i++, pPlayer++)
	{
		// 頂点バッファをデータストリーム設定
		pDevice->SetStreamSource(0, pPlayer->pVtxBuff, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		if (pPlayer->bUse)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, pPlayer->pTexture);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		}
	}
}

//=========================================
// プレイヤーの移動処理
//=========================================
void MovePlayer(int nIdxPlayer)
{
	D3DXVECTOR3 inputMove(0.0f,0.0f,0.0f);
	float moveLength = 0.0f;
	Player *pPlayer = &s_player[nIdxPlayer];

	if (IsJoyPadUse(nIdxPlayer))
	{
		inputMove = GetJoypadStick(JOYKEY_LEFT_STICK, nIdxPlayer);

		if (inputMove.x != 0.0f || inputMove.y != 0.0f)
		{
			moveLength = D3DXVec3Length(&inputMove);

			if (moveLength > 1.0f)
			{
				moveLength = 1.0f;
			}
		}
	}
	else
	{
		if (GetKeyboardPress(DIK_W))
		{
			pPlayer->move.y -= 1.0f;
		}
		if (GetKeyboardPress(DIK_A))
		{
			pPlayer->move.x -= 1.0f;
		}
		if (GetKeyboardPress(DIK_S))
		{
			pPlayer->move.y += 1.0f;
		}
		if (GetKeyboardPress(DIK_D))
		{
			pPlayer->move.x += 1.0f;
		}
	}

	if (moveLength > 0.0f)
	{
		D3DXVec3Normalize(&inputMove, &inputMove);
	}

	pPlayer->move = inputMove * 0.9f * 5.0f;
}

//=========================================
// プレイヤーのジャンプ処理
//=========================================
void JumpPlayer(int nIdxPlayer)
{
	Player *pPlayer = &s_player[nIdxPlayer];

}

//=========================================
// プレイヤーの投げる処理
//=========================================
void ThrowPlayer(int nIdxPlayer)
{
	Player *pPlayer = &s_player[nIdxPlayer];

	D3DXVECTOR3 vec(2.0f,0.0f,0.0f);


	if (IsJoyPadUse(nIdxPlayer))
	{
		vec = GetJoypadStick(JOYKEY_LEFT_STICK, nIdxPlayer);

		if (GetJoypadTrigger(JOYKEY_B, nIdxPlayer))
		{
			SetDisk(pPlayer->pos, vec, D3DXVECTOR3(0.0f, 0.0f, 0.0f), DISK_TYPE_NORMAL, 40.0f);
		}
		if (GetJoypadTrigger(JOYKEY_A, nIdxPlayer))
		{
			SetDisk(pPlayer->pos, vec, D3DXVECTOR3(0.0f, 0.0f, 0.0f), DISK_TYPE_LOB, 40.0f);
		}
	}
	else
	{
		if (GetKeyboardPress(DIK_W))
		{
			vec.y -= 1.0f;
		}
		if (GetKeyboardPress(DIK_A))
		{
			vec.x -= 1.0f;
		}
		if (GetKeyboardPress(DIK_S))
		{
			vec.y += 1.0f;
		}
		if (GetKeyboardPress(DIK_D))
		{
			vec.x += 1.0f;
		}

		if (GetKeyboardPress(DIK_RETURN))
		{
			SetDisk(pPlayer->pos, vec, D3DXVECTOR3(0.0f, 0.0f, 0.0f), DISK_TYPE_NORMAL, 40.0f);
		}
		if (GetKeyboardPress(DIK_SPACE))
		{
			SetDisk(pPlayer->pos, vec, D3DXVECTOR3(0.0f, 0.0f, 0.0f), DISK_TYPE_LOB, 40.0f);
		}
	}
}

//=========================================
// 設定
//=========================================
void SetPlayer(void)
{
}

//=========================================
// プレイヤーの取得処理
//=========================================
Player* GetPlayer(void)
{
	return s_player;
}
