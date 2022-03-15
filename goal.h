//=========================================
// 
// �S�[��(�w�b�_�[�t�@�C��)
// Author Tanimoto_Kosuke
//
// Update 22/03/14
// 
//=========================================
#ifndef _GOAL_H_
#define _GOAL_H_

#include "main.h"

//****************************************************************************
//�}�N����`
//****************************************************************************
#define GOAL_WIDTH				(100)	//�S�[���̉��T�C�Y
#define GOAL_HEIGHT				(230)	//�S�[���̏c�T�C�Y
#define GOAL_POP_WIDTH			(150)	//�S�[���̉��T�C�Y
#define GOAL_POP_HEIGHT			(50)	//�S�[���̏c�T�C�Y
#define STRIKE_GOAL_HEIGHT		(60)	//�^�񒆃S�[���̉��T�C�Y
#define MAX_GOAL				(12)	//�S�[���̍ő吔
#define MAX_IMAGE_GOAL			(4)		//�S�[���摜�̍ő吔

//****************************************************************************
//�S�[���̎��
//****************************************************************************
typedef enum
{
	GOAL_TYPE_NORMAL = 0,	//3�_�S�[��
	GOAL_TYPE_STRIKE,		//5�_�S�[��
	GOAL_TYPE_NORMAL_POP,	//3�_�S�[���|�b�v
	GOAL_TYPE_STRIKE_POP,	//5�_�S�[���|�b�v
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
	float fAngle;		//�p�x(�A�[�N�^���W�F���g)
	float fLength;		//����(�X�N�E�F�A���[�g)
	bool bUse;			//�g�p����
	bool bGoal;			//�S�[������
}GOAL;

//****************************************************************************
//�v���g�^�C�v�錾
//****************************************************************************
void InitGoal(void);
void UninitGoal(void);
void UpdateGoal(void);
void DrawGoal(void);
void SetGoalPop(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nIdxGoal);

void ColisionGoal(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pLastPos, float fWidth, float fHeight);
bool ColSegmentsGoal(D3DXVECTOR3 &seg1Start, D3DXVECTOR3 &seg1Vec, D3DXVECTOR3 &seg2Start, D3DXVECTOR3 &seg2Vec);
float Vec3CrossGoal(D3DXVECTOR3* vec1, D3DXVECTOR3* vec2);

GOAL *GetGoal(void);

#endif