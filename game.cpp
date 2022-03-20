//=========================================
// 
// �Q�[��������
// Author YudaKaito
// 
//=========================================
//------------------------------------
// include
//------------------------------------
#include "main.h"
#include "game.h"
#include "player.h"
#include "input.h"
#include "sound.h"
#include "stage.h"
#include <stdio.h>
#include "wall.h"
#include "disk.h"
#include "shadow.h"
#include "effect.h"
#include "referee.h"
#include "landingPoint.h"
#include "score.h"
#include "result.h"
#include "time.h"
#include "ui.h"

//------------------------------------
// �ÓI�ϐ�
//------------------------------------
static int s_nNeedTime;			// �K�v�^�C����
static int s_nNeedPoint;		// �K�v�|�C���g��
static int s_nNeedSet;			// �K�v�Z�b�g��
static int s_nPlayerSet[2];		// �v���C���[���ǂ�قǃZ�b�g������Ă��邩
static bool bIsResult;			// ���U���g�̕\������
static bool bIsResetRound;		// ���Z�b�g����

//------------------------------------
// �v���g�^�C�v�錾
//------------------------------------

//=========================================
// ������
//=========================================
void InitGame(void)
{
	InitDisk();			// �f�B�X�N
	InitShadow();		// �e
	InitPlayer();		// �v���C���[
	InitStage();		// �X�e�[�W
	InitLandingMark();	// �f�B�X�N�̗����n�_
	InitEffect();		// �G�t�F�N�g
	InitUI();			// UI
	InitResult();		// ���U���g

	// ������
	s_nPlayerSet[0] = 0;
	s_nPlayerSet[1] = 0;
	bIsResult = false;
	bIsResetRound = false;
	GetReferee()->bThrow = true;
	SetTime(s_nNeedTime);
}

//=========================================
// �I��
//=========================================
void UninitGame(void)
{
	UninitPlayer();			// �v���C���[
	UninitShadow();			// �e
	UninitDisk();			// �f�B�X�N
	UninitStage();			// �X�e�[�W
	UninitLandingMark();	// �f�B�X�N�̗����n�_
	UninitEffect();			// �G�t�F�N�g
	UninitUI();				// UI
	UninitResult();			// ���U���g
}

//=========================================
// �X�V
//=========================================
void UpdateGame(void)
{
	if (bIsResult)
	{
		UpdateResult();			// ���U���g
	}
	else
	{
		if (bIsResetRound)
		{	
			if (ResetPosPlayer())
			{
				bIsResetRound = false;
			}
		}

		UpdateStage();			// �X�e�[�W
		UpdatePlayer();			// �v���C���[
		UpdateDisk();			// �f�B�X�N
		UpdateLandingMark();	// �f�B�X�N�̗����n�_
		UpdateShadow();			// �e
		UpdateEffect();			// �G�t�F�N�g
		UpdateUI();				// UI
	}

	// �X�R�A�֌W�̏���
	{
		SCORE* pScore = GetScore();

		// �X�R�A�����ȏォ�ۂ�
		for (int i = 0; i < 2; i++)
		{
			if (pScore[i].nScore[0] >= s_nNeedPoint)
			{
				// �P�Z�b�g�̃��Z�b�g
				s_nPlayerSet[i]++;	// �Z�b�g���̎擾
				AddScore(-pScore[0].nScore[0], 0);	// �X�R�A�̃��Z�b�g
				AddScore(-pScore[1].nScore[0], 1);	// �X�R�A�̃��Z�b�g
				SetTime(s_nNeedTime);				// �^�C�����Z�b�g
				bIsResetRound = true;
			}
		}

		// �Z�b�g�������ȏォ�ۂ�
		if (s_nPlayerSet[0] >= s_nNeedSet)
		{
			bIsResult = true;
		}
		else if (s_nPlayerSet[1] >= s_nNeedSet)
		{
			bIsResult = true;
		}
	}
}

//=========================================
// �`��
//=========================================
void DrawGame()
{
	DrawStage();		// �X�e�[�W
	DrawShadow();		// �e
	DrawLandingMark();	// �f�B�X�N�̗����n�_
	DrawEffect();		// �G�t�F�N�g
	DrawPlayer();		// �v���C���[
	DrawDisk();			// �f�B�X�N
	DrawUI();			// UI

	if (bIsResult)
	{
		DrawResult();		// ���U���g
	}
}

//=========================================
// �K�v�^�C���̐ݒ�
//=========================================
void SetNeedTime(int nPoint)
{
	s_nNeedTime = nPoint;
}

//=========================================
// �K�v�|�C���g�̐ݒ�
//=========================================
void SetNeedPoint(int nPoint)
{
	s_nNeedPoint = nPoint;
}

//=========================================
// �K�v�Z�b�g���̐ݒ�
//=========================================
void SetNeedSet(int nPoint)
{
	s_nNeedSet = nPoint;
}

//=========================================
// ���Z�b�g�����ۂ�
//=========================================
bool GetResetRound(void)
{
	return bIsResetRound;
}
