//================================//
//								  //
//       �f�B�X�N�̃t�@�C��		  //
//       Author: Ricci Alex		  //
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
#include "stage.h"
#include "player.h"
#include "input.h"
#include "landingPoint.h"
#include "effect.h"

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
		"data\\TEXTURE\\SilverGear.png",
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

			if (g_aDisk[nCntDisk].fHeight <= 15.0f)
			{
				SetEffect((g_aDisk[nCntDisk].pos - g_aDisk[nCntDisk].move), 0.0f, EFFECT_TYPE_TRAIL);
				SetEffect((g_aDisk[nCntDisk].pos - g_aDisk[nCntDisk].move), 0.0f, EFFECT_TYPE_TRAIL);
			}

			switch (g_aDisk[nCntDisk].type)
			{
			default:
				
				break;

			case DISK_TYPE_LOB:

				if (g_aDisk[nCntDisk].fHeight > 0.0f)
				{
					g_aDisk[nCntDisk].fHeight += g_aDisk[nCntDisk].fVerticalSpeed;
					g_aDisk[nCntDisk].fVerticalSpeed += GRAVITY_ACCELERATION;

					if (g_aDisk[nCntDisk].fHeight <= 0.0f)
					{
						g_aDisk[nCntDisk].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						g_aDisk[nCntDisk].acc = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					}
				}
				break;

			case DISK_TYPE_JUMP:

				if (g_aDisk[nCntDisk].fHeight > 0.0f)
				{
					g_aDisk[nCntDisk].fHeight += g_aDisk[nCntDisk].fVerticalSpeed;
					g_aDisk[nCntDisk].fVerticalSpeed += GRAVITY_ACCELERATION;

					if (g_aDisk[nCntDisk].fHeight <= 0.0f)
					{
						g_aDisk[nCntDisk].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						g_aDisk[nCntDisk].acc = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					}
				}
				break;

			case DISK_TYPE_BLOCKED:

				g_aDisk[nCntDisk].fHeight += g_aDisk[nCntDisk].fVerticalSpeed;
				g_aDisk[nCntDisk].fVerticalSpeed += GRAVITY_ACCELERATION;

				break;

			case DISK_TYPE_SPECIAL_0:

				UpdateSpecialDisk(nCntDisk);

				

				break;

			case DISK_TYPE_SPECIAL_1:

				UpdateSpecialDisk(nCntDisk);

				break;

			case DISK_TYPE_SPECIAL_2:

				UpdateSpecialDisk(nCntDisk);

				SetEffect(g_aDisk[nCntDisk].pos, g_aDisk[nCntDisk].rot, EFFECT_TYPE_SPECIAL_TRAIL2);

				break;

			case DISK_TYPE_SPECIAL_3:

				UpdateSpecialDisk(nCntDisk);

				break;

			case DISK_TYPE_SPECIAL_4:

				UpdateSpecialDisk(nCntDisk);

				for (int nCntEffect = 0; nCntEffect < 10; nCntEffect++)
				{
					SetEffect(g_aDisk[nCntDisk].pos, 0.0f, EFFECT_TYPE_SPECIAL_TRAIL4);
				}

				break;
			}

			if (g_aDisk[nCntDisk].bBounce == true)
			{
				//�ǂƂ̓����蔻��
				WallBounce(&g_aDisk[nCntDisk].pos, &g_aDisk[nCntDisk].lastPos, &g_aDisk[nCntDisk].move, &g_aDisk[nCntDisk].acc, (g_aDisk[nCntDisk].fSize * 0.5f));
			}

			//�S�[���Ƃ̓����蔻��(pos, lastPos, fWidth, fHeight)
			ColisionGoal(&g_aDisk[nCntDisk].pos, &g_aDisk[nCntDisk].lastPos);

			g_aDisk[nCntDisk].lastPos = g_aDisk[nCntDisk].pos;							//�O��̈ʒu�̍X�V

			g_aDisk[nCntDisk].rot += g_aDisk[nCntDisk].fRotSpeed;
	
			VERTEX_2D *pVtx = NULL;					//���_���ւ̃|�C���^

			//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
			g_pVtxBuffDisk->Lock(0, 0, (void**)&pVtx, 0);

			g_aDisk[nCntDisk].fVertexLenght = (g_aDisk[nCntDisk].fSize * 0.5f * (1 + (g_aDisk[nCntDisk].fHeight * 0.005f)));

			pVtx[(nCntDisk * 4) + 0].pos.x = g_aDisk[nCntDisk].pos.x + sinf(g_aDisk[nCntDisk].rot + g_aDisk[nCntDisk].fAngle + (D3DX_PI * 0.75f)) * g_aDisk[nCntDisk].fVertexLenght;
			pVtx[(nCntDisk * 4) + 0].pos.y = g_aDisk[nCntDisk].pos.y + cosf(g_aDisk[nCntDisk].rot + g_aDisk[nCntDisk].fAngle + (D3DX_PI * 0.75f)) * g_aDisk[nCntDisk].fVertexLenght - (g_aDisk[nCntDisk].fHeight * 0.2f);
			pVtx[(nCntDisk * 4) + 0].pos.z = 0.0f;

			pVtx[(nCntDisk * 4) + 1].pos.x = g_aDisk[nCntDisk].pos.x + sinf(g_aDisk[nCntDisk].rot + g_aDisk[nCntDisk].fAngle + (D3DX_PI * 0.25f)) * g_aDisk[nCntDisk].fVertexLenght;
			pVtx[(nCntDisk * 4) + 1].pos.y = g_aDisk[nCntDisk].pos.y + cosf(g_aDisk[nCntDisk].rot + g_aDisk[nCntDisk].fAngle + (D3DX_PI * 0.25f)) * g_aDisk[nCntDisk].fVertexLenght - (g_aDisk[nCntDisk].fHeight * 0.2f);
			pVtx[(nCntDisk * 4) + 1].pos.z = 0.0f;

			pVtx[(nCntDisk * 4) + 2].pos.x = g_aDisk[nCntDisk].pos.x + sinf(g_aDisk[nCntDisk].rot + g_aDisk[nCntDisk].fAngle + (-D3DX_PI * 0.75f)) * g_aDisk[nCntDisk].fVertexLenght;
			pVtx[(nCntDisk * 4) + 2].pos.y = g_aDisk[nCntDisk].pos.y + cosf(g_aDisk[nCntDisk].rot + g_aDisk[nCntDisk].fAngle + (-D3DX_PI * 0.75f)) * g_aDisk[nCntDisk].fVertexLenght - (g_aDisk[nCntDisk].fHeight * 0.2f);
			pVtx[(nCntDisk * 4) + 2].pos.z = 0.0f;

			pVtx[(nCntDisk * 4) + 3].pos.x = g_aDisk[nCntDisk].pos.x + sinf(g_aDisk[nCntDisk].rot + g_aDisk[nCntDisk].fAngle + (-D3DX_PI * 0.25f)) * g_aDisk[nCntDisk].fVertexLenght;
			pVtx[(nCntDisk * 4) + 3].pos.y = g_aDisk[nCntDisk].pos.y + cosf(g_aDisk[nCntDisk].rot + g_aDisk[nCntDisk].fAngle + (-D3DX_PI * 0.25f)) * g_aDisk[nCntDisk].fVertexLenght - (g_aDisk[nCntDisk].fHeight * 0.2f);
			pVtx[(nCntDisk * 4) + 3].pos.z = 0.0f;

			//���_�o�b�t�@���A�����b�N����
			g_pVtxBuffDisk->Unlock();

			SetPositionShadow(g_aDisk[nCntDisk].nIdxShadow, g_aDisk[nCntDisk].pos);		//�e�̈ʒu�̍X�V
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
			g_aDisk[nCntDisk].nIdxShadow = SetShadow(pos, size * 1.25f);	//�f�B�X�N�̉e�̃C���f�b�N�X
			g_aDisk[nCntDisk].bUse = true;									//�g�p����Ă����Ԃɂ���

			//��]�p�̕ϐ��̐ݒ�
			g_aDisk[nCntDisk].fLenght = sqrtf((g_aDisk[nCntDisk].fSize * g_aDisk[nCntDisk].fSize) + (g_aDisk[nCntDisk].fSize * g_aDisk[nCntDisk].fSize)) * 0.5f;
			g_aDisk[nCntDisk].fAngle = atan2f(g_aDisk[nCntDisk].fSize * 0.5f, g_aDisk[nCntDisk].fSize * 0.5f);
			g_aDisk[nCntDisk].rot = 0.0f;
			g_aDisk[nCntDisk].fVertexLenght = size * 0.5f;
			g_aDisk[nCntDisk].fRotSpeed = BASE_ROTATION_SPEED * (sqrtf((move.x * move.x) + (move.y * move.y)) * 0.65);

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

			int a = 0;
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

				SetLandingMark(g_aDisk[nCntDisk].pos, g_aDisk[nCntDisk].move, g_aDisk[nCntDisk].fHeight, g_aDisk[nCntDisk].fVerticalSpeed, GRAVITY_ACCELERATION, g_aDisk[nCntDisk].fSize);

				break;

			case DISK_TYPE_JUMP:

				g_aDisk[nCntDisk].fHeight = JUMP_MAX_HEIGHT;
				g_aDisk[nCntDisk].fVerticalSpeed = -JUMP_MAX_HEIGHT / JUMP_ATTACK_TIME;
				g_aDisk[nCntDisk].bBounce = false;

				g_aDisk[nCntDisk].move = SetJumpAttackSpeed(g_aDisk[nCntDisk].pos);

				SetLandingMark(g_aDisk[nCntDisk].pos, g_aDisk[nCntDisk].move, g_aDisk[nCntDisk].fHeight, g_aDisk[nCntDisk].fVerticalSpeed, GRAVITY_ACCELERATION, g_aDisk[nCntDisk].fSize);
				break;

			case DISK_TYPE_BLOCKED:

				g_aDisk[nCntDisk].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_aDisk[nCntDisk].acc = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_aDisk[nCntDisk].fHeight = JUMP_MAX_HEIGHT;
				g_aDisk[nCntDisk].fVerticalSpeed = JUMP_MAX_HEIGHT / JUMP_ATTACK_TIME;
				g_aDisk[nCntDisk].bBounce = false;

				break;

			case DISK_TYPE_SPECIAL_0:

				if (pos.x < SCREEN_WIDTH * 0.5f)
				{
					g_aDisk[nCntDisk].move.x = 7.5f;
				}
				else
				{
					g_aDisk[nCntDisk].move.x = -7.5f;
				}

				if (pos.y <= SCREEN_HEIGHT * 0.5f)
				{
					g_aDisk[nCntDisk].move.y = 0.0f;
					g_aDisk[nCntDisk].acc.y = 0.5f;
				}
				else
				{
					g_aDisk[nCntDisk].move.y = 0.0f;
					g_aDisk[nCntDisk].acc.y = -0.5f;
				}

				g_aDisk[nCntDisk].fHeight = NORMAL_DISK_HEIGHT;
				g_aDisk[nCntDisk].fVerticalSpeed = NORMAL_VERTICAL_SPEED;
				g_aDisk[nCntDisk].bBounce = false;

				break;

			case DISK_TYPE_SPECIAL_1:

				if (pos.x < SCREEN_WIDTH * 0.5f)
				{
					g_aDisk[nCntDisk].move.x = 7.5f;
				}
				else
				{
					g_aDisk[nCntDisk].move.x = -7.5f;
				}

				g_aDisk[nCntDisk].move.y = 0.0f;
				g_aDisk[nCntDisk].fHeight = NORMAL_DISK_HEIGHT;
				g_aDisk[nCntDisk].fVerticalSpeed = NORMAL_VERTICAL_SPEED;
				g_aDisk[nCntDisk].bBounce = false;

				break;

			case DISK_TYPE_SPECIAL_2:

				if (pos.x < SCREEN_WIDTH * 0.5f)
				{
					g_aDisk[nCntDisk].move.x = 7.5f;
				}
				else
				{
					g_aDisk[nCntDisk].move.x = -7.5f;
				}

				g_aDisk[nCntDisk].move.y = 0.0f;
				g_aDisk[nCntDisk].fHeight = NORMAL_DISK_HEIGHT;
				g_aDisk[nCntDisk].fVerticalSpeed = NORMAL_VERTICAL_SPEED;
				g_aDisk[nCntDisk].bBounce = false;

				break;

			case DISK_TYPE_SPECIAL_3:

				if (pos.x < SCREEN_WIDTH * 0.5f)
				{
					g_aDisk[nCntDisk].move.x = 7.5f;
					g_aDisk[nCntDisk].acc.x = 0.05f;
				}
				else
				{
					g_aDisk[nCntDisk].move.x = -7.5f;
					g_aDisk[nCntDisk].acc.x = -0.05f;
				}

				g_aDisk[nCntDisk].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_aDisk[nCntDisk].move.y = 0.0f;
				g_aDisk[nCntDisk].fHeight = NORMAL_DISK_HEIGHT * 3.0f;
				g_aDisk[nCntDisk].fVerticalSpeed = LOB_STARTING_SPEED * 3.0f;
				g_aDisk[nCntDisk].bBounce = false;

				break;

			case DISK_TYPE_SPECIAL_4:

				int nPos;

				if (pos.y <= SCREEN_HEIGHT * 0.5f)
				{
					nPos = 1;
				}
				else
				{
					nPos = -1;
				}

				if (pos.x < SCREEN_WIDTH * 0.5f)
				{
					g_aDisk[nCntDisk].move.x = 15.0f;
				}
				else
				{
					g_aDisk[nCntDisk].move.x = -15.0f;
				}

				g_aDisk[nCntDisk].move.y = 0.0f;
				g_aDisk[nCntDisk].acc = D3DXVECTOR3(0.0f, 0.25f * nPos, 0.0f);
				g_aDisk[nCntDisk].fHeight = NORMAL_DISK_HEIGHT;
				g_aDisk[nCntDisk].fVerticalSpeed = NORMAL_VERTICAL_SPEED;
				g_aDisk[nCntDisk].bBounce = false;

				break;
			}

			break;
		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffDisk->Unlock();
}

