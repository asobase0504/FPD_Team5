//================================//
//								  //
//       �e�̃w�b�_�[			  //
//       Author: Ricci Alex		  //
//								  //
//================================//
#ifndef _SHADOW_H_		//���̃}�N������`����ĂȂ�������
#define _SHADOW_H_		//2�d�C���N���[�h�h�~�̃}�N����`

#include "main.h" 

//�}�N����`
#define MAX_SHADOW			(8)				//�e�̍ő吔

//�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;						//�f�B�X�N�̌��݂̈ʒu
	D3DXVECTOR3 move;						//�f�B�X�N�̈ړ���
	D3DXVECTOR3 acc;						//�f�B�X�N�̉���
	float fSize;							//�f�B�X�N�̑傫��
	bool bUse;								//�g�p����Ă��邩�ǂ���
}Shadow;

//�v���g�^�C�v�錾
void InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);
int SetShadow(D3DXVECTOR3 pos, float size);
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos);

#endif#pragma once
