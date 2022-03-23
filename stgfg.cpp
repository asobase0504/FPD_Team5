//=========================================
// 
// �X�e�[�W�O�i����
// Author Tanimoto_Kosuke
//
// Update 22/03/23
// 
//=========================================
//------------------------------------
// include
//------------------------------------
#include "stgfg.h"
#include <stdio.h>
#include "goal.h"
#include "wall.h"
#include "game.h"
#include "stage.h"
#include "referee.h"

//------------------------------------
// �X�^�e�B�b�N�ϐ�
//------------------------------------
static LPDIRECT3DTEXTURE9		s_pTextureStgFg[MAX_IMAGE_STGFG] = {};	//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffStgFg = NULL;					//���_�o�b�t�@�ւ̃|�C���^
static STGFG s_aStgFg[MAX_STGFG_OBJECT];

//=========================================
// �X�e�[�W�O�i�̏���������
//=========================================
void InitStgFg(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^

	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\stage\\wall.png",	//�e�N�X�`���̃t�@�C����
		&s_pTextureStgFg[STGFG_TYPE_WALL]
	);

	//��(����)
	s_aStgFg[0].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, MAX_HEIGHT + 5, 0.0f);
	s_aStgFg[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_aStgFg[0].fAngle = atan2f(STAGE_WALL_WIDTH, STAGE_WALL_HEIGHT);
	s_aStgFg[0].fLength = sqrtf((STAGE_WALL_WIDTH * STAGE_WALL_WIDTH) + (STAGE_WALL_HEIGHT * STAGE_WALL_HEIGHT)) / 2.0f;
	s_aStgFg[0].type = STGFG_TYPE_WALL;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_STGFG_OBJECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffStgFg,
		NULL
	);

	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffStgFg->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntStgFg = 0; nCntStgFg < MAX_STGFG_OBJECT; nCntStgFg++, pVtx += 4)
	{
		//���_���W�̐ݒ� = (�z�u�ʒu �} ����(�Ίp���̊p�x �} ����) * �Ίp���̒���)
		pVtx[0].pos.x = s_aStgFg[nCntStgFg].pos.x - sinf(s_aStgFg[nCntStgFg].fAngle + s_aStgFg[nCntStgFg].rot.x) * s_aStgFg[nCntStgFg].fLength;
		pVtx[0].pos.y = s_aStgFg[nCntStgFg].pos.y - cosf(s_aStgFg[nCntStgFg].fAngle + s_aStgFg[nCntStgFg].rot.y) * s_aStgFg[nCntStgFg].fLength;
		pVtx[0].pos.z = s_aStgFg[nCntStgFg].pos.z;

		pVtx[1].pos.x = s_aStgFg[nCntStgFg].pos.x + sinf(s_aStgFg[nCntStgFg].fAngle - s_aStgFg[nCntStgFg].rot.x) * s_aStgFg[nCntStgFg].fLength;
		pVtx[1].pos.y = s_aStgFg[nCntStgFg].pos.y - cosf(s_aStgFg[nCntStgFg].fAngle - s_aStgFg[nCntStgFg].rot.y) * s_aStgFg[nCntStgFg].fLength;
		pVtx[1].pos.z = s_aStgFg[nCntStgFg].pos.z;

		pVtx[2].pos.x = s_aStgFg[nCntStgFg].pos.x - sinf(s_aStgFg[nCntStgFg].fAngle - s_aStgFg[nCntStgFg].rot.x) * s_aStgFg[nCntStgFg].fLength;
		pVtx[2].pos.y = s_aStgFg[nCntStgFg].pos.y + cosf(s_aStgFg[nCntStgFg].fAngle - s_aStgFg[nCntStgFg].rot.y) * s_aStgFg[nCntStgFg].fLength;
		pVtx[2].pos.z = s_aStgFg[nCntStgFg].pos.z;

		pVtx[3].pos.x = s_aStgFg[nCntStgFg].pos.x + sinf(s_aStgFg[nCntStgFg].fAngle + s_aStgFg[nCntStgFg].rot.x) * s_aStgFg[nCntStgFg].fLength;
		pVtx[3].pos.y = s_aStgFg[nCntStgFg].pos.y + cosf(s_aStgFg[nCntStgFg].fAngle + s_aStgFg[nCntStgFg].rot.y) * s_aStgFg[nCntStgFg].fLength;
		pVtx[3].pos.z = s_aStgFg[nCntStgFg].pos.z;

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
	s_pVtxBuffStgFg->Unlock();

	InitReferee();		// ���t�F���[
}

