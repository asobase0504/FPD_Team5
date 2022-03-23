//=========================================
// 
// �S�[������
// Author Tanimoto_Kosuke
//
// Update 22/03/22
// 
//=========================================
//------------------------------------
// include
//------------------------------------
#include "pop.h"
#include <stdio.h>
#include "disk.h"
#include "stage.h"
#include "score.h"
#include "goal.h"

//------------------------------------
// �X�^�e�B�b�N�ϐ�
//------------------------------------
static LPDIRECT3DTEXTURE9		s_pTexturePop[MAX_IMAGE_POP] = {};	//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffPop = NULL;				//���_�o�b�t�@�ւ̃|�C���^
static POP s_aPop[MAX_POP];											//�S�[���̏��

//=========================================
// �|�b�v�̏���������
//=========================================
void InitPop(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^
										
	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\goal\\Arrow.png",	//�e�N�X�`���̃t�@�C����
		&s_pTexturePop[POP_TYPE_NORMAL]
	);

	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\goal\\Arrow.png",	//�e�N�X�`���̃t�@�C����
		&s_pTexturePop[POP_TYPE_STRIKE]
	);

	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\pop\\life000.png",	//�e�N�X�`���̃t�@�C����
		&s_pTexturePop[POP_TYPE_FELL]
	);

	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\gotou\\TEXTURE\\number000.png",	//�e�N�X�`���̃t�@�C����
		&s_pTexturePop[POP_TYPE_SCORE]
	);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_POP,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffPop,
		NULL
	);

	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffPop->Lock(0, 0, (void**)&pVtx, 0);

	//�\���̂̏���������
	for (int nCntPop = 0; nCntPop < MAX_POP; nCntPop++, pVtx += 4)
	{
		s_aPop[nCntPop].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		s_aPop[nCntPop].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		s_aPop[nCntPop].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		s_aPop[nCntPop].fPopCounter = 0;
		s_aPop[nCntPop].bUse = false;
		s_aPop[nCntPop].bSide = false;

		//���_���W�̐ݒ� = (�z�u�ʒu �} ����(�Ίp���̊p�x �} ����) * �Ίp���̒���)
		pVtx[0].pos.x = s_aPop[nCntPop].pos.x - sinf(s_aPop[nCntPop].fAngle + s_aPop[nCntPop].rot.x) * s_aPop[nCntPop].fLength;
		pVtx[0].pos.y = s_aPop[nCntPop].pos.y - cosf(s_aPop[nCntPop].fAngle + s_aPop[nCntPop].rot.y) * s_aPop[nCntPop].fLength;
		pVtx[0].pos.z = s_aPop[nCntPop].pos.z;

		pVtx[1].pos.x = s_aPop[nCntPop].pos.x + sinf(s_aPop[nCntPop].fAngle - s_aPop[nCntPop].rot.x) * s_aPop[nCntPop].fLength;
		pVtx[1].pos.y = s_aPop[nCntPop].pos.y - cosf(s_aPop[nCntPop].fAngle - s_aPop[nCntPop].rot.y) * s_aPop[nCntPop].fLength;
		pVtx[1].pos.z = s_aPop[nCntPop].pos.z;

		pVtx[2].pos.x = s_aPop[nCntPop].pos.x - sinf(s_aPop[nCntPop].fAngle - s_aPop[nCntPop].rot.x) * s_aPop[nCntPop].fLength;
		pVtx[2].pos.y = s_aPop[nCntPop].pos.y + cosf(s_aPop[nCntPop].fAngle - s_aPop[nCntPop].rot.y) * s_aPop[nCntPop].fLength;
		pVtx[2].pos.z = s_aPop[nCntPop].pos.z;

		pVtx[3].pos.x = s_aPop[nCntPop].pos.x + sinf(s_aPop[nCntPop].fAngle + s_aPop[nCntPop].rot.x) * s_aPop[nCntPop].fLength;
		pVtx[3].pos.y = s_aPop[nCntPop].pos.y + cosf(s_aPop[nCntPop].fAngle + s_aPop[nCntPop].rot.y) * s_aPop[nCntPop].fLength;
		pVtx[3].pos.z = s_aPop[nCntPop].pos.z;

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//�e�N�X�`�����W�̐ݒ�(0.0f ~ (1 / x�p�^�[����)f)
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffPop->Unlock();
}

