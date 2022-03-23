//=========================================
// 
// ���t�F���[(�w�b�_�[�t�@�C��)
// Author Tanimoto_Kosuke
//
// Update 22/03/14
// 
//=========================================
#ifndef _REFEREE_H_
#define _REFEREE_H_

#include "main.h"

//****************************************************************************
// �\���̂̒�`
//****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	// �ʒu
	D3DXVECTOR3 rot;	// ��]
	D3DXVECTOR3 nor;	// �@��
	D3DXCOLOR	col;	// �F
	float fAngle;		// �p�x(�A�[�N�^���W�F���g)
	float fLength;		// ����(�X�N�E�F�A���[�g)
	bool bThrow;		// �����邩�ۂ�
	bool bUse;			// �g�p����
}REFEREE;

//****************************************************************************
// �v���g�^�C�v�錾
//****************************************************************************
void InitReferee(void);
void UninitReferee(void);
void UpdateReferee(void);
void DrawReferee(void);
REFEREE *GetReferee(void);
void SetThoThrowRefreeIdx(int nIdx);

#endif