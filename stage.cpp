//=========================================
// 
// �X�e�[�W����
// Author Tanimoto_Kosuke
//
// Update 22/03/07
// 
//=========================================
//------------------------------------
// include
//------------------------------------
#include "stage.h"
#include "main.h"
#include <stdio.h>

//------------------------------------
// �X�^�e�B�b�N�ϐ�
//------------------------------------
static LPDIRECT3DTEXTURE9		s_pTextureStage = NULL;	//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffStage = NULL;	//���_�o�b�t�@�ւ̃|�C���^
static STAGE s_aStage;									//�X�e�[�W�̏��
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
		"data\\TEXTURE\\block004.jpg",	//�e�N�X�`���̃t�@�C����
		&s_pTextureStage
	);

	//�\���̂̏���������
	s_aStage.pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
	s_aStage.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			
	s_aStage.nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_aStage.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	s_aStage.fAngle = atan2f(STAGE_WIDTH, STAGE_HEIGHT);
	s_aStage.fLength = sqrtf((STAGE_WIDTH * STAGE_WIDTH) + (STAGE_HEIGHT * STAGE_HEIGHT)) / 2.0f;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4,
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

	//���_���W�̐ݒ� = (�z�u�ʒu �} ����(�Ίp���̊p�x �} ����) * �Ίp���̒���)
	pVtx[0].pos.x = s_aStage.pos.x - sinf(s_aStage.fAngle + s_aStage.rot.x) * s_aStage.fLength;
	pVtx[0].pos.y = s_aStage.pos.y - cosf(s_aStage.fAngle + s_aStage.rot.y) * s_aStage.fLength;
	pVtx[0].pos.z = s_aStage.pos.z;

	pVtx[1].pos.x = s_aStage.pos.x + sinf(s_aStage.fAngle - s_aStage.rot.x) * s_aStage.fLength;
	pVtx[1].pos.y = s_aStage.pos.y - cosf(s_aStage.fAngle - s_aStage.rot.y) * s_aStage.fLength;
	pVtx[1].pos.z = s_aStage.pos.z;

	pVtx[2].pos.x = s_aStage.pos.x - sinf(s_aStage.fAngle - s_aStage.rot.x) * s_aStage.fLength;
	pVtx[2].pos.y = s_aStage.pos.y + cosf(s_aStage.fAngle - s_aStage.rot.y) * s_aStage.fLength;
	pVtx[2].pos.z = s_aStage.pos.z;

	pVtx[3].pos.x = s_aStage.pos.x + sinf(s_aStage.fAngle + s_aStage.rot.x) * s_aStage.fLength;
	pVtx[3].pos.y = s_aStage.pos.y + cosf(s_aStage.fAngle + s_aStage.rot.y) * s_aStage.fLength;
	pVtx[3].pos.z = s_aStage.pos.z;

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
	s_pVtxBuffStage->Unlock();
}

//=========================================
// �X�e�[�W�̏I������
//=========================================
void UninitStage(void)
{
	//�e�N�X�`���̔j��
	if (s_pTextureStage != NULL)
	{
		s_pTextureStage->Release();
		s_pTextureStage = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (s_pVtxBuffStage != NULL)
	{
		s_pVtxBuffStage->Release();
		s_pVtxBuffStage = NULL;
	}
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

	//���_���W�̐ݒ� = (�z�u�ʒu �} ����(�Ίp���̊p�x �} ����) * �Ίp���̒���)
	pVtx[0].pos.x = s_aStage.pos.x - sinf(s_aStage.fAngle + s_aStage.rot.x) * s_aStage.fLength;
	pVtx[0].pos.y = s_aStage.pos.y - cosf(s_aStage.fAngle + s_aStage.rot.y) * s_aStage.fLength;
	pVtx[0].pos.z = s_aStage.pos.z;

	pVtx[1].pos.x = s_aStage.pos.x + sinf(s_aStage.fAngle - s_aStage.rot.x) * s_aStage.fLength;
	pVtx[1].pos.y = s_aStage.pos.y - cosf(s_aStage.fAngle - s_aStage.rot.y) * s_aStage.fLength;
	pVtx[1].pos.z = s_aStage.pos.z;

	pVtx[2].pos.x = s_aStage.pos.x - sinf(s_aStage.fAngle - s_aStage.rot.x) * s_aStage.fLength;
	pVtx[2].pos.y = s_aStage.pos.y + cosf(s_aStage.fAngle - s_aStage.rot.y) * s_aStage.fLength;
	pVtx[2].pos.z = s_aStage.pos.z;

	pVtx[3].pos.x = s_aStage.pos.x + sinf(s_aStage.fAngle + s_aStage.rot.x) * s_aStage.fLength;
	pVtx[3].pos.y = s_aStage.pos.y + cosf(s_aStage.fAngle + s_aStage.rot.y) * s_aStage.fLength;
	pVtx[3].pos.z = s_aStage.pos.z;

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffStage->Unlock();
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

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTextureStage);

	//�|���S���̕`��
	pDevice->DrawPrimitive
	(
		D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
		0,						//�`�悷��ŏ��̒��_�C���f�b�N�X
		2						//�v���~�e�B�u�A�C�R���̌�
	);
}

