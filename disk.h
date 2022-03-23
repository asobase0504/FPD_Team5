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
#define MAX_DISK					(1)					//�f�B�X�N�̍ő吔
#define LOB_STARTING_SPEED			(5.0f)				//�㓊���̂͂��߂̏㉺�ړ���
#define NORMAL_VERTICAL_SPEED		(0.0f)				//�f�B�X�N�̕��ʂ̏㉺�ړ���
#define NORMAL_DISK_HEIGHT			(15.0f)				//�f�B�X�N�̕��ʍ���
#define JUMP_MAX_HEIGHT				(260.0f)			//�W�����v������̍ő卂��
#define JUMP_ATTACK_TIME			(60.0f)				//�W�����v�����̎��A�f�B�X�N��������܂ł̎���
#define GRAVITY_ACCELERATION_LOB	(-0.05f)			//�d�ʂ̉���
#define BASE_ROTATION_SPEED			(-D3DX_PI * 0.01f)	//�x�[�X�̉�]�X�s�[�h

//�f�B�X�N�̎��
typedef enum
{
	DISK_TYPE_NORMAL = 0,			//����
	DISK_TYPE_LOB,					//�㓊��
	DISK_TYPE_JUMP,					//�W�����v����
	DISK_TYPE_BLOCKED,
	DISK_TYPE_SPECIAL_0,			//�K�E�Z0
	DISK_TYPE_SPECIAL_1,			//�K�E�Z1
	DISK_TYPE_SPECIAL_2,			//�K�E�Z2
	DISK_TYPE_SPECIAL_3,			//�K�E�Z3
	DISK_TYPE_SPECIAL_4,			//�K�E�Z4
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
	float fAngle;							//��]�p�̊p�x
	float fLenght;							//�f�B�X�N�̔��a
	float fVertexLenght;					//��]�A�j���[�V�����p�̒���
	float rot;								//��]�p�x
	float fRotSpeed;						//��]�X�s�[�h
	int nCntPhase;							//�K�E�Z�p�̃J�E���^�[
	int nPlayer;							//�f�B�X�N�𓊂����v���C���[�̃C���f�b�N�X
	int nCntAnim;							//�A�j���[�V�����J�E���^�[
	int nAnimPattern;						//�A�j���[�V�����̃p�^�[���ԍ�
	int nIdxShadow;							//�f�B�X�N�̉e�̃C���f�b�N�X
	bool bBounce;							//�ǂɓ���������A���˂��邩�ǂ���
	bool bUse;								//�g�p����Ă��邩�ǂ���
}Disk;

//�v���g�^�C�v�錾
void InitDisk(void);
void UninitDisk(void);
void UpdateDisk(void);
void DrawDisk(void);
void SetDisk(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 acc, DISK_TYPE type, int nPlayer, float size);
void UpdateSpecialDisk(int nCntDisk);
D3DXVECTOR3 SetLobSpeed(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nCntDisk, float fHeight, float fVerticalSpeed);
D3DXVECTOR3 SetJumpAttackSpeed(D3DXVECTOR3 pos);
void DestroyDisk(void);

Disk *GetDisk(void);

#endif