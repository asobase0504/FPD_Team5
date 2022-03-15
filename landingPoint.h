//==========================================//
//										    //
//       �㓊���̗�����ʒu�̃w�b�_�[	    //
//       Author: Ricci Alex				    //
//										    //
//==========================================//

#ifndef _LANDING_POINT_H_		//���̃}�N������`����ĂȂ�������
#define _LANDING_POINT_H_		//2�d�C���N���[�h�h�~�̃}�N����`

#include "main.h" 

//�}�N����`
#define MAX_LANDING_POINT			(1)			//�㓊���̗�����ʒu�̃}�[�N�̍ő吔

//�\����
typedef struct
{
	D3DXVECTOR3 pos;
	int nCntAnim;
	int nAnimPattern;
	float fSize;
	bool bUse;
}LandingMark;

//�v���g�^�C�v�錾
void InitLandingMark(void);
void UninitLandingMark(void);
void UpdateLandingMark(void);
void DrawLandingMark(void);
void SetLandingMark(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fHeight, float fSpeed, float fAcc, float fSize);
void DestroyLandingMark(void);

#endif