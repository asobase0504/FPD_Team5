//============================================
//
//�`�[������<�t���C���O�p���[�f�B�X�N>[option.cpp]
//Author: goto yuuki
//Author:Kishimoto Eiji
//
//============================================
#include "option.h"
#include "select.h"
#include "fade.h"
#include "input.h"
#include "menu.h"
#include "mode.h"
#include "game.h"
#include "gear.h"
#include "sound.h"

#include <assert.h>

//***********************************
//�}�N����`
//***********************************
#define MAX_TEXTURE		(4)			//�g�p����e�N�X�`���̍ő吔
#define MENU_WIDTH		(850.0f)	//���j���[�̕�
#define MENU_HEIGHT		(85.0f)		//���j���[�̍���

//***********************************
//�X�^�e�B�b�N�ϐ�
//***********************************
static LPDIRECT3DTEXTURE9		s_pTexture = NULL;				//�e�N�X�`���ւ̃|�C���^(�w�i)
static LPDIRECT3DTEXTURE9		s_apTextureMenu[MAX_TEXTURE];	//�e�N�X�`���ւ̃|�C���^(���j���[)
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuff = NULL;				//���_�o�b�t�@�ւ̃|�C���^

static int s_nSelectMenuNow;	//���ݑI�΂�Ă��郁�j���[
static int s_nSelectMenuBefore;	//�O��I�΂�Ă������j���[
static int s_nSelectTimeLimit;	//�I��������������
static int s_nSelectPoint;		//�I�������|�C���g��
static int s_nSelectSetCount;	//�I�������Z�b�g��

//***********************************
//�v���g�^�C�v�錾
//***********************************

static void SelectMenu(void);

//============================================
//�I�v�V�����̏�����
//============================================
void InitOption(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/OldPaper.png",
								&s_pTexture);

	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/WORD/Option/FPD_OptionUI_TimenLimit_HGPmintyou.png",
								&s_apTextureMenu[0]);

	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/WORD/Option/FPD_OptionUI_Point_HGPmintyou.png",
								&s_apTextureMenu[1]);

	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/WORD/Option/FPD_OptionUI_SetCount_HGPmintyou.png",
								&s_apTextureMenu[2]);

	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/WORD/Option/FPD_OptionUI_BattleStart_HGPmintyou.png",
								&s_apTextureMenu[3]);

	//�ϐ��̏�����
	s_nSelectMenuNow = 0;
	s_nSelectMenuBefore = 0;
	
	VERTEX_2D * pVtx = NULL;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&s_pVtxBuff,
								NULL);

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(0.8f,0.8f, 0.8f, 1.0f);
	pVtx[1].col = D3DXCOLOR(0.8f,0.8f, 0.8f, 1.0f);
	pVtx[2].col = D3DXCOLOR(0.8f,0.8f, 0.8f, 1.0f);
	pVtx[3].col = D3DXCOLOR(0.8f,0.8f, 0.8f, 1.0f);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();

	/*********** ���j���[�̃Z�b�g **********/

	//���j���[�̈����̏��
	MenuArgument menu;
	menu.nNumUse = OPTION_MAX;
	menu.fLeft = 0.0f;
	menu.fRight = SCREEN_WIDTH;
	menu.fTop = 0.0f;
	menu.fBottom = SCREEN_HEIGHT;
	menu.fWidth = MENU_WIDTH;
	menu.fHeight = MENU_HEIGHT;

	for (int i = 0; i < OPTION_MAX; i++)
	{
		menu.pTexture[i] = s_apTextureMenu[i];
	}

	//�g�̈����̏��
	FrameArgument frame;
	frame.pTexture = NULL;
	frame.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	frame.bUse = false;

	//���j���[�̃Z�b�g
	InitMenu();
	SetMenu(menu, frame);

	InitSelect();

	//���Ԃ̏���������
	InitGear();

	//===============================================================================
	//�I�v�V������ʂ̎��Ԃ̐ݒ�
	SetGear(D3DXVECTOR3(353.0f, 273.0f, 0.0f), 150.0f, -D3DX_PI * 0.015f, 0);
	SetGear(D3DXVECTOR3(690.0f, 410.0f, 0.0f), 200.0f, -D3DX_PI * 0.005f, 1);
	SetGear(D3DXVECTOR3(1020.0f, 40.0f, 0.0f), 250.0f, D3DX_PI * 0.011f, 2);
	SetGear(D3DXVECTOR3(350.0f, 500.0f, 0.0f), 230.0f, D3DX_PI * 0.009f, 3);
	SetGear(D3DXVECTOR3(950.0f, 460.0f, 0.0f), 280.0f, -D3DX_PI * 0.008f, 4);
	SetGear(D3DXVECTOR3(45.0f, 0.0f, 0.0f), 280.0f, -D3DX_PI * 0.018f, 2);
	SetGear(D3DXVECTOR3(750.0f, 730.0f, 0.0f), 300.0f, D3DX_PI * 0.008f, 1);
	SetGear(D3DXVECTOR3(1250.0f, 700.0f, 0.0f), 300.0f, D3DX_PI * 0.022f, 0);
	SetGear(D3DXVECTOR3(1145.0f, 415.0f, 0.0f), 220.0f, D3DX_PI * 0.008f, 4);
	SetGear(D3DXVECTOR3(165.0f, 625.0f, 0.0f), 300.0f, -D3DX_PI * 0.009f, 3);
	SetGear(D3DXVECTOR3(60.0f, 233.0f, 0.0f), 280.0f, D3DX_PI * 0.018f, 2);
	SetGear(D3DXVECTOR3(460.0f, 340.0f, 0.0f), 150.0f, D3DX_PI * 0.015f, 0);
	SetGear(D3DXVECTOR3(500.0f, 0.0f, 0.0f), 100.0f, -D3DX_PI * 0.03f, 4);
	SetGear(D3DXVECTOR3(565.0f, 45.0f, 0.0f), 100.0f, D3DX_PI * 0.03f, 4);
	SetGear(D3DXVECTOR3(260.0f, 45.0f, 0.0f), 240.0f, D3DX_PI * 0.018f, 2);
	//===============================================================================

	// �����l�̓���
	s_nSelectTimeLimit = SelectTimeLimit(s_nSelectMenuNow);
	s_nSelectPoint = SelectPoint(s_nSelectMenuNow);
	s_nSelectSetCount = SelectSetCount(s_nSelectMenuNow);

	//�Q�[���J�n���I������Ă����Ԃɂ���
	s_nSelectMenuNow = OPTION_GOTOGAME;

	ChangeOption(s_nSelectMenuNow);	//�I������ύX
}