//=========================================
// �X�e�[�W�O�i�̏I������
//=========================================
void UninitStgFg(void)
{
	for (int nCntImgStgFg = 0; nCntImgStgFg < MAX_IMAGE_STGFG; nCntImgStgFg++)
	{
		//�e�N�X�`���̔j��
		if (s_pTextureStgFg[nCntImgStgFg] != NULL)
		{
			s_pTextureStgFg[nCntImgStgFg]->Release();
			s_pTextureStgFg[nCntImgStgFg] = NULL;
		}
		//���_�o�b�t�@�̔j��
		if (s_pVtxBuffStgFg != NULL)
		{
			s_pVtxBuffStgFg->Release();
			s_pVtxBuffStgFg = NULL;
		}
	}
	UninitReferee();		// ���t�F���[
}

//=========================================
// �X�e�[�W�O�i�̍X�V����
//=========================================
void UpdateStgFg(void)
{
	VERTEX_2D *pVtx;				//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffStgFg->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntStgFg = 0; nCntStgFg < MAX_STGFG_OBJECT; nCntStgFg++, pVtx += 4)
	{
		//���_���W�̐ݒ� = (�z�u�ʒu �} ����(�Ίp���̊p�x �} ����) * �Ίp���̒���)
		pVtx[0].pos.x = s_aStgFg[nCntStgFg].pos.x - sinf(s_aStgFg[nCntStgFg].fAngle + s_aStgFg[nCntStgFg].rot.x) * s_aStgFg[nCntStgFg].fLength;
		pVtx[0].pos.y = s_aStgFg[nCntStgFg].pos.y - cosf(s_aStgFg[nCntStgFg].fAngle + s_aStgFg[nCntStgFg].rot.y) * s_aStgFg[nCntStgFg].fLength;
		pVtx[0].pos.z = s_aStgFg[nCntStgFg].pos.z;

		pVtx[1].pos.x = s_aStgFg[nCntStgFg].pos.x + sinf(s_aStgFg[nCntStgFg].fAngle - s_aStgFg[nCntStgFg].rot.x) * s_aStgFg[nCntStgFg].fLength;
		pVtx[1].pos.y = s_aStgFg[nCntStgFg].pos.y - cosf(s_aStgFg[nCntStgFg].fAngle - s_aStgFg[nCntStgFg].rot.y) * s_aStgFg[nCntStgFg].fLength;
		pVtx[1].pos.z = s_aStgFg[nCntStgFg].pos.z;

		pVtx[2].pos.x = s_aStgFg[nCntStgFg].pos.x - sinf(s_aStgFg[nCntStgFg].fAngle - s_aStgFg[nCntStgFg].rot.x) * s_aStgFg[nCntStgFg].fLength;
		pVtx[2].pos.y = s_aStgFg[nCntStgFg].pos.y + cosf(s_aStgFg[nCntStgFg].fAngle - s_aStgFg[nCntStgFg].rot.y) * s_aStgFg[nCntStgFg].fLength;
		pVtx[2].pos.z = s_aStgFg[nCntStgFg].pos.z;

		pVtx[3].pos.x = s_aStgFg[nCntStgFg].pos.x + sinf(s_aStgFg[nCntStgFg].fAngle + s_aStgFg[nCntStgFg].rot.x) * s_aStgFg[nCntStgFg].fLength;
		pVtx[3].pos.y = s_aStgFg[nCntStgFg].pos.y + cosf(s_aStgFg[nCntStgFg].fAngle + s_aStgFg[nCntStgFg].rot.y) * s_aStgFg[nCntStgFg].fLength;
		pVtx[3].pos.z = s_aStgFg[nCntStgFg].pos.z;
	}
	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffStgFg->Unlock();

	UpdateReferee();		// ���t�F���[
}

//=========================================
// �X�e�[�W�O�i�̕`�揈��
//=========================================
void DrawStgFg()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^

												//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource
	(
		0,
		s_pVtxBuffStgFg,		//���_�o�b�t�@�ւ̃|�C���^
		0,
		sizeof(VERTEX_2D)		//���_���\���̂̃T�C�Y
	);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntStgFg = 0; nCntStgFg < MAX_STGFG_OBJECT; nCntStgFg++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, s_pTextureStgFg[s_aStgFg[nCntStgFg].type]);

		//�|���S���̕`��
		pDevice->DrawPrimitive
		(
			D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
			nCntStgFg * 4,			//�`�悷��ŏ��̒��_�C���f�b�N�X
			2						//�v���~�e�B�u�A�C�R���̌�
		);
	}

	DrawReferee();		// ���t�F���[
}