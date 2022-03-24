//================================//
//								  //
//       ���̃w�b�_�[		 	  //
//       Author: Ricci Alex		  //
//								  //
//================================//
#ifndef _CHAIN_H_		//���̃}�N������`����ĂȂ�������
#define _CHAIN_H_		//2�d�C���N���[�h�h�~�̃}�N����`

#include "main.h" 

//�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;				//�ʒu
	D3DXVECTOR3 size;				//�T�C�Y
	bool bUse;						//�g�p����Ă��邩�ǂ���
}Chain;

//�v���g�^�C�v�錾
void InitChain(void);
void UninitChain(void);
void UpdateChain(void);
void DrawChain(void);
void SetChain(D3DXVECTOR3 pos, D3DXVECTOR3 size);

#endif