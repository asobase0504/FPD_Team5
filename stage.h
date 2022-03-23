//=========================================
// 
// �X�e�[�W(�w�b�_�[�t�@�C��)
// Author Tanimoto_Kosuke
//
// Update 22/03/23
// 
//=========================================
#ifndef _STAGE_H_
#define _STAGE_H_

#include "main.h"

//****************************************************************************
//�}�N����`
//****************************************************************************
#define STAGE_HEIGHT_DOWN		(50)								//�X�e�[�W�c�̃Y��(����)
#define STAGE_WIDTH				(SCREEN_WIDTH + 50)					//�X�e�[�W�̉��T�C�Y
#define STAGE_HEIGHT			(SCREEN_HEIGHT - 250)				//�X�e�[�W�̏c�T�C�Y
#define STAGE_NET_WIDTH			(10)								//�Ԃ̉��T�C�Y
#define STAGE_NET_HEIGHT		(SCREEN_HEIGHT - 250)				//�Ԃ̏c�T�C�Y
#define STAGE_WALL_WIDTH		(SCREEN_WIDTH - 20)					//�ǂ̉��T�C�Y
#define STAGE_WALL_HEIGHT		(50)								//�ǂ̏c�T�C�Y
#define MAX_STAGE_OBJECT		(4)									//�X�e�[�W�I�u�W�F�N�g�̍ő吔
#define MAX_IMAGE_STAGE			(4)									//�X�e�[�W�摜�̍ő吔
#define DISK_DELETE				(30)								//�������f�B�X�N�������鎞��

#define MIN_WIDTH				(80)								//���ŏ��l
#define MIN_HEIGHT				(150)								//�c�ŏ��l
#define MAX_WIDTH				(SCREEN_WIDTH - MIN_WIDTH)			//���ő�l
#define MAX_HEIGHT				(SCREEN_HEIGHT - MIN_HEIGHT + 50)	//�c�ő�l

//****************************************************************************
//�X�e�[�W�̎��
//****************************************************************************
typedef enum
{
	STAGE_TYPE_FRONT = 0,	//�X�e�[�W�O��
	STAGE_TYPE_BACK,		//�X�e�[�W��둤
	STAGE_TYPE_NET,			//��
	STAGE_TYPE_WALL,		//��(�f�U�C��)
	STAGE_TYPE_MAX
}STAGE_TYPE;

//****************************************************************************
//�X�e�[�W�̒���
//****************************************************************************
typedef struct
{
	D3DXVECTOR3 min;	//�ŏ��l
	D3DXVECTOR3 max;	//�ő�l
}STAGE_LENGTH;

//****************************************************************************
//�\���̂̒�`
//****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 rot;	//��]
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

STAGE_LENGTH *GetP1StgLng(void);
STAGE_LENGTH *GetP2StgLng(void);

#endif