//================================//
//								  //
//       �f�B�X�N�̃t�@�C��		  //
//       Author: Ricci Alex		  //
//       Author: tanimoto		  //
//								  //
//================================//

//====================================
//�C���N���[�h�t�@�C��
//====================================
#include "disk.h"
#include "wall.h"
#include "goal.h"
#include <time.h>
#include "shadow.h"

//====================================
//�O���[�o���ϐ�
//====================================
static Disk g_aDisk[MAX_DISK];									//�f�B�X�N�^�̃O���[�o���ϐ���錾����
static LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffDisk = NULL;			//���_�o�b�t�@�ւ̃|�C���^
static LPDIRECT3DTEXTURE9 g_apTexDisk = NULL;					//�e�N�X�`���ւ̃|�C���^

//====================================
//�f�B�X�N�̏���������
//====================================
void InitDisk(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X���̎擾
	VERTEX_2D*pVtx;									//���_���ւ̃|�C���^
	srand((unsigned)time(NULL));

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\PlayerSpriteSheet.png",
		&g_apTexDisk);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_DISK,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffDisk,
		NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffDisk->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntDisk = 0; nCntDisk < MAX_DISK; nCntDisk++)
	{
		ZeroMemory(&g_aDisk[nCntDisk], sizeof(Disk));

		//���_���W�̐ݒ�
		pVtx[(nCntDisk * 4) + 0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[(nCntDisk * 4) + 1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[(nCntDisk * 4) + 2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[(nCntDisk * 4) + 3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//rhw�̐ݒ�
		pVtx[(nCntDisk * 4) + 0].rhw = 1.0f;
		pVtx[(nCntDisk * 4) + 1].rhw = 1.0f;
		pVtx[(nCntDisk * 4) + 2].rhw = 1.0f;
		pVtx[(nCntDisk * 4) + 3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[(nCntDisk * 4) + 0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[(nCntDisk * 4) + 1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[(nCntDisk * 4) + 2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[(nCntDisk * 4) + 3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[(nCntDisk * 4) + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[(nCntDisk * 4) + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[(nCntDisk * 4) + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[(nCntDisk * 4) + 3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffDisk->Unlock();
}

//====================================
//�f�B�X�N�̏I������
//====================================
void UninitDisk(void)
{
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffDisk != NULL)
	{
		g_pVtxBuffDisk->Release();
		g_pVtxBuffDisk = NULL;
	}

	//�e�N�X�`���|�C���^�̔j��
	if (g_apTexDisk != NULL)
	{
		g_apTexDisk->Release();
		g_apTexDisk = NULL;
	}
}

//====================================
//�f�B�X�N�̍X�V����
//====================================
void UpdateDisk(void)
{
	for (int nCntDisk = 0; nCntDisk < MAX_DISK; nCntDisk++)
	{
		if (g_aDisk[nCntDisk].bUse == true)
		{
			g_aDisk[nCntDisk].pos += g_aDisk[nCntDisk].move;				//���݈ʒu�̍X�V
			g_aDisk[nCntDisk].move += g_aDisk[nCntDisk].acc;				//�ړ��ʂ̍X�V

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
				//�ǂƂ̓����蔻��
				WallBounce(&g_aDisk[nCntDisk].pos, &g_aDisk[nCntDisk].lastPos, &g_aDisk[nCntDisk].move, &g_aDisk[nCntDisk].acc, 10.0f);
			}

			g_aDisk[nCntDisk].lastPos = g_aDisk[nCntDisk].pos;				//�O��̈ʒu�̍X�V

			//�S�[���Ƃ̓����蔻��(pos, lastPos, fWidth, fHeight)
			ColisionGoal(&g_aDisk[nCntDisk].pos, &g_aDisk[nCntDisk].lastPos, 10.0f, 10.0f);
	
			VERTEX_2D *pVtx = NULL;					//���_���ւ̃|�C���^

			//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
			g_pVtxBuffDisk->Lock(0, 0, (void**)&pVtx, 0);

			pVtx[(nCntDisk * 4) + 0].pos = D3DXVECTOR3(g_aDisk[nCntDisk].pos.x - (g_aDisk[nCntDisk].fSize * 0.5f * (1 + (g_aDisk[nCntDisk].fHeight * 0.005))), 
				g_aDisk[nCntDisk].pos.y - ((g_aDisk[nCntDisk].fHeight - 10.0f) * 0.5f) - (g_aDisk[nCntDisk].fSize * 0.5f * (1 + (g_aDisk[nCntDisk].fHeight * 0.005))), 0.0f);

			pVtx[(nCntDisk * 4) + 1].pos = D3DXVECTOR3(g_aDisk[nCntDisk].pos.x + (g_aDisk[nCntDisk].fSize * 0.5f * (1 + (g_aDisk[nCntDisk].fHeight * 0.005))),
				g_aDisk[nCntDisk].pos.y - ((g_aDisk[nCntDisk].fHeight - 10.0f) * 0.5f) - (g_aDisk[nCntDisk].fSize * 0.5f * (1 + (g_aDisk[nCntDisk].fHeight * 0.005))), 0.0f);

			pVtx[(nCntDisk * 4) + 2].pos = D3DXVECTOR3(g_aDisk[nCntDisk].pos.x - (g_aDisk[nCntDisk].fSize * 0.5f * (1 + (g_aDisk[nCntDisk].fHeight * 0.005))),
				g_aDisk[nCntDisk].pos.y - ((g_aDisk[nCntDisk].fHeight - 10.0f) * 0.5f) + (g_aDisk[nCntDisk].fSize * 0.5f * (1 + (g_aDisk[nCntDisk].fHeight * 0.005))), 0.0f);

			pVtx[(nCntDisk * 4) + 3].pos = D3DXVECTOR3(g_aDisk[nCntDisk].pos.x + (g_aDisk[nCntDisk].fSize * 0.5f * (1 + (g_aDisk[nCntDisk].fHeight * 0.005))), 
				g_aDisk[nCntDisk].pos.y - ((g_aDisk[nCntDisk].fHeight - 10.0f) * 0.5f) + (g_aDisk[nCntDisk].fSize * 0.5f * (1 + (g_aDisk[nCntDisk].fHeight * 0.005))), 0.0f);

			//���_�o�b�t�@���A�����b�N����
			g_pVtxBuffDisk->Unlock();

			SetPositionShadow(g_aDisk[nCntDisk].nIdxShadow, g_aDisk[nCntDisk].pos);
		}
	}
}

//====================================
//�f�B�X�N�̕`�揈��
//====================================
void DrawDisk(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffDisk, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntDisk = 0; nCntDisk < MAX_DISK; nCntDisk++)
	{
		if (g_aDisk[nCntDisk].bUse == true)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTexDisk);

			//�f�B�X�N��`�悷��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntDisk * 4, 2);
		}
	}
}

//====================================
//�f�B�X�N�̐ݒ菈��
//====================================
void SetDisk(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 acc, DISK_TYPE type, int nPlayer, float size)
{
	VERTEX_2D *pVtx = NULL;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffDisk->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntDisk = 0; nCntDisk < MAX_DISK; nCntDisk++)
	{
		if (g_aDisk[nCntDisk].bUse == false)
		{
			g_aDisk[nCntDisk].pos = pos;									//�f�B�X�N�̌��݂̈ʒu�̐ݒ�
			g_aDisk[nCntDisk].lastPos = pos - move;							//�f�B�X�N�̑O��̈ʒu�̐ݒ�
			g_aDisk[nCntDisk].move = move;									//�f�B�X�N�̈ړ��ʂ̐ݒ�
			g_aDisk[nCntDisk].fSize = size;									//�f�B�X�N�̑傫���̐ݒ�
			g_aDisk[nCntDisk].acc = acc;									//�f�B�X�N�̉����̐ݒ�
			g_aDisk[nCntDisk].type = type;									//�f�B�X�N�̎�ނ̐ݒ�
			g_aDisk[nCntDisk].nCntPhase = 0;								//�f�B�X�N�̕K�E�Z�p�̃J�E���^�[�̐ݒ�
			g_aDisk[nCntDisk].nPlayer = nPlayer;							//�f�B�X�N�𓊂����v���C���[�̃C���f�b�N�X�̐ݒ�
			g_aDisk[nCntDisk].nIdxShadow = SetShadow(pos, size);			//�f�B�X�N�̉e�̃C���f�b�N�X

			//���_���W�̐ݒ�
			pVtx[(nCntDisk * 4) + 0].pos = D3DXVECTOR3(pos.x - (size * 0.5f), pos.y - (size * 0.5f), 0.0f);
			pVtx[(nCntDisk * 4) + 1].pos = D3DXVECTOR3(pos.x + (size * 0.5f), pos.y - (size * 0.5f), 0.0f);
			pVtx[(nCntDisk * 4) + 2].pos = D3DXVECTOR3(pos.x - (size * 0.5f), pos.y + (size * 0.5f), 0.0f);
			pVtx[(nCntDisk * 4) + 3].pos = D3DXVECTOR3(pos.x + (size * 0.5f), pos.y + (size * 0.5f), 0.0f);

			//���_�J���[�̐ݒ�
			pVtx[(nCntDisk * 4) + 0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[(nCntDisk * 4) + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[(nCntDisk * 4) + 2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[(nCntDisk * 4) + 3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			switch (type)
			{
			default:

				g_aDisk[nCntDisk].fHeight = 10.0f;
				g_aDisk[nCntDisk].fVerticalSpeed = 0.0f;
				g_aDisk[nCntDisk].bBounce = true;

				break;

			case DISK_TYPE_LOB:

				g_aDisk[nCntDisk].fHeight = 15.0f;
				g_aDisk[nCntDisk].fVerticalSpeed = LOB_STARTING_SPEED;
				g_aDisk[nCntDisk].bBounce = true;
				
				g_aDisk[nCntDisk].move = SetLobSpeed(g_aDisk[nCntDisk].pos, g_aDisk[nCntDisk].move, nCntDisk, g_aDisk[nCntDisk].fHeight, g_aDisk[nCntDisk].fVerticalSpeed);

				break;

			case DISK_TYPE_SPECIAL_0:

				g_aDisk[nCntDisk].fHeight = 10.0f;
				g_aDisk[nCntDisk].fVerticalSpeed = 0.0f;
				g_aDisk[nCntDisk].bBounce = false;

				break;
			}

			g_aDisk[nCntDisk].bUse = true;			//�g�p����Ă����Ԃɂ���

			break;
		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffDisk->Unlock();
}

//�K�E�Z(DISK_TYPE_SPECIAL_0)
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
				int a = 0;
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
//�f�B�X�N�̎擾����
//============================================================================
Disk *GetDisk(void)
{
	return g_aDisk;	//�f�B�X�N���̐擪�A�h���X��Ԃ�
}


D3DXVECTOR3 SetLobSpeed(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nCntDisk, float fHeight, float fVerticalSpeed)
{
	D3DXVECTOR3 newSpeed = move;
	D3DXVECTOR3 lastPos = pos;
	D3DXVECTOR3 endPos = pos;
	D3DXVECTOR3 newPos;
	D3DXVECTOR3 initialSpeedDir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 initialSpeedDirNor;
	int dTime = 0;

	while (fHeight > 0.0f)
	{
		fHeight += fVerticalSpeed;
		fVerticalSpeed -= 0.05f;
		dTime++;
		endPos += move;
	}

	initialSpeedDir = endPos - pos;

	bool bImpact = SpecialWallBounce(&endPos, &lastPos, &newSpeed, g_aDisk[nCntDisk].fSize);

	if (bImpact == true)
	{
		D3DXVECTOR3 xDir = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		D3DXVECTOR3 postImpact = endPos - pos;
		D3DXVec3Normalize(&initialSpeedDirNor, &initialSpeedDir);

		float fDot = D3DXVec3Dot(&xDir, &initialSpeedDir);
		float fLenght = sqrtf(((postImpact.x * postImpact.x) + (postImpact.y * postImpact.y)));

		newPos = endPos + (xDir * (fDot * fLenght));
		newSpeed = newPos - lastPos;

		D3DXVec3Normalize(&newSpeed, &newSpeed);

		fLenght = sqrtf(((move.x * move.x) + (move.y * move.y)));
		newSpeed *= fLenght;
	}

	return newSpeed;
}