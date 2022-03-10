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
	InitStage();
	InitPlayer();
	InitWall();
	InitDisk();
}

//=========================================
// �I��
//=========================================
void UninitGame(void)
{
	UninitStage();
	UninitPlayer();
	UninitWall();
	UninitDisk();
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
	DrawStage();
	DrawPlayer();
	DrawWall();
	DrawDisk();
}