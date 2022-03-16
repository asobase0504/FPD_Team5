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
#include "effect.h"

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
void WallBounce(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pLastPos, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pAcc, float fRadius)
{
		VERTEX_2D *pVtx = NULL;													//���_���ւ̃|�C���^
		D3DXVECTOR3 edge, position, lastPosition, result1, result2, point;		//�v�Z�p�̃x�N�g��

		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

		for (int nIdx = 0; nIdx < MAX_WALL; nIdx++)
		{
			if (g_aWall[nIdx].bUse == true)
			{
				D3DXVECTOR3 Vtx1, Vtx2;

				//�e�̈�ԋ߂��_�����߂�
				point.x = pPos->x + (-g_aWall[nIdx].nor.x * fRadius);
				point.y = pPos->y + (-g_aWall[nIdx].nor.y * fRadius);
				point.z = 0.0f;

				edge = pVtx[(nIdx * 4) + 3].pos - pVtx[(nIdx * 4) + 2].pos;		//�ǂ̕ӂ̃x�N�g��
				position = pVtx[(nIdx * 4) + 2].pos - point;					//���݂̈ʒu����ǂ̒��_�܂ł̃x�N�g��
				lastPosition = pVtx[(nIdx * 4) + 2].pos - *(pLastPos);			//�O��̈ʒu����ǂ̒��_�܂ł̃x�N�g��

				//�O�ς��v�Z����B���ʂ̕������Ⴄ�ꍇ�A�e�͕ǂɓ�������
				D3DXVec3Cross(&result1, &edge, &position);						
				D3DXVec3Cross(&result2, &edge, &lastPosition);					

				if (result1.z * result2.z <= 0)
				{//���������ꍇ

					//�Փ˓_�̍��W�̌v�Z=======================================================================================

					D3DXVECTOR3 impact, newPosition;

					position = point - *(pLastPos);						//�O��̈ʒu���猻�݂̈ʒu�܂ł̃x�N�g��

					//�O��̈ʒu����Փ˓_�܂ł̋������v�Z����
					D3DXVec3Cross(&result1, &lastPosition, &edge);
					D3DXVec3Cross(&result2, &position, &edge);			

					float fLenght = (result1.z / result2.z) - 0.1f;		//�O��̈ʒu����Փ˓_�܂ł̋���

					D3DXVec3Normalize(&position, &position);			//�O��̈ʒu���猻�݂̈ʒu�܂ł̃x�N�g���̌���

					impact = *(pLastPos)+(fLenght * position);			//�Փ˓_

					lastPosition = *(pLastPos)-impact;					//�O��̈ʒu����Փ˓_�܂ł̃x�N�g��


					////=========================================================================================================

					////���ˏ���=================================================================================================

					float fCostrLenght, fDot, fAlpha, fEdgeLenght, fheight, fBounceLenght, fmove;
					D3DXVECTOR3 constrPoint, dP, pN, bouncePoint, bounceDir, finalPos;

					fmove = sqrtf((pMove->x * pMove->x) + (pMove->y * pMove->y));																				//�ړ��ʂ̃x�N�g���̑傫��
					fBounceLenght = sqrtf((((pPos->x - impact.x) * (pPos->x - impact.x)) + ((pPos->y - impact.y) * (pPos->y - impact.y))));						//�Փ˓_���猻�݂̈ʒu�܂ł̃x�N�g���̑傫��
					fCostrLenght = sqrtf((((impact.x - pLastPos->x) * (impact.x - pLastPos->x)) + ((impact.y - pLastPos->y) * (impact.y - pLastPos->y))));		//�O��̈ʒu����Փ˓_�܂ł̃x�N�g���̑傫��
					fEdgeLenght = sqrtf((edge.x * edge.x) + (edge.y * edge.y));																					//�ǂ̕ӂ̒���

					dP = D3DXVECTOR3(*(pPos)-*(pLastPos));		
					D3DXVec3Normalize(&pN, &dP);				//�O��̈ʒu���猻�݂̈ʒu�܂ł̃x�N�g���̌���

					//���ˌ�̓_�̍��W�̌v�Z============================================
					constrPoint = impact + (pN * fCostrLenght);	
					dP = constrPoint - impact;					
					
					fDot = D3DXVec3Dot(&dP, &edge);
					fAlpha = acosf(fDot / (fEdgeLenght * fCostrLenght));

					fheight = fLenght * sinf(fAlpha);

					bouncePoint = constrPoint + (g_aWall[nIdx].nor * 2.0f * fheight);
					bounceDir = bouncePoint - impact;
					D3DXVec3Normalize(&bounceDir, &bounceDir);

					finalPos = impact + (bounceDir * fBounceLenght);
					//==================================================================

					*(pPos) = finalPos;							//�e�̐V�����ʒu�̐ݒ�
					*(pMove) = fmove * bounceDir;				//�e�̐V�����ړ��ʂ̐ݒ�
					*(pAcc) = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					float fY;
					float fBeta;

					if (impact.y < SCREEN_HEIGHT * 0.5f)
					{
						fY = 110.0f;
						fBeta = -D3DX_PI * 0.45f;
					}
					else
					{
						fY = SCREEN_HEIGHT - 110.0f;
						fBeta = D3DX_PI * 0.55f;
					}
					
					for (int nCntAngle = 0; nCntAngle < 10; nCntAngle++, fBeta += D3DX_PI * 0.1f)
					{
						SetEffect(D3DXVECTOR3(impact.x, fY, 0.0f), fBeta, EFFECT_TYPE_WALL_IMPACT);
					}
				}
			}
		}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffWall->Unlock();
}

