//================================//
//								  //
//       ���Ԃ̃w�b�_�[		 	  //
//       Author: Ricci Alex		  //
//								  //
//================================//
#ifndef _GEAR_H_		//���̃}�N������`����ĂȂ�������
#define _GEAR_H_		//2�d�C���N���[�h�h�~�̃}�N����`

#include "main.h" 

//�}�N����`
#define MAX_GEAR			(32)			//�͂���܂̍ő吔

//�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;				//�ʒu
	float rot;						//��]�p�x
	float fAngle;					//��]�p�̊p�x
	float fLenght;					//��]�p�̒���
	float fSize;					//�T�C�Y
	float fFrameRot;				//��t���[���̉�]�p�x
	int nType;						//���
	bool bUse;						//�g�p����Ă��邩�ǂ���
}Gear;

//�v���g�^�C�v�錾
void InitGear(void);
void UninitGear(void);
void UpdateGear(void);
void DrawGear(void);

//�ʒu�E�T�C�Y�E��t���[���̉�]�p�x�E���
void SetGear(D3DXVECTOR3 pos, float size, float frameRot, int type);

#endif