//================================//
//								  //
//       �ǂ̃w�b�_�[	    	  //
//       Author: Ricci Alex		  //
//								  //
//================================//
#ifndef _WALL_H_		//���̃}�N������`����ĂȂ�������
#define _WALL_H_		//2�d�C���N���[�h�h�~�̃}�N����`

#include "main.h" 

//�}�N����`
#define MAX_WALL			(2)			//�ǂ̍ő吔

//�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;						//�ǂ̌��݂̈ʒu
	float fWidth;							//�ǂ̕�
	float fHeight;							//�ǂ̍���
	float fAlpha;							//�ǂ̉�]�p�x
	D3DXVECTOR3 nor;						//�ǂ̖@��(�����蔻��p)
	bool bUse;								//�g�p����Ă��邩�ǂ���
}Wall;

//�v���g�^�C�v�錾
void InitWall(void);
void UninitWall(void);
void UpdateWall(void);
void DrawWall(void);
void SetWall(D3DXVECTOR3 pos, float width, float height, float angle);
void WallBounce(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pLastPos, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pAcc, float fRadius);
bool SpecialWallBounce(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pLastPos, D3DXVECTOR3 *pMove, float fRadius);

#endif