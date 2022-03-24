//=============================================================================
// 
// �T�E���h���� [sound.h]
// Author : AKIRA TANAKA
// 
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

// *****************************************************************************
// �T�E���h�t�@�C��
// *****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM_TITLE = 0,		// BGM�^�C�g��
	SOUND_LABEL_SE_SELECT,
	SOUND_LABEL_SE_YES,
	SOUND_LABEL_SE_NO,
	SOUND_LABEL_SE_GEARSTOP,
	SOUND_LABEL_SE_WHISTLE_START,
	SOUND_LABEL_SE_WHISTLE_STOP,
	SOUND_LABEL_SE_WHISTLE_FINISH,
	SOUND_LABEL_MAX,
} SOUND_LABEL;

// *****************************************************************************
// �v���g�^�C�v�錾
// *****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
