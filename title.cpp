//==================================================
//
// �^�C�g����� ( title.cpp )
// Author  : katsuki mizuki
// Author:Teruto Sato
// Author:Kishimoto Eiji
//
//==================================================
#include "title.h"
#include "menu.h"
#include "fade.h"
//#include "input,h"

#include <assert.h>

//**************************************************
// �}�N����`
//**************************************************
#define MENU_WIDTH		(500.0f)	//���j���[�̕�
#define MENU_HEIGHT		(60.0f)		//���j���[�̍���

//**************************************************
//�^�C�g�����j���[�̗񋓌^
//**************************************************
typedef enum
{
	MENU_GAMESTART = 0,	//�Q�[���J�n
	MENU_OPTION,		//�ݒ�
	MENU_EXIT,			//�I��
	MENU_MAX
}MENU;

//**************************************************
//�X�^�e�B�b�N�ϐ�
//**************************************************
static LPDIRECT3DTEXTURE9		s_pTexture = NULL;		//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuff = NULL;		//���_�o�b�t�@�ւ̃|�C���^
static int						s_nSelectMenu;			//�I�΂�Ă��郁�j���[

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
	//memset(s_pTexture, NULL, sizeof(s_pTexture));

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/��łȂ񂩓����.png",
								&s_pTexture);

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

	//�g�̈����̏��
	FrameArgument frame;
	frame.pTexture = NULL;
	frame.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	frame.bUse = false;

	//���j���[�̃Z�b�g
	InitMenu();
	SetMenu(menu, frame);
}

//==================================================
// �I��
//==================================================
void UninitTitle(void)
{
	UninitMenu();

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
	UpdateMenu();
}

//==================================================
// �`��
//==================================================
void DrawTitle(void)
{
	DrawMenu();

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
}

//--------------------------------------------------
// ����
//--------------------------------------------------
static void SelectMenu(void)
{
	if (GetFade() != FADE_NONE)
	{// �t�F�[�h���Ă���
		return;
	}

	//if (GetKeyboardTrigger(DIK_W) || GetJoypadTrigger(JOYKEY_UP, 0))
	//{// W�L�[�������ꂽ���ǂ���
	//	s_nSelectMenu = ((s_nSelectMenu - 1) + MENU_MAX) % MENU_MAX;

	//}
	//else if (GetKeyboardTrigger(DIK_S) || GetJoypadTrigger(JOYKEY_DOWN, 0))
	//{// S�L�[�������ꂽ���ǂ���
	//	s_nSelectMenu = ((s_nSelectMenu + 1) + MENU_MAX) % MENU_MAX;
	//}

	//if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(JOYKEY_START, 0))
	{//����L�[(ENTER�L�[)�������ꂽ���ǂ���
		switch (s_nSelectMenu)
		{
		case MENU_GAMESTART:		//�Q�[���J�n

			SetFade(MODE_GAME);		//�Q�[����ʂ�

			break;

		case MENU_OPTION:			//�ݒ�

			//SetFade(MODE_RANKING);	//�ݒ��ʂ�

			break;

		case MENU_EXIT:				//�I��

									/* ���������A��낵�����肢�������܂� */

			break;

		default:
			assert(false);
			break;
		}
	}
}