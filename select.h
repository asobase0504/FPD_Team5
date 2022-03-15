//============================================
//
//�`�[������<�t���C���O�p���[�f�B�X�N>[select.h]
//Author:Kishimoto Eiji
//
//============================================
#ifndef _SELECT_H_
#define _SELECT_H_

#include "main.h"

//***********************************
//�������Ԃ̗񋓌^
//***********************************
typedef enum
{
	TIMELIMIT_15 = 0,		//15�b
	TIMELIMIT_30,			//30�b
	TIMELIMIT_45,			//45�b
	TIMELIMIT_90,			//90�b
	TIMELIMIT_INFINITY,		//����(�������ԂȂ�)
	TIMELIMIT_MAX
}TIMELIMIT;

//***********************************
//�|�C���g���̗񋓌^
//***********************************
typedef enum
{
	POINTCOUNT_12 = 0,	//12�_
	POINTCOUNT_15,		//15�_
	POINTCOUNT_21,		//21�_
	POINTCOUNT_25,		//25�_
	POINTCOUNT_MAX
}POINTCOUNT;

//***********************************
//�Z�b�g���̗񋓌^
//***********************************
typedef enum
{
	SETCOUNT_ONE = 0,	//1�Z�b�g
	SETCOUNT_TWO,		//2�Z�b�g
	SETCOUNT_THREE,		//3�Z�b�g
	SETCOUNT_FOUR,		//4�Z�b�g
	SETCOUNT_MAX
}SETCOUNT;

//***********************************
//�v���g�^�C�v�錾
//***********************************

void InitSelect(void);
void UninitSelect(void);
void UpdateSelect(void);
void DrawSelect(void);

int SelectTimeLimit(int nSelectMenu);
int SelectPoint(int nSelectMenu);
int SelectSetCount(int nSelectMenu);

#endif