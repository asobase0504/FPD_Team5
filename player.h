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

//------------------------------------
// �}�N����`
//------------------------------------
#define PLAYER_TEX		"data/TEXTURE/player00.png"		// �v���C���[�̃e�N�X�`��
#define PLAYER_WIDTH	(35.0f)							// �v���C���[�̕�
#define PLAYER_HEIGTH	(35.0f)							// �v���C���[�̍���

//------------------------------------
// �v���C���[�̏��
//------------------------------------
typedef enum
{
	PLAYERSTATE_APPEAR = 0,	// �o�����(�_��)
	PLAYERSTATE_NORMAL,		// �ʏ���
	PLAYERSTATE_DEATH,		// ���S���
	PLAYERSTATE_REVIVAL,	// �������
	PLAYERSTATE_MAX
}PLAYRSTATE;

//------------------------------------
// �v���C���[�̏��
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
	D3DXVECTOR3 pos;			// �ʒu
	D3DXVECTOR3 posOld;			// �ʒu
	D3DXVECTOR3 move;			// �ړ���
	D3DXVECTOR3 rot;			// ����
	float fWidth;				// �摜�̕�
	float fHeigth;				// �摜�̍���
	float fMove;				// �ړ���
	JUMPSTATE jumpstate;		// ����̏��
	int nDirectionMove;			// ����
	PLAYRSTATE state;			// �X�e�[�^�X
	int RevivalInterval;		// �����^�C�~���O
	int nCntAnim;				// �A�j���[�V�����̃J�E���^�[
	int nPatternAnim;			// �A�j���[�V�����̃p�^�[���ԍ�
	bool bUse;					// �\�����
}Player;

//------------------------------------
// �v���g�^�C�v�錾
//------------------------------------
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
void SetPlayer(D3DXVECTOR3* pos);
Player* GetPlayer(void);

#endif // !_PLAYER_H_
