//=========================================
// 
// �S�[������
// Author Tanimoto_Kosuke
//
// Update 22/03/23
// 
//=========================================
//------------------------------------
// include
//------------------------------------
#include "goal.h"
#include <stdio.h>
#include "disk.h"
#include "stage.h"
#include "effect.h"
#include "score.h"
#include "game.h"
#include "pop.h"
#include "referee.h"
#include "sound.h"

//------------------------------------
// �X�^�e�B�b�N�ϐ�
//------------------------------------
static LPDIRECT3DTEXTURE9		s_pTextureGoal[MAX_IMAGE_DRUM] = {};	//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffGoal = NULL;	//���_�o�b�t�@�ւ̃|�C���^
static Goal s_aGoal[MAX_GOAL];							//�S�[���̏��

//=========================================
// �S�[���̏���������
//=========================================
void InitGoal(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^

										//�e�N�X�`���[�̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\goal\\stripe03.png",	//�e�N�X�`���̃t�@�C����
		&s_pTextureGoal[GOAL_TYPE_NORMAL]
	);

	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\goal\\stripe04.png",	//�e�N�X�`���̃t�@�C����
		&s_pTextureGoal[GOAL_TYPE_STRIKE]
	);

	//�S�[���̈ʒu
	SetGoal(D3DXVECTOR3(GOAL_WIDTH / 2 + 25, 200.0f + (STAGE_HEIGHT_DOWN / 2), 0.1f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), false, GOAL_TYPE_NORMAL, 0);
	SetGoal(D3DXVECTOR3(GOAL_WIDTH / 2 + 25, SCREEN_HEIGHT / 2 + (STAGE_HEIGHT_DOWN / 2), 0.1f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), false, GOAL_TYPE_STRIKE, 1);
	SetGoal(D3DXVECTOR3(GOAL_WIDTH / 2 + 25, SCREEN_HEIGHT - 200.0f + (STAGE_HEIGHT_DOWN / 2), 0.1f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), false, GOAL_TYPE_NORMAL, 2);
	SetGoal(D3DXVECTOR3(SCREEN_WIDTH - GOAL_WIDTH / 2 - 25, 200.0f + (STAGE_HEIGHT_DOWN / 2), 0.1f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), true, GOAL_TYPE_NORMAL, 3);
	SetGoal(D3DXVECTOR3(SCREEN_WIDTH - GOAL_WIDTH / 2 - 25, SCREEN_HEIGHT / 2 + (STAGE_HEIGHT_DOWN / 2), 0.1f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), true, GOAL_TYPE_STRIKE, 4);
	SetGoal(D3DXVECTOR3(SCREEN_WIDTH - GOAL_WIDTH / 2 - 25, SCREEN_HEIGHT - 200.0f + (STAGE_HEIGHT_DOWN / 2), 0.1f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), true, GOAL_TYPE_NORMAL, 5);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_GOAL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffGoal,
		NULL
	);

	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffGoal->Lock(0, 0, (void**)&pVtx, 0);

	//�\���̂̏���������
	for (int nCntGoal = 0; nCntGoal < MAX_GOAL; nCntGoal++, pVtx += 4)
	{
		s_aGoal[nCntGoal].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		s_aGoal[nCntGoal].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		s_aGoal[nCntGoal].bUse = true;

		//���_���W�̐ݒ� = (�z�u�ʒu �} ����(�Ίp���̊p�x �} ����) * �Ίp���̒���)
		pVtx[0].pos.x = s_aGoal[nCntGoal].pos.x - sinf(s_aGoal[nCntGoal].fAngle + s_aGoal[nCntGoal].rot.x) * s_aGoal[nCntGoal].fLength;
		pVtx[0].pos.y = s_aGoal[nCntGoal].pos.y - cosf(s_aGoal[nCntGoal].fAngle + s_aGoal[nCntGoal].rot.y) * s_aGoal[nCntGoal].fLength;
		pVtx[0].pos.z = s_aGoal[nCntGoal].pos.z;

		pVtx[1].pos.x = s_aGoal[nCntGoal].pos.x + sinf(s_aGoal[nCntGoal].fAngle - s_aGoal[nCntGoal].rot.x) * s_aGoal[nCntGoal].fLength;
		pVtx[1].pos.y = s_aGoal[nCntGoal].pos.y - cosf(s_aGoal[nCntGoal].fAngle - s_aGoal[nCntGoal].rot.y) * s_aGoal[nCntGoal].fLength;
		pVtx[1].pos.z = s_aGoal[nCntGoal].pos.z;

		pVtx[2].pos.x = s_aGoal[nCntGoal].pos.x - sinf(s_aGoal[nCntGoal].fAngle - s_aGoal[nCntGoal].rot.x) * s_aGoal[nCntGoal].fLength;
		pVtx[2].pos.y = s_aGoal[nCntGoal].pos.y + cosf(s_aGoal[nCntGoal].fAngle - s_aGoal[nCntGoal].rot.y) * s_aGoal[nCntGoal].fLength;
		pVtx[2].pos.z = s_aGoal[nCntGoal].pos.z;

		pVtx[3].pos.x = s_aGoal[nCntGoal].pos.x + sinf(s_aGoal[nCntGoal].fAngle + s_aGoal[nCntGoal].rot.x) * s_aGoal[nCntGoal].fLength;
		pVtx[3].pos.y = s_aGoal[nCntGoal].pos.y + cosf(s_aGoal[nCntGoal].fAngle + s_aGoal[nCntGoal].rot.y) * s_aGoal[nCntGoal].fLength;
		pVtx[3].pos.z = s_aGoal[nCntGoal].pos.z;

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//�e�N�X�`�����W�̐ݒ�(0.0f ~ (1 / x�p�^�[����)f)
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		if (s_aGoal[nCntGoal].bSide == 0)
		{
			if (s_aGoal[nCntGoal].type == POP_TYPE_NORMAL)
			{
				SetPop(D3DXVECTOR3(GOAL_POP_WIDTH / 2, s_aGoal[nCntGoal].pos.y, 0.0f), s_aGoal[nCntGoal].rot, s_aGoal[nCntGoal].bSide, POP_TYPE_NORMAL, nCntGoal);
			}
			else if (s_aGoal[nCntGoal].type == POP_TYPE_STRIKE)
			{
				SetPop(D3DXVECTOR3(GOAL_POP_WIDTH / 2, s_aGoal[nCntGoal].pos.y, 0.0f), s_aGoal[nCntGoal].rot, s_aGoal[nCntGoal].bSide, POP_TYPE_STRIKE, nCntGoal);
			}
		}
		else
		{
			if (s_aGoal[nCntGoal].type == POP_TYPE_NORMAL)
			{
				SetPop(D3DXVECTOR3(SCREEN_WIDTH - GOAL_POP_WIDTH / 2, s_aGoal[nCntGoal].pos.y, 0.0f), s_aGoal[nCntGoal].rot, s_aGoal[nCntGoal].bSide, POP_TYPE_NORMAL, nCntGoal);
			}
			else if (s_aGoal[nCntGoal].type == POP_TYPE_STRIKE)
			{
				SetPop(D3DXVECTOR3(SCREEN_WIDTH - GOAL_POP_WIDTH / 2, s_aGoal[nCntGoal].pos.y, 0.0f), s_aGoal[nCntGoal].rot, s_aGoal[nCntGoal].bSide, POP_TYPE_STRIKE, nCntGoal);
			}
		}
	}
}

