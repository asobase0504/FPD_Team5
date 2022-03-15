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
#include <stdio.h>
#include "shadow.h"
#include "stage.h"

//-----------------------------------------
// マクロ定義
//-----------------------------------------
#define NUM_PLAYER		(2)								// プレイヤーの数
#define PLAYER_FILE		"data/player.txt"				// プレイヤー読み込みファイル
#define PLAYER_TEX		"data/TEXTURE/player00.png"		// プレイヤーのテクスチャ
#define PLAYER_SIZ		(45.0f)							// プレイヤーの大きさ
#define ZERO_VECTOR		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))	// ゼロベクトル

//-----------------------------------------
// スタティック変数
//-----------------------------------------
static Player s_player[NUM_PLAYER] = {};
static Player s_playerType[PLAYERTYPE_MAX] = {};
static D3DXVECTOR3 moveCurve;
static bool s_bCurveInput;

//-----------------------------------------
// プロトタイプ宣言
//-----------------------------------------
static void MovePlayer(int nIdxPlayer);
static void JumpPlayer(int nIdxPlayer);
static void ThrowPlayer(int nIdxPlayer);
static void CatchDiscPlayer(int nIdxPlayer);
static void LoadPlayer(void);
static bool CollisionCircle(D3DXVECTOR3 Pos1, float fRadius1, D3DXVECTOR3 Pos2, float fRadius2);	// 円同士の読み込み処理
static D3DXVECTOR3 InputMovePlayer(int nIdxPlayer);

//=========================================
// プレイヤーの初期化処理
//=========================================
void InitPlayer(void)
{
	//初期化
	ZeroMemory(s_player, sizeof(s_player));
	ZeroMemory(s_playerType, sizeof(s_playerType));

	moveCurve = ZERO_VECTOR;
	LoadPlayer();	// 読み込み

	// プレイヤーの設定
	SetPlayer(D3DXVECTOR3(200.0f, SCREEN_HEIGHT * 0.5f, 0.0f), PLAYERTYPE_1);
	s_player[0].nIdxShadow = SetShadow(s_player[0].pos, PLAYER_SIZ * 3.0f);

	SetPlayer(D3DXVECTOR3(SCREEN_WIDTH - 200.0f, SCREEN_HEIGHT * 0.5f, 0.0f), PLAYERTYPE_2);
	s_player[1].nIdxShadow = SetShadow(s_player[1].pos, PLAYER_SIZ * 3.0f);
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
		SetPositionShadow(pPlayer->nIdxShadow, pPlayer->pos);

		if (pPlayer->bHaveDisk)
		{ //ディスクを所持してる場合
			pPlayer->move = ZERO_VECTOR;
			// 投げる
			ThrowPlayer(nIdxPlayer);

			if (pPlayer->jumpstate == JUMP_NOW && pPlayer->fVerticalSpeed > 0.0f)
			{
				pPlayer->fHeight += pPlayer->fVerticalSpeed;
				pPlayer->fVerticalSpeed -= 0.15f;
			}
		}
		else
		{
			// 受け取る
			CatchDiscPlayer(nIdxPlayer);

			switch (pPlayer->jumpstate)
			{
			case JUMP_NONE:
				// 移動
				MovePlayer(nIdxPlayer);
				// 跳躍
				JumpPlayer(nIdxPlayer);
				break;
			case JUMP_NOW:
				pPlayer->fHeight += pPlayer->fVerticalSpeed;
				pPlayer->fVerticalSpeed -= 0.15f;

				if (pPlayer->fHeight <= 0.0f)
				{
					pPlayer->fVerticalSpeed = 5.0f;
					pPlayer->fHeight = 0.0f;
					pPlayer->jumpstate = JUMP_NONE;
				}

				break;
			default:
				assert(false);
				break;
			}
		}

		// 移動制限
		{
			STAGE_LENGTH stageLength;
			if (nIdxPlayer == 0)
			{
				stageLength = *GetP1StgLng();
			}
			else
			{
				stageLength = *GetP2StgLng();
			}

			if (stageLength.max.y <= pPlayer->pos.y + PLAYER_SIZ)
			{
				pPlayer->pos.y = stageLength.max.y - PLAYER_SIZ;
			}
			if (stageLength.max.x <= pPlayer->pos.x + PLAYER_SIZ)
			{
				pPlayer->pos.x = stageLength.max.x - PLAYER_SIZ;
			}
			if (stageLength.min.y >= pPlayer->pos.y - PLAYER_SIZ)
			{
				pPlayer->pos.y = stageLength.min.y + PLAYER_SIZ;
			}
			if (stageLength.min.x >= pPlayer->pos.x - PLAYER_SIZ)
			{
				pPlayer->pos.x = stageLength.min.x + PLAYER_SIZ;
			}
		}

		VERTEX_2D *pVtx;	// 頂点情報へのポインタ

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		pPlayer->pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(pPlayer->pos.x - (pPlayer->fSize * (1 + (pPlayer->fHeight * 0.015f))),
			pPlayer->pos.y - ((pPlayer->fHeight - 10.0f) * 0.5f) - (pPlayer->fSize * (1 + (pPlayer->fHeight * 0.015f))), 0.0f);

		pVtx[1].pos = D3DXVECTOR3(pPlayer->pos.x + (pPlayer->fSize * (1 + (pPlayer->fHeight * 0.015f))),
			pPlayer->pos.y - ((pPlayer->fHeight - 10.0f) * 0.5f) - (pPlayer->fSize * (1 + (pPlayer->fHeight * 0.015f))), 0.0f);

		pVtx[2].pos = D3DXVECTOR3(pPlayer->pos.x - (pPlayer->fSize * (1 + (pPlayer->fHeight * 0.015f))),
			pPlayer->pos.y - ((pPlayer->fHeight - 10.0f) * 0.5f) + (pPlayer->fSize * (1 + (pPlayer->fHeight * 0.015f))), 0.0f);

		pVtx[ 3].pos = D3DXVECTOR3(pPlayer->pos.x + (pPlayer->fSize * (1 + (pPlayer->fHeight * 0.015f))),
			pPlayer->pos.y - ((pPlayer->fHeight - 10.0f) * 0.5f) + (pPlayer->fSize * (1 + (pPlayer->fHeight * 0.015f))), 0.0f);

		// 頂点バッファをアンロックする
		pPlayer->pVtxBuff->Unlock();
	}
}

