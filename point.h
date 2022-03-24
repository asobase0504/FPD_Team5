//=========================================
// 
// �|�b�v�A�b�v(�w�b�_�[�t�@�C��)
// Author YudaKaito
// Author Tanimoto
// 
//=========================================
#ifndef _POINT_H_
#define _POINT_H_

#include "main.h"

//------------------------------------
//�}�N����`
//------------------------------------
#define POINT_WIDTH		(100)	//�X�R�A�|�b�v�̉��T�C�Y
#define POINT_HEIGHT	(100)	//�X�R�A�|�b�v�̏c�T�C�Y

//------------------------------------
//�|�C���g�̍\����
//------------------------------------
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;	//���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3 pos;					//���݂̈ʒu
	D3DXCOLOR col;
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
void SetPointRound(int* nIdx);

#endif