//=========================================
// �S�[���̏I������
//=========================================
void UninitGoal(void)
{
	for (int nCntImgGoal = 0; nCntImgGoal < MAX_IMAGE_DRUM; nCntImgGoal++)
	{
		//�e�N�X�`���̔j��
		if (s_pTextureGoal[nCntImgGoal] != NULL)
		{
			s_pTextureGoal[nCntImgGoal]->Release();
			s_pTextureGoal[nCntImgGoal] = NULL;
		}
		//���_�o�b�t�@�̔j��
		if (s_pVtxBuffGoal != NULL)
		{
			s_pVtxBuffGoal->Release();
			s_pVtxBuffGoal = NULL;
		}
	}
}

//=========================================
// �S�[���̍X�V����
//=========================================
void UpdateGoal(void)
{	
	VERTEX_2D *pVtx;				//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffGoal->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntGoal = 0; nCntGoal < MAX_GOAL; nCntGoal++, pVtx += 4)
	{
		if (s_aGoal[nCntGoal].bUse == true)
		{
			//���_���W�̐ݒ� = (�z�u�ʒu �} ����(�Ίp���̊p�x �} ����) * �Ίp���̒���)
			pVtx[0].pos.x = s_aGoal[nCntGoal].pos.x - sinf(s_aGoal[nCntGoal].fAngle + s_aGoal[nCntGoal].rot.x) * s_aGoal[nCntGoal].fLength;
			pVtx[0].pos.y = s_aGoal[nCntGoal].pos.y - cosf(s_aGoal[nCntGoal].fAngle + s_aGoal[nCntGoal].rot.y) * s_aGoal[nCntGoal].fLength;
			pVtx[0].pos.z = s_aGoal[nCntGoal].pos.z;

			pVtx[1].pos.x = s_aGoal[nCntGoal].pos.x + sinf(s_aGoal[nCntGoal].fAngle - s_aGoal[nCntGoal].rot.x) * s_aGoal[nCntGoal].fLength;
			pVtx[1].pos.y = s_aGoal[nCntGoal].pos.y - cosf(s_aGoal[nCntGoal].fAngle - s_aGoal[nCntGoal].rot.y) * s_aGoal[nCntGoal].fLength;
			pVtx[1].pos.z = s_aGoal[nCntGoal].pos.z;

			pVtx[2].pos.x = s_aGoal[nCntGoal].pos.x - sinf(s_aGoal[nCntGoal].fAngle - s_aGoal[nCntGoal].rot.x) * s_aGoal[nCntGoal].fLength;
			pVtx[2].pos.y = s_aGoal[nCntGoal].pos.y + cosf(s_aGoal[nCntGoal].fAngle - s_aGoal[nCntGoal].rot.y) * s_aGoal[nCntGoal].fLength;
			pVtx[2].pos.z = s_aGoal[nCntGoal].pos.z;

			pVtx[3].pos.x = s_aGoal[nCntGoal].pos.x + sinf(s_aGoal[nCntGoal].fAngle + s_aGoal[nCntGoal].rot.x) * s_aGoal[nCntGoal].fLength;
			pVtx[3].pos.y = s_aGoal[nCntGoal].pos.y + cosf(s_aGoal[nCntGoal].fAngle + s_aGoal[nCntGoal].rot.y) * s_aGoal[nCntGoal].fLength;
			pVtx[3].pos.z = s_aGoal[nCntGoal].pos.z;

			//�e�N�X�`�����W�̐ݒ�(0.0f ~ (1 / x�p�^�[����)f)
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			//�S�[���T�C�h�Ō�����ς��鏈��
			if (s_aGoal[nCntGoal].bSide == 0)
			{
				//�e�N�X�`�����W�̐ݒ�(0.0f ~ (1 / x�p�^�[����)f)
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}
			else
			{
				//�e�N�X�`�����W�̐ݒ�(0.0f ~ (1 / x�p�^�[����)f)
				pVtx[0].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.0f, 1.0f);
			}
		}
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffGoal->Unlock();
}