//=========================================
// プレイヤーの描画処理
//=========================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();;	// デバイスへのポイント
	Player *pPlayer = GetPlayer();

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
	Player *pPlayer = &s_player[nIdxPlayer];

	// 方向入力処理
	D3DXVECTOR3 inputMove = InputMovePlayer(nIdxPlayer);

	if (IsJoyPadUse(nIdxPlayer))
	{ // JoyPadの有無
		if (GetJoypadTrigger(JOYKEY_A))
		{ //スライディングの使用
			pPlayer->bUseSliding = true;
		}
	}
	else
	{
		if (GetKeyboardTrigger(DIK_B))
		{ //スライディングの使用
			pPlayer->bUseSliding = true;
		}
	}

	if (pPlayer->bUseSliding)
	{ // スライディング
		if (pPlayer->nSlidingRigorCnt == 0)
		{
			D3DXVec3Normalize(&inputMove, &inputMove);	// 長さの正規化
			pPlayer->move = inputMove * pPlayer->fSlidingVolume;
			pPlayer->nSlidingRigorCnt++;
		}
		else if (pPlayer->nSlidingRigorCnt >= pPlayer->nSlidingRigorMax)
		{
			pPlayer->nSlidingRigorCnt = 0;
			pPlayer->bUseSliding = false;
		}
		else
		{
			pPlayer->move -= pPlayer->move * pPlayer->fAttenuationSlidingSpead;
			pPlayer->nSlidingRigorCnt++;
		}
	}
	else
	{
		if (D3DXVec3Length(&inputMove) > 0.0f)
		{
			pPlayer->move = inputMove * pPlayer->fMoveSpeed;
			D3DXVec3Normalize(&inputMove, &inputMove);	// 長さの正規化
		}

		// 減衰処理
		pPlayer->move -= pPlayer->move * pPlayer->fAttenuationMoveSpead;
	}
}

