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
//#include "effect.h"
#include"score.h"
#include "time.h"
#include "referee.h"
#include "landingPoint.h"

//=========================================
// ������
//=========================================
void InitGame(void)
{
	InitWall();		// ��
	InitDisk();		// �f�B�X�N
	InitShadow();	// �e
	InitPlayer();	// �v���C���[
	InitStage();	// �X�e�[�W
	InitLandingMark();
	//InitEffect();
	InitScore();
	//InitTime();
	InitRef();
}

//=========================================
// �I��
//=========================================
void UninitGame(void)
{
	UninitPlayer();	// �v���C���[
	UninitWall();	// ��
	UninitShadow();	// �e
	UninitDisk();	// �f�B�X�N
	UninitStage();	// �X�e�[�W
	UninitLandingMark();
//	UninitEffect();
	UninitScore();
	//UninitTime();
	UninitRef();
}

//=========================================
// �X�V
//=========================================
void UpdateGame(void)
{
	UpdateStage();	// �X�e�[�W
	UpdatePlayer();	// �v���C���[
	UpdateWall();	// ��
	UpdateDisk();	// �f�B�X�N
	UpdateLandingMark();
	UpdateShadow();	// �e
//	UpdateEffect();
	UpdateScore();
	//UpdateTime();
	UpdateRef();
}

//=========================================
// �`��
//=========================================
void DrawGame()
{
	DrawStage();	// �X�e�[�W
	DrawWall();		// ��
	DrawShadow();	// �e
	DrawLandingMark();
	DrawPlayer();	// �v���C���[
	DrawDisk();		// �f�B�X�N
//	DrawEffect();
	DrawScore();
	//DrawTime();
	DrawRef();
}