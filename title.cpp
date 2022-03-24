//==================================================
//
// �^�C�g����� ( title.cpp )
// Author : Yuda Kaito
// Author:Kishimoto Eiji
//
//==================================================
#include "title.h"
#include "menu.h"
#include "fade.h"
#include "input.h"
#include "mode.h"
#include "gear.h"
#include "sound.h"

#include <assert.h>

//**************************************************
// �}�N����`
//**************************************************
#define MAX_TEXTURE		(3)			//�g�p����e�N�X�`���̍ő吔
#define MENU_WIDTH		(400.0f)	//���j���[�̕�
#define MENU_HEIGHT		(100.0f)	//���j���[�̍���

//**************************************************
//�^�C�g�����j���[�̗񋓌^
//**************************************************
typedef enum
{
	MENU_GAMESTART = 0,	//�Q�[���J�n
	MENU_TUTORIAL,		//�`���[�g���A��
	MENU_EXIT,			//�I��
	MENU_MAX
}MENU;

//**************************************************
//�X�^�e�B�b�N�ϐ�
//**************************************************
static LPDIRECT3DTEXTURE9		s_pTexture = NULL;				//�e�N�X�`���ւ̃|�C���^(�^�C�g���w�i)
static LPDIRECT3DTEXTURE9		s_pTextureTitle = NULL;			//�e�N�X�`���ւ̃|�C���^(�^�C�g���w�i)
static LPDIRECT3DTEXTURE9		s_apTextureMenu[MAX_TEXTURE];	//�e�N�X�`���ւ̃|�C���^(�^�C�g�����j���[)
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuff = NULL;				//���_�o�b�t�@�ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffTitle = NULL;				//���_�o�b�t�@�ւ̃|�C���^
static int						s_nSelectMenu;					//�I�΂�Ă��郁�j���[

//**************************************************
//�v���g�^�C�v�錾
//**************************************************
static void SelectMenu(void);

//==================================================
// ������
//==================================================
void InitTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾
	VERTEX_2D *pVtx;							//���_���ւ̃|�C���^

	//�e�N�X�`���|�C���^�̏�����
	memset(s_apTextureMenu, NULL, sizeof(s_apTextureMenu));

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/OldPaper.png",
								&s_pTexture);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
								NULL,
								&s_pTextureTitle);

	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/TitleUI/FPD_TitleUI_GameStart.png",
								&s_apTextureMenu[MENU_GAMESTART]);

	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/TitleUI/FPD_TitleUI_Tutorial.png",
								&s_apTextureMenu[MENU_TUTORIAL]);

	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/TitleUI/FPD_TitleUI_GameEnd.png",
								&s_apTextureMenu[MENU_EXIT]);

	//�ϐ��̏�����
	s_nSelectMenu = 0;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffTitle,
		NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 size = D3DXVECTOR3(560.0f, 96.0f, 0.0f);
	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 150.0f, 0.0f);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(pos.x - size.x, pos.y - size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pos.x + size.x, pos.y - size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pos.x - size.x, pos.y + size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pos.x + size.x, pos.y + size.y, 0.0f);

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuffTitle->Unlock();


	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&s_pVtxBuff,
								NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();

	/*********** ���j���[�̃Z�b�g **********/

	//���j���[�̈����̏��
	MenuArgument menu;
	menu.nNumUse	= MENU_MAX;
	menu.fLeft		= 0.0f;
	menu.fRight		= SCREEN_WIDTH;
	menu.fTop		= SCREEN_HEIGHT * 0.5f;
	menu.fBottom	= SCREEN_HEIGHT;
	menu.fWidth		= MENU_WIDTH;
	menu.fHeight	= MENU_HEIGHT;

	for (int i = 0; i < MENU_MAX; i++)
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

	InitGear();				//���Ԃ̏���������
	
	//===============================================================================
	//�^�C�g����ʂ̎��Ԃ̐ݒ�
	/*SetGear(D3DXVECTOR3(353.0f, 273.0f, 0.0f), 150.0f, -D3DX_PI * 0.015f, 0);
	SetGear(D3DXVECTOR3(690.0f, 410.0f, 0.0f), 200.0f, -D3DX_PI * 0.005f, 1);
	SetGear(D3DXVECTOR3(1020.0f, 40.0f, 0.0f), 250.0f, D3DX_PI * 0.011f, 2);
	SetGear(D3DXVECTOR3(350.0f, 500.0f, 0.0f), 230.0f, D3DX_PI * 0.009f, 3);
	SetGear(D3DXVECTOR3(950.0f, 460.0f, 0.0f), 280.0f, -D3DX_PI * 0.008f, 4);
	SetGear(D3DXVECTOR3(45.0f, 0.0f, 0.0f), 280.0f, -D3DX_PI * 0.018f, 2);
	SetGear(D3DXVECTOR3(750.0f, 730.0f, 0.0f), 300.0f, D3DX_PI * 0.008f, 1);
	SetGear(D3DXVECTOR3(1250.0f, 700.0f, 0.0f), 300.0f, D3DX_PI * 0.022f, 0);
	SetGear(D3DXVECTOR3(1145.0f, 415.0f, 0.0f), 220.0f, D3DX_PI * 0.008f, 4);
	SetGear(D3DXVECTOR3(165.0f, 625.0f, 0.0f), 300.0f, -D3DX_PI * 0.009f, 3);
	SetGear(D3DXVECTOR3(60.0f, 235.0f, 0.0f), 280.0f, D3DX_PI * 0.018f, 2);
	SetGear(D3DXVECTOR3(460.0f, 340.0f, 0.0f), 150.0f, D3DX_PI * 0.015f, 0);
	SetGear(D3DXVECTOR3(500.0f, 0.0f, 0.0f), 100.0f, -D3DX_PI * 0.03f, 4);
	SetGear(D3DXVECTOR3(565.0f, 45.0f, 0.0f), 100.0f, D3DX_PI * 0.03f, 4);*/
	//===============================================================================

	SetGear(D3DXVECTOR3(1235.0f, SCREEN_HEIGHT - 597.0f, 0.0f), 280.0f, -D3DX_PI * 0.008f, 4);
	SetGear(D3DXVECTOR3(1050.0f, SCREEN_HEIGHT - 560.0f, 0.0f), 170.0f, D3DX_PI * 0.008f, 3);
	SetGear(D3DXVECTOR3(865.0f,  SCREEN_HEIGHT - 595.0f, 0.0f), 280.0f, -D3DX_PI * 0.008f, 4);
	SetGear(D3DXVECTOR3(660.0f,  SCREEN_HEIGHT - 555.0f, 0.0f), 240.0f, D3DX_PI * 0.008f, 3);
	SetGear(D3DXVECTOR3(495.0f,  SCREEN_HEIGHT - 590.0f, 0.0f), 170.0f, -D3DX_PI * 0.008f, 3);
	SetGear(D3DXVECTOR3(348.0f,  SCREEN_HEIGHT - 618.0f, 0.0f), 200.0f, D3DX_PI * 0.008f, 4);
	SetGear(D3DXVECTOR3(165.0f,  SCREEN_HEIGHT - 585.0f, 0.0f), 260.0f, -D3DX_PI * 0.008f, 4);

	SetGear(D3DXVECTOR3(1235.0f, 597.0f, 0.0f), 280.0f, -D3DX_PI * 0.008f, 4);
	SetGear(D3DXVECTOR3(1050.0f, 560.0f, 0.0f), 170.0f, D3DX_PI * 0.008f, 3);
	SetGear(D3DXVECTOR3(865.0f, 595.0f, 0.0f), 280.0f, -D3DX_PI * 0.008f, 4);
	SetGear(D3DXVECTOR3(660.0f, 555.0f, 0.0f), 240.0f, D3DX_PI * 0.008f, 3);
	SetGear(D3DXVECTOR3(495.0f, 590.0f, 0.0f), 170.0f, -D3DX_PI * 0.008f, 3);
	SetGear(D3DXVECTOR3(348.0f, 618.0f, 0.0f), 200.0f, D3DX_PI * 0.008f, 4);
	SetGear(D3DXVECTOR3(165.0f, 585.0f, 0.0f), 260.0f, -D3DX_PI * 0.008f, 4);

	SetGear(D3DXVECTOR3(530.0f, 359.0f, 0.0f), 320.0f, -D3DX_PI * 0.008f, 3);
	SetGear(D3DXVECTOR3(65.0f, 280.0f, 0.0f), 170.0f, D3DX_PI * 0.008f, 4);
	SetGear(D3DXVECTOR3(65.0f, 440.0f, 0.0f), 170.0f, D3DX_PI * 0.008f, 4);
	SetGear(D3DXVECTOR3(170.0f, 360.0f, 0.0f), 150.0f, -D3DX_PI * 0.008f, 4);
}

