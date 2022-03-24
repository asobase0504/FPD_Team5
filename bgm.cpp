//=========================================
// 
// bgm(�w�b�_�[�t�@�C��)
// Author Tanimoto
// 
// Update 22/03/24
//
//=========================================
//------------------------------------
// include
//------------------------------------
#include "bgm.h"
#include "sound.h"

#include <time.h>			//�^�C���֐����g�p���邽�߂ɕK�v
#include <stdlib.h>			//�����_���֐���system("cls")���g�p���邽�߂ɕK�v

//------------------------------------
// �ÓI�ϐ�
//------------------------------------
static BGM s_aBgm;
static int s_nRndBgm;

//=========================================
// ������
//=========================================
void InitBgm(void)
{
	srand((unsigned int)time(0));

	s_nRndBgm = rand() % MAX_BGM;

	if (s_nRndBgm == BGM_TYPE_1)
	{
		PlaySound(SOUND_LABEL_BGM_GAME1);
	}
	else if (s_nRndBgm == BGM_TYPE_2)
	{
		PlaySound(SOUND_LABEL_BGM_GAME2);
	}
	else if (s_nRndBgm == BGM_TYPE_3)
	{
		PlaySound(SOUND_LABEL_BGM_GAME3);
	}
}

//=========================================
// �I��
//=========================================
void UninitBgm(void)
{
	if (s_nRndBgm == BGM_TYPE_1)
	{
		StopSound(SOUND_LABEL_BGM_GAME1);
	}
	else if (s_nRndBgm == BGM_TYPE_2)
	{
		StopSound(SOUND_LABEL_BGM_GAME2);
	}
	else if (s_nRndBgm == BGM_TYPE_3)
	{
		StopSound(SOUND_LABEL_BGM_GAME3);
	}
}

//=========================================
// �X�V
//=========================================
void UpdateBgm(void)
{}

//=========================================
// �`��
//=========================================
void DrawBgm(void)
{}

//=========================================
// �ݒ�
//=========================================
void SetBgm(void)
{
	InitBgm();
}
