//================================//
//								  //
//       ディスクのファイル		  //
//       Author: Ricci Alex		  //
//       Author: tanimoto		  //
//								  //
//================================//

//====================================
//インクルードファイル
//====================================
#include "disk.h"
#include "wall.h"
#include "goal.h"
#include <time.h>
#include "shadow.h"

//====================================
//グローバル変数
//====================================
static Disk g_aDisk[MAX_DISK];									//ディスク型のグローバル変数を宣言する
static LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffDisk = NULL;			//頂点バッファへのポインタ
static LPDIRECT3DTEXTURE9 g_apTexDisk = NULL;					//テクスチャへのポインタ

//====================================
//ディスクの初期化処理
//====================================
void InitDisk(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイス情報の取得
	VERTEX_2D*pVtx;									//頂点情報へのポインタ
	srand((unsigned)time(NULL));

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\PlayerSpriteSheet.png",
		&g_apTexDisk);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_DISK,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffDisk,
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffDisk->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntDisk = 0; nCntDisk < MAX_DISK; nCntDisk++)
	{
		ZeroMemory(&g_aDisk[nCntDisk], sizeof(Disk));

		//頂点座標の設定
		pVtx[(nCntDisk * 4) + 0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[(nCntDisk * 4) + 1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[(nCntDisk * 4) + 2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[(nCntDisk * 4) + 3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//rhwの設定
		pVtx[(nCntDisk * 4) + 0].rhw = 1.0f;
		pVtx[(nCntDisk * 4) + 1].rhw = 1.0f;
		pVtx[(nCntDisk * 4) + 2].rhw = 1.0f;
		pVtx[(nCntDisk * 4) + 3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[(nCntDisk * 4) + 0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[(nCntDisk * 4) + 1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[(nCntDisk * 4) + 2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[(nCntDisk * 4) + 3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//テクスチャ座標の設定
		pVtx[(nCntDisk * 4) + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[(nCntDisk * 4) + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[(nCntDisk * 4) + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[(nCntDisk * 4) + 3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//頂点バッファをアンロックする
	g_pVtxBuffDisk->Unlock();
}

//====================================
//ディスクの終了処理
//====================================
void UninitDisk(void)
{
	//頂点バッファの破棄
	if (g_pVtxBuffDisk != NULL)
	{
		g_pVtxBuffDisk->Release();
		g_pVtxBuffDisk = NULL;
	}

	//テクスチャポインタの破棄
	if (g_apTexDisk != NULL)
	{
		g_apTexDisk->Release();
		g_apTexDisk = NULL;
	}
}

//====================================
//ディスクの更新処理
//====================================
void UpdateDisk(void)
{
	for (int nCntDisk = 0; nCntDisk < MAX_DISK; nCntDisk++)
	{
		if (g_aDisk[nCntDisk].bUse == true)
		{
			g_aDisk[nCntDisk].pos += g_aDisk[nCntDisk].move;				//現在位置の更新
			g_aDisk[nCntDisk].move += g_aDisk[nCntDisk].acc;				//移動量の更新

			switch (g_aDisk[nCntDisk].type)
			{
			default:
				break;

			case DISK_TYPE_LOB:

				if (g_aDisk[nCntDisk].fHeight > 0.0f)
				{
					g_aDisk[nCntDisk].fHeight += g_aDisk[nCntDisk].fVerticalSpeed;
					g_aDisk[nCntDisk].fVerticalSpeed -= 0.05f;

					if (g_aDisk[nCntDisk].fHeight <= 0.0f)
					{
						g_aDisk[nCntDisk].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						g_aDisk[nCntDisk].acc = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					}
				}
				break;

			case DISK_TYPE_SPECIAL_0:

				UpdateSpecialDisk(nCntDisk, 0);

				break;

			case DISK_TYPE_SPECIAL_1:

				UpdateSpecialDisk(nCntDisk, 0);

				break;
			}

			if (g_aDisk[nCntDisk].bBounce == true)
			{
				//壁との当たり判定
				WallBounce(&g_aDisk[nCntDisk].pos, &g_aDisk[nCntDisk].lastPos, &g_aDisk[nCntDisk].move, &g_aDisk[nCntDisk].acc, 10.0f);
			}

			//ゴールとの当たり判定(pos, lastPos, fWidth, fHeight)
			ColisionGoal(&g_aDisk[nCntDisk].pos, &g_aDisk[nCntDisk].lastPos, 10.0f, 10.0f);

			g_aDisk[nCntDisk].lastPos = g_aDisk[nCntDisk].pos;							//前回の位置の更新
	
			VERTEX_2D *pVtx = NULL;					//頂点情報へのポインタ

			//頂点バッファをロックし、頂点情報へのポインタを取得
			g_pVtxBuffDisk->Lock(0, 0, (void**)&pVtx, 0);

			pVtx[(nCntDisk * 4) + 0].pos = D3DXVECTOR3(g_aDisk[nCntDisk].pos.x - (g_aDisk[nCntDisk].fSize * 0.5f * (1 + (g_aDisk[nCntDisk].fHeight * 0.005f))), 
				g_aDisk[nCntDisk].pos.y - ((g_aDisk[nCntDisk].fHeight - 10.0f) * 0.5f) - (g_aDisk[nCntDisk].fSize * 0.5f * (1 + (g_aDisk[nCntDisk].fHeight * 0.005f))), 0.0f);

			pVtx[(nCntDisk * 4) + 1].pos = D3DXVECTOR3(g_aDisk[nCntDisk].pos.x + (g_aDisk[nCntDisk].fSize * 0.5f * (1 + (g_aDisk[nCntDisk].fHeight * 0.005f))),
				g_aDisk[nCntDisk].pos.y - ((g_aDisk[nCntDisk].fHeight - 10.0f) * 0.5f) - (g_aDisk[nCntDisk].fSize * 0.5f * (1 + (g_aDisk[nCntDisk].fHeight * 0.005f))), 0.0f);

			pVtx[(nCntDisk * 4) + 2].pos = D3DXVECTOR3(g_aDisk[nCntDisk].pos.x - (g_aDisk[nCntDisk].fSize * 0.5f * (1 + (g_aDisk[nCntDisk].fHeight * 0.005f))),
				g_aDisk[nCntDisk].pos.y - ((g_aDisk[nCntDisk].fHeight - 10.0f) * 0.5f) + (g_aDisk[nCntDisk].fSize * 0.5f * (1 + (g_aDisk[nCntDisk].fHeight * 0.005f))), 0.0f);

			pVtx[(nCntDisk * 4) + 3].pos = D3DXVECTOR3(g_aDisk[nCntDisk].pos.x + (g_aDisk[nCntDisk].fSize * 0.5f * (1 + (g_aDisk[nCntDisk].fHeight * 0.005f))), 
				g_aDisk[nCntDisk].pos.y - ((g_aDisk[nCntDisk].fHeight - 10.0f) * 0.5f) + (g_aDisk[nCntDisk].fSize * 0.5f * (1 + (g_aDisk[nCntDisk].fHeight * 0.005f))), 0.0f);

			//頂点バッファをアンロックする
			g_pVtxBuffDisk->Unlock();

			SetPositionShadow(g_aDisk[nCntDisk].nIdxShadow, g_aDisk[nCntDisk].pos);		//影の位置の更新
		}
	}
}

//====================================
//ディスクの描画処理
//====================================
void DrawDisk(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffDisk, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntDisk = 0; nCntDisk < MAX_DISK; nCntDisk++)
	{
		if (g_aDisk[nCntDisk].bUse == true)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_apTexDisk);

			//ディスクを描画する
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntDisk * 4, 2);
		}
	}
}

//====================================
//ディスクの設定処理
//====================================
void SetDisk(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 acc, DISK_TYPE type, int nPlayer, float size)
{
	VERTEX_2D *pVtx = NULL;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffDisk->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntDisk = 0; nCntDisk < MAX_DISK; nCntDisk++)
	{
		if (g_aDisk[nCntDisk].bUse == false)
		{
			g_aDisk[nCntDisk].pos = pos;									//ディスクの現在の位置の設定
			g_aDisk[nCntDisk].lastPos = pos - move;							//ディスクの前回の位置の設定
			g_aDisk[nCntDisk].move = move;									//ディスクの移動量の設定
			g_aDisk[nCntDisk].fSize = size;									//ディスクの大きさの設定
			g_aDisk[nCntDisk].acc = acc;									//ディスクの加速の設定
			g_aDisk[nCntDisk].type = type;									//ディスクの種類の設定
			g_aDisk[nCntDisk].nCntPhase = 0;								//ディスクの必殺技用のカウンターの設定
			g_aDisk[nCntDisk].nPlayer = nPlayer;							//ディスクを投げたプレイヤーのインデックスの設定
			g_aDisk[nCntDisk].nIdxShadow = SetShadow(pos, size);			//ディスクの影のインデックス
			g_aDisk[nCntDisk].bUse = true;									//使用されている状態にする

			//頂点座標の設定
			pVtx[(nCntDisk * 4) + 0].pos = D3DXVECTOR3(pos.x - (size * 0.5f), pos.y - (size * 0.5f), 0.0f);
			pVtx[(nCntDisk * 4) + 1].pos = D3DXVECTOR3(pos.x + (size * 0.5f), pos.y - (size * 0.5f), 0.0f);
			pVtx[(nCntDisk * 4) + 2].pos = D3DXVECTOR3(pos.x - (size * 0.5f), pos.y + (size * 0.5f), 0.0f);
			pVtx[(nCntDisk * 4) + 3].pos = D3DXVECTOR3(pos.x + (size * 0.5f), pos.y + (size * 0.5f), 0.0f);

			//頂点カラーの設定
			pVtx[(nCntDisk * 4) + 0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[(nCntDisk * 4) + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[(nCntDisk * 4) + 2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[(nCntDisk * 4) + 3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			switch (type)
			{
			default:

				g_aDisk[nCntDisk].fHeight = NORMAL_DISK_HEIGHT;
				g_aDisk[nCntDisk].fVerticalSpeed = NORMAL_VERTICAL_SPEED;
				g_aDisk[nCntDisk].bBounce = true;

				break;

			case DISK_TYPE_LOB:

				g_aDisk[nCntDisk].fHeight = NORMAL_DISK_HEIGHT;
				g_aDisk[nCntDisk].fVerticalSpeed = LOB_STARTING_SPEED;
				g_aDisk[nCntDisk].bBounce = true;
				
				g_aDisk[nCntDisk].move = SetLobSpeed(g_aDisk[nCntDisk].pos, g_aDisk[nCntDisk].move, nCntDisk, g_aDisk[nCntDisk].fHeight, g_aDisk[nCntDisk].fVerticalSpeed);

				break;

			case DISK_TYPE_JUMP:

				g_aDisk[nCntDisk].fHeight = JUMP_MAX_HEIGHT;
				g_aDisk[nCntDisk].fVerticalSpeed = JUMP_MAX_HEIGHT / JUMP_ATTACK_TIME;
				g_aDisk[nCntDisk].bBounce = false;

				g_aDisk[nCntDisk].move = SetLobSpeed(g_aDisk[nCntDisk].pos, g_aDisk[nCntDisk].move, nCntDisk, g_aDisk[nCntDisk].fHeight, g_aDisk[nCntDisk].fVerticalSpeed);

				break;

			case DISK_TYPE_SPECIAL_0:

				g_aDisk[nCntDisk].fHeight = NORMAL_DISK_HEIGHT;
				g_aDisk[nCntDisk].fVerticalSpeed = NORMAL_VERTICAL_SPEED;
				g_aDisk[nCntDisk].bBounce = false;

				break;

			case DISK_TYPE_SPECIAL_1:

				g_aDisk[nCntDisk].fHeight = NORMAL_DISK_HEIGHT;
				g_aDisk[nCntDisk].fVerticalSpeed = NORMAL_VERTICAL_SPEED;
				g_aDisk[nCntDisk].bBounce = false;

				break;
			}

			break;
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffDisk->Unlock();
}

//必殺技
void UpdateSpecialDisk(int nCntDisk, int nPlayer)
{
	float fChangePoint;
	bool bImpact = false;

	fChangePoint = SCREEN_WIDTH * (0.7f);

	switch (g_aDisk[nCntDisk].type)
	{
	//====================================================================================================================================
	case DISK_TYPE_SPECIAL_0:

		switch (g_aDisk[nCntDisk].nCntPhase)
		{
		default:
			break;

		case 0:

			bImpact = SpecialWallBounce(&g_aDisk[nCntDisk].pos, &g_aDisk[nCntDisk].lastPos, &g_aDisk[nCntDisk].move, 10.0f);

			if (bImpact == true)
			{
				g_aDisk[nCntDisk].nCntPhase++;
				g_aDisk[nCntDisk].move = D3DXVECTOR3(15.0f, 0.0f, 0.0f);
				g_aDisk[nCntDisk].acc = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}

			break;

		case 1:

			fChangePoint = SCREEN_WIDTH * (0.7f);

			if (g_aDisk[nCntDisk].pos.x >= fChangePoint)
			{
				g_aDisk[nCntDisk].nCntPhase++;
				g_aDisk[nCntDisk].move = D3DXVECTOR3(-0.5f, 15.0f, 0.0f);
				g_aDisk[nCntDisk].acc = D3DXVECTOR3(0.3f, 0.0f, 0.0f);
			}

			break;

		case 2:

			bImpact = SpecialWallBounce(&g_aDisk[nCntDisk].pos, &g_aDisk[nCntDisk].lastPos, &g_aDisk[nCntDisk].move, 10.0f);

			if (bImpact == true)
			{
				g_aDisk[nCntDisk].nCntPhase++;
				g_aDisk[nCntDisk].move = D3DXVECTOR3(15.0f, -6.0f, 0.0f);
				g_aDisk[nCntDisk].acc = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}

			break;
		}

		break;
		//====================================================================================================================================

	case DISK_TYPE_SPECIAL_1:

		switch (g_aDisk[nCntDisk].nCntPhase)
		{
		case 0:

			fChangePoint = SCREEN_WIDTH * (0.3f);

			if (g_aDisk[nCntDisk].pos.x >= fChangePoint)
			{
				g_aDisk[nCntDisk].nCntPhase++;
				g_aDisk[nCntDisk].move = D3DXVECTOR3(0.0f, -15.0f, 0.0f);
				g_aDisk[nCntDisk].acc = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}

			break;

		case 1:

			bImpact = SpecialWallBounce(&g_aDisk[nCntDisk].pos, &g_aDisk[nCntDisk].lastPos, &g_aDisk[nCntDisk].move, 10.0f);

			if (bImpact == true)
			{
				g_aDisk[nCntDisk].nCntPhase++;
				g_aDisk[nCntDisk].move = D3DXVECTOR3(15.0f, 0.0f, 0.0f);
				g_aDisk[nCntDisk].acc = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}

			break;

		case 2:

			fChangePoint = SCREEN_WIDTH * (0.7f);

			if (g_aDisk[nCntDisk].pos.x >= fChangePoint)
			{
				g_aDisk[nCntDisk].nCntPhase++;
				g_aDisk[nCntDisk].move = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
				g_aDisk[nCntDisk].acc = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}

			break;

		case 3:

			fChangePoint = 400.0f + (rand() % 201);

			if (g_aDisk[nCntDisk].pos.y >= fChangePoint)
			{
				g_aDisk[nCntDisk].nCntPhase++;
				g_aDisk[nCntDisk].move = D3DXVECTOR3(15.0f, 00.0f, 0.0f);
				g_aDisk[nCntDisk].acc = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}

			break;
		}

		break;
		//====================================================================================================================================
	}
}

//============================================================================
//ディスクの取得処理
//============================================================================
Disk *GetDisk(void)
{
	return g_aDisk;	//ディスク情報の先頭アドレスを返す
}

//============================================================================
//上投げの移動量の設定処理
//============================================================================
D3DXVECTOR3 SetLobSpeed(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nCntDisk, float fHeight, float fVerticalSpeed)
{
	D3DXVECTOR3 newSpeed = move;										//新しい移動量
	D3DXVECTOR3 lastPos = pos;											//最初の位置
	D3DXVECTOR3 endPos = pos;											//落ちた後の位置
	D3DXVECTOR3 newPos;													//新しい位置
	D3DXVECTOR3 initialSpeedDir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//最初の移動量
	D3DXVECTOR3 initialSpeedDirNor;										//最初の移動量の向き
	float height, vSpeed;
	height = fHeight;
	vSpeed = fVerticalSpeed;

	while (height > 0.0f)
	{//ディスクが落ちた時の位置を計算する
		height += vSpeed;
		vSpeed -= 0.05f;
		endPos += move;
	}
	newPos = endPos;
	height = fHeight;
	vSpeed = fVerticalSpeed;

	initialSpeedDir = endPos - pos;			//最初の移動量ベクトル

	//壁との当たり判定
	bool bImpact = SpecialWallBounce(&endPos, &lastPos, &newSpeed, g_aDisk[nCntDisk].fSize);	

	float fLenght;							//ベクトルの長さ

	if (bImpact == true)
	{//壁と当たらないようにする処理
		D3DXVECTOR3 xDir = D3DXVECTOR3(1.0f, 0.0f, 0.0f);				//X軸に平行ベクトル
		D3DXVECTOR3 postImpact = endPos - pos;							//衝突点を超えたベクトルの部分
		D3DXVec3Normalize(&initialSpeedDirNor, &initialSpeedDir);		//ベクトルの正規化

		float fDot = D3DXVec3Dot(&xDir, &initialSpeedDirNor);									//X軸とはじめの移動量の間の角度のコサインを計算する
		fLenght = sqrtf(((postImpact.x * postImpact.x) + (postImpact.y * postImpact.y)));		//衝突点を超えたベクトルの部分の長さを計算する

		newPos = endPos + (xDir * (fDot * fLenght));							//新しい位置

		//新しい移動量の向きの計算
		newSpeed = newPos - lastPos;											
		D3DXVec3Normalize(&newSpeed, &newSpeed);

		fLenght = sqrtf(((move.x * move.x) + (move.y * move.y)));				//はじめの移動量のベクトルの長さを計算する
		newSpeed *= fLenght;													//新しい移動量
	}

	{//エリアを出ないようにする処理
		D3DXVECTOR3 secondTerm;
		float coefficient;

		endPos = lastPos;

		while (height > 0.0f)
		{//ディスクが落ちた時の位置を計算する
			height += vSpeed;
			vSpeed -= 0.05f;
			endPos += newSpeed;
		}
		newPos = endPos;

		if (newPos.x > SCREEN_WIDTH - 150.0f && newSpeed.x > 0.0f)
		{//画面の右側を出る場合
			secondTerm = newPos;
			secondTerm.x = SCREEN_WIDTH - 150.0f;
			coefficient = (secondTerm.x - lastPos.x) / (newPos.x - lastPos.x);
			newSpeed.x *= coefficient;
		}
		else if (newPos.x < 150.0f && newSpeed.x < 0.0f)
		{//画面の左側を出る場合
			secondTerm = newPos;
			secondTerm.x = 150.0f;
			coefficient = (secondTerm.x - lastPos.x) / (newPos.x - lastPos.x);
			newSpeed.x *= coefficient;
		}
	}

	return newSpeed;				//新しい移動量を返す
}

//======================================
//ディスクの破棄
//======================================
void DestroyDisk(void)
{
	Shadow *pShadow = GetShadow();						//影の情報へのポインタ

	g_aDisk[0].bUse = false;							//ディスクを使用されていない状態にする
	pShadow[g_aDisk[0].nIdxShadow].bUse = false;		//影を使用されていない状態にする
}