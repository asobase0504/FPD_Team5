//================================//
//								  //
//       �G�t�F�N�g�̃w�b�_�[	  //
//       Author: Ricci Alex		  //
//								  //
//================================//

#ifndef _EFFECT_H_
#define _EFFECT_H_

// �C���N���[�h�t�@�C��
#include "main.h"

// �G�t�F�N�g�̎��
typedef enum
{
	EFFECT_TYPE_SLIDING_IMPACT_1 = 0,	// �X���C�f�B���O�G�t�F�N�g1
	EFFECT_TYPE_SLIDING_IMPACT_2,		// �X���C�f�B���O�G�t�F�N�g2
	EFFECT_TYPE_SLIDING_IMPACT_3,		// �X���C�f�B���O�G�t�F�N�g3
	EFFECT_TYPE_WALL_IMPACT,			// �ǂƓ�����G�t�F�N�g
	EFFECT_TYPE_TRAIL,					//�f�B�X�N�̋O�ՃG�t�F�N�g
	EFFECT_TYPE_POINT_SMOKE_RED,		//�Ԃ����̃G�t�F�N�g
	EFFECT_TYPE_POINT_SMOKE_BLUE,		//�����̃G�t�F�N�g
	EFFECT_TYPE_SPECIAL_GENERAL,		//�K�E�Z�p�̋O�ՃG�t�F�N�g
	EFFECT_TYPE_SPECIAL_TRAIL0,			//�K�E�Z0�̋O�ՃG�t�F�N�g
	EFFECT_TYPE_SPECIAL_TRAIL2,			//�K�E�Z2�̋O�ՃG�t�F�N�g
	EFFECT_TYPE_SPECIAL_TRAIL3,			//�K�E�Z3�̋O�ՃG�t�F�N�g
	EFFECT_TYPE_SPECIAL_TRAIL4,			//�K�E�Z4�̋O�ՃG�t�F�N�g
	EFFECT_TYPE_MAX
}EFFECT_TYPE;

// �G�t�F�N�g�̍\����
typedef struct
{
	D3DXVECTOR3 pos;			// �G�t�F�N�g�̈ʒu
	D3DXVECTOR3 move;			// �G�t�F�N�g�̈ړ���
	float rot;			// �G�t�F�N�g�̈ړ���
	D3DXVECTOR3 acceleration;	// �G�t�F�N�g�̉���
	D3DXVECTOR3 fSize;			// �G�t�F�N�g�̃T�C�Y
	D3DXVECTOR3 fDeltaSize;		// �G�t�F�N�g�̃T�C�Y�̌���
	D3DXCOLOR col;				// �G�t�F�N�g�̃J���[
	D3DXCOLOR fDeltaCol;		// �G�t�F�N�g�̃J���[�̌���
	int nLife;					// �G�t�F�N�g�̃��C�t
	EFFECT_TYPE type;			// �G�t�F�N�g�̎��
	float fAngle;				// �Ίp���̊p�x
	float fLenght;				// �Ίp���̒���
	bool bUse;					// �g�p����Ă��邩�ǂ���
}Effect;

// �v���g�^�C�v�錾
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEffect(D3DXVECTOR3 pos, float rot, EFFECT_TYPE Type);

#endif // !_EFFECT_H_