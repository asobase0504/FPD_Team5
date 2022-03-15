//============================================
//
//�`�[������<�t���C���O�p���[�f�B�X�N>[option.h]
//Author: goto yuuki
//Author:Kishimoto Eiji
//
//============================================
#ifndef _OPTION_H_		//���̃}�N������`����ĂȂ�������
#define _OPTION_H_		//2�d�C���N���[�h�h�~�̃}�N����`

#include "main.h"

//***********************************
//�I�v�V�����̗񋓌^
//***********************************
typedef enum
{
	OPTION_TIMELIMIT = 0,	//��������
	OPTION_POINT,			//�|�C���g��
	OPTION_SETCOUNT,		//�Z�b�g��
	OPTION_GOTOGAME,		//�Q�[���֐i��
	OPTION_MAX
}OPTION;

//***********************************
//�v���g�^�C�v�錾
//***********************************

void InitOption(void);
void UninitOption(void);
void UpdateOption(void);
void DrawOption(void);
void SetOption(void);

#endif
