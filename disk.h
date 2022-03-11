//================================//
//								  //
//       �f�B�X�N�̃w�b�_�[		  //
//       Author: Ricci Alex		  //
//       Author: tanimoto		  //
//								  //
//================================//
#ifndef _DISK_H_		//���̃}�N������`����ĂȂ�������
#define _DISK_H_		//2�d�C���N���[�h�h�~�̃}�N����`

#include "main.h" 

//�}�N����`
#define MAX_DISK			(1)				//�f�B�X�N�̍ő吔
#define LOB_STARTING_SPEED	(5.0f)			//�㓊���̂͂��߂̏㉺�ړ���

//�f�B�X�N�̎��
typedef enum
{
	DISK_TYPE_NORMAL = 0,			//����
	DISK_TYPE_LOB,					//�㓊��
	DISK_TYPE_SPECIAL_0,
	DISK_TYPE_SPECIAL_1,
	DISK_TYPE_MAX
}DISK_TYPE;

//�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;						//�f�B�X�N�̌��݂̈ʒu
	D3DXVECTOR3 lastPos;					//�f�B�X�N�̑O��̈ʒu
	D3DXVECTOR3 move;						//�f�B�X�N�̈ړ���
	D3DXVECTOR3 acc;						//�f�B�X�N�̉���
	DISK_TYPE type;							//�f�B�X�N�̎��
	float fSize;							//�f�B�X�N�̑傫��
	float fHeight;							//�㓊���p�̍���
	float fVerticalSpeed;					//�㓊���̈ړ���
	int nCntPhase;							//�K�E�Z�p�̃J�E���^�[
	int nPlayer;							//�f�B�X�N�𓊂����v���C���[�̃C���f�b�N�X
	bool bBounce;							//�ǂɓ���������A���˂��邩�ǂ���
	bool bUse;								//�g�p����Ă��邩�ǂ���
}Disk;

//�v���g�^�C�v�錾
void InitDisk(void);
void UninitDisk(void);
void UpdateDisk(void);
void DrawDisk(void);
void SetDisk(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 acc, DISK_TYPE type, int nPlayer, float size);
void UpdateSpecialDisk(int nCntDisk, int nPlayer);

Disk *GetDisk(void);

#endif