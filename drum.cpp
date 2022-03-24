//=========================================
// 
// �h�����ʂ̏���
// Author Tanimoto_Kosuke
//
// Update 22/03/22
// 
//=========================================
//------------------------------------
// include
//------------------------------------
#include "drum.h"
#include <stdio.h>
#include "disk.h"
#include "stage.h"
#include "score.h"
#include "game.h"
#include "pop.h"
#include "goal.h"

//------------------------------------
// �X�^�e�B�b�N�ϐ�
//------------------------------------
static LPDIRECT3DTEXTURE9		s_pTextureDrum[MAX_IMAGE_DRUM] = {};	//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffDrum = NULL;	//���_�o�b�t�@�ւ̃|�C���^
static LPDIRECT3DTEXTURE9		s_pTexturePost = {};	//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffPost = NULL;	//���_�o�b�t�@�ւ̃|�C���^
static DRUM s_aDrum[MAX_DRUM];
static D3DXVECTOR3 s_posPost;

//=========================================
// �h�����ʂ̏���������
//=========================================
void InitDrum(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^

	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\goal\\Drum4.png",	//�e�N�X�`���̃t�@�C����
		&s_pTextureDrum[DRUM_TYPE_NORMAL]
	);

	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\goal\\Drum5.png",	//�e�N�X�`���̃t�@�C����
		&s_pTextureDrum[DRUM_TYPE_STRIKE]
	);

	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\post.png",	//�e�N�X�`���̃t�@�C����
		&s_pTexturePost
	);

	//�S�[���̈ʒu
	SetDrum(D3DXVECTOR3(GOAL_WIDTH / 2 + 25, 200.0f + (STAGE_HEIGHT_DOWN / 2), 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), false, DRUM_TYPE_NORMAL, 0);
	SetDrum(D3DXVECTOR3(GOAL_WIDTH / 2 + 25, SCREEN_HEIGHT / 2 + (STAGE_HEIGHT_DOWN / 2), 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), false, DRUM_TYPE_STRIKE, 1);
	SetDrum(D3DXVECTOR3(GOAL_WIDTH / 2 + 25, SCREEN_HEIGHT - 200.0f + (STAGE_HEIGHT_DOWN / 2), 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), false, DRUM_TYPE_NORMAL, 2);
	SetDrum(D3DXVECTOR3(SCREEN_WIDTH - GOAL_WIDTH / 2 - 25, 200.0f + (STAGE_HEIGHT_DOWN / 2), 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), true, DRUM_TYPE_NORMAL, 3);
	SetDrum(D3DXVECTOR3(SCREEN_WIDTH - GOAL_WIDTH / 2 - 25, SCREEN_HEIGHT / 2 + (STAGE_HEIGHT_DOWN / 2), 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), true, DRUM_TYPE_STRIKE, 4);
	SetDrum(D3DXVECTOR3(SCREEN_WIDTH - GOAL_WIDTH / 2 - 25, SCREEN_HEIGHT - 200.0f + (STAGE_HEIGHT_DOWN / 2), 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), true, DRUM_TYPE_NORMAL, 5);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_DRUM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffDrum,
		NULL
	);

	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffDrum->Lock(0, 0, (void**)&pVtx, 0);

	//�\���̂̏���������
	for (int nCntDrum = 0; nCntDrum < MAX_DRUM; nCntDrum++, pVtx += 4)
	{
		s_aDrum[nCntDrum].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		s_aDrum[nCntDrum].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		s_aDrum[nCntDrum].bUse = true;

		//���_���W�̐ݒ� = (�z�u�ʒu �} ����(�Ίp���̊p�x �} ����) * �Ίp���̒���)
		pVtx[0].pos.x = s_aDrum[nCntDrum].pos.x - sinf(s_aDrum[nCntDrum].fAngle + s_aDrum[nCntDrum].rot.x) * s_aDrum[nCntDrum].fLength;
		pVtx[0].pos.y = s_aDrum[nCntDrum].pos.y - cosf(s_aDrum[nCntDrum].fAngle + s_aDrum[nCntDrum].rot.y) * s_aDrum[nCntDrum].fLength;
		pVtx[0].pos.z = s_aDrum[nCntDrum].pos.z;

		pVtx[1].pos.x = s_aDrum[nCntDrum].pos.x + sinf(s_aDrum[nCntDrum].fAngle - s_aDrum[nCntDrum].rot.x) * s_aDrum[nCntDrum].fLength;
		pVtx[1].pos.y = s_aDrum[nCntDrum].pos.y - cosf(s_aDrum[nCntDrum].fAngle - s_aDrum[nCntDrum].rot.y) * s_aDrum[nCntDrum].fLength;
		pVtx[1].pos.z = s_aDrum[nCntDrum].pos.z;

		pVtx[2].pos.x = s_aDrum[nCntDrum].pos.x - sinf(s_aDrum[nCntDrum].fAngle - s_aDrum[nCntDrum].rot.x) * s_aDrum[nCntDrum].fLength;
		pVtx[2].pos.y = s_aDrum[nCntDrum].pos.y + cosf(s_aDrum[nCntDrum].fAngle - s_aDrum[nCntDrum].rot.y) * s_aDrum[nCntDrum].fLength;
		pVtx[2].pos.z = s_aDrum[nCntDrum].pos.z;

		pVtx[3].pos.x = s_aDrum[nCntDrum].pos.x + sinf(s_aDrum[nCntDrum].fAngle + s_aDrum[nCntDrum].rot.x) * s_aDrum[nCntDrum].fLength;
		pVtx[3].pos.y = s_aDrum[nCntDrum].pos.y + cosf(s_aDrum[nCntDrum].fAngle + s_aDrum[nCntDrum].rot.y) * s_aDrum[nCntDrum].fLength;
		pVtx[3].pos.z = s_aDrum[nCntDrum].pos.z;

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
	s_pVtxBuffDrum->Unlock();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_POST,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffPost,
		NULL
	);

	s_posPost = D3DXVECTOR3(GOAL_WIDTH * 0.5f + 50.0f,SCREEN_HEIGHT * 0.5f - 20.0f,0.0f);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffPost->Lock(0, 0, (void**)&pVtx, 0);

	//�\���̂̏���������
	for (int nCntDrum = 0; nCntDrum < MAX_POST; nCntDrum++, pVtx += 4)
	{
		pVtx[0].pos.x = s_posPost.x + -GOAL_WIDTH;
		pVtx[0].pos.y = s_posPost.y + -GOAL_HEIGHT * 2.0f;
		pVtx[0].pos.z = s_posPost.z + 0.0f;

		pVtx[1].pos.x = s_posPost.x + GOAL_WIDTH * 0.75f;
		pVtx[1].pos.y = s_posPost.y + -GOAL_HEIGHT * 2.0f;
		pVtx[1].pos.z = s_posPost.z + 0.0f;

		pVtx[2].pos.x = s_posPost.x + -GOAL_WIDTH;
		pVtx[2].pos.y = s_posPost.y + GOAL_HEIGHT * 2.0f;
		pVtx[2].pos.z = s_posPost.z + 0.0f;

		pVtx[3].pos.x = s_posPost.x + GOAL_WIDTH * 0.75f;
		pVtx[3].pos.y = s_posPost.y + GOAL_HEIGHT * 2.0f;
		pVtx[3].pos.z = s_posPost.z + 0.0f;

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
		pVtx[0].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.0f, 1.0f);
	}
	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffPost->Unlock();
}

