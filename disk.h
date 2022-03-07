//================================//
//								  //
//       �f�B�X�N�̃w�b�_�[		  //
//       Author: Ricci Alex		  //
//								  //
//================================//
#ifndef _DISK_H_		//���̃}�N������`����ĂȂ�������
#define _DISK_H_		//2�d�C���N���[�h�h�~�̃}�N����`

#include "main.h" 

//�}�N����`
#define MAX_DISK			(1)				//�f�B�X�N�̍ő吔

//�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;						//�f�B�X�N�̌��݂̈ʒu
	D3DXVECTOR3 lastPos;					//�f�B�X�N�̑O��̈ʒu
	D3DXVECTOR3 move;						//�f�B�X�N�̈ړ���
	float fSize;							//�f�B�X�N�̑傫��
	bool bUse;								//�g�p����Ă��邩�ǂ���
}Disk;

//�v���g�^�C�v�錾
void InitDisk(void);
void UninitDisk(void);
void UpdateDisk(void);
void DrawDisk(void);
void SetDisk(D3DXVECTOR3 pos, D3DXVECTOR3 move, float size);

#endif