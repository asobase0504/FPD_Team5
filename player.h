//=========================================
// 
// �v���C���[�̃w�b�_�[�t�@�C��
// Author YudaKaito
// 
//=========================================
#ifndef _PLAYER_H_			// ���̃}�N����`������ĂȂ�������
#define _PLAYER_H_			// 2�d�C���N���[�h�h�~�̃}�N����`

//------------------------------------
// include
//------------------------------------
#include "main.h"
#include "disk.h"

//------------------------------------
// �}�N����`
//------------------------------------

//------------------------------------
// �v���C���[�̎��
//------------------------------------
typedef enum
{
	PLAYERTYPE_1,
	PLAYERTYPE_2,
	PLAYERTYPE_3,
	PLAYERTYPE_4,
	PLAYERTYPE_MAX,
}PLAYERTYPE;

//------------------------------------
// ������
//------------------------------------
typedef enum
{
	JUMP_NONE = 0,
	JUMP_NOW,
	JUMP_MAX
}JUMPSTATE;

//------------------------------------
// �v���C���[�\���̂̒�`
//------------------------------------
typedef struct
{
	LPDIRECT3DTEXTURE9 pTexture;		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;	// ���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3 pos;					// �ʒu
	D3DXVECTOR3 move;					// �ړ���
	JUMPSTATE jumpstate;				// ����̏��
	DISK_TYPE typeSpecialDisk;			// �K�E�Z
	float fSize;						// �傫��
	float fHeight;						// ����
	float fVerticalSpeed = 5.0f;		// �㏸�X�s�[�h
	float fThrowCurvePower;				// �Ȃ��ē������
	float fMaxThrowPower;				// ������ő�̗�
	float fThrowPower;					// ������ő�̗�
	int nSpecialSkillCnt;				// �K�E�Z�܂ł̃J�E���g
	float fMoveSpeed;					// �ړ���
	float fSlidingVolume;				// �X���C�f�B���O��
	int nSlidingRigorCnt;				// �X���C�f�B���O�̍d�����Ԃ̃J�E���g
	int nSlidingRigorMax;				// �X���C�f�B���O�̍d������
	float fAttenuationSlidingSpead;		// �X���C�f�B���O���̌����l
	float fAttenuationMoveSpead;		// �ړ����̌����l
	int nIdxShadow;						// �e�̎g�p�ԍ�
	bool bHaveDisk;						// �f�B�X�N���������Ă��邩
	bool bUseSliding;					// �X���C�f�B���O���
	bool bUse;							// �\�����
}Player;

//------------------------------------
// �v���g�^�C�v�錾
//------------------------------------
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
void SetPlayer(const D3DXVECTOR3& pos, PLAYERTYPE type);
Player* GetPlayer(void);

#endif // !_PLAYER_H_