//=========================================
// プレイヤーのジャンプ処理
//=========================================
void JumpPlayer(int nIdxPlayer)
{
	Player *pPlayer = &s_player[nIdxPlayer];

	if ((IsJoyPadUse(nIdxPlayer) && GetJoypadTrigger(JOYKEY_Y, nIdxPlayer))
		|| !IsJoyPadUse(nIdxPlayer) && GetKeyboardTrigger(DIK_J))
	{
		pPlayer->move = ZERO_VECTOR;
		pPlayer->jumpstate = JUMP_NOW;
	}
}

//=========================================
// プレイヤーの投げる処理
//=========================================
void ThrowPlayer(int nIdxPlayer)
{
	Player *pPlayer = &s_player[nIdxPlayer];
	D3DXVECTOR3 inputVec;

	if (nIdxPlayer == 0)
	{
		inputVec = D3DXVECTOR3(2.0f, 0.0f, 0.0f);	// 入力方向
	}
	else
	{
		inputVec = D3DXVECTOR3(-2.0f, 0.0f, 0.0f);	// 入力方向
	}

	// 方向入力処理
	inputVec += InputMovePlayer(nIdxPlayer);

	if (IsJoyPadUse(nIdxPlayer))
	{ // JoyPad入力

		float fVecLength = D3DXVec3Length(&inputVec);

		if (fVecLength >= 3.0f)
		{
			if (!s_bCurveInput)
			{
				// どれくらい曲がるかの入力
				moveCurve.y = -inputVec.y * pPlayer->fThrowCurvePower;
			}
			s_bCurveInput = true;
		}
		else
		{
			moveCurve = ZERO_VECTOR;
			s_bCurveInput = false;
		}

		if (GetJoypadTrigger(JOYKEY_B, nIdxPlayer))
		{
			SetDisk(pPlayer->pos, inputVec * pPlayer->fThrowPower, moveCurve, DISK_TYPE_NORMAL, nIdxPlayer, 40.0f);
			pPlayer->bHaveDisk = false;
		}
		else if (GetJoypadTrigger(JOYKEY_A, nIdxPlayer))
		{
			SetDisk(pPlayer->pos, inputVec * pPlayer->fThrowPower, ZERO_VECTOR, DISK_TYPE_LOB, nIdxPlayer, 40.0f);
			pPlayer->bHaveDisk = false;
		}
		else if (GetJoypadTrigger(JOYKEY_X, nIdxPlayer))
		{
			SetDisk(pPlayer->pos, inputVec * pPlayer->fThrowPower, ZERO_VECTOR, DISK_TYPE_SPECIAL_0, nIdxPlayer, 40.0f);
			pPlayer->bHaveDisk = false;
		}

	}
	else
	{ // キーボード入力
		if (GetKeyboardPress(DIK_RETURN))
		{
			SetDisk(pPlayer->pos, inputVec * pPlayer->fThrowPower, moveCurve, DISK_TYPE_NORMAL, nIdxPlayer, 40.0f);
			pPlayer->bHaveDisk = false;
		}
		else if (GetKeyboardPress(DIK_SPACE))
		{
			SetDisk(pPlayer->pos, inputVec * pPlayer->fThrowPower, ZERO_VECTOR, DISK_TYPE_LOB, nIdxPlayer, 40.0f);
			pPlayer->bHaveDisk = false;
		}
		else if (GetKeyboardPress(DIK_M))
		{
			SetDisk(pPlayer->pos, inputVec * pPlayer->fThrowPower, ZERO_VECTOR, DISK_TYPE_SPECIAL_0, nIdxPlayer, 40.0f);
			pPlayer->bHaveDisk = false;
		}
	}
}