//============================================
//�I�v�V�����̏I��
//============================================
void UninitOption(void)
{
	for (int i = 0; i < MAX_TEXTURE; i++)
	{
		if (s_apTextureMenu[i] != NULL)
		{//�e�N�X�`���̔j��
			s_apTextureMenu[i]->Release();
			s_apTextureMenu[i] = NULL;
		}
	}
	if (s_pTexture != NULL)
	{//�e�N�X�`���̔j��
		s_pTexture->Release();
		s_pTexture = NULL;
	}

	if (s_pVtxBuff != NULL)
	{//���_�o�b�t�@�̔j��
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}

	UninitGear();		//���Ԃ̏I������
	UninitSelect();		//�I�𕔕��̏I������
}

//============================================
//�I�v�V�����̍X�V
//============================================
void UpdateOption(void)
{
	s_nSelectMenuBefore = s_nSelectMenuNow;		//�O��I�����Ă����ԍ���ۑ�

	UpdateGear();	//���Ԃ̍X�V����
	SelectMenu();	//���j���[�I��
	UpdateMenu();	//���j���[�X�V
	UpdateSelect();
}

//============================================
//�I�v�V�����̕`��
//============================================
void DrawOption(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTexture);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�|���S���̌`
							0,					//���_�̊J�n�ꏊ
							2);					//�v���~�e�B�u�̐�

	DrawGear();		//���Ԃ̕`�揈��
	DrawMenu();		//���j���[�`��
	DrawSelect();
}

//--------------------------------------------
// ���j���[�̑I��
//--------------------------------------------
static void SelectMenu(void)
{
	if (GetFade() != FADE_NONE)
	{// �t�F�[�h���Ă���
		return;
	}

	if (GetKeyboardTrigger(DIK_W) || GetJoypadTrigger(JOYKEY_UP, 0))
	{// ������( W�L�[ or �\���L�[�� )
		InitColorOption();		//�I�����̐F��������

		//�I��������ɂ��炷
		s_nSelectMenuNow = ((s_nSelectMenuNow - 1) + OPTION_MAX) % OPTION_MAX;

		ChangeOption(s_nSelectMenuNow);	//�I������ύX

		PlaySound(SOUND_LABEL_SE_SELECT);
	}
	else if (GetKeyboardTrigger(DIK_S) || GetJoypadTrigger(JOYKEY_DOWN, 0))
	{// �������( S�L�[ or �\���L�[�� )
		InitColorOption();		//�I�����̐F��������

		//�I���������ɂ��炷
		s_nSelectMenuNow = ((s_nSelectMenuNow + 1) + OPTION_MAX) % OPTION_MAX;

		ChangeOption(s_nSelectMenuNow);	//�I������ύX

		PlaySound(SOUND_LABEL_SE_SELECT);
	}

	switch (s_nSelectMenuNow)
	{
	case OPTION_TIMELIMIT:	//��������

		//�������Ԃ̑I��
		s_nSelectTimeLimit = SelectTimeLimit(s_nSelectMenuNow);
		break;

	case OPTION_POINT:	//�|�C���g��
		
		//�|�C���g���̑I��
		s_nSelectPoint = SelectPoint(s_nSelectMenuNow);
		break;

	case OPTION_SETCOUNT:	//�Z�b�g��

		//�Z�b�g���̑I��
		s_nSelectSetCount = SelectSetCount(s_nSelectMenuNow);
		break;

	case OPTION_GOTOGAME:	//�Q�[���֐i��

		if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(JOYKEY_START, 0))
		{//����L�[(ENTER�L�[)�������ꂽ���ǂ���

			SetNeedTime(s_nSelectTimeLimit);	//��������
			SetNeedPoint(s_nSelectPoint);	//�|�C���g��
			SetNeedSet(s_nSelectSetCount);	//�Z�b�g��

			ChangeMode(MODE_GAME);

			PlaySound(SOUND_LABEL_SE_GEARSTOP);
		}
		break;

	default:
		assert(false);
		break;
	}

	if (GetKeyboardTrigger(DIK_BACKSPACE) || GetJoypadTrigger(JOYKEY_B, 0))
	{
		ChangeMode(MODE_TITLE);
		PlaySound(SOUND_LABEL_SE_NO);
	}
}

//============================================
//���ݑI������Ă��郁�j���[���擾
//============================================
int GetSelectMenuNow(void)
{
	return s_nSelectMenuNow;
}

//============================================
//�O��I������Ă������j���[���擾
//============================================
int GetSelectMenuBefore(void)
{
	return s_nSelectMenuBefore;
}