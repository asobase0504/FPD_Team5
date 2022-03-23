//=========================================
// 
// �|�b�v�A�b�v(�w�b�_�[�t�@�C��)
// Author Tanimoto_Kosuke
//
// Update 22/03/23
// 
//=========================================
#ifndef _POP_H_
#define _POP_H_

#include "main.h"

//****************************************************************************
//�}�N����`
//****************************************************************************
#define GOAL_POP_WIDTH		(150)				//�S�[���|�b�v�̉��T�C�Y
#define GOAL_POP_HEIGHT		(50)				//�S�[���|�b�v�̏c�T�C�Y
#define FELL_POP_WIDTH		(50)				//�t�H�[���|�b�v�̉��T�C�Y
#define FELL_POP_HEIGHT		(100)				//�t�H�[���|�b�v�̏c�T�C�Y
#define SCORE_POP_WIDTH		(SCREEN_WIDTH / 10)	//�X�R�A�|�b�v�̉��T�C�Y
#define SCORE_POP_HEIGHT	(SCREEN_HEIGHT / 4)	//�X�R�A�|�b�v�̏c�T�C�Y
#define MAX_POP				(11)				//�|�b�v�̍ő吔
#define MAX_IMAGE_POP		(3)					//�|�b�v�摜�̍ő吔
#define POP_SPEAD			(8)					//�|�b�v�̈ړ����x

//****************************************************************************
//�|�b�v�̎��
//****************************************************************************
typedef enum
{
	POP_TYPE_NORMAL = 0,	//3�_�S�[���|�b�v
	POP_TYPE_STRIKE,		//5�_�S�[���|�b�v
	POP_TYPE_FELL,			//2�_�S�[���|�b�v
	POP_TYPE_MAX
}POP_TYPE;

//****************************************************************************
//�\���̂̒�`
//****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 move;	//�ړ���
	D3DXVECTOR3 rot;	//��]
	D3DXCOLOR	col;	//�F
	POP_TYPE type;		//���
	bool bSide;			//�ǂ���������(false:���� true:�E��)
	float fAngle;		//�p�x(�A�[�N�^���W�F���g)
	float fLength;		//����(�X�N�E�F�A���[�g)
	float fPopCounter;	//�|�b�v�J�E���^�[
	bool bUse;			//�g�p����
}POP;

//****************************************************************************
//�v���g�^�C�v�錾
//****************************************************************************
void InitPop(void);
void UninitPop(void);
void UpdatePop(void);
void DrawPop(void);
void SetPop(D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool side, POP_TYPE type, int nIdxGoal);

void PopCounter(int nIdxPop);

POP *GetPop(void);

#endif