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
#define MAX_TIME	(5)	//�ő�^�C���J�E���^�[��

//�v���g�^�C�v�錾
void InitTime(void);
void UninitTime(void);
void UpdateTime(void);
void DrawTime(void);
void SetTime(void);
#endif