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
#include "stgfg.h"
#include "pipe.h"
#include "gear.h"

//------------------------------------
// �}�N����`
//------------------------------------
#define RESET_DELAY	(20)	// ���J�n�܂ł̒x��

//------------------------------------
// �ÓI�ϐ�
//------------------------------------
static int s_nNeedTime;			// �K�v�^�C����
static int s_nNeedPoint;		// �K�v�|�C���g��
static int s_nNeedSet;			// �K�v�Z�b�g��
static int s_nPlayerSet[2];		// �v���C���[���ǂ�قǃZ�b�g������Ă��邩
static bool bIsResult;			// ���U���g�̕\������
static bool bIsResetGame;		// �X�R�A���Z�b�g����
static int s_nResetGamePosCnt;	// ����̈ʒu�ɖ߂��Ăǂ�قǌo�߂�����

//------------------------------------
// �v���g�^�C�v�錾
//------------------------------------
static void RoundReset(void);

//=========================================
// ������
//=========================================
void InitGame(void)
{
	InitDisk();			// �f�B�X�N
	InitShadow();		// �e
	InitPlayer();		// �v���C���[
	InitUI();			// UI
	InitStage();		// �X�e�[�W
	InitStgFg();		// �X�e�[�W�O�i
	InitLandingMark();	// �f�B�X�N�̗����n�_
	InitEffect();		// �G�t�F�N�g
	InitResult();		// ���U���g
	InitPipe();			// �z��
	InitGear();			// ����

	// ������
	s_nPlayerSet[0] = 0;
	s_nPlayerSet[1] = 0;
	s_nResetGamePosCnt = 0;
	bIsResult = false;
	RoundReset();

	SetPipe(D3DXVECTOR3(150.0f, 70.0f, 0.0f), D3DXVECTOR3(25.5f, 70.0f, 0.0f), PIPE_TYPE_VERTICAL);
	SetPipe(D3DXVECTOR3(250.0f, 70.0f, 0.0f), D3DXVECTOR3(25.5f, 70.0f, 0.0f), PIPE_TYPE_VERTICAL);
	SetPipe(D3DXVECTOR3(350.0f, 70.0f, 0.0f), D3DXVECTOR3(25.5f, 70.0f, 0.0f), PIPE_TYPE_VERTICAL);
	SetPipe(D3DXVECTOR3(450.0f, 70.0f, 0.0f), D3DXVECTOR3(25.5f, 70.0f, 0.0f), PIPE_TYPE_VERTICAL);
	SetPipe(D3DXVECTOR3(420.0f, 35.0f, 0.0f), D3DXVECTOR3(70.0f, 21.85f, 0.0f), PIPE_TYPE_STEAM);
	SetPipe(D3DXVECTOR3(420.0f, 90.0f, 0.0f), D3DXVECTOR3(70.0f, 21.85f, 0.0f), PIPE_TYPE_STEAM);
	SetPipe(D3DXVECTOR3(820.0f, 70.0f, 0.0f), D3DXVECTOR3(60.0f, 21.85f, 0.0f), PIPE_TYPE_STEAM);
	SetGear(D3DXVECTOR3(1060.0f, 40.0f, 0.0f), 100.0f, -D3DX_PI * 0.008f, 2);
	SetGear(D3DXVECTOR3(1116.0f, 90.0f, 0.0f), 70.0f, D3DX_PI * 0.008f, 2);
	SetGear(D3DXVECTOR3(1174.0f, 52.0f, 0.0f), 90.0f, -D3DX_PI * 0.008f, 2);
	SetGear(D3DXVECTOR3(1265.0f, 81.0f, 0.0f), 130.0f, D3DX_PI * 0.008f, 2);
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
	UninitStgFg();			// �X�e�[�W�O�i
	UninitLandingMark();	// �f�B�X�N�̗����n�_
	UninitEffect();			// �G�t�F�N�g
	UninitUI();				// UI
	UninitResult();			// ���U���g
	UninitPipe();			// �z��
	UninitGear();			// ����
}

//=========================================
// �X�V
//=========================================
void UpdateGame(void)
{
	if (bIsResult)
	{ // ���U���g��
		UpdateResult();			// ���U���g
	}
	else
	{
		if (bIsResetGame && ResetPosPlayer())
		{
			s_nResetGamePosCnt++;
			if (s_nResetGamePosCnt == RESET_DELAY)
			{
				DestroyDisk();
				GetReferee()->bThrow = true;
			}

			if (GetPlayer()[0].bHaveDisk || GetPlayer()[1].bHaveDisk)
			{
				s_nResetGamePosCnt = 0;
				bIsResetGame = false;	// ���Z�b�g�̏I��
			}
		}

		UpdateStage();			// �X�e�[�W
		UpdateStgFg();			// �X�e�[�W�O�i
		UpdatePlayer();			// �v���C���[
		UpdateDisk();			// �f�B�X�N
		UpdateLandingMark();	// �f�B�X�N�̗����n�_
		UpdateShadow();			// �e
		UpdateEffect();			// �G�t�F�N�g
		UpdateUI();				// UI
		UpdatePipe();			// �z��
		UpdateGear();			// ����
	}

	// �X�R�A�֌W�̏���
	{
		SCORE* pScore = GetScore();

		// �X�R�A�����ȏォ�ۂ�
		for (int i = 0; i < 2; i++)
		{
			if (pScore[i].nScore >= s_nNeedPoint)
			{
				// �P�Z�b�g�̃��Z�b�g
				s_nPlayerSet[i]++;	// �Z�b�g���̎擾
				RoundReset();
			}
		}

		if (GetTime()->nTime <= 0)
		{
			DestroyDisk();	// �f�B�X�N�̍폜
			if (s_nPlayerSet[0] > s_nPlayerSet[1])
			{ // P1�̏���
				s_nPlayerSet[0]++;	// �Z�b�g���̎擾
				SetThoThrowRefreeIdx(1);	// ����������̑I��
				RoundReset();
			}
			else if (s_nPlayerSet[0] < s_nPlayerSet[1])
			{ // P2�̏���
				s_nPlayerSet[1]++;	// �Z�b�g���̎擾
				SetThoThrowRefreeIdx(0);	// ����������̑I��
				RoundReset();
			}
			else
			{ // ��������
				RoundReset();
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
	DrawGear();			// ����
	DrawShadow();		// �e
	DrawLandingMark();	// �f�B�X�N�̗����n�_
	DrawEffect();		// �G�t�F�N�g
	DrawPipe();			// �z��

	if (GetDisk()->type == DISK_TYPE_LOB)
	{
		DrawPlayer();		// �v���C���[
		DrawDisk();			// �f�B�X�N
	}
	else
	{
		DrawDisk();			// �f�B�X�N
		DrawPlayer();		// �v���C���[
	}
	DrawStgFg();		// �X�e�[�W�O�i

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
bool* GetResetScore(void)
{
	return &bIsResetGame;
}

//=========================================
// ���E���h���Z�b�g
//=========================================
void RoundReset(void)
{
	SCORE* pScore = GetScore();
	Player * pPlayer = GetPlayer();

	for (int nIdxPlayer = 0; nIdxPlayer < 2; nIdxPlayer++, pPlayer++)
	{
		AddScore(-pScore[nIdxPlayer].nScore, 0);	// �X�R�A�̃��Z�b�g
		pPlayer->bHaveDisk = false;
	}

	SetTime(s_nNeedTime);				// �^�C�����Z�b�g
	bIsResetGame = true;
}