//=========================================
// �S�[���̕`�揈��
//=========================================
void DrawGoal()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource
	(
		0,
		s_pVtxBuffGoal,		//���_�o�b�t�@�ւ̃|�C���^
		0,
		sizeof(VERTEX_2D)		//���_���\���̂̃T�C�Y
	);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntGoal = 0; nCntGoal < MAX_GOAL; nCntGoal++)
	{
		if (s_aGoal[nCntGoal].bUse == true)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, s_pTextureGoal[s_aGoal[nCntGoal].type]);

			//�|���S���̕`��
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
				nCntGoal * 4,			//�`�悷��ŏ��̒��_�C���f�b�N�X
				2						//�v���~�e�B�u�A�C�R���̌�
			);
		}
	}
}

//=========================================
// �S�[���̐ݒ菈��
//=========================================
void SetGoal(D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool side, GOAL_TYPE type, int nIdxGoal)
{
	s_aGoal[nIdxGoal].pos = pos;
	s_aGoal[nIdxGoal].rot = rot;
	s_aGoal[nIdxGoal].bSide = side;
	s_aGoal[nIdxGoal].type = type;
	s_aGoal[nIdxGoal].bUse = true;

	if (s_aGoal[nIdxGoal].type == GOAL_TYPE_NORMAL)
	{
		s_aGoal[nIdxGoal].fAngle = atan2f(GOAL_WIDTH, GOAL_HEIGHT);
		s_aGoal[nIdxGoal].fLength = sqrtf((GOAL_WIDTH * GOAL_WIDTH) + (GOAL_HEIGHT * GOAL_HEIGHT)) / 2.0f;
	}
	else if (s_aGoal[nIdxGoal].type == GOAL_TYPE_STRIKE)
	{
		s_aGoal[nIdxGoal].fAngle = atan2f(GOAL_WIDTH, STRIKE_GOAL_HEIGHT);
		s_aGoal[nIdxGoal].fLength = sqrtf((GOAL_WIDTH * GOAL_WIDTH) + (STRIKE_GOAL_HEIGHT * STRIKE_GOAL_HEIGHT)) / 2.0f;
	}
}