////=========================================
//// �X�e�[�W�̓����蔻�菈��
////=========================================
//STAGE_COLLISION ColisionStage(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pNor, float fWidth, float fHeight)
//{
//	VERTEX_2D *pVtx;			//���_���ւ̃|�C���^
//	STAGE_COLLISION stageCollision = STAGE_COLLISION_NONE;
//
//	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
//	s_pVtxBuffStage->Lock(0, 0, (void**)&pVtx, 0);
//
//	//�u���b�N�㑤�̏���
//	if (pPosOld->y <= -STAGE_HEIGHT
//		&& pPos->y >= -STAGE_HEIGHT
//		&& -STAGE_WIDTH< pPos->x + fWidth / 2
//		&& STAGE_WIDTH> pPos->x - fWidth / 2
//		)
//	{//�v���C���[�O��ʒu�̑����A�u���b�N�̓���荂���ʒu�ɂ���ꍇ(P�O���0�u.y <= B���݈ʒu.y)
//	 //�v���C���[���݈ʒu�̑����u���b�N���ɂ߂肱�񂾏ꍇ
//		stageCollision = STAGE_COLLISION_TOP;
//	}
//	//�u���b�N�����̏���
//	else if (pPosOld->y - fHeight >= STAGE_HEIGHT
//		&& pPos->y - fHeight <= STAGE_HEIGHT
//		&& -STAGE_WIDTH < pPos->x + fWidth / 2
//		&& STAGE_WIDTH > pPos->x - fWidth / 2
//		)
//	{//�v���C���[�O��ʒu�̓����A�u���b�N���݈ʒu�̉�����ʒu�̏ꍇ(P�O��ʒu.y >= B���݈ʒu.y)
//	 //�v���C���[�̓����A�u���b�N���ɂ߂肱�񂾏ꍇ
//		stageCollision = STAGE_COLLISION_BOTTOM;
//	}
//	//�u���b�N�����̏���
//	else if (pPosOld->x + fWidth / 2 <= -STAGE_WIDTH
//		&& pPos->x + fWidth / 2 >= -STAGE_WIDTH
//		&& pPos->y > -STAGE_HEIGHT
//		&& pPos->y - fHeight < STAGE_HEIGHT
//		)
//	{//�v���C���[�O��ʒu�E�����A�u���b�N���݈ʒu��荶�ɂ���ꍇ(P�O��ʒu.x <= B���݈ʒu.x)
//	 //�v���C���[���݈ʒu�E�����A�u���b�N���ɂ߂肱�񂾏ꍇ
//		stageCollision = STAGE_COLLISION_LEFT;
//	}
//	//�u���b�N�E���̏���
//	else if (pPosOld->x - fWidth / 2 >= STAGE_WIDTH
//		&& pPos->x - fWidth / 2 <= STAGE_WIDTH
//		&& pPos->y > -STAGE_HEIGHT
//		&& pPos->y - fHeight < STAGE_HEIGHT
//		)
//	{//�v���C���[�O��ʒu�̍������A�u���b�N�̉E����荶�ɂ���ꍇ(P�O��ʒu.x >= B���݈ʒu.x)
//	 //�v���C���[�������A�u���b�N���ɂ߂肱�񂾏ꍇ
//		stageCollision = STAGE_COLLISION_RIGHT;
//	}
//
//	//���_�o�b�t�@���A�����b�N����
//	s_pVtxBuffStage->Unlock();
//	return stageCollision;
//}