//==============================================
//�K�E�Z�p�̓����蔻��
//==============================================
bool SpecialWallBounce(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pLastPos, D3DXVECTOR3 *pMove, float fRadius)
{
	VERTEX_2D *pVtx = NULL;													//���_���ւ̃|�C���^
	D3DXVECTOR3 edge, position, lastPosition, result1, result2, point;		//�v�Z�p�̃x�N�g��
	bool bImpact = false;													//�����������ǂ���

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	for (int nIdx = 0; nIdx < MAX_WALL; nIdx++)
	{
		if (g_aWall[nIdx].bUse == true)
		{
			D3DXVECTOR3 Vtx1, Vtx2;

			//�e�̈�ԋ߂��_�����߂�
			point.x = pPos->x + (-g_aWall[nIdx].nor.x * fRadius);
			point.y = pPos->y + (-g_aWall[nIdx].nor.y * fRadius);
			point.z = 0.0f;

			edge = pVtx[(nIdx * 4) + 3].pos - pVtx[(nIdx * 4) + 2].pos;		//�ǂ̕ӂ̃x�N�g��
			position = pVtx[(nIdx * 4) + 2].pos - point;					//���݂̈ʒu����ǂ̒��_�܂ł̃x�N�g��
			lastPosition = pVtx[(nIdx * 4) + 2].pos - *(pLastPos);			//�O��̈ʒu����ǂ̒��_�܂ł̃x�N�g��

			//�O�ς��v�Z����B���ʂ̕������Ⴄ�ꍇ�A�e�͕ǂɓ�������
			D3DXVec3Cross(&result1, &edge, &position);
			D3DXVec3Cross(&result2, &edge, &lastPosition);

			if (result1.z * result2.z <= 0)
			{//���������ꍇ

			 //�Փ˓_�̍��W�̌v�Z=======================================================================================

				D3DXVECTOR3 impact, newPosition;

				position = point - *(pLastPos);						//�O��̈ʒu���猻�݂̈ʒu�܂ł̃x�N�g��

																	//�O��̈ʒu����Փ˓_�܂ł̋������v�Z����
				D3DXVec3Cross(&result1, &lastPosition, &edge);
				D3DXVec3Cross(&result2, &position, &edge);

				float fLenght = (result1.z / result2.z) - 0.1f;		//�O��̈ʒu����Փ˓_�܂ł̋���

				D3DXVec3Normalize(&position, &position);			//�O��̈ʒu���猻�݂̈ʒu�܂ł̃x�N�g���̌���

				impact = *(pLastPos)+(fLenght * position);			//�Փ˓_

				lastPosition = *(pLastPos)-impact;					//�O��̈ʒu����Փ˓_�܂ł̃x�N�g��
				////=========================================================================================================

				*(pPos) = impact /*+ (g_aWall[nIdx].nor * fRadius)*/;	//
				bImpact = true;										//
			}
		}
	}

	return bImpact;

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffWall->Unlock();
}