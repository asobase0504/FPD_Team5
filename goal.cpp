//=========================================
// 
// �S�[������
// Author Tanimoto_Kosuke
//
// Update 22/03/07
// 
//=========================================
//------------------------------------
// include
//------------------------------------
#include "goal.h"
#include "main.h"
#include <stdio.h>

//------------------------------------
// �X�^�e�B�b�N�ϐ�
//------------------------------------
static LPDIRECT3DTEXTURE9		s_pTextureGoal[MAX_IMAGE_GOAL] = {};	//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffGoal = NULL;	//���_�o�b�t�@�ւ̃|�C���^
static GOAL s_aGoal[MAX_GOAL];							//�S�[���̏��
static bool s_bPause;	// �|�[�Y�����ǂ���

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
		"data\\TEXTURE\\block000.jpg",	//�e�N�X�`���̃t�@�C����
		&s_pTextureGoal[GOAL_TYPE_NORMAL]
	);

	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\block001.jpg",	//�e�N�X�`���̃t�@�C����
		&s_pTextureGoal[GOAL_TYPE_STRIKE]
	);

	s_aGoal[0].pos = D3DXVECTOR3(125.0f, 215.0f, 0.0f);
	s_aGoal[0].rot = D3DXVECTOR3(D3DX_PI / 2, D3DX_PI / 2, 0.0f);
	s_aGoal[0].fAngle = atan2f(GOAL_WIDTH, GOAL_HEIGHT);
	s_aGoal[0].fLength = sqrtf((GOAL_WIDTH * GOAL_WIDTH) + (GOAL_HEIGHT * GOAL_HEIGHT)) / 2.0f;
	s_aGoal[0].type = GOAL_TYPE_NORMAL;

	s_aGoal[1].pos = D3DXVECTOR3(125.0f, SCREEN_HEIGHT / 2, 0.0f);
	s_aGoal[1].rot = D3DXVECTOR3(D3DX_PI / 2, D3DX_PI / 2, 0.0f);
	s_aGoal[1].fAngle = atan2f(STRIKE_GOAL_WIDTH, GOAL_HEIGHT);
	s_aGoal[1].fLength = sqrtf((STRIKE_GOAL_WIDTH * STRIKE_GOAL_WIDTH) + (GOAL_HEIGHT * GOAL_HEIGHT)) / 2.0f;
	s_aGoal[1].type = GOAL_TYPE_STRIKE;

	s_aGoal[2].pos = D3DXVECTOR3(125.0f, SCREEN_HEIGHT - 215.0f, 0.0f);
	s_aGoal[2].rot = D3DXVECTOR3(D3DX_PI / 2, D3DX_PI / 2, 0.0f);
	s_aGoal[2].fAngle = atan2f(GOAL_WIDTH, GOAL_HEIGHT);
	s_aGoal[2].fLength = sqrtf((GOAL_WIDTH * GOAL_WIDTH) + (GOAL_HEIGHT * GOAL_HEIGHT)) / 2.0f;
	s_aGoal[2].type = GOAL_TYPE_NORMAL;

	s_aGoal[3].pos = D3DXVECTOR3(SCREEN_WIDTH - 125.0f, 215.0f, 0.0f);
	s_aGoal[3].rot = D3DXVECTOR3(-D3DX_PI / 2, -D3DX_PI / 2, 0.0f);
	s_aGoal[3].fAngle = atan2f(GOAL_WIDTH, GOAL_HEIGHT);
	s_aGoal[3].fLength = sqrtf((GOAL_WIDTH * GOAL_WIDTH) + (GOAL_HEIGHT * GOAL_HEIGHT)) / 2.0f;
	s_aGoal[3].type = GOAL_TYPE_NORMAL;

	s_aGoal[4].pos = D3DXVECTOR3(SCREEN_WIDTH - 125.0f, SCREEN_HEIGHT / 2, 0.0f);
	s_aGoal[4].rot = D3DXVECTOR3(-D3DX_PI / 2, -D3DX_PI / 2, 0.0f);
	s_aGoal[4].fAngle = atan2f(STRIKE_GOAL_WIDTH, GOAL_HEIGHT);
	s_aGoal[4].fLength = sqrtf((STRIKE_GOAL_WIDTH * STRIKE_GOAL_WIDTH) + (GOAL_HEIGHT * GOAL_HEIGHT)) / 2.0f;
	s_aGoal[4].type = GOAL_TYPE_STRIKE;

	s_aGoal[5].pos = D3DXVECTOR3(SCREEN_WIDTH - 125.0f, SCREEN_HEIGHT - 215.0f, 0.0f);
	s_aGoal[5].rot = D3DXVECTOR3(-D3DX_PI / 2, -D3DX_PI / 2, 0.0f);
	s_aGoal[5].fAngle = atan2f(GOAL_WIDTH, GOAL_HEIGHT);
	s_aGoal[5].fLength = sqrtf((GOAL_WIDTH * GOAL_WIDTH) + (GOAL_HEIGHT * GOAL_HEIGHT)) / 2.0f;
	s_aGoal[5].type = GOAL_TYPE_NORMAL;

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
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffGoal->Unlock();
}

//=========================================
// �S�[���̏I������
//=========================================
void UninitGoal(void)
{
	for (int nCntImgGoal = 0; nCntImgGoal < MAX_IMAGE_GOAL; nCntImgGoal++)
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
// �S�[���̏���������
//=========================================
void UpdateGoal(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^

	VERTEX_2D *pVtx;				//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffGoal->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntGoal = 0; nCntGoal < MAX_GOAL; nCntGoal++, pVtx += 4)
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
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffGoal->Unlock();
}

//=========================================
// �S�[���̏���������
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