//=========================================
// 
// ���t�F���[����
// Author Tanimoto_Kosuke
//
// Update 22/03/14
// 
//=========================================
//------------------------------------
// include
//------------------------------------
#include "referee.h"
#include "main.h"
#include <stdio.h>
#include "disk.h"
#include "player.h"

//------------------------------------
// �X�^�e�B�b�N�ϐ�
//------------------------------------
static LPDIRECT3DTEXTURE9		s_pTextureRef = NULL;	//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffRef = NULL;	//���_�o�b�t�@�ւ̃|�C���^
static REFEREE s_aRef;									//���t�F���[�̏��
static bool s_bPause;	// �|�[�Y�����ǂ���

//=========================================
// ���t�F���[�̏���������
//=========================================
void InitRef(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^

												//�e�N�X�`���[�̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\referee\\field000.jpg",	//�e�N�X�`���̃t�@�C����
		&s_pTextureRef
	);

	//�S�[���̈ʒu
	s_aRef.pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - REFEREE_HEIGHT * 0.75, 0.0f);
	s_aRef.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_aRef.nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_aRef.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	s_aRef.fAngle = atan2f(REFEREE_WIDTH, REFEREE_HEIGHT);
	s_aRef.fLength = sqrtf((REFEREE_WIDTH * REFEREE_WIDTH) + (REFEREE_HEIGHT * REFEREE_HEIGHT)) / 2.0f;
	s_aRef.bUse = true;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffRef,
		NULL
	);

	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffRef->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ� = (�z�u�ʒu �} ����(�Ίp���̊p�x �} ����) * �Ίp���̒���)
	pVtx[0].pos.x = s_aRef.pos.x - sinf(s_aRef.fAngle + s_aRef.rot.x) * s_aRef.fLength;
	pVtx[0].pos.y = s_aRef.pos.y - cosf(s_aRef.fAngle + s_aRef.rot.y) * s_aRef.fLength;
	pVtx[0].pos.z = s_aRef.pos.z;

	pVtx[1].pos.x = s_aRef.pos.x + sinf(s_aRef.fAngle - s_aRef.rot.x) * s_aRef.fLength;
	pVtx[1].pos.y = s_aRef.pos.y - cosf(s_aRef.fAngle - s_aRef.rot.y) * s_aRef.fLength;
	pVtx[1].pos.z = s_aRef.pos.z;

	pVtx[2].pos.x = s_aRef.pos.x - sinf(s_aRef.fAngle - s_aRef.rot.x) * s_aRef.fLength;
	pVtx[2].pos.y = s_aRef.pos.y + cosf(s_aRef.fAngle - s_aRef.rot.y) * s_aRef.fLength;
	pVtx[2].pos.z = s_aRef.pos.z;

	pVtx[3].pos.x = s_aRef.pos.x + sinf(s_aRef.fAngle + s_aRef.rot.x) * s_aRef.fLength;
	pVtx[3].pos.y = s_aRef.pos.y + cosf(s_aRef.fAngle + s_aRef.rot.y) * s_aRef.fLength;
	pVtx[3].pos.z = s_aRef.pos.z;

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

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffRef->Unlock();
}

//=========================================
// ���t�F���[�̏I������
//=========================================
void UninitRef(void)
{
	//�e�N�X�`���̔j��
	if (s_pTextureRef != NULL)
	{
		s_pTextureRef->Release();
		s_pTextureRef = NULL;
	}
	//���_�o�b�t�@�̔j��
	if (s_pVtxBuffRef != NULL)
	{
		s_pVtxBuffRef->Release();
		s_pVtxBuffRef = NULL;
	}
}

//=========================================
// ���t�F���[�̍X�V����
//=========================================
void UpdateRef(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^

	VERTEX_2D *pVtx;				//���_���ւ̃|�C���^

	Player *pPlayer = GetPlayer();
	Disk *pDisk = GetDisk();

	if (pPlayer->bHaveDisk == false && pDisk->bUse == false)
	{
		//pos, move, acc, type, nPlayer, size
		//SetDisk(s_aRef.pos + D3DXVECTOR3(0.0f, -50.0f, 0.0f), D3DXVECTOR3(-10.0f, -1.0f, 0.0f) , D3DXVECTOR3(0.0f, 0.0f, 0.0f), DISK_TYPE_NORMAL, 0, 40.0f);
	}

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffRef->Lock(0, 0, (void**)&pVtx, 0);

	if (s_aRef.bUse == true)
	{
		//���_���W�̐ݒ� = (�z�u�ʒu �} ����(�Ίp���̊p�x �} ����) * �Ίp���̒���)
		pVtx[0].pos.x = s_aRef.pos.x - sinf(s_aRef.fAngle + s_aRef.rot.x) * s_aRef.fLength;
		pVtx[0].pos.y = s_aRef.pos.y - cosf(s_aRef.fAngle + s_aRef.rot.y) * s_aRef.fLength;
		pVtx[0].pos.z = s_aRef.pos.z;

		pVtx[1].pos.x = s_aRef.pos.x + sinf(s_aRef.fAngle - s_aRef.rot.x) * s_aRef.fLength;
		pVtx[1].pos.y = s_aRef.pos.y - cosf(s_aRef.fAngle - s_aRef.rot.y) * s_aRef.fLength;
		pVtx[1].pos.z = s_aRef.pos.z;

		pVtx[2].pos.x = s_aRef.pos.x - sinf(s_aRef.fAngle - s_aRef.rot.x) * s_aRef.fLength;
		pVtx[2].pos.y = s_aRef.pos.y + cosf(s_aRef.fAngle - s_aRef.rot.y) * s_aRef.fLength;
		pVtx[2].pos.z = s_aRef.pos.z;

		pVtx[3].pos.x = s_aRef.pos.x + sinf(s_aRef.fAngle + s_aRef.rot.x) * s_aRef.fLength;
		pVtx[3].pos.y = s_aRef.pos.y + cosf(s_aRef.fAngle + s_aRef.rot.y) * s_aRef.fLength;
		pVtx[3].pos.z = s_aRef.pos.z;
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffRef->Unlock();
}

//=========================================
// ���t�F���[�̕`�揈��
//=========================================
void DrawRef()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^

												//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource
	(
		0,
		s_pVtxBuffRef,		//���_�o�b�t�@�ւ̃|�C���^
		0,
		sizeof(VERTEX_2D)		//���_���\���̂̃T�C�Y
	);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (s_aRef.bUse == true)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, s_pTextureRef);

		//�|���S���̕`��
		pDevice->DrawPrimitive
		(
			D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
			0,						//�`�悷��ŏ��̒��_�C���f�b�N�X
			2						//�v���~�e�B�u�A�C�R���̌�
		);
	}
}

//============================================================================
//���t�F���[�̎擾����
//============================================================================
REFEREE *GetRef(void)
{
	return &s_aRef;	//�S�[�����̐擪�A�h���X��Ԃ�
}