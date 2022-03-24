//------------------------------
//���Ԑݒ�̃w�b�_�[
//ACG000
//Author: goto yuuki
//
//------------------------------
#ifndef _TIME_H_		//���̃}�N����`������ĂȂ�������
#define _TIME_H_		//�Q�d�C���N���[�h�h�~�̃}�N����`

#include "main.h"
#include "fade.h"

//�}�N����`
#define MAX_TIME		(30)							//�ő�^�C���ݒ萔
#define MAX_CNT_TIME	(2)								//�ő�J�E���^�[��

//�^�C���̍\����
typedef struct
{
	D3DXVECTOR3 pos[MAX_CNT_TIME];									//���݂̈ʒu
	int nTime;											//�^�C���̐ݒ萔
	int nMinusTime;										//�^�C����������
	bool bTimeFlag;										//�^�C�g���̈ړ��̃t���O
}TIME;

//�v���g�^�C�v�錾
void InitTime(void);
void UninitTime(void);
void UpdateTime(void);
void DrawTime(void);
TIME *GetTime(void);
void SetTime(int nTime);
#endif