//=========================================
// �h�����ʂ̏I������
//=========================================
void UninitDrum(void)
{
	for (int nCntImgDrum = 0; nCntImgDrum < MAX_IMAGE_DRUM; nCntImgDrum++)
	{
		//�e�N�X�`���̔j��
		if (s_pTextureDrum[nCntImgDrum] != NULL)
		{
			s_pTextureDrum[nCntImgDrum]->Release();
			s_pTextureDrum[nCntImgDrum] = NULL;
		}
		//���_�o�b�t�@�̔j��
		if (s_pVtxBuffDrum != NULL)
		{
			s_pVtxBuffDrum->Release();
			s_pVtxBuffDrum = NULL;
		}
	}

	//�e�N�X�`���̔j��
	if (s_pTexturePost != NULL)
	{
		s_pTexturePost->Release();
		s_pTexturePost = NULL;
	}
	//���_�o�b�t�@�̔j��
	if (s_pVtxBuffPost != NULL)
	{
		s_pVtxBuffPost->Release();
		s_pVtxBuffPost = NULL;
	}
}

//=========================================
// �h�����ʂ̍X�V����
//=========================================
void UpdateDrum(void)
{
	VERTEX_2D *pVtx;				//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffDrum->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntDrum = 0; nCntDrum < MAX_DRUM; nCntDrum++, pVtx += 4)
	{
		if (s_aDrum[nCntDrum].bUse == true)
		{
			//���_���W�̐ݒ� = (�z�u�ʒu �} ����(�Ίp���̊p�x �} ����) * �Ίp���̒���)
			pVtx[0].pos.x = s_aDrum[nCntDrum].pos.x - sinf(s_aDrum[nCntDrum].fAngle + s_aDrum[nCntDrum].rot.x) * s_aDrum[nCntDrum].fLength;
			pVtx[0].pos.y = s_aDrum[nCntDrum].pos.y - cosf(s_aDrum[nCntDrum].fAngle + s_aDrum[nCntDrum].rot.y) * s_aDrum[nCntDrum].fLength;
			pVtx[0].pos.z = s_aDrum[nCntDrum].pos.z;

			pVtx[1].pos.x = s_aDrum[nCntDrum].pos.x + sinf(s_aDrum[nCntDrum].fAngle - s_aDrum[nCntDrum].rot.x) * s_aDrum[nCntDrum].fLength;
			pVtx[1].pos.y = s_aDrum[nCntDrum].pos.y - cosf(s_aDrum[nCntDrum].fAngle - s_aDrum[nCntDrum].rot.y) * s_aDrum[nCntDrum].fLength;
			pVtx[1].pos.z = s_aDrum[nCntDrum].pos.z;

			pVtx[2].pos.x = s_aDrum[nCntDrum].pos.x - sinf(s_aDrum[nCntDrum].fAngle - s_aDrum[nCntDrum].rot.x) * s_aDrum[nCntDrum].fLength;
			pVtx[2].pos.y = s_aDrum[nCntDrum].pos.y + cosf(s_aDrum[nCntDrum].fAngle - s_aDrum[nCntDrum].rot.y) * s_aDrum[nCntDrum].fLength;
			pVtx[2].pos.z = s_aDrum[nCntDrum].pos.z;

			pVtx[3].pos.x = s_aDrum[nCntDrum].pos.x + sinf(s_aDrum[nCntDrum].fAngle + s_aDrum[nCntDrum].rot.x) * s_aDrum[nCntDrum].fLength;
			pVtx[3].pos.y = s_aDrum[nCntDrum].pos.y + cosf(s_aDrum[nCntDrum].fAngle + s_aDrum[nCntDrum].rot.y) * s_aDrum[nCntDrum].fLength;
			pVtx[3].pos.z = s_aDrum[nCntDrum].pos.z;

			//�e�N�X�`�����W�̐ݒ�(0.0f ~ (1 / x�p�^�[����)f)
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			//�S�[���T�C�h�Ō�����ς��鏈��
			if (s_aDrum[nCntDrum].bSide == 0)
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
	s_pVtxBuffDrum->Unlock();

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffPost->Lock(0, 0, (void**)&pVtx, 0);

	//�\���̂̏���������
	for (int nCntDrum = 0; nCntDrum < MAX_POST; nCntDrum++, pVtx += 4)
	{
		//���_���W�̐ݒ� = (�z�u�ʒu �} ����(�Ίp���̊p�x �} ����) * �Ίp���̒���)
		pVtx[0].pos.x = s_posPost.x + -GOAL_WIDTH;
		pVtx[0].pos.y = s_posPost.y + -GOAL_HEIGHT * 2.0f;
		pVtx[0].pos.z = s_posPost.z + 0.0f;

		pVtx[1].pos.x = s_posPost.x + GOAL_WIDTH * 0.75f;
		pVtx[1].pos.y = s_posPost.y + -GOAL_HEIGHT * 2.0f;
		pVtx[1].pos.z = s_posPost.z + 0.0f;

		pVtx[2].pos.x = s_posPost.x + -GOAL_WIDTH;
		pVtx[2].pos.y = s_posPost.y + GOAL_HEIGHT * 2.0f;
		pVtx[2].pos.z = s_posPost.z + 0.0f;

		pVtx[3].pos.x = s_posPost.x + GOAL_WIDTH * 0.75f;
		pVtx[3].pos.y = s_posPost.y + GOAL_HEIGHT * 2.0f;
		pVtx[3].pos.z = s_posPost.z + 0.0f;
	}
	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffPost->Unlock();

}

//=========================================
// �h�����ʂ̕`�揈��
//=========================================
void DrawDrum()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffPost, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTexturePost);

	//�|���S���̕`��
	pDevice->DrawPrimitive
	(
		D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
		0,			//�`�悷��ŏ��̒��_�C���f�b�N�X
		2						//�v���~�e�B�u�A�C�R���̌�
	);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource
	(
		0,
		s_pVtxBuffDrum,		//���_�o�b�t�@�ւ̃|�C���^
		0,
		sizeof(VERTEX_2D)		//���_���\���̂̃T�C�Y
	);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntDrum = 0; nCntDrum < MAX_DRUM; nCntDrum++)
	{
		if (s_aDrum[nCntDrum].bUse == true)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, s_pTextureDrum[s_aDrum[nCntDrum].type]);

			//�|���S���̕`��
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
				nCntDrum * 4,			//�`�悷��ŏ��̒��_�C���f�b�N�X
				2						//�v���~�e�B�u�A�C�R���̌�
			);
		}
	}

}

