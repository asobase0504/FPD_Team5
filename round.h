//=========================================
// 
// ���E���h����(�w�b�_�[�t�@�C��)
// Author YudaKaito
// 
//=========================================
#ifndef _ROUND_H_
#define _ROUND_H_

#include "main.h"

//------------------------------------
//�|�C���g�̍\����
//------------------------------------
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;	//���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3 pos;					//���݂̈ʒu
	int nRound;							//�X�R�A�̒l
	bool bUse;
}Round;

//------------------------------------
//�v���g�^�C�v�錾
//------------------------------------
void InitRound(void);
void UninitRound(void);
void UpdateRound(void);
void DrawRound(void);
void SetRound(void);

#endif