//=========================================
// �S�[���̓����蔻�菈��
//=========================================
void ColisionGoal(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pLastPos)
{
	VERTEX_2D *pVtx;			//���_���ւ̃|�C���^

	Disk *pDisk = GetDisk();

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffGoal->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntGoal = 0; nCntGoal < MAX_GOAL; nCntGoal++)
	{
		if (s_aGoal[nCntGoal].bUse == true)
		{
			if (s_aGoal[nCntGoal].type == GOAL_TYPE_NORMAL)
			{//3�_�S�[���̏ꍇ
				if (ColSegmentsGoal(*pPos, *pPos - *pLastPos,
					s_aGoal[nCntGoal].pos - D3DXVECTOR3(0.0f, (GOAL_HEIGHT / 2), 0.0f),
					(s_aGoal[nCntGoal].pos + D3DXVECTOR3(0.0f, (GOAL_HEIGHT / 2), 0.0f)) - (s_aGoal[nCntGoal].pos - D3DXVECTOR3(0.0f, (GOAL_HEIGHT / 2), 0.0f))) == true)
				{
					if (s_aGoal[nCntGoal].bSide == false)
					{
						for (int nCntEffect = 0; nCntEffect < 50; nCntEffect++)
						{
							SetEffect(D3DXVECTOR3(300.0f, 150.0f, 0.0f), 0.0f, EFFECT_TYPE_POINT_SMOKE_BLUE);
							SetEffect(D3DXVECTOR3(300.0f, SCREEN_HEIGHT - 100.0f, 0.0f), 0.0f, EFFECT_TYPE_POINT_SMOKE_BLUE);
						}

						for (int nCntEffect = 0; nCntEffect < 50; nCntEffect++)
						{
							SetEffect(D3DXVECTOR3(SCREEN_WIDTH - 300.0f, 150.0f, 0.0f), 0.0f, EFFECT_TYPE_POINT_SMOKE_BLUE);
							SetEffect(D3DXVECTOR3(SCREEN_WIDTH - 300.0f, SCREEN_HEIGHT - 100.0f, 0.0f), 0.0f, EFFECT_TYPE_POINT_SMOKE_BLUE);
						}

						SetPop(D3DXVECTOR3 (GOAL_POP_WIDTH / 2, pDisk->pos.y,0.0f), s_aGoal[nCntGoal].rot, s_aGoal[nCntGoal].bSide, POP_TYPE_NORMAL, nCntGoal);
						AddScore(3, 1);
						SetThoThrowRefreeIdx(0);	// ����������̑I��
						*GetResetScore() = true;
						PlaySound(SOUND_LABEL_SE_GOAL);
					}
					else
					{
						for (int nCntEffect = 0; nCntEffect < 50; nCntEffect++)
						{
							SetEffect(D3DXVECTOR3(300.0f, 150.0f, 0.0f), 0.0f, EFFECT_TYPE_POINT_SMOKE_RED);
							SetEffect(D3DXVECTOR3(300.0f, SCREEN_HEIGHT - 100.0f, 0.0f), 0.0f, EFFECT_TYPE_POINT_SMOKE_RED);
						}

						for (int nCntEffect = 0; nCntEffect < 50; nCntEffect++)
						{
							SetEffect(D3DXVECTOR3(SCREEN_WIDTH - 300.0f, 150.0f, 0.0f), 0.0f, EFFECT_TYPE_POINT_SMOKE_RED);
							SetEffect(D3DXVECTOR3(SCREEN_WIDTH - 300.0f, SCREEN_HEIGHT - 100.0f, 0.0f), 0.0f, EFFECT_TYPE_POINT_SMOKE_RED);
						}

						SetPop(D3DXVECTOR3(SCREEN_WIDTH - GOAL_POP_WIDTH / 2, pDisk->pos.y, 0.0f), s_aGoal[nCntGoal].rot, s_aGoal[nCntGoal].bSide, POP_TYPE_NORMAL, nCntGoal);
						AddScore(3, 0);
						SetThoThrowRefreeIdx(1);	// ����������̑I��
						*GetResetScore() = true;
						PlaySound(SOUND_LABEL_SE_GOAL);
					}
					pDisk->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				}
			}
			else if(s_aGoal[nCntGoal].type == GOAL_TYPE_STRIKE)
			{//5�_�S�[���̏ꍇ
				if (ColSegmentsGoal(*pPos, *pPos - *pLastPos,
					s_aGoal[nCntGoal].pos - D3DXVECTOR3(0.0f, (STRIKE_GOAL_HEIGHT / 2), 0.0f),
					(s_aGoal[nCntGoal].pos + D3DXVECTOR3(0.0f, (STRIKE_GOAL_HEIGHT / 2), 0.0f)) - (s_aGoal[nCntGoal].pos - D3DXVECTOR3(0.0f, (STRIKE_GOAL_HEIGHT / 2), 0.0f))) == true)
				{
					if (s_aGoal[nCntGoal].bSide == false)
					{
						for (int nCntEffect = 0; nCntEffect < 50; nCntEffect++)
						{
							SetEffect(D3DXVECTOR3(300.0f, 150.0f, 0.0f), 0.0f, EFFECT_TYPE_POINT_SMOKE_BLUE);
							SetEffect(D3DXVECTOR3(300.0f, SCREEN_HEIGHT - 100.0f, 0.0f), 0.0f, EFFECT_TYPE_POINT_SMOKE_BLUE);
						}

						for (int nCntEffect = 0; nCntEffect < 50; nCntEffect++)
						{
							SetEffect(D3DXVECTOR3(SCREEN_WIDTH - 300.0f, 150.0f, 0.0f), 0.0f, EFFECT_TYPE_POINT_SMOKE_BLUE);
							SetEffect(D3DXVECTOR3(SCREEN_WIDTH - 300.0f, SCREEN_HEIGHT - 100.0f, 0.0f), 0.0f, EFFECT_TYPE_POINT_SMOKE_BLUE);
						}
						
						SetPop(D3DXVECTOR3(GOAL_POP_WIDTH / 2, pDisk->pos.y, 0.0f), s_aGoal[nCntGoal].rot, s_aGoal[nCntGoal].bSide, POP_TYPE_STRIKE, nCntGoal);
						AddScore(5,1);
						SetThoThrowRefreeIdx(0);	// ����������̑I��
						*GetResetScore() = true;
						PlaySound(SOUND_LABEL_SE_GOAL);
					}
					else
					{
						for (int nCntEffect = 0; nCntEffect < 150; nCntEffect++)
						{
							SetEffect(D3DXVECTOR3(300.0f, 0.0f, 0.0f), 0.0f, EFFECT_TYPE_POINT_SMOKE_RED);
							SetEffect(D3DXVECTOR3(300.0f, SCREEN_HEIGHT, 0.0f), 0.0f, EFFECT_TYPE_POINT_SMOKE_RED);
							SetEffect(D3DXVECTOR3(SCREEN_WIDTH - 300.0f, 0.0f, 0.0f), 0.0f, EFFECT_TYPE_POINT_SMOKE_RED);
							SetEffect(D3DXVECTOR3(SCREEN_WIDTH - 300.0f, SCREEN_HEIGHT, 0.0f), 0.0f, EFFECT_TYPE_POINT_SMOKE_RED);
						}

						SetPop(D3DXVECTOR3(SCREEN_WIDTH - GOAL_POP_WIDTH / 2, pDisk->pos.y, 0.0f), s_aGoal[nCntGoal].rot, s_aGoal[nCntGoal].bSide, POP_TYPE_STRIKE, nCntGoal);
						AddScore(5, 0);
						SetThoThrowRefreeIdx(1);	// ����������̑I��
						*GetResetScore() = true;
						PlaySound(SOUND_LABEL_SE_GOAL);
					}
					pDisk->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				}
			}
		}
	}
	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffGoal->Unlock();
}