//=========================================
// �h�����ʂ̐ݒ菈��
//=========================================
void SetDrum(D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool side, DRUM_TYPE type, int nIdxGoal)
{
	s_aDrum[nIdxGoal].pos = pos;
	s_aDrum[nIdxGoal].rot = rot;
	s_aDrum[nIdxGoal].bSide = side;
	s_aDrum[nIdxGoal].type = type;
	s_aDrum[nIdxGoal].bUse = true;

	if (s_aDrum[nIdxGoal].type == DRUM_TYPE_NORMAL)
	{
		s_aDrum[nIdxGoal].fAngle = atan2f(DRUM_WIDTH, DRUM_HEIGHT);
		s_aDrum[nIdxGoal].fLength = sqrtf((DRUM_WIDTH * DRUM_WIDTH) + (DRUM_HEIGHT * DRUM_HEIGHT)) / 2.0f;
	}
	else if (s_aDrum[nIdxGoal].type == DRUM_TYPE_STRIKE)
	{
		s_aDrum[nIdxGoal].fAngle = atan2f(DRUM_WIDTH, DRUM_HEIGHT);
		s_aDrum[nIdxGoal].fLength = sqrtf((DRUM_WIDTH * DRUM_WIDTH) + (DRUM_HEIGHT * DRUM_HEIGHT)) / 2.0f;
	}
}

//============================================================================
//�h�����ʂ̎擾����
//============================================================================
DRUM *GetDrum(void)
{
	return s_aDrum;	//�h�����ʏ��̐擪�A�h���X��Ԃ�
}