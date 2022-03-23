//=========================================
// 
// �h������(�w�b�_�[�t�@�C��)
// Author Tanimoto_Kosuke
//
// Update 22/03/22
// 
//=========================================
#ifndef _DRUM_H_
#define _DRUM_H_

#include "main.h"

//****************************************************************************
//�}�N����`
//****************************************************************************
#define DRUM_WIDTH				(96.0f * 0.55f)	//�h�����ʂ̉��T�C�Y
#define DRUM_HEIGHT				(160.0f * 0.75f)	//�h�����ʂ̏c�T�C�Y
#define MAX_DRUM				(6)		//�h�����ʂ̍ő吔
#define MAX_IMAGE_DRUM			(2)		//�h�����ʉ摜�̍ő吔

//****************************************************************************
//�h�����ʂ̎��
//****************************************************************************
typedef enum
{
	DRUM_TYPE_NORMAL = 0,	//3�_�h������
	DRUM_TYPE_STRIKE,		//5�_�h������
	DRUM_TYPE_MAX
}DRUM_TYPE;

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
	DRUM_TYPE type;		//���
	bool bSide;			//�ǂ���������(false:���� true:�E��)
	float fAngle;		//�p�x(�A�[�N�^���W�F���g)
	float fLength;		//����(�X�N�E�F�A���[�g)
	bool bUse;			//�g�p����
}DRUM;

//****************************************************************************
//�v���g�^�C�v�錾
//****************************************************************************
void InitDrum(void);
void UninitDrum(void);
void UpdateDrum(void);
void DrawDrum(void);
void SetDrum(D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool side, DRUM_TYPE type, int nIdxGoal);

DRUM *GetDrum(void);

#endif