//============================================================================
//�S�[���̊O�Ϗ���
//============================================================================
bool ColSegmentsGoal(D3DXVECTOR3 &seg1Start, D3DXVECTOR3 &seg1Vec, D3DXVECTOR3 &seg2Start, D3DXVECTOR3 &seg2Vec)
{
	D3DXVECTOR3 vec = seg2Start - seg1Start;
	float Crs_v1_v2 = Vec3CrossGoal(&seg1Vec, &seg2Vec);

	if (Crs_v1_v2 == 0.0f) {
		// ���s���
		return false;
	}

	float Crs_v_v1 = Vec3CrossGoal(&vec, &seg1Vec);
	float Crs_v_v2 = Vec3CrossGoal(&vec, &seg2Vec);

	float t1 = Crs_v_v2 / Crs_v1_v2;
	float t2 = Crs_v_v1 / Crs_v1_v2;

	float eps = 0.00001f;

	if (t1 + eps < 0 || t1 - eps > 1 || t2 + eps < 0 || t2 - eps > 1) {
		// �������Ă��Ȃ�
		return false;
	}

	return true;
}

//============================================================================
//�S�[���̓��Ϗ���
//============================================================================
float Vec3CrossGoal(D3DXVECTOR3* vec1, D3DXVECTOR3* vec2) 
{
	return (vec1->x * vec2->y) - (vec1->y * vec2->x);
}

//============================================================================
//�S�[���̎擾����
//============================================================================
Goal *GetGoal(void)
{
	return s_aGoal;	//�S�[�����̐擪�A�h���X��Ԃ�
}