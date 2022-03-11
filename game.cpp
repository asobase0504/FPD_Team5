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
	InitShadow();
	InitDisk();
	InitStage();
	InitEffect();
}

//=========================================
// �I��
//=========================================
void UninitGame(void)
{
	UninitPlayer();
	UninitWall();
	UninitShadow();
	UninitDisk();
	UninitStage();
	UninitEffect();
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
	UpdateShadow();
	UpdateEffect();
}

//=========================================
// �`��
//=========================================
void DrawGame()
{
	DrawStage();
	DrawPlayer();
	DrawWall();
	DrawShadow();
	DrawDisk();
	DrawEffect();
}