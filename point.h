//=========================================
// 
// �|�b�v�A�b�v(�w�b�_�[�t�@�C��)
// Author YudaKaito
// 
//=========================================
#ifndef _POINT_H_
#define _POINT_H_

#include "main.h"

//------------------------------------
//�}�N����`
//------------------------------------

//------------------------------------
//�|�C���g�̍\����
//------------------------------------
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;	//���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3 pos;					//���݂̈ʒu
	int nScore;							//�X�R�A�̒l
	bool bUse;
}Point;

//------------------------------------
//�v���g�^�C�v�錾
//------------------------------------
void InitPoint(void);
void UninitPoint(void);
void UpdatePoint(void);
void DrawPoint(void);
void SetPoint(void);

#endif