//==================================================
// �I��
//==================================================
void UninitTitle(void)
{
	UninitMenu();	//���j���[
	UninitGear();	//���Ԃ̏I������

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
}

//==================================================
// �X�V
//==================================================
void UpdateTitle(void)
{
	UpdateGear();	//���Ԃ̍X�V����
	SelectMenu();	//���j���[�I��
	UpdateMenu();	//���j���[�X�V
}

//==================================================
// �`��
//==================================================
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTexture);

	//�`���[�g���A���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
							0,						//�`�悷��ŏ��̒��_�C���f�b�N�X
							2);						//�`�悷��v���~�e�B�u��

	DrawGear();		//���Ԃ̕`�揈��
	DrawMenu();		//���j���[�`��

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffTitle, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTextureTitle);

	//�`���[�g���A���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
							0,						//�`�悷��ŏ��̒��_�C���f�b�N�X
							2);						//�`�悷��v���~�e�B�u��
}

//--------------------------------------------------
// ���j���[�̑I��
//--------------------------------------------------
static void SelectMenu(void)
{
	if (GetFade() != FADE_NONE)
	{// �t�F�[�h���Ă���
		return;
	}

	if (GetKeyboardTrigger(DIK_W) || GetJoypadTrigger(JOYKEY_UP, 0))
	{// W�L�[�������ꂽ���ǂ���
		InitColorOption();

		s_nSelectMenu = ((s_nSelectMenu - 1) + MENU_MAX) % MENU_MAX;

		ChangeOption(s_nSelectMenu);

		PlaySound(SOUND_LABEL_SE_SELECT);
	}
	else if (GetKeyboardTrigger(DIK_S) || GetJoypadTrigger(JOYKEY_DOWN, 0))
	{// S�L�[�������ꂽ���ǂ���
		InitColorOption();

		s_nSelectMenu = ((s_nSelectMenu + 1) + MENU_MAX) % MENU_MAX;

		ChangeOption(s_nSelectMenu);

		PlaySound(SOUND_LABEL_SE_SELECT);
	}

	if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(JOYKEY_START, 0))
	{//����L�[(ENTER�L�[)�������ꂽ���ǂ���
		switch (s_nSelectMenu)
		{
		case MENU_GAMESTART:	//�Q�[���J�n

			ChangeMode(MODE_OPTION);

			break;

		case MENU_TUTORIAL:		//�ݒ�

			//SetFade(MODE_RANKING);	//�ݒ��ʂ�

			break;

		case MENU_EXIT:		//�I��

			ExitExe();

			break;

		default:
			assert(false);
			break;
		}
		PlaySound(SOUND_LABEL_SE_YES);
	}
}