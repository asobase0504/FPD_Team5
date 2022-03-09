//=========================================
// 
// �X�e�[�W����
// Author Tanimoto_Kosuke
//
// Update 22/03/09
// 
//=========================================
//------------------------------------
// include
//------------------------------------
#include "stage.h"
#include "main.h"
#include <stdio.h>
#include "goal.h"

//------------------------------------
// �X�^�e�B�b�N�ϐ�
//------------------------------------
static LPDIRECT3DTEXTURE9		s_pTextureStage[MAX_IMAGE_STAGE] = {};	//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffStage = NULL;	//���_�o�b�t�@�ւ̃|�C���^
static STAGE s_aStage[MAX_STAGE];									//�X�e�[�W�̏��
static bool s_bPause;	// �|�[�Y�����ǂ���

//=========================================
// �X�e�[�W�̏���������
//=========================================
void InitStage(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^

	//�e�N�X�`���[�̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\tanimoto\\TEXTURE\\block004.jpg",	//�e�N�X�`���̃t�@�C����
		&s_pTextureStage[STAGE_TYPE_FRONT]
	);

	//�e�N�X�`���[�̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\tanimoto\\TEXTURE\\block005.jpg",	//�e�N�X�`���̃t�@�C����
		&s_pTextureStage[STAGE_TYPE_BACK]
	);

	//�\���̂̏���������
	s_aStage[STAGE_TYPE_FRONT].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
	s_aStage[STAGE_TYPE_FRONT].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_aStage[STAGE_TYPE_FRONT].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_aStage[STAGE_TYPE_FRONT].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	s_aStage[STAGE_TYPE_FRONT].fAngle = atan2f(STAGE_WIDTH, STAGE_HEIGHT);
	s_aStage[STAGE_TYPE_FRONT].fLength = sqrtf((STAGE_WIDTH * STAGE_WIDTH) + (STAGE_HEIGHT * STAGE_HEIGHT)) / 2.0f;
	s_aStage[STAGE_TYPE_FRONT].type = STAGE_TYPE_FRONT;

	s_aStage[STAGE_TYPE_BACK].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.1f);
	s_aStage[STAGE_TYPE_BACK].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_aStage[STAGE_TYPE_BACK].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_aStage[STAGE_TYPE_BACK].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	s_aStage[STAGE_TYPE_BACK].fAngle = atan2f(SCREEN_WIDTH, SCREEN_HEIGHT);
	s_aStage[STAGE_TYPE_BACK].fLength = sqrtf((SCREEN_WIDTH * SCREEN_WIDTH) + (SCREEN_HEIGHT * SCREEN_HEIGHT)) / 2.0f;
	s_aStage[STAGE_TYPE_BACK].type = STAGE_TYPE_BACK;
	
	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_STAGE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffStage,
		NULL
	);

	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffStage->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntStage = 0; nCntStage < MAX_STAGE; nCntStage++,pVtx += 4)
	{
		//���_���W�̐ݒ� = (�z�u�ʒu �} ����(�Ίp���̊p�x �} ����) * �Ίp���̒���)
		pVtx[0].pos.x = s_aStage[nCntStage].pos.x - sinf(s_aStage[nCntStage].fAngle + s_aStage[nCntStage].rot.x) * s_aStage[nCntStage].fLength;
		pVtx[0].pos.y = s_aStage[nCntStage].pos.y - cosf(s_aStage[nCntStage].fAngle + s_aStage[nCntStage].rot.y) * s_aStage[nCntStage].fLength;
		pVtx[0].pos.z = s_aStage[nCntStage].pos.z;

		pVtx[1].pos.x = s_aStage[nCntStage].pos.x + sinf(s_aStage[nCntStage].fAngle - s_aStage[nCntStage].rot.x) * s_aStage[nCntStage].fLength;
		pVtx[1].pos.y = s_aStage[nCntStage].pos.y - cosf(s_aStage[nCntStage].fAngle - s_aStage[nCntStage].rot.y) * s_aStage[nCntStage].fLength;
		pVtx[1].pos.z = s_aStage[nCntStage].pos.z;

		pVtx[2].pos.x = s_aStage[nCntStage].pos.x - sinf(s_aStage[nCntStage].fAngle - s_aStage[nCntStage].rot.x) * s_aStage[nCntStage].fLength;
		pVtx[2].pos.y = s_aStage[nCntStage].pos.y + cosf(s_aStage[nCntStage].fAngle - s_aStage[nCntStage].rot.y) * s_aStage[nCntStage].fLength;
		pVtx[2].pos.z = s_aStage[nCntStage].pos.z;

		pVtx[3].pos.x = s_aStage[nCntStage].pos.x + sinf(s_aStage[nCntStage].fAngle + s_aStage[nCntStage].rot.x) * s_aStage[nCntStage].fLength;
		pVtx[3].pos.y = s_aStage[nCntStage].pos.y + cosf(s_aStage[nCntStage].fAngle + s_aStage[nCntStage].rot.y) * s_aStage[nCntStage].fLength;
		pVtx[3].pos.z = s_aStage[nCntStage].pos.z;

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
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffStage->Unlock();

	InitGoal();
}

