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

//------------------------------------
// �}�N����`
//------------------------------------

//------------------------------------
// �X�^�e�B�b�N�ϐ�
//------------------------------------

//=========================================
// ������
//=========================================
void InitGame(void)
{
	InitShadow();	// �e
	InitWall();		// ��
	InitDisk();		// �f�B�X�N
	InitPlayer();	// �v���C���[
	InitStage();	// �X�e�[�W
	//InitEffect();
}

//=========================================
// �I��
//=========================================
void UninitGame(void)
{
	UninitShadow();	// �e
	UninitPlayer();	// �v���C���[
	UninitWall();	// ��
	UninitDisk();	// �f�B�X�N
	UninitStage();	// �X�e�[�W
//	UninitEffect();
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
	UpdateShadow();	// �e
//	UpdateEffect();
}

//=========================================
// �`��
//=========================================
void DrawGame()
{
	DrawStage();	// �X�e�[�W
	DrawWall();		// ��
	DrawShadow();	// �e
	DrawPlayer();	// �v���C���[
	DrawDisk();		// �f�B�X�N
//	DrawEffect();
}