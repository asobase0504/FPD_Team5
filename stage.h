//=========================================
// 
// �X�e�[�W(�w�b�_�[�t�@�C��)
// Author Tanimoto_Kosuke
//
// Update 22/03/09
// 
//=========================================
#ifndef _STAGE_H_
#define _STAGE_H_

#include "main.h"

//****************************************************************************
//�}�N����`
//****************************************************************************
#define STAGE_WIDTH			(SCREEN_WIDTH)	//�X�e�[�W�̉��T�C�Y
#define STAGE_HEIGHT		(SCREEN_HEIGHT - 200)	//�X�e�[�W�̏c�T�C�Y
#define MAX_STAGE			(2)						//�X�e�[�W�̍ő吔
#define MAX_IMAGE_STAGE		(2)						//�X�e�[�W�摜�̍ő吔

//****************************************************************************
//�X�e�[�W�̎��
//****************************************************************************
typedef enum
{
	STAGE_TYPE_FRONT = 0,	//�X�e�[�W�O��
	STAGE_TYPE_BACK,		//�X�e�[�W��둤
	STAGE_TYPE_MAX
}STAGE_TYPE;

//****************************************************************************
//�\���̂̒�`
//****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 rot;	//��]
	D3DXVECTOR3 nor;	//�@��
	D3DXCOLOR	col;	//�F
	STAGE_TYPE type;	//���
	float fAngle;		//�p�x(�A�[�N�^���W�F���g)
	float fLength;		//����(�X�N�E�F�A���[�g)
}STAGE;

//****************************************************************************
//�v���g�^�C�v�錾
//****************************************************************************
void InitStage(void);
void UninitStage(void);
void UpdateStage(void);
void DrawStage(void);

#endif