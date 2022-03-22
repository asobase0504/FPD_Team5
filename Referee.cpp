//=========================================
// 
// ���t�F���[����
// Author Tanimoto_Kosuke
// Author Yuda Kaito
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
// �}�N����`
//------------------------------------
#define REFEREE_WIDTH		(100)	// ���t�F���[�̉��T�C�Y
#define REFEREE_HEIGHT		(100)	// ���t�F���[�̏c�T�C�Y
#define REFEREE_TEXTURE		"data\\TEXTURE\\referee\\field000.jpg"	// �e�N�X�`���t�@�C��
#define THROW_POWER			(10.0f)

//------------------------------------
// �X�^�e�B�b�N�ϐ�
//------------------------------------
static LPDIRECT3DTEXTURE9		s_pTexture = NULL;	// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuff = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
static REFEREE s_aRefree;							// ���t�F���[�̏��

//------------------------------------
// �v���g�^�C�v�錾
//------------------------------------
static void ThrowRefree(int nIdxPlayer);

//=========================================
// ������
//=========================================
void InitReferee(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^

	// �e�N�X�`���[�̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\referee\\field000.jpg",&s_pTexture);

	// �S�[���̈ʒu
	s_aRefree.pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - REFEREE_HEIGHT * 0.75f, 0.0f);
	s_aRefree.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_aRefree.nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_aRefree.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	s_aRefree.fAngle = atan2f(REFEREE_WIDTH, REFEREE_HEIGHT);
	s_aRefree.fLength = sqrtf((REFEREE_WIDTH * REFEREE_WIDTH) + (REFEREE_HEIGHT * REFEREE_HEIGHT)) / 2.0f;
	s_aRefree.bUse = true;

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	// ���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ� = (�z�u�ʒu �} ����(�Ίp���̊p�x �} ����) * �Ίp���̒���)
	pVtx[0].pos.x = s_aRefree.pos.x - sinf(s_aRefree.fAngle + s_aRefree.rot.x) * s_aRefree.fLength;
	pVtx[0].pos.y = s_aRefree.pos.y - cosf(s_aRefree.fAngle + s_aRefree.rot.y) * s_aRefree.fLength;
	pVtx[0].pos.z = s_aRefree.pos.z;

	pVtx[1].pos.x = s_aRefree.pos.x + sinf(s_aRefree.fAngle - s_aRefree.rot.x) * s_aRefree.fLength;
	pVtx[1].pos.y = s_aRefree.pos.y - cosf(s_aRefree.fAngle - s_aRefree.rot.y) * s_aRefree.fLength;
	pVtx[1].pos.z = s_aRefree.pos.z;

	pVtx[2].pos.x = s_aRefree.pos.x - sinf(s_aRefree.fAngle - s_aRefree.rot.x) * s_aRefree.fLength;
	pVtx[2].pos.y = s_aRefree.pos.y + cosf(s_aRefree.fAngle - s_aRefree.rot.y) * s_aRefree.fLength;
	pVtx[2].pos.z = s_aRefree.pos.z;

	pVtx[3].pos.x = s_aRefree.pos.x + sinf(s_aRefree.fAngle + s_aRefree.rot.x) * s_aRefree.fLength;
	pVtx[3].pos.y = s_aRefree.pos.y + cosf(s_aRefree.fAngle + s_aRefree.rot.y) * s_aRefree.fLength;
	pVtx[3].pos.z = s_aRefree.pos.z;

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//=========================================
// �I��
//=========================================
void UninitReferee(void)
{
	// �e�N�X�`���̔j��
	if (s_pTexture != NULL)
	{
		s_pTexture->Release();
		s_pTexture = NULL;
	}
	// ���_�o�b�t�@�̔j��
	if (s_pVtxBuff != NULL)
	{
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//=========================================
// �X�V
//=========================================
void UpdateReferee(void)
{
	if (s_aRefree.bUse)
	{
		ThrowRefree(0);	// �����̃v���C���[�����݂ɕύX

		VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ� = (�z�u�ʒu �} ����(�Ίp���̊p�x �} ����) * �Ίp���̒���)
		pVtx[0].pos.x = s_aRefree.pos.x - sinf(s_aRefree.fAngle + s_aRefree.rot.x) * s_aRefree.fLength;
		pVtx[0].pos.y = s_aRefree.pos.y - cosf(s_aRefree.fAngle + s_aRefree.rot.y) * s_aRefree.fLength;
		pVtx[0].pos.z = s_aRefree.pos.z;

		pVtx[1].pos.x = s_aRefree.pos.x + sinf(s_aRefree.fAngle - s_aRefree.rot.x) * s_aRefree.fLength;
		pVtx[1].pos.y = s_aRefree.pos.y - cosf(s_aRefree.fAngle - s_aRefree.rot.y) * s_aRefree.fLength;
		pVtx[1].pos.z = s_aRefree.pos.z;

		pVtx[2].pos.x = s_aRefree.pos.x - sinf(s_aRefree.fAngle - s_aRefree.rot.x) * s_aRefree.fLength;
		pVtx[2].pos.y = s_aRefree.pos.y + cosf(s_aRefree.fAngle - s_aRefree.rot.y) * s_aRefree.fLength;
		pVtx[2].pos.z = s_aRefree.pos.z;

		pVtx[3].pos.x = s_aRefree.pos.x + sinf(s_aRefree.fAngle + s_aRefree.rot.x) * s_aRefree.fLength;
		pVtx[3].pos.y = s_aRefree.pos.y + cosf(s_aRefree.fAngle + s_aRefree.rot.y) * s_aRefree.fLength;
		pVtx[3].pos.z = s_aRefree.pos.z;

		// ���_�o�b�t�@���A�����b�N����
		s_pVtxBuff->Unlock();
	}
}

//=========================================
// ��������
//=========================================
void ThrowRefree(int nIdxPlayer)
{
	// ������ꍇ
	if (s_aRefree.bThrow)
	{
		s_aRefree.bThrow = false;

		D3DXVECTOR3 posThrow = s_aRefree.pos;
		posThrow.y -= 80.0f;

		D3DXVECTOR3 move = GetPlayer()[nIdxPlayer].pos - posThrow;

		D3DXVec3Normalize(&move, &move);

		// �f�B�X�N����
		SetDisk(posThrow, move * THROW_POWER, D3DXVECTOR3(0.0f, 0.0f, 0.0f), DISK_TYPE_NORMAL, -1, 60.0f);
	}
}


//=========================================
// �`��
//=========================================
void DrawReferee()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (s_aRefree.bUse)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, s_pTexture);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//============================================================================
// �擾
//============================================================================
REFEREE* GetReferee(void)
{
	return &s_aRefree;	// �S�[�����̐擪�A�h���X��Ԃ�
}
