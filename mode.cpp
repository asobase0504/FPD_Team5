//==================================================
//
// FPD���� ( mode.cpp )
// Author  : katsuki mizuki
// Author:Kishimoto Eiji
//
//==================================================

//--------------------------------------------------
// �C���N���[�h
//--------------------------------------------------
#include "mode.h"
#include "fade.h"
#include "title.h"
//#include "tutorial.h"
#include "option.h"
#include "game.h"

#include <assert.h>

//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static MODE		s_mode = MODE_NONE;			// ���݂̃��[�h
static MODE		s_modeNext = MODE_NONE;		// ���̃��[�h

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitMode(void)
{
	// �^�C�g���̏I��
	UninitTitle();

	// �Q�[���̏I��
	UninitGame();
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateMode(void)
{
	switch (s_mode)
	{// �ǂ̃��[�h�H
	case MODE_TITLE:		// �^�C�g��
		UpdateTitle();
		break;

	case MODE_TUTORIAL:		// �`���[�g���A��
		//UpdateTutorial();
		break;

	case MODE_OPTION:		// �I�v�V����
		UpdateOption();
		break;

	case MODE_GAME:			// �Q�[��
		UpdateGame();
		break;

	case MODE_NONE:
		/* �����Ȃ� */
		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void DrawMode(void)
{
	switch (s_mode)
	{// �ǂ̃��[�h�H
	case MODE_TITLE:		// �^�C�g��
		DrawTitle();
		break;

	case MODE_TUTORIAL:		// �`���[�g���A��
		//DrawTutorial();
		break;

	case MODE_OPTION:		// �I�v�V����
		DrawOption();
		break;

	case MODE_GAME:			// �Q�[��
		DrawGame();
		break;

	case MODE_NONE:
		/* �����Ȃ� */
		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
// ���[�h�̐ݒ�
//--------------------------------------------------
void SetMode(void)
{
	if (s_modeNext == MODE_NONE)
	{// ���̃��[�h�����܂��ĂȂ�
		return;
	}

	if (GetFade() == FADE_NONE)
	{// �������Ă��Ȃ���ԂȂ�
		StartFadeOut();
	}

	if (GetFade() != FADE_IN)
	{// �t�F�[�h�C����Ԃ���Ȃ�
		return;
	}

	switch (s_mode)
	{// ���݂̃��[�h�̏I��
	case MODE_TITLE:		// �^�C�g��
		UninitTitle();
		break;

	case MODE_TUTORIAL:		// �`���[�g���A��
		//UninitTutorial();
		break;

	case MODE_OPTION:		// �I�v�V����
		UninitOption();
		break;

	case MODE_GAME:			// �Q�[��
		UninitGame();
		break;

	case MODE_NONE:
		/* �����Ȃ� */
		break;

	default:
		assert(false);
		break;
	}

	switch (s_modeNext)
	{// ���̃��[�h�̏�����
	case MODE_TITLE:		// �^�C�g��
		InitTitle();
		break;

	case MODE_TUTORIAL:		// �`���[�g���A��
		//InitTutorial();
		break;

	case MODE_OPTION:		// �I�v�V����
		InitOption();
		break;

	case MODE_GAME:			// �Q�[��
		InitGame();
		break;

	case MODE_NONE:
	default:
		assert(false);
		break;
	}

	s_mode = s_modeNext;		// ���݂̉��(���[�h)��؂�ւ���
	s_modeNext = MODE_NONE;
}

//--------------------------------------------------
// ���[�h�̎擾
//--------------------------------------------------
MODE GetMode(void)
{
	return s_mode;
}

//--------------------------------------------------
// ���[�h�̕ύX
//--------------------------------------------------
void ChangeMode(MODE mode)
{
	s_modeNext = mode;
}
