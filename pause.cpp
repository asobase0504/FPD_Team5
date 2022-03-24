//==================================================
//
// �`�[������<�t���C���O�p���[�f�B�X�N>( pause.cpp )
// Author  : katsuki mizuki
// Author  : Kishimoto Eiji
//
//==================================================

//--------------------------------------------------
// �C���N���[�h
//--------------------------------------------------
#include "main.h"
#include "input.h"
#include "fade.h"
#include "menu.h"
#include "game.h"
#include "mode.h"

#include <assert.h>

//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define MENU_WIDTH		(500.0f)								// ���j���[�̕�
#define MENU_HEIGHT		(150.0f)								// ���j���[�̍���
#define MENU_LEFT		(SCREEN_WIDTH * 0.25f)					// ���j���[�̍��[
#define MENU_RIGHT		(SCREEN_WIDTH * 0.75f)					// ���j���[�̉E�[
#define MENU_TOP		(SCREEN_HEIGHT * 0.1f)					// ���j���[�̏�[
#define MENU_BOTTOM		(SCREEN_HEIGHT * 0.9f)					// ���j���[�̉��[
#define BG_COLOR		(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.6f))		// �w�i�̐F
#define FRAME_COLOR		(D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f))		// �t���[���̐F

//--------------------------------------------------
// �񋓌^
//--------------------------------------------------
typedef enum
{
	MENU_GAME = 0,		// �Q�[��
	MENU_TITLE,			// �^�C�g��
	MENU_MAX
}MENU;

//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffBG = NULL;			// �w�i�̒��_�o�b�t�@�ւ̃|�C���^
static LPDIRECT3DTEXTURE9			s_pTextureMenu[MENU_MAX];		// ���j���[�̃e�N�X�`���ւ̃|�C���^
static int							s_nSelectMenu;					// �I�΂�Ă��郁�j���[
static int							s_nIdxMenu;						// �g���Ă��郁�j���[�̔ԍ�

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
static void Input(void);

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitPause(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	s_nSelectMenu = 0;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/WORD/FPD_PauseUI_ReturnToGame.png",
								&s_pTextureMenu[MENU_GAME]);

	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/WORD/FPD_PauseUI_BackToTitle.png",
								&s_pTextureMenu[MENU_TITLE]);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&s_pVtxBuffBG,
								NULL);

	VERTEX_2D *pVtx = NULL;		// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);

	float fWidth = SCREEN_WIDTH * 0.5f;
	float fHeight = SCREEN_HEIGHT* 0.5f;

	D3DXVECTOR3 pos = D3DXVECTOR3(fWidth, fHeight, 0.0f);

	// ���_���W�̐ݒ�
	pVtx[0].pos = pos + D3DXVECTOR3(-fWidth, -fHeight, 0.0f);
	pVtx[1].pos = pos + D3DXVECTOR3( fWidth, -fHeight, 0.0f);
	pVtx[2].pos = pos + D3DXVECTOR3(-fWidth,  fHeight, 0.0f);
	pVtx[3].pos = pos + D3DXVECTOR3( fWidth,  fHeight, 0.0f);

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = BG_COLOR;
	pVtx[1].col = BG_COLOR;
	pVtx[2].col = BG_COLOR;
	pVtx[3].col = BG_COLOR;

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuffBG->Unlock();
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitPause(void)
{
	for (int i = 0; i < MENU_MAX; i++)
	{
		if (s_pTextureMenu[i] != NULL)
		{// �e�N�X�`���̉��
			s_pTextureMenu[i]->Release();
			s_pTextureMenu[i] = NULL;
		}
	}
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdatePause(void)
{
	// ����
	Input();
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void DrawPause(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffBG, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
							0,						// �`�悷��ŏ��̒��_�C���f�b�N�X
							2);						// �`�悷��v���~�e�B�u��
}

//--------------------------------------------------
// �ݒ�
//--------------------------------------------------
void SetPause(void)
{
	//���j���[�̃��Z�b�g
	ResetMenu(s_nIdxMenu);

	//���j���[�̐ݒ�
	MenuArgument menu;
	menu.nNumUse = MENU_MAX;
	menu.fLeft = MENU_LEFT;
	menu.fRight = MENU_RIGHT;
	menu.fTop = MENU_TOP;
	menu.fBottom = MENU_BOTTOM;
	menu.fWidth = MENU_WIDTH;
	menu.fHeight = MENU_HEIGHT;
	menu.bSort = true;

	for (int i = 0; i < MENU_MAX; i++)
	{
		menu.pTexture[i] = s_pTextureMenu[i];
	}
	
	//���j���[�̘g�̐ݒ�
	FrameArgument Frame;
	Frame.bUse = false;
	Frame.col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
	Frame.pTexture = NULL;

	// ���j���[�̐ݒ�
	s_nIdxMenu = SetMenu(menu, Frame);
}

//--------------------------------------------------
// ����
//--------------------------------------------------
static void Input(void)
{
	if (GetFade() != FADE_NONE)
	{// �t�F�[�h���Ă���
		return;
	}

	if (GetKeyboardTrigger(DIK_W) || GetJoypadTrigger(JOYKEY_UP, 0))
	{// W�L�[�������ꂽ���ǂ���
		// �I�����̐F�̏�����
		InitColorOption();

		s_nSelectMenu = ((s_nSelectMenu - 1) + MENU_MAX) % MENU_MAX;

		// �I�����̕ύX
		ChangeOption(s_nSelectMenu);

	}
	else if (GetKeyboardTrigger(DIK_S) || GetJoypadTrigger(JOYKEY_DOWN, 0))
	{// S�L�[�������ꂽ���ǂ���
		// �I�����̐F�̏�����
		InitColorOption();

		s_nSelectMenu = ((s_nSelectMenu + 1) + MENU_MAX) % MENU_MAX;

		// �I�����̕ύX
		ChangeOption(s_nSelectMenu);
	}

	if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(JOYKEY_START, 0))
	{//����L�[(ENTER�L�[)�������ꂽ���ǂ���
		switch (s_nSelectMenu)
		{
		case MENU_GAME:			// �Q�[��
			SetEnablePause(false);
			ResetMenu(s_nIdxMenu);
			break;

		case MENU_TITLE:		// �^�C�g��
			ChangeMode(MODE_TITLE);
			break;

		default:
			assert(false);
			break;
		}

		// �I�����̌���
		DecisionOption();
	}
}