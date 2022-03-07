//================================//
//								  //
//       壁のファイル	    	  //
//       Author: Ricci Alex		  //
//								  //
//================================//

//====================================
//インクルードファイル
//====================================
#include "wall.h"

//====================================
//グローバル変数
//====================================
static Wall g_aWall[MAX_WALL];									//ディスク型のグローバル変数を宣言する
static LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWall = NULL;			//頂点バッファへのポインタ
static LPDIRECT3DTEXTURE9 g_apTexWall = NULL;					//テクスチャへのポインタ

//====================================
//壁の初期化処理
//====================================
void InitWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイス情報の取得
	VERTEX_2D*pVtx;									//頂点情報へのポインタ

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\PlayerSpriteSheet.png",
		&g_apTexWall);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_WALL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffWall,
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		ZeroMemory(&g_aWall[nCntWall], sizeof(Wall));

		//頂点座標の設定
		pVtx[(nCntWall * 4) + 0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[(nCntWall * 4) + 1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[(nCntWall * 4) + 2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[(nCntWall * 4) + 3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//rhwの設定
		pVtx[(nCntWall * 4) + 0].rhw = 1.0f;
		pVtx[(nCntWall * 4) + 1].rhw = 1.0f;
		pVtx[(nCntWall * 4) + 2].rhw = 1.0f;
		pVtx[(nCntWall * 4) + 3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[(nCntWall * 4) + 0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[(nCntWall * 4) + 1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[(nCntWall * 4) + 2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[(nCntWall * 4) + 3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//テクスチャ座標の設定
		pVtx[(nCntWall * 4) + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[(nCntWall * 4) + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[(nCntWall * 4) + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[(nCntWall * 4) + 3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//頂点バッファをアンロックする
	g_pVtxBuffWall->Unlock();

	SetWall(D3DXVECTOR3(SCREEN_WIDTH * 0.8f, SCREEN_HEIGHT * 0.5f, 0.0f), SCREEN_WIDTH , 10.0f, D3DX_PI * 0.35f);
	
}

//====================================
//壁の終了処理
//====================================
void UninitWall(void)
{
	//頂点バッファの破棄
	if (g_pVtxBuffWall != NULL)
	{
		g_pVtxBuffWall->Release();
		g_pVtxBuffWall = NULL;
	}

	//テクスチャポインタの破棄
	{
		if (g_apTexWall != NULL)
		{
			g_apTexWall->Release();
			g_apTexWall = NULL;
		}
	}
}

//====================================
//壁の更新処理
//====================================
void UpdateWall(void)
{
	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if (g_aWall[nCntWall].bUse == true)
		{

		}
	}
}

//====================================
//壁の描画処理
//====================================
void DrawWall(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffWall, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_apTexWall);

		//ディスクを描画する
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntWall * 4, 2);
	}
}

//====================================
//壁の設定処理
//====================================
void SetWall(D3DXVECTOR3 pos, float width, float height, float angle)
{
	VERTEX_2D *pVtx = NULL;							//頂点情報へのポインタ
	D3DXMATRIX mtxRot, mtxTrans, mtxOut;			//計算用のマトリックス

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if (g_aWall[nCntWall].bUse == false)
		{
			g_aWall[nCntWall].pos = pos;										//壁の位置の設定
			g_aWall[nCntWall].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);				//壁の法線の設定
			g_aWall[nCntWall].fWidth = width;									//壁の幅の設定
			g_aWall[nCntWall].fHeight = height;									//壁の高さの設定
			g_aWall[nCntWall].fAlpha = angle;									//壁の回転角度の設定

			//向きを反映
			D3DXMatrixIdentity(&mtxRot);
			D3DXMatrixRotationZ(&mtxRot, angle);

			//壁の法線を計算する
			D3DXVec3TransformCoord(&g_aWall[nCntWall].nor, &g_aWall[nCntWall].nor, &mtxRot);

			//位置を反映
			D3DXMatrixIdentity(&mtxTrans);
			D3DXMatrixTranslation(&mtxTrans, g_aWall[nCntWall].pos.x, g_aWall[nCntWall].pos.y, g_aWall[nCntWall].pos.z);
			D3DXMatrixMultiply(&mtxOut, &mtxRot, &mtxTrans);

			//頂点座標の設定
			pVtx[(nCntWall * 4) + 0].pos = D3DXVECTOR3(- (width * 0.5f), - (height * 0.5f), 0.0f);
			pVtx[(nCntWall * 4) + 1].pos = D3DXVECTOR3(+ (width * 0.5f), - (height * 0.5f), 0.0f);
			pVtx[(nCntWall * 4) + 2].pos = D3DXVECTOR3(- (width * 0.5f), + (height * 0.5f), 0.0f);
			pVtx[(nCntWall * 4) + 3].pos = D3DXVECTOR3(+ (width * 0.5f), + (height * 0.5f), 0.0f);

			//回転後の頂点座標を計算する
			D3DXVec3TransformCoord(&pVtx[(nCntWall * 4) + 0].pos, &pVtx[(nCntWall * 4) + 0].pos, &mtxOut);
			D3DXVec3TransformCoord(&pVtx[(nCntWall * 4) + 1].pos, &pVtx[(nCntWall * 4) + 1].pos, &mtxOut);
			D3DXVec3TransformCoord(&pVtx[(nCntWall * 4) + 2].pos, &pVtx[(nCntWall * 4) + 2].pos, &mtxOut);
			D3DXVec3TransformCoord(&pVtx[(nCntWall * 4) + 3].pos, &pVtx[(nCntWall * 4) + 3].pos, &mtxOut);

			//頂点カラーの設定
			pVtx[(nCntWall * 4) + 0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[(nCntWall * 4) + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[(nCntWall * 4) + 2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[(nCntWall * 4) + 3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			g_aWall[nCntWall].bUse = true;			//使用されている状態にする

			break;
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffWall->Unlock();
}

//壁に当たったら、反射する処理
void WallBounce(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pLastPos, D3DXVECTOR3 *pMove, float fRadius, int nIdx)
{
	VERTEX_2D *pVtx = NULL;											//頂点情報へのポインタ
	D3DXVECTOR3 edge, position, lastPosition, result1, result2;		//計算用のベクトル

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	edge = pVtx[(nIdx * 4) + 3].pos - pVtx[(nIdx * 4) + 2].pos;
	position = pVtx[(nIdx * 4) + 2].pos - *(pPos);
	lastPosition = pVtx[(nIdx * 4) + 2].pos - *(pLastPos);

	D3DXVec3Cross(&result1, &edge, &position);
	D3DXVec3Cross(&result2, &edge, &lastPosition);

	if (result1.z * result2.z <= 0)
	{//当たった場合
		D3DXVECTOR3 impact, move;
		float fAngleCos;

		position = *(pPos) - *(pLastPos);

		D3DXVec3Cross(&result1, &lastPosition, &edge);
		D3DXVec3Cross(&result2, &position, &edge);

		float fLenght = result1.z / result2.z;

		impact = *(pPos)+(fLenght * position);

		fAngleCos = (D3DXVec3Dot(&g_aWall[nIdx].nor, &position)) / sqrtf((position.x * position.x) + (position.y * position.y));

		position = impact - *(pPos);

		fLenght = 2 * sqrtf((position.x * position.x) + (position.y * position.y));

		result1 = *(pPos)+(g_aWall[nIdx].nor * fLenght);
		result2 = result1 - *(pPos);

		D3DXVec3Normalize(&result2, &result2);

		move.x = result2.x * sqrtf((pMove->x * pMove->x) + (pMove->y * pMove->y));
		move.y = result2.y * sqrtf((pMove->x * pMove->x) + (pMove->y * pMove->y));
		move.z = 0.0f;

		*(pMove) = move;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffWall->Unlock();
}