//=========================================
// ディスクを受け取る処理
//=========================================
void CatchDiscPlayer(int nIdxPlayer)
{
	Player *pPlayer = &s_player[nIdxPlayer];
	Disk* pDisk = GetDisk();
	Shadow *pShadow = GetShadow();

	if ((CollisionCircle(pPlayer->pos, pPlayer->fSize * (1 + (pPlayer->fHeight * 0.005)), pDisk->pos, pDisk->fSize)) && (pDisk->nPlayer != nIdxPlayer))
	{
		if ((pDisk->type != DISK_TYPE_LOB || (pDisk->type == DISK_TYPE_LOB && pDisk->fHeight <= 0.0f)) && pPlayer->jumpstate == JUMP_NONE)
		{
			DestroyDisk();	// ディスクの破棄
			pPlayer->bHaveDisk = true;
		}

		if (pDisk->type == DISK_TYPE_LOB && pPlayer->jumpstate == JUMP_NOW)
		{
			DestroyDisk();	// ディスクの破棄
			pPlayer->bHaveDisk = true;
		}
	}
}

//=========================================
// 設定
//=========================================
void SetPlayer(const D3DXVECTOR3& pos, PLAYERTYPE type)
{
	Player *pPlayer = s_player;

	for (int i = 0; i < NUM_PLAYER; i++, pPlayer++)
	{
		if (pPlayer->bUse)
		{
			continue;
		}

		// データの初期化
		ZeroMemory(&s_player[i],sizeof(s_player[0]));

		// 種類別のデータの代入
		s_player[i] = s_playerType[type];

		pPlayer->pos = pos; 			// 位置を初期化
		pPlayer->fSize = PLAYER_SIZ;	// プレイヤーの大きさ
		pPlayer->bUse = true;			// プレイヤーの表示の有無

		// 頂点バッファの生成
		GetDevice()->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&pPlayer->pVtxBuff,
			NULL);

		VERTEX_2D *pVtx;		// 頂点情報へのポインタ

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		pPlayer->pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(pPlayer->pos.x - pPlayer->fSize, pPlayer->pos.y - pPlayer->fSize, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pPlayer->pos.x + pPlayer->fSize, pPlayer->pos.y - pPlayer->fSize, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pPlayer->pos.x - pPlayer->fSize, pPlayer->pos.y + pPlayer->fSize, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pPlayer->pos.x + pPlayer->fSize, pPlayer->pos.y + pPlayer->fSize, 0.0f);

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

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

		break;
	}

}

