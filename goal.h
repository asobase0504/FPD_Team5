//=========================================
// 
// �S�[��(�w�b�_�[�t�@�C��)
// Author Tanimoto_Kosuke
//
// Update 22/03/10
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
#define STRIKE_GOAL_HEIGHT		(60)	//�^�񒆃S�[���̉��T�C�Y
#define MAX_GOAL				(6)		//�S�[���̍ő吔
#define MAX_IMAGE_GOAL			(2)		//�S�[���摜�̍ő吔

//****************************************************************************
//�S�[���̎��
//****************************************************************************
typedef enum
{
	GOAL_TYPE_NORMAL = 0,	//3�_�S�[��
	GOAL_TYPE_STRIKE,		//5�_�S�[��
	GOAL_TYPE_MAX
}GOAL_TYPE;

typedef enum
{
	GOAL_COLLISION_NONE = 0,
	GOAL_COLLISION_TOP,
	GOAL_COLLISION_BOTTOM,
	GOAL_COLLISION_RIGHT,
	GOAL_COLLISION_LEFT,
	GOAL_COLLISION_MAX
}GOAL_COLLISION;

//****************************************************************************
//�\���̂̒�`
//****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 rot;	//��]
	D3DXVECTOR3 nor;	//�@��
	D3DXCOLOR	col;	//�F
	GOAL_TYPE type;		//���
	float fAngle;		//�p�x(�A�[�N�^���W�F���g)
	float fLength;		//����(�X�N�E�F�A���[�g)
	bool bUse;			//�g�p����
}GOAL;

//****************************************************************************
//�v���g�^�C�v�錾
//****************************************************************************
void InitGoal(void);
void UninitGoal(void);
void UpdateGoal(void);
void DrawGoal(void);

void ColisionGoal(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pLastPos, float fWidth, float fHeight);
bool ColSegmentsGoal(D3DXVECTOR3 &seg1Start, D3DXVECTOR3 &seg1Vec, D3DXVECTOR3 &seg2Start, D3DXVECTOR3 &seg2Vec);
float Vec3CrossGoal(D3DXVECTOR3* vec1, D3DXVECTOR3* vec2);

GOAL *GetGoal(void);

#endif