//================================//
//								  //
//       �ǂ̃t�@�C��	    	  //
//       Author: Ricci Alex		  //
//								  //
//================================//

//====================================
//�C���N���[�h�t�@�C��
//====================================
#include "wall.h"

//====================================
//�O���[�o���ϐ�
//====================================
static Wall g_aWall[MAX_WALL];									//�f�B�X�N�^�̃O���[�o���ϐ���錾����
static LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWall = NULL;			//���_�o�b�t�@�ւ̃|�C���^
static LPDIRECT3DTEXTURE9 g_apTexWall = NULL;					//�e�N�X�`���ւ̃|�C���^

//====================================
//�ǂ̏���������
//====================================
void InitWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X���̎擾
	VERTEX_2D*pVtx;									//���_���ւ̃|�C���^

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\PlayerSpriteSheet.png",
		&g_apTexWall);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_WALL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffWall,
		NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		ZeroMemory(&g_aWall[nCntWall], sizeof(Wall));

		//���_���W�̐ݒ�
		pVtx[(nCntWall * 4) + 0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[(nCntWall * 4) + 1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[(nCntWall * 4) + 2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[(nCntWall * 4) + 3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//rhw�̐ݒ�
		pVtx[(nCntWall * 4) + 0].rhw = 1.0f;
		pVtx[(nCntWall * 4) + 1].rhw = 1.0f;
		pVtx[(nCntWall * 4) + 2].rhw = 1.0f;
		pVtx[(nCntWall * 4) + 3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[(nCntWall * 4) + 0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[(nCntWall * 4) + 1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[(nCntWall * 4) + 2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[(nCntWall * 4) + 3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[(nCntWall * 4) + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[(nCntWall * 4) + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[(nCntWall * 4) + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[(nCntWall * 4) + 3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffWall->Unlock();

	SetWall(D3DXVECTOR3(SCREEN_WIDTH * 0.8f, SCREEN_HEIGHT * 0.5f, 0.0f), SCREEN_WIDTH , 10.0f, D3DX_PI * 0.35f);
	
}

//====================================
//�ǂ̏I������
//====================================
void UninitWall(void)
{
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffWall != NULL)
	{
		g_pVtxBuffWall->Release();
		g_pVtxBuffWall = NULL;
	}

	//�e�N�X�`���|�C���^�̔j��
	{
		if (g_apTexWall != NULL)
		{
			g_apTexWall->Release();
			g_apTexWall = NULL;
		}
	}
}

//====================================
//�ǂ̍X�V����
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
//�ǂ̕`�揈��
//====================================
void DrawWall(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffWall, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_apTexWall);

		//�f�B�X�N��`�悷��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntWall * 4, 2);
	}
}

//====================================
//�ǂ̐ݒ菈��
//====================================
void SetWall(D3DXVECTOR3 pos, float width, float height, float angle)
{
	VERTEX_2D *pVtx = NULL;							//���_���ւ̃|�C���^
	D3DXMATRIX mtxRot, mtxTrans, mtxOut;			//�v�Z�p�̃}�g���b�N�X

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if (g_aWall[nCntWall].bUse == false)
		{
			g_aWall[nCntWall].pos = pos;										//�ǂ̈ʒu�̐ݒ�
			g_aWall[nCntWall].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);				//�ǂ̖@���̐ݒ�
			g_aWall[nCntWall].fWidth = width;									//�ǂ̕��̐ݒ�
			g_aWall[nCntWall].fHeight = height;									//�ǂ̍����̐ݒ�
			g_aWall[nCntWall].fAlpha = angle;									//�ǂ̉�]�p�x�̐ݒ�

			//�����𔽉f
			D3DXMatrixIdentity(&mtxRot);
			D3DXMatrixRotationZ(&mtxRot, angle);

			//�ǂ̖@�����v�Z����
			D3DXVec3TransformCoord(&g_aWall[nCntWall].nor, &g_aWall[nCntWall].nor, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixIdentity(&mtxTrans);
			D3DXMatrixTranslation(&mtxTrans, g_aWall[nCntWall].pos.x, g_aWall[nCntWall].pos.y, g_aWall[nCntWall].pos.z);
			D3DXMatrixMultiply(&mtxOut, &mtxRot, &mtxTrans);

			//���_���W�̐ݒ�
			pVtx[(nCntWall * 4) + 0].pos = D3DXVECTOR3(- (width * 0.5f), - (height * 0.5f), 0.0f);
			pVtx[(nCntWall * 4) + 1].pos = D3DXVECTOR3(+ (width * 0.5f), - (height * 0.5f), 0.0f);
			pVtx[(nCntWall * 4) + 2].pos = D3DXVECTOR3(- (width * 0.5f), + (height * 0.5f), 0.0f);
			pVtx[(nCntWall * 4) + 3].pos = D3DXVECTOR3(+ (width * 0.5f), + (height * 0.5f), 0.0f);

			//��]��̒��_���W���v�Z����
			D3DXVec3TransformCoord(&pVtx[(nCntWall * 4) + 0].pos, &pVtx[(nCntWall * 4) + 0].pos, &mtxOut);
			D3DXVec3TransformCoord(&pVtx[(nCntWall * 4) + 1].pos, &pVtx[(nCntWall * 4) + 1].pos, &mtxOut);
			D3DXVec3TransformCoord(&pVtx[(nCntWall * 4) + 2].pos, &pVtx[(nCntWall * 4) + 2].pos, &mtxOut);
			D3DXVec3TransformCoord(&pVtx[(nCntWall * 4) + 3].pos, &pVtx[(nCntWall * 4) + 3].pos, &mtxOut);

			//���_�J���[�̐ݒ�
			pVtx[(nCntWall * 4) + 0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[(nCntWall * 4) + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[(nCntWall * 4) + 2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[(nCntWall * 4) + 3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			g_aWall[nCntWall].bUse = true;			//�g�p����Ă����Ԃɂ���

			break;
		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffWall->Unlock();
}

//�ǂɓ���������A���˂��鏈��
void WallBounce(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pLastPos, D3DXVECTOR3 *pMove, float fRadius, int nIdx)
{
	VERTEX_2D *pVtx = NULL;											//���_���ւ̃|�C���^
	D3DXVECTOR3 edge, position, lastPosition, result1, result2;		//�v�Z�p�̃x�N�g��

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	edge = pVtx[(nIdx * 4) + 3].pos - pVtx[(nIdx * 4) + 2].pos;
	position = pVtx[(nIdx * 4) + 2].pos - *(pPos);
	lastPosition = pVtx[(nIdx * 4) + 2].pos - *(pLastPos);

	D3DXVec3Cross(&result1, &edge, &position);
	D3DXVec3Cross(&result2, &edge, &lastPosition);

	if (result1.z * result2.z <= 0)
	{//���������ꍇ
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

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffWall->Unlock();
}