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
#include "ui.h"

//------------------------------------
// �ÓI�ϐ�
//------------------------------------
static int nNeedPoint;

//------------------------------------
// �v���g�^�C�v�錾
//------------------------------------
static void SetNeedPoint(int nPoint);

//=========================================
// ������
//=========================================
void InitGame(void)
{
	InitWall();			// ��
	InitDisk();			// �f�B�X�N
	InitShadow();		// �e
	InitPlayer();		// �v���C���[
	InitStage();		// �X�e�[�W
	InitLandingMark();	// �f�B�X�N�̗����n�_
	InitEffect();		// �G�t�F�N�g
	InitReferee();		// ���t�F���[
	InitUI();			// UI

	GetReferee()->bThrow = true;
}

//=========================================
// �I��
//=========================================
void UninitGame(void)
{
	UninitPlayer();			// �v���C���[
	UninitWall();			// ��
	UninitShadow();			// �e
	UninitDisk();			// �f�B�X�N
	UninitStage();			// �X�e�[�W
	UninitLandingMark();	// �f�B�X�N�̗����n�_
	UninitEffect();			// �G�t�F�N�g
	UninitReferee();		// ���t�F���[
	UninitUI();				// UI
}

//=========================================
// �X�V
//=========================================
void UpdateGame(void)
{
	UpdateStage();			// �X�e�[�W
	UpdatePlayer();			// �v���C���[
	UpdateWall();			// ��
	UpdateDisk();			// �f�B�X�N
	UpdateLandingMark();	// �f�B�X�N�̗����n�_
	UpdateShadow();			// �e
	UpdateEffect();			// �G�t�F�N�g
	UpdateReferee();		// ���t�F���[
	UpdateUI();				// UI
}

//=========================================
// �`��
//=========================================
void DrawGame()
{
	DrawStage();		// �X�e�[�W
	DrawWall();			// ��
	DrawShadow();		// �e
	DrawLandingMark();	// �f�B�X�N�̗����n�_
	DrawEffect();		// �G�t�F�N�g
	DrawDisk();			// �f�B�X�N
	DrawPlayer();		// �v���C���[
	DrawReferee();		// ���t�F���[
	DrawUI();			// UI
}

//=========================================
// �K�v�|�C���g�̐ݒ�
//=========================================
void SetNeedPoint(int nPoint)
{
	nNeedPoint = nPoint;
}
