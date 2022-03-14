//================================//
//								  //
//       �G�t�F�N�g�̃w�b�_�[	  //
//       Author: Ricci Alex		  //
//								  //
//================================//

#ifndef _EFFECT_H_
#define _EFFECT_H_

//�C���N���[�h�t�@�C��
#include "main.h"

//�}�N����`
#define MAX_EFFECT				(2400)					//�G�t�F�N�g�̍ő吔

//�G�t�F�N�g�̎��
typedef enum
{
	EFFECT_TYPE_WALL_IMPACT = 0,						//�ǂƓ�����G�t�F�N�g
	EFFECT_TYPE_MAX
}EFFECT_TYPE;

//�G�t�F�N�g�̍\����
typedef struct
{
	D3DXVECTOR3 pos;			//�G�t�F�N�g�̈ʒu
	D3DXVECTOR3 move;			//�G�t�F�N�g�̈ړ���
	D3DXVECTOR3 acceleration;	//�G�t�F�N�g�̉���
	D3DXVECTOR3 fSize;			//�G�t�F�N�g�̃T�C�Y
	D3DXVECTOR3 fDeltaSize;		//�G�t�F�N�g�̃T�C�Y�̌���
	D3DXCOLOR col;				//�G�t�F�N�g�̃J���[
	D3DXCOLOR fDeltaCol;		//�G�t�F�N�g�̃J���[�̌���
	int nLife;					//�G�t�F�N�g�̃��C�t
	EFFECT_TYPE type;			//�G�t�F�N�g�̎��
	bool bUse;					//�g�p����Ă��邩�ǂ���
}Effect;

//�v���g�^�C�v�錾
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 acceleration,
	D3DXVECTOR3 Size, D3DXVECTOR3 DeltaSize, D3DXCOLOR col, D3DXCOLOR Delta, int Life, EFFECT_TYPE Type);

#endif // !_EFFECT_H_