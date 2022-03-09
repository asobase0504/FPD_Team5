//=========================================
// 
// �S�[��(�w�b�_�[�t�@�C��)
// Author Tanimoto_Kosuke
//
// Update 22/03/09
// 
//=========================================
#ifndef _GOAL_H_
#define _GOAL_H_

#include "main.h"

//****************************************************************************
//�}�N����`
//****************************************************************************
#define GOAL_WIDTH				(230)	//�S�[���̉��T�C�Y
#define STRIKE_GOAL_WIDTH		(60)	//�^�񒆃S�[���̉��T�C�Y
#define GOAL_HEIGHT				(50)	//�S�[���̏c�T�C�Y
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

void ColisionGoal(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pNor,float fWidth, float fHeight);
GOAL *GetGoal(void);


#endif