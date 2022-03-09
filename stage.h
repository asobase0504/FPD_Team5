//=========================================
// 
// �X�e�[�W(�w�b�_�[�t�@�C��)
// Author Tanimoto_Kosuke
//
// Update 22/03/07
// 
//=========================================
#ifndef _STAGE_H_
#define _STAGE_H_

#include "main.h"

//****************************************************************************
//�}�N����`
//****************************************************************************
#define STAGE_WIDTH		(SCREEN_WIDTH - 200)	//�X�e�[�W�̉��T�C�Y
#define STAGE_HEIGHT	(SCREEN_HEIGHT - 200)	//�X�e�[�W�̏c�T�C�Y

//****************************************************************************
//�\���̂̒�`
//****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 rot;	//��]
	D3DXVECTOR3 nor;	//�@��
	D3DXCOLOR	col;	//�F
	float fAngle;		//�p�x(�A�[�N�^���W�F���g)
	float fLength;		//����(�X�N�E�F�A���[�g)
}STAGE;

typedef enum
{
	STAGE_COLLISION_NONE = 0,
	STAGE_COLLISION_TOP,
	STAGE_COLLISION_BOTTOM,
	STAGE_COLLISION_RIGHT,
	STAGE_COLLISION_LEFT,
	STAGE_COLLISION_MAX
}STAGE_COLLISION;

//****************************************************************************
//�v���g�^�C�v�錾
//****************************************************************************
void InitStage(void);
void UninitStage(void);
void UpdateStage(void);
void DrawStage(void);

//STAGE_COLLISION ColisionStage(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pNor,float fWidth, float fHeight);

#endif