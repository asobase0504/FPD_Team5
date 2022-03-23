//=========================================
// 
// �S�[��(�w�b�_�[�t�@�C��)
// Author Tanimoto_Kosuke
//
// Update 22/03/22
// 
//=========================================
#ifndef _GOAL_H_
#define _GOAL_H_

#include "main.h"

//****************************************************************************
//�}�N����`
//****************************************************************************
#define GOAL_WIDTH				(80)	//�S�[���̉��T�C�Y
#define GOAL_HEIGHT				(150)	//�S�[���̏c�T�C�Y
#define STRIKE_GOAL_HEIGHT		(170)	//�^�񒆃S�[���̏c�T�C�Y
#define MAX_GOAL				(6)		//�S�[���̍ő吔
#define MAX_IMAGE_DRUM			(2)		//�S�[���摜�̍ő吔

//****************************************************************************
//�S�[���̎��
//****************************************************************************
typedef enum
{
	GOAL_TYPE_NORMAL = 0,	//3�_�S�[��
	GOAL_TYPE_STRIKE,		//5�_�S�[��
	GOAL_TYPE_MAX
}GOAL_TYPE;

//****************************************************************************
//�\���̂̒�`
//****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 move;	//�ړ���
	D3DXVECTOR3 rot;	//��]
	D3DXVECTOR3 nor;	//�@��
	D3DXCOLOR	col;	//�F
	GOAL_TYPE type;		//���
	bool bSide;			//�ǂ���������(false:���� true:�E��)
	float fAngle;		//�p�x(�A�[�N�^���W�F���g)
	float fLength;		//����(�X�N�E�F�A���[�g)
	bool bUse;			//�g�p����
}Goal;

//****************************************************************************
//�v���g�^�C�v�錾
//****************************************************************************
void InitGoal(void);
void UninitGoal(void);
void UpdateGoal(void);
void DrawGoal(void);
void SetGoal(D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool side, GOAL_TYPE type, int nIdxGoal);

void ColisionGoal(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pLastPos);
bool ColSegmentsGoal(D3DXVECTOR3 &seg1Start, D3DXVECTOR3 &seg1Vec, D3DXVECTOR3 &seg2Start, D3DXVECTOR3 &seg2Vec);
float Vec3CrossGoal(D3DXVECTOR3* vec1, D3DXVECTOR3* vec2);

Goal *GetGoal(void);

#endif