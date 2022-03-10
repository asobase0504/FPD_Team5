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
	InitPlayer();
	InitWall();
	InitDisk();
	InitStage();
}

//=========================================
// �I��
//=========================================
void UninitGame(void)
{
	UninitPlayer();
	UninitWall();
	UninitDisk();
	UninitStage();
}

//=========================================
// �X�V
//=========================================
void UpdateGame(void)
{
	UpdateStage();
	UpdatePlayer();
	UpdateWall();
	UpdateDisk();
}

//=========================================
// �`��
//=========================================
void DrawGame()
{
	DrawPlayer();
	DrawWall();
	DrawDisk();
	DrawStage();
}