//=========================================
// �X�e�[�W�̏I������
//=========================================
void UninitStage(void)
{
	for (int nCntImgStage = 0; nCntImgStage < MAX_IMAGE_STAGE; nCntImgStage++)
	{
		//�e�N�X�`���̔j��
		if (s_pTextureStage[nCntImgStage] != NULL)
		{
			s_pTextureStage[nCntImgStage]->Release();
			s_pTextureStage[nCntImgStage] = NULL;
		}
		//���_�o�b�t�@�̔j��
		if (s_pVtxBuffStage != NULL)
		{
			s_pVtxBuffStage->Release();
			s_pVtxBuffStage = NULL;
		}
	}

	UninitGoal();
}

//=========================================
// �X�e�[�W�̏���������
//=========================================
void UpdateStage(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^
	
	VERTEX_2D *pVtx;				//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffStage->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntStage = 0; nCntStage < MAX_STAGE; nCntStage++, pVtx += 4)
	{
		//���_���W�̐ݒ� = (�z�u�ʒu �} ����(�Ίp���̊p�x �} ����) * �Ίp���̒���)
		pVtx[0].pos.x = s_aStage[nCntStage].pos.x - sinf(s_aStage[nCntStage].fAngle + s_aStage[nCntStage].rot.x) * s_aStage[nCntStage].fLength;
		pVtx[0].pos.y = s_aStage[nCntStage].pos.y - cosf(s_aStage[nCntStage].fAngle + s_aStage[nCntStage].rot.y) * s_aStage[nCntStage].fLength;
		pVtx[0].pos.z = s_aStage[nCntStage].pos.z;

		pVtx[1].pos.x = s_aStage[nCntStage].pos.x + sinf(s_aStage[nCntStage].fAngle - s_aStage[nCntStage].rot.x) * s_aStage[nCntStage].fLength;
		pVtx[1].pos.y = s_aStage[nCntStage].pos.y - cosf(s_aStage[nCntStage].fAngle - s_aStage[nCntStage].rot.y) * s_aStage[nCntStage].fLength;
		pVtx[1].pos.z = s_aStage[nCntStage].pos.z;

		pVtx[2].pos.x = s_aStage[nCntStage].pos.x - sinf(s_aStage[nCntStage].fAngle - s_aStage[nCntStage].rot.x) * s_aStage[nCntStage].fLength;
		pVtx[2].pos.y = s_aStage[nCntStage].pos.y + cosf(s_aStage[nCntStage].fAngle - s_aStage[nCntStage].rot.y) * s_aStage[nCntStage].fLength;
		pVtx[2].pos.z = s_aStage[nCntStage].pos.z;

		pVtx[3].pos.x = s_aStage[nCntStage].pos.x + sinf(s_aStage[nCntStage].fAngle + s_aStage[nCntStage].rot.x) * s_aStage[nCntStage].fLength;
		pVtx[3].pos.y = s_aStage[nCntStage].pos.y + cosf(s_aStage[nCntStage].fAngle + s_aStage[nCntStage].rot.y) * s_aStage[nCntStage].fLength;
		pVtx[3].pos.z = s_aStage[nCntStage].pos.z;
	}
	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffStage->Unlock();

	UpdateGoal();
}

//=========================================
// �X�e�[�W�̏���������
//=========================================
void DrawStage()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource
	(
		0,
		s_pVtxBuffStage,		//���_�o�b�t�@�ւ̃|�C���^
		0,
		sizeof(VERTEX_2D)		//���_���\���̂̃T�C�Y
	);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntStage = 0; nCntStage < MAX_STAGE; nCntStage++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, s_pTextureStage[s_aStage[nCntStage].type]);

		//�|���S���̕`��
		pDevice->DrawPrimitive
		(
			D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
			nCntStage * 4,			//�`�悷��ŏ��̒��_�C���f�b�N�X
			2						//�v���~�e�B�u�A�C�R���̌�
		);
	}

	DrawGoal();
}