//�K�E�Z
void UpdateSpecialDisk(int nCntDisk)
{
	float fChangePoint;
	bool bImpact = false;	

	STAGE_LENGTH *pArea1 = GetP1StgLng();					//�v���C���[�P�̃G���A���ւ̃|�C���^
	STAGE_LENGTH *pArea2 = GetP2StgLng();					//�v���C���[�Q�̃G���A���ւ̃|�C���^

	float playArea = pArea2->max.x - pArea1->min.x;

	switch (g_aDisk[nCntDisk].type)
	{
	//====================================================================================================================================
	case DISK_TYPE_SPECIAL_0:

		switch (g_aDisk[nCntDisk].nCntPhase)
		{
		default:
			for (int nCntEffect = 0; nCntEffect < 8; nCntEffect++)
			{
				SetEffect(g_aDisk[nCntDisk].pos, 0.0f, EFFECT_TYPE_SPECIAL_GENERAL);
			}
			break;

		case 0:

			bImpact = SpecialWallBounce(&g_aDisk[nCntDisk].pos, &g_aDisk[nCntDisk].lastPos, &g_aDisk[nCntDisk].move, 10.0f);

			for (int nCntEffect = 0; nCntEffect < 8; nCntEffect++)
			{
				SetEffect(g_aDisk[nCntDisk].pos, 0.0f, EFFECT_TYPE_SPECIAL_GENERAL);
			}

			if (bImpact == true)
			{
				g_aDisk[nCntDisk].nCntPhase++;
				g_aDisk[nCntDisk].move = D3DXVECTOR3(15.0f - (30.0f * g_aDisk[nCntDisk].nPlayer), 0.0f, 0.0f);
				g_aDisk[nCntDisk].acc = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}

			break;

		case 1:

			for (int nCntEffect = 0; nCntEffect < 15; nCntEffect++)
			{
				SetEffect(g_aDisk[nCntDisk].pos, 0.0f, EFFECT_TYPE_SPECIAL_TRAIL0);
			}

			fChangePoint = SCREEN_WIDTH * (0.7f - (0.3f * g_aDisk[nCntDisk].nPlayer));

			if (g_aDisk[nCntDisk].pos.x >= fChangePoint && g_aDisk[nCntDisk].move.x > 0.0f)
			{
				int nDir = (int)(g_aDisk[nCntDisk].pos.y / SCREEN_HEIGHT + 1.0f);

				g_aDisk[nCntDisk].nCntPhase++;
				g_aDisk[nCntDisk].move = D3DXVECTOR3(-2.5f + (1.0f * g_aDisk[nCntDisk].nPlayer), 15.0f - (30.0f * nDir), 0.0f);
				g_aDisk[nCntDisk].acc = D3DXVECTOR3(0.5f - (1.0f * g_aDisk[nCntDisk].nPlayer), 0.0f, 0.0f);
			}
			else if (g_aDisk[nCntDisk].pos.x <= fChangePoint && g_aDisk[nCntDisk].move.x < 0.0f)
			{
				int nDir = (int)(g_aDisk[nCntDisk].pos.y / SCREEN_HEIGHT + 1.0f);

				g_aDisk[nCntDisk].nCntPhase++;
				g_aDisk[nCntDisk].move = D3DXVECTOR3(-2.5f + (1.0f * g_aDisk[nCntDisk].nPlayer), 15.0f - (30.0f * nDir), 0.0f);
				g_aDisk[nCntDisk].acc = D3DXVECTOR3(0.5f - (1.0f * g_aDisk[nCntDisk].nPlayer), 0.0f, 0.0f);
			}

			break;

		case 2:

			bImpact = SpecialWallBounce(&g_aDisk[nCntDisk].pos, &g_aDisk[nCntDisk].lastPos, &g_aDisk[nCntDisk].move, 10.0f);

			for (int nCntEffect = 0; nCntEffect < 8; nCntEffect++)
			{
				SetEffect(g_aDisk[nCntDisk].pos, 0.0f, EFFECT_TYPE_SPECIAL_GENERAL);
			}

			if (bImpact == true)
			{
				int nDir = (int)(g_aDisk[nCntDisk].pos.y / SCREEN_HEIGHT + 1.0f);

				g_aDisk[nCntDisk].nCntPhase++;
				g_aDisk[nCntDisk].move = D3DXVECTOR3(15.0f - (30.0f * g_aDisk[nCntDisk].nPlayer), -6.0f + (12.0f * (nDir)), 0.0f);
				g_aDisk[nCntDisk].acc = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}

			break;
		}

		break;
		//====================================================================================================================================
		//====================================================================================================================================
	case DISK_TYPE_SPECIAL_1:

		switch (g_aDisk[nCntDisk].nCntPhase)
		{

		default:

			SetEffect(g_aDisk[nCntDisk].pos, g_aDisk[nCntDisk].rot, EFFECT_TYPE_SPECIAL_TRAIL2);

			break;

		case 0:

			fChangePoint = SCREEN_WIDTH * (0.3f + (0.4f * g_aDisk[nCntDisk].nPlayer));

			SetEffect(g_aDisk[nCntDisk].pos, g_aDisk[nCntDisk].rot, EFFECT_TYPE_SPECIAL_TRAIL2);

			if (g_aDisk[nCntDisk].move.x >= 0)
			{
				if (g_aDisk[nCntDisk].pos.x >= fChangePoint)
				{
					int nDir = (int)(g_aDisk[nCntDisk].pos.y / ((SCREEN_HEIGHT * 0.5f) + 1.0f));

					g_aDisk[nCntDisk].nCntPhase++;
					g_aDisk[nCntDisk].move = D3DXVECTOR3(0.0f, -15.0f + (30.0f * nDir), 0.0f);
					g_aDisk[nCntDisk].acc = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				}
			}
			else
			{
				if (g_aDisk[nCntDisk].pos.x < fChangePoint)
				{
					int nDir = (int)(g_aDisk[nCntDisk].pos.y / ((SCREEN_HEIGHT * 0.5f) + 1.0f));

					g_aDisk[nCntDisk].nCntPhase++;
					g_aDisk[nCntDisk].move = D3DXVECTOR3(0.0f, -15.0f + (30.0f * nDir), 0.0f);
					g_aDisk[nCntDisk].acc = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				}
			}

			break;

		case 1:

			bImpact = SpecialWallBounce(&g_aDisk[nCntDisk].pos, &g_aDisk[nCntDisk].lastPos, &g_aDisk[nCntDisk].move, 10.0f);

			SetEffect(g_aDisk[nCntDisk].pos, g_aDisk[nCntDisk].rot, EFFECT_TYPE_SPECIAL_TRAIL2);

			if (bImpact == true)
			{
				g_aDisk[nCntDisk].nCntPhase++;
				g_aDisk[nCntDisk].move = D3DXVECTOR3(15.0f - (30.0f * g_aDisk[nCntDisk].nPlayer), 0.0f, 0.0f);
				g_aDisk[nCntDisk].acc = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}

			break;

		case 2:

			fChangePoint = SCREEN_WIDTH * (0.7f - (0.4f * g_aDisk[nCntDisk].nPlayer));

			for (int nCntEffect = 0; nCntEffect < 15; nCntEffect++)
			{
				SetEffect(g_aDisk[nCntDisk].pos, 0.0f, EFFECT_TYPE_SPECIAL_TRAIL0);
			}

			if (g_aDisk[nCntDisk].move.x < 0)
			{
				if (g_aDisk[nCntDisk].pos.x <= fChangePoint)
				{
					int nDir = (int)(g_aDisk[nCntDisk].pos.y / ((SCREEN_HEIGHT * 0.5f) + 1.0f));
					g_aDisk[nCntDisk].nCntPhase++;
					g_aDisk[nCntDisk].move = D3DXVECTOR3(0.0f, 10.0f - (20.0f * nDir), 0.0f);
					g_aDisk[nCntDisk].acc = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				}
			}
			else
			{
				if (g_aDisk[nCntDisk].pos.x >= fChangePoint)
				{
					int nDir = (int)(g_aDisk[nCntDisk].pos.y / ((SCREEN_HEIGHT * 0.5f) + 1.0f));
					g_aDisk[nCntDisk].nCntPhase++;
					g_aDisk[nCntDisk].move = D3DXVECTOR3(0.0f, 10.0f - (20.0f * nDir), 0.0f);
					g_aDisk[nCntDisk].acc = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				}
			}

			break;

		case 3:

			fChangePoint = (SCREEN_HEIGHT * 0.5f) + ((rand() % 201) - 100);

			SetEffect(g_aDisk[nCntDisk].pos, g_aDisk[nCntDisk].rot, EFFECT_TYPE_SPECIAL_TRAIL2);

			if (g_aDisk[nCntDisk].move.y > 0)
			{
				if (g_aDisk[nCntDisk].pos.y >= fChangePoint)
				{
					g_aDisk[nCntDisk].nCntPhase++;
					g_aDisk[nCntDisk].move = D3DXVECTOR3(15.0f - (30.0f * g_aDisk[nCntDisk].nPlayer), 0.0f, 0.0f);
					g_aDisk[nCntDisk].acc = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				}
			}
			else
			{
				if (g_aDisk[nCntDisk].pos.y <= fChangePoint)
				{
					g_aDisk[nCntDisk].nCntPhase++;
					g_aDisk[nCntDisk].move = D3DXVECTOR3(15.0f - (30.0f * g_aDisk[nCntDisk].nPlayer), 0.0f, 0.0f);
					g_aDisk[nCntDisk].acc = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				}
			}

			break;
		}

		break;
		//====================================================================================================================================
		//====================================================================================================================================
		case DISK_TYPE_SPECIAL_2:		

			switch (g_aDisk[nCntDisk].nCntPhase)
			{
			case 0:

				fChangePoint = playArea * (0.25f + (0.5f * g_aDisk[nCntDisk].nPlayer));

				if (g_aDisk[nCntDisk].move.x > 0.0f && g_aDisk[nCntDisk].pos.x > fChangePoint)
				{
					g_aDisk[nCntDisk].nCntPhase++;

					if (g_aDisk[nCntDisk].pos.y <= SCREEN_HEIGHT * 0.5f)
					{
						g_aDisk[nCntDisk].pos.y = SCREEN_HEIGHT - 200.0f;
					}
					else
					{
						g_aDisk[nCntDisk].pos.y = 200.0f;
					}
				}
				else if (g_aDisk[nCntDisk].move.x < 0.0f && g_aDisk[nCntDisk].pos.x < fChangePoint)
				{
					g_aDisk[nCntDisk].nCntPhase++;

					if (g_aDisk[nCntDisk].pos.y <= SCREEN_HEIGHT * 0.5f)
					{
						g_aDisk[nCntDisk].pos.y = SCREEN_HEIGHT - 200.0f;
					}
					else
					{
						g_aDisk[nCntDisk].pos.y = 200.0f;
					}
				}

				break;

			case 1:

				fChangePoint = playArea * 0.5f;

				if (g_aDisk[nCntDisk].move.x > 0.0f && g_aDisk[nCntDisk].pos.x > fChangePoint)
				{
					g_aDisk[nCntDisk].nCntPhase++;

					if (g_aDisk[nCntDisk].pos.y <= SCREEN_HEIGHT * 0.5f)
					{
						g_aDisk[nCntDisk].pos.y = SCREEN_HEIGHT - 200.0f;
					}
					else
					{
						g_aDisk[nCntDisk].pos.y = 200.0f;
					}
				}
				else if (g_aDisk[nCntDisk].move.x < 0.0f && g_aDisk[nCntDisk].pos.x < fChangePoint)
				{
					g_aDisk[nCntDisk].nCntPhase++;

					if (g_aDisk[nCntDisk].pos.y <= SCREEN_HEIGHT * 0.5f)
					{
						g_aDisk[nCntDisk].pos.y = SCREEN_HEIGHT - 200.0f;
					}
					else
					{
						g_aDisk[nCntDisk].pos.y = 200.0f;
					}
				}
				break;

			case 2:

				fChangePoint = playArea * (0.7f - (0.4f * g_aDisk[nCntDisk].nPlayer));

				if (g_aDisk[nCntDisk].move.x > 0.0f && g_aDisk[nCntDisk].pos.x >= fChangePoint)
				{
					g_aDisk[nCntDisk].nCntPhase++;
					g_aDisk[nCntDisk].pos.y = SCREEN_HEIGHT * 0.5f + ((rand() % 200) - 100);
				}
				else if (g_aDisk[nCntDisk].move.x < 0.0f && g_aDisk[nCntDisk].pos.x <= fChangePoint)
				{
					g_aDisk[nCntDisk].nCntPhase++;
					g_aDisk[nCntDisk].pos.y = SCREEN_HEIGHT * 0.5f + ((rand() % 200) - 100);
				}
				
				break;

			default:
				break;
			}
			break;
			//====================================================================================================================================
			//====================================================================================================================================
		case DISK_TYPE_SPECIAL_3:

			switch (g_aDisk[nCntDisk].nCntPhase)
			{
			case 0:

				g_aDisk[nCntDisk].fHeight += g_aDisk[nCntDisk].fVerticalSpeed;
				g_aDisk[nCntDisk].fVerticalSpeed -= 0.75f;

				if (g_aDisk[nCntDisk].fHeight <= 0.0f)
				{
					g_aDisk[nCntDisk].nCntPhase++;
					g_aDisk[nCntDisk].fHeight = 0.0f;
					g_aDisk[nCntDisk].fVerticalSpeed = 0.0f;
					g_aDisk[nCntDisk].move = D3DXVECTOR3(20.0f + (-40.0f * g_aDisk[nCntDisk].nPlayer), 0.0f, 0.0f);
				}

				break;

			default:

				for (int nCntEffect = 0; nCntEffect < 15; nCntEffect++)
				{
					SetEffect(g_aDisk[nCntDisk].pos, 0.0f, EFFECT_TYPE_SPECIAL_TRAIL3);
				}

				break;
			}
			break;
			//====================================================================================================================================
			//====================================================================================================================================
		case DISK_TYPE_SPECIAL_4:

			switch (g_aDisk[nCntDisk].nCntPhase)
			{
			default:
				break;

			case 0:

				if (g_aDisk[nCntDisk].move.y >= 0 && g_aDisk[nCntDisk].pos.y > SCREEN_HEIGHT * 0.5f)
				{
					g_aDisk[nCntDisk].nCntPhase++;
					g_aDisk[nCntDisk].move.y = 0.0f;
					g_aDisk[nCntDisk].acc.y = 0.0f;
				}
				else if (g_aDisk[nCntDisk].move.y < 0 && g_aDisk[nCntDisk].pos.y < SCREEN_HEIGHT * 0.5f)
				{
					g_aDisk[nCntDisk].nCntPhase++;
					g_aDisk[nCntDisk].move.y = 0.0f;
					g_aDisk[nCntDisk].acc.y = 0.0f;
				}

				break;

			case 1:

				g_aDisk[nCntDisk].pos.y = 150.0f * sinf(g_aDisk[nCntDisk].pos.x / 50.0f) + 360.0f;

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

//============================================================================
//�㓊���̈ړ��ʂ̐ݒ菈��
//============================================================================
D3DXVECTOR3 SetLobSpeed(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nCntDisk, float fHeight, float fVerticalSpeed)
{
	STAGE_LENGTH *pArea1 = GetP1StgLng();								//�v���C���[�P�̃G���A���ւ̃|�C���^
	STAGE_LENGTH *pArea2 = GetP2StgLng();								//�v���C���[�Q�̃G���A���ւ̃|�C���^
	D3DXVECTOR3 newSpeed = move;										//�V�����ړ���
	D3DXVECTOR3 lastPos = pos;											//�ŏ��̈ʒu
	D3DXVECTOR3 endPos = pos;											//��������̈ʒu
	D3DXVECTOR3 newPos;													//�V�����ʒu
	float fTime = 0.0f;													//�f�B�X�N��������܂ł̕K�v�ȃt���[��
	float height, vSpeed;												//�f�B�X�N��������܂ł̕K�v�ȃt���[�����v�Z����p�̕ϐ�
	height = fHeight;
	vSpeed = fVerticalSpeed;

	while (height > 0.0f)
	{//�f�B�X�N�����������̈ʒu�ƕK�v�Ȏ��Ԃ��v�Z����
		height += vSpeed;
		vSpeed += GRAVITY_ACCELERATION;
		endPos += move;
		fTime += 1.0f;
	}
	newPos = endPos;						//�ړI�̈ʒu

	//����̃G���A���o�Ȃ��悤�ɂ��鏈��
	if (pos.x < SCREEN_WIDTH * 0.5f)
	{//������v���C���[�̓v���C���[�P��������
		if (newPos.y < pArea2->min.y + g_aDisk[nCntDisk].fSize)
		{//�G���A�̏㑤���o�Ȃ��悤�ɂ���
			newPos.y = pArea2->min.y + g_aDisk[nCntDisk].fSize;
		}
		else if (newPos.y > pArea2->max.y - g_aDisk[nCntDisk].fSize)
		{//�G���A�̉������o�Ȃ��悤�ɂ���
			newPos.y = pArea2->max.y - g_aDisk[nCntDisk].fSize;
		}

		if (newPos.x < pArea2->min.x + g_aDisk[nCntDisk].fSize)
		{//�G���A�̍������o�Ȃ��悤�ɂ���
			newPos.x = pArea2->min.x + g_aDisk[nCntDisk].fSize;
		}
		else if (newPos.x > pArea2->max.x - g_aDisk[nCntDisk].fSize)
		{//�G���A�̉E�����o�Ȃ��悤�ɂ���
			newPos.x = pArea2->max.x - g_aDisk[nCntDisk].fSize;
		}
	}
	else
	{//������v���C���[�̓v���C���[�Q��������
		if (newPos.y < pArea1->min.y + g_aDisk[nCntDisk].fSize)
		{//�G���A�̏㑤���o�Ȃ��悤�ɂ���
			newPos.y = pArea1->min.y + g_aDisk[nCntDisk].fSize;
		}
		else if (newPos.y > pArea1->max.y - g_aDisk[nCntDisk].fSize)
		{//�G���A�̉������o�Ȃ��悤�ɂ���
			newPos.y = pArea1->max.y - g_aDisk[nCntDisk].fSize;
		}

		if (newPos.x < pArea1->min.x + g_aDisk[nCntDisk].fSize)
		{//�G���A�̍������o�Ȃ��悤�ɂ���
			newPos.x = pArea1->min.x + g_aDisk[nCntDisk].fSize;
		}
		else if (newPos.x > pArea1->max.x - g_aDisk[nCntDisk].fSize)
		{//�G���A�̉E�����o�Ȃ��悤�ɂ���
			newPos.x = pArea1->max.x - g_aDisk[nCntDisk].fSize;
		}
	}

	newSpeed = (newPos - pos) / fTime;						//�V�����ړ��ʂ��v�Z����

	return newSpeed;										//�V�����ړ��ʂ�Ԃ�
}

//============================================================================
//�W�����v�����̈ړ��ʂ̐ݒ菈��
//============================================================================
D3DXVECTOR3 SetJumpAttackSpeed(D3DXVECTOR3 pos)
{
	STAGE_LENGTH *pArea1 = GetP1StgLng();					//�v���C���[�P�̃G���A���ւ̃|�C���^
	STAGE_LENGTH *pArea2 = GetP2StgLng();					//�v���C���[�Q�̃G���A���ւ̃|�C���^
	Player *pPlayer = GetPlayer();							//�v���C���[���ւ̃|�C���^

	D3DXVECTOR3 endPos, speed;								//�ړI�̈ʒu�ƈړ��ʂ̃��[�J���ϐ�

	int nPlayer;											//�f�B�X�N�𓊂����v���C���[

	if (pos.x < SCREEN_WIDTH * 0.5f)
	{//�v���C���[�P
		nPlayer = 0;
	}
	else
	{//�v���C���[�Q
		nPlayer = 1;
	}

	if (nPlayer == 0)
	{//�v���C���[�P��������
		endPos.x = pArea2->min.x + 200.0f + (pPlayer->fThrowPower * 5.0f);			//�ړI�̈ʒu��X���W��ݒ肷��
		endPos.y = pos.y;															//�ړI�̈ʒu��Y���W��ݒ肷��
		endPos.z = 0.0f;															//�ړI�̈ʒu��Z���W��0�ɂ���
	}
	else
	{//�v���C���[�Q��������
		endPos.x = pArea1->max.x - 200.0f - (pPlayer->fThrowPower * 5.0f);			//�ړI�̈ʒu��X���W��ݒ肷��
		endPos.y = pos.y;															//�ړI�̈ʒu��Y���W��ݒ肷��
		endPos.z = 0.0f;															//�ړI�̈ʒu��Z���W��0�ɂ���
	}																				
		
	//�V�����ړ��ʂ��v�Z����
	speed = endPos - pos;															
	speed.x /= JUMP_ATTACK_TIME;

	return speed;																	//�V�����ړ��ʂ�Ԃ�
}

//======================================
//�f�B�X�N�̔j��
//======================================
void DestroyDisk(void)
{
	g_aDisk[0].bUse = false;							//�f�B�X�N���g�p����Ă��Ȃ���Ԃɂ���
	GetShadow()[g_aDisk[0].nIdxShadow].bUse = false;	//�e���g�p����Ă��Ȃ���Ԃɂ���
	DestroyLandingMark();								// �󒆂̓����n�_�̃}�[�N���폜
	g_aDisk[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu�̏�����
}