//=========================================
// プレイヤーの読み込み処理
//=========================================
void LoadPlayer(void)
{
	char s_aString[256];	// 読み込み用文字列
	int nNumType = 0;		// 読み込んだタイプ数

	//ファイルを開く
	FILE* pFile = fopen(PLAYER_FILE, "r");

	if (pFile != NULL)
	{//ファイルが開いた場合
		fscanf(pFile, "%s", s_aString);

		while (strncmp(s_aString, "SCRIPT", 6) != 0)
		{//スタート来るまで空白読み込む
			s_aString[0] = {};
			fscanf(pFile, "%s", s_aString);
		}

		while (strncmp(s_aString, "END_SCRIPT", 10) != 0)
		{// 文字列の初期化と読み込み

			fscanf(pFile, "%s", s_aString);

			if (strncmp(s_aString, "#", 1) == 0)
			{//これのあとコメント
				fgets(s_aString, sizeof(s_aString), pFile);
				continue;
			}

			if (strcmp(s_aString, "TYPESTATE") == 0)
			{// 文字列が一致した場合

				assert(PLAYERTYPE_MAX != nNumType);	// 想定より多いファイル読み込み

				while (strncmp(s_aString, "ENDSTATE", 8) != 0)
				{
					fscanf(pFile, "%s", s_aString);	// =の読み込み

					if (strcmp(s_aString, "TEXTURE") == 0)
					{
						fscanf(pFile, "%s", s_aString);	// =の読み込み
						fscanf(pFile, "%s", s_aString);

						//テクスチャの読み込み
						D3DXCreateTextureFromFile(GetDevice(), s_aString, &s_playerType[nNumType].pTexture);
					}
					else if (strcmp(s_aString, "MOVESPEED") == 0)
					{
						fscanf(pFile, "%s", s_aString);	// =の読み込み
						fscanf(pFile, "%f", &s_playerType[nNumType].fMoveSpeed);
					}
					else if (strcmp(s_aString, "MOVE_ATTENUATION") == 0)
					{
						fscanf(pFile, "%s", s_aString);	// =の読み込み
						fscanf(pFile, "%f", &s_playerType[nNumType].fAttenuationMoveSpead);
					}
					else if (strcmp(s_aString, "SLIDING_ATTENUATION") == 0)
					{
						fscanf(pFile, "%s", s_aString);	// =の読み込み
						fscanf(pFile, "%f", &s_playerType[nNumType].fAttenuationSlidingSpead);
					}
					else if (strcmp(s_aString, "THROW_POWER") == 0)
					{
						fscanf(pFile, "%s", s_aString);	// =の読み込み
						fscanf(pFile, "%f", &s_playerType[nNumType].fThrowPower);
					}
					else if (strcmp(s_aString, "THROW_CURVE") == 0)
					{
						fscanf(pFile, "%s", s_aString);	// =の読み込み
						fscanf(pFile, "%f", &s_playerType[nNumType].fThrowCurvePower);
					}
					else if (strcmp(s_aString, "SLIDING_VOLUME") == 0)
					{
						fscanf(pFile, "%s", s_aString);	// =の読み込み
						fscanf(pFile, "%f", &s_playerType[nNumType].fSlidingVolume);
					}
					else if (strcmp(s_aString, "SLIDING_RIGOR") == 0)
					{
						fscanf(pFile, "%s", s_aString);	// =の読み込み
						fscanf(pFile, "%d", &s_playerType[nNumType].nSlidingRigorMax);
					}
				}

				nNumType++;
			}
		}
	}
	//ファイルを閉じる
	fclose(pFile);
}

//-----------------------------------------------------------------------------
//円の当たり判定
//-----------------------------------------------------------------------------
static bool CollisionCircle(D3DXVECTOR3 Pos1, float fRadius1, D3DXVECTOR3 Pos2, float fRadius2)
{
	float fDiff = fRadius1 + fRadius2;		//2個の物体の半径同士の和
	float fCalculationX = Pos1.x - Pos2.x;	//Xの差分
	float fCalculationY = Pos1.y - Pos2.y;	//Yの差分

	//現在の２点の距離
	float fLength = sqrtf(fCalculationX * fCalculationX + fCalculationY * fCalculationY);

	return fDiff >= fLength;
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
D3DXVECTOR3 InputMovePlayer(int nIdxPlayer)
{
	D3DXVECTOR3 inputVec = ZERO_VECTOR;

	if (IsJoyPadUse(nIdxPlayer))
	{ // JoyPad入力
		inputVec = GetJoypadStick(JOYKEY_LEFT_STICK, nIdxPlayer);
	}
	else
	{ // キーボード入力
		switch (nIdxPlayer)
		{
		case 0:
			if (GetKeyboardPress(DIK_W))
			{
				inputVec.y -= 1.0f;
			}
			if (GetKeyboardPress(DIK_A))
			{
				inputVec.x -= 1.0f;
			}
			if (GetKeyboardPress(DIK_S))
			{
				inputVec.y += 1.0f;
			}
			if (GetKeyboardPress(DIK_D))
			{
				inputVec.x += 1.0f;
			}
			break;
		case 1:
			if (GetKeyboardPress(DIK_UP))
			{
				inputVec.y -= 1.0f;
			}
			if (GetKeyboardPress(DIK_LEFT))
			{
				inputVec.x -= 1.0f;
			}
			if (GetKeyboardPress(DIK_DOWN))
			{
				inputVec.y += 1.0f;
			}
			if (GetKeyboardPress(DIK_RIGHT))
			{
				inputVec.x += 1.0f;
			}
			break;
		default:
			break;
		}

		D3DXVec3Normalize(&inputVec, &inputVec);

	}

	return inputVec;
}

//=========================================
// プレイヤーの取得処理
//=========================================
Player* GetPlayer(void)
{
	return s_player;
}