//=========================================
// �|�b�v�̏I������
//=========================================
void UninitPop(void)
{
	for (int nCntImgPop = 0; nCntImgPop < MAX_IMAGE_POP; nCntImgPop++)
	{
		//�e�N�X�`���̔j��
		if (s_pTexturePop[nCntImgPop] != NULL)
		{
			s_pTexturePop[nCntImgPop]->Release();
			s_pTexturePop[nCntImgPop] = NULL;
		}
		//���_�o�b�t�@�̔j��
		if (s_pVtxBuffPop != NULL)
		{
			s_pVtxBuffPop->Release();
			s_pVtxBuffPop = NULL;
		}
	}
}
//=========================================
// �|�b�v�̍X�V����
//=========================================
void UpdatePop(void)
{
	SCORE *pScore = GetScore();

	int aPosTexU[4];	//�e���̐������i�[
	int nCntScore = 0;

	aPosTexU[0] = (pScore[0].nScore[0] % 100) / 10;
	aPosTexU[1] = (pScore[0].nScore[1] % 10) / 1;
	aPosTexU[2] = (pScore[1].nScore[0] % 100) / 10;
	aPosTexU[3] = (pScore[1].nScore[1] % 10) / 1;

	VERTEX_2D *pVtx;				//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffPop->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPop = 0; nCntPop < MAX_POP; nCntPop++, pVtx += 4)
	{
		if (s_aPop[nCntPop].bUse == true)
		{
			//�|�b�v�b������
			PopCounter(nCntPop);

			//�ړ��ʂ��ʒu�ɕϊ�
			s_aPop[nCntPop].pos += s_aPop[nCntPop].move;

			//�ړ�����
			s_aPop[nCntPop].move *= 0.9f;

			//���_���W�̐ݒ� = (�z�u�ʒu �} ����(�Ίp���̊p�x �} ����) * �Ίp���̒���)
			pVtx[0].pos.x = s_aPop[nCntPop].pos.x - sinf(s_aPop[nCntPop].fAngle + s_aPop[nCntPop].rot.x) * s_aPop[nCntPop].fLength;
			pVtx[0].pos.y = s_aPop[nCntPop].pos.y - cosf(s_aPop[nCntPop].fAngle + s_aPop[nCntPop].rot.y) * s_aPop[nCntPop].fLength;
			pVtx[0].pos.z = s_aPop[nCntPop].pos.z;

			pVtx[1].pos.x = s_aPop[nCntPop].pos.x + sinf(s_aPop[nCntPop].fAngle - s_aPop[nCntPop].rot.x) * s_aPop[nCntPop].fLength;
			pVtx[1].pos.y = s_aPop[nCntPop].pos.y - cosf(s_aPop[nCntPop].fAngle - s_aPop[nCntPop].rot.y) * s_aPop[nCntPop].fLength;
			pVtx[1].pos.z = s_aPop[nCntPop].pos.z;

			pVtx[2].pos.x = s_aPop[nCntPop].pos.x - sinf(s_aPop[nCntPop].fAngle - s_aPop[nCntPop].rot.x) * s_aPop[nCntPop].fLength;
			pVtx[2].pos.y = s_aPop[nCntPop].pos.y + cosf(s_aPop[nCntPop].fAngle - s_aPop[nCntPop].rot.y) * s_aPop[nCntPop].fLength;
			pVtx[2].pos.z = s_aPop[nCntPop].pos.z;

			pVtx[3].pos.x = s_aPop[nCntPop].pos.x + sinf(s_aPop[nCntPop].fAngle + s_aPop[nCntPop].rot.x) * s_aPop[nCntPop].fLength;
			pVtx[3].pos.y = s_aPop[nCntPop].pos.y + cosf(s_aPop[nCntPop].fAngle + s_aPop[nCntPop].rot.y) * s_aPop[nCntPop].fLength;
			pVtx[3].pos.z = s_aPop[nCntPop].pos.z;

			if (s_aPop[nCntPop].type == POP_TYPE_NORMAL)
			{
				//���_�J���[�̐ݒ�
				pVtx[0].col = D3DCOLOR_RGBA(0, 255, 255, 255);
				pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			}
			else if (s_aPop[nCntPop].type == POP_TYPE_STRIKE)
			{
				//���_�J���[�̐ݒ�
				pVtx[0].col = D3DCOLOR_RGBA(255, 0, 255, 255);
				pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			}
			else if (s_aPop[nCntPop].type == POP_TYPE_SCORE)
			{
				//�e�N�X�`�����W�̐ݒ�(0.0f ~ (1 / x�p�^�[����)f)
				pVtx[0].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntScore], 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntScore] + 0.1f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntScore], 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntScore] + 0.1f, 1.0f);
				nCntScore++;
			}

			if (s_aPop[nCntPop].bSide == true)
			{//�E���̃e�N�X�`�����t����
				pVtx[0].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.0f, 1.0f);
			}
		}
	}
	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffPop->Unlock();
}

//=========================================
// �|�b�v�̕`�揈��
//=========================================
void DrawPop()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^

	pDevice->SetStreamSource
	(
		0,
		s_pVtxBuffPop,		//���_�o�b�t�@�ւ̃|�C���^
		0,
		sizeof(VERTEX_2D)		//���_���\���̂̃T�C�Y
	);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntPop = 0; nCntPop < MAX_POP; nCntPop++)
	{
		if (s_aPop[nCntPop].bUse == true)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, s_pTexturePop[s_aPop[nCntPop].type]);

			//�|���S���̕`��
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
				nCntPop * 4,			//�`�悷��ŏ��̒��_�C���f�b�N�X
				2						//�v���~�e�B�u�A�C�R���̌�
			);
		}
	}
}

//============================================================================
//�|�b�v�̐ݒ菈��
//============================================================================
void SetPop(D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool side, POP_TYPE type, int nIdxGoal)
{
	s_aPop[nIdxGoal].pos = pos;
	s_aPop[nIdxGoal].rot = rot;
	s_aPop[nIdxGoal].bSide = side;
	s_aPop[nIdxGoal].type = type;
	s_aPop[nIdxGoal].bUse = true;

	if (s_aPop[nIdxGoal].type == POP_TYPE_NORMAL || s_aPop[nIdxGoal].type == POP_TYPE_STRIKE)
	{
		s_aPop[nIdxGoal].fAngle = atan2f(GOAL_POP_WIDTH, GOAL_POP_HEIGHT);
		s_aPop[nIdxGoal].fLength = sqrtf((GOAL_POP_WIDTH * GOAL_POP_WIDTH) + (GOAL_POP_HEIGHT * GOAL_POP_HEIGHT)) / 2.0f;
	}
	else if (s_aPop[nIdxGoal].type == POP_TYPE_FELL)
	{
		s_aPop[nIdxGoal].fAngle = atan2f(FELL_POP_WIDTH, FELL_POP_HEIGHT);
		s_aPop[nIdxGoal].fLength = sqrtf((FELL_POP_WIDTH * FELL_POP_WIDTH) + (FELL_POP_HEIGHT * FELL_POP_HEIGHT)) / 2.0f;
	}
	else if (s_aPop[nIdxGoal].type == POP_TYPE_SCORE)
	{
		s_aPop[nIdxGoal].fAngle = atan2f(SCORE_POP_WIDTH, SCORE_POP_HEIGHT);
		s_aPop[nIdxGoal].fLength = sqrtf((SCORE_POP_WIDTH * SCORE_POP_WIDTH) + (SCORE_POP_HEIGHT * SCORE_POP_HEIGHT)) / 2.0f;
	}
}

//============================================================================
//�|�b�v�̕b������
//============================================================================
void PopCounter(int nIdxPop)
{
	if (s_aPop[nIdxPop].type == POP_TYPE_NORMAL || s_aPop[nIdxPop].type == POP_TYPE_STRIKE)
	{
		//�|�b�v����
		if (s_aPop[nIdxPop].fPopCounter >= 80)
		{
			if (s_aPop[nIdxPop].bSide == 0)
			{
				s_aPop[nIdxPop].move.x = -POP_SPEAD;
			}
			else
			{
				s_aPop[nIdxPop].move.x = POP_SPEAD;
			}
		}
		if (s_aPop[nIdxPop].fPopCounter >= 120)
		{
			s_aPop[nIdxPop].bUse = false;
			s_aPop[nIdxPop].fPopCounter = 0;
			s_aPop[nIdxPop].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}
		else
		{
			s_aPop[nIdxPop].fPopCounter++;
		}
	}
	else if (s_aPop[nIdxPop].type == POP_TYPE_FELL || s_aPop[nIdxPop].type == POP_TYPE_SCORE)
	{
		//�|�b�v����
		if (s_aPop[nIdxPop].fPopCounter >= 120)
		{
			s_aPop[nIdxPop].bUse = false;
			s_aPop[nIdxPop].fPopCounter = 0;
			s_aPop[nIdxPop].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}
		else
		{
			s_aPop[nIdxPop].fPopCounter++;
		}
	}
}

//============================================================================
//�|�b�v�̎擾����
//============================================================================
POP *GetPop(void)
{
	return s_aPop;	//�S�[�����̐擪�A�h���X��Ԃ�
}