//================================//
//								  //
//       �z�ǂ̃w�b�_�[		 	  //
//       Author: Ricci Alex		  //
//								  //
//================================//
#ifndef _PIPE_H_		//���̃}�N������`����ĂȂ�������
#define _PIPE_H_		//2�d�C���N���[�h�h�~�̃}�N����`

#include "main.h" 

//�}�N����`
#define MAX_PIPE			(16)			//�z�ǂ̍ő吔

typedef enum
{
	PIPE_TYPE_STEAM = 0,
	PIPE_TYPE_VERTICAL,
	PIPE_TYPE_MAX
}PIPE_TYPE;

//�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;				//�ʒu
	D3DXVECTOR3 size;				//�T�C�Y
	int nType;						//���
	int nDelay;						//�A�j���[�V�����p�̕ϐ�
	int nCntAnim;					//�A�j���[�V�����J�E���^�[
	int nCntMove;					//�ړ��p�̕ϐ�
	float fRand;					//�A�j���[�V�����ړ��p�̕ϐ�
	int nScored;					
	bool bUse;						//�g�p����Ă��邩�ǂ���
}Pipe;

//�v���g�^�C�v�錾
void InitPipe(void);
void UninitPipe(void);
void UpdatePipe(void);
void DrawPipe(void);
void PipeAnimation(int nCntPipe);

//�ʒu�E�T�C�Y�E��t���[���̉�]�p�x�E���
void SetPipe(D3DXVECTOR3 pos, D3DXVECTOR3 size, PIPE_TYPE type);

void PointSmokeAnimation(int nPlayer);

#endif