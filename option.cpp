//============================================
//�ݒ�
//ACG000
//Author: goto yuuki
//Author:Kishimoto Eiji
//
//============================================
#include "option.h"
#include "fade.h"
#include "input.h"
#include "menu.h"

#include <assert.h>

//***********************************
//�}�N����`
//***********************************
#define MAX_TEXTURE		(4)			//�g�p����e�N�X�`���̍ő吔
#define MENU_WIDTH		(400.0f)	//���j���[�̕�
#define MENU_HEIGHT		(140.0f)	//���j���[�̍���

//***********************************
//�I�v�V�����̗񋓌^
//***********************************
typedef enum
{
	OPTION_TIMELIMIT = 0,	//��������
	OPTION_POINT,			//�|�C���g��
	OPTION_SETCOUNT,		//�Z�b�g��
	OPTION_GOTOGAME,		//�Q�[���֐i��
	OPTION_MAX
}OPTION;

//***********************************
//�������Ԃ̗񋓌^
//***********************************
typedef enum
{
	TIMELIMIT_15 = 0,		//15�b
	TIMELIMIT_30,			//30�b
	TIMELIMIT_45,			//45�b
	TIMELIMIT_90,			//90�b
	TIMELIMIT_INFINITY,		//����(�������ԂȂ�)
	TIMELIMIT_MAX
}TIMELIMIT;

//***********************************
//�|�C���g���̗񋓌^
//***********************************
typedef enum
{
	POINTCOUNT_12 = 0,	//12�_
	POINTCOUNT_15,		//15�_
	POINTCOUNT_21,		//21�_
	POINTCOUNT_MAX
}POINTCOUNT;

//***********************************
//�Z�b�g���̗񋓌^
//***********************************
typedef enum
{
	SETCOUNT_ONE = 0,	//1�Z�b�g
	SETCOUNT_TWO,		//2�Z�b�g
	SETCOUNT_THREE,		//3�Z�b�g
	SETCOUNT_MAX
}SETCOUNT;

//***********************************
//�X�^�e�B�b�N�ϐ�
//***********************************
static LPDIRECT3DTEXTURE9		s_pTexture = NULL;				//�e�N�X�`���ւ̃|�C���^(�w�i)
static LPDIRECT3DTEXTURE9		s_apTextureMenu[MAX_TEXTURE];	//�e�N�X�`���ւ̃|�C���^(���j���[)
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuff = NULL;				//���_�o�b�t�@�ւ̃|�C���^

static int s_nSelectMenu;		//�I�΂�Ă��郁�j���[
static int s_nSelectTimeLimit;	//�I�΂�Ă��鐧������
static int s_nSelectPoint;		//�I�΂�Ă���|�C���g��
static int s_nSelectSetCount;	//�I�΂�Ă���Z�b�g��

static int s_nTimeLimit;	//�������Ԃ�ۑ�����
static int s_nPoint;		//�|�C���g����ۑ�����
static int s_nSetCount;		//�Z�b�g����ۑ�����

//***********************************
//�v���g�^�C�v�錾
//***********************************

static void SelectMenu(void);
static int SelectTimeLimit(void);
static int SelectPoint(void);
static int SelectSetCount(void);

//============================================
//�I�v�V�����̏�����
//============================================
void InitOption(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾

	//�e�N�X�`���|�C���^�̏�����
	memset(s_apTextureMenu, NULL, sizeof(s_apTextureMenu));

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/�w�i",
								&s_pTexture);

	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/�^�C�}�[",
								&s_apTextureMenu[0]);

	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/�|�C���g��",
								&s_apTextureMenu[1]);

	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/�Z�b�g��",
								&s_apTextureMenu[2]);

	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/�^�C�g���֖߂�",
								&s_apTextureMenu[2]);

	//�ϐ��̏�����
	s_nSelectMenu = 0;
	s_nSelectTimeLimit = 0;
	s_nSelectPoint = 0;
	s_nSelectSetCount = 0;
	s_nTimeLimit = 0;
	s_nPoint = 0;
	s_nSetCount = 0;

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
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

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
	menu.fTop = SCREEN_HEIGHT * 0.5f;
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

	if (s_pVtxBuff != NULL)
	{//���_�o�b�t�@�̔j��
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//============================================
//�I�v�V�����̍X�V
//============================================
void UpdateOption(void)
{
	SelectMenu();	//���j���[�I��
	UpdateMenu();	//���j���[�X�V
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

	DrawMenu();		//���j���[�`��
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
		s_nSelectMenu = ((s_nSelectMenu - 1) + OPTION_MAX) % OPTION_MAX;

		ChangeOption(s_nSelectMenu);	//�I������ύX
	}
	else if (GetKeyboardTrigger(DIK_S) || GetJoypadTrigger(JOYKEY_DOWN, 0))
	{// �������( S�L�[ or �\���L�[�� )
		InitColorOption();		//�I�����̐F��������

		//�I���������ɂ��炷
		s_nSelectMenu = ((s_nSelectMenu + 1) + OPTION_MAX) % OPTION_MAX;

		ChangeOption(s_nSelectMenu);	//�I������ύX
	}

	switch (s_nSelectMenu)
	{
	case OPTION_TIMELIMIT:	//��������

		//�������Ԃ̑I��
		s_nSelectTimeLimit = SelectTimeLimit();
		break;

	case OPTION_POINT:	//�|�C���g��
		
		//�|�C���g���̑I��
		s_nSelectPoint = SelectPoint();
		break;

	case OPTION_SETCOUNT:	//�Z�b�g��

		//�Z�b�g���̑I��
		s_nSelectSetCount = SelectSetCount();
		break;

	case OPTION_GOTOGAME:	//�Q�[���֐i��

		if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(JOYKEY_START, 0))
		{//����L�[(ENTER�L�[)�������ꂽ���ǂ���
			/* �C���[�W��

			SetTime(s_nSelectTimeLimit);	//��������
			SetPoint(s_nSelectPoint);		//�|�C���g��
			SetSetCount(s_nSelectSetCount);	//�Z�b�g��

			<ChangeMode�ŃQ�[����>
			*/
		}
		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------
// �������Ԃ̑I��
//--------------------------------------------
static int SelectTimeLimit(void)
{
	if (GetKeyboardTrigger(DIK_A) || GetJoypadTrigger(JOYKEY_LEFT, 0))
	{// �������( A�L�[ or �\���L�[�� )

		//�I��������O(��)�ɂ���
		s_nSelectTimeLimit = ((s_nSelectTimeLimit - 1) + TIMELIMIT_MAX) % TIMELIMIT_MAX;
	}
	else if (GetKeyboardTrigger(DIK_D) || GetJoypadTrigger(JOYKEY_RIGHT, 0))
	{// �E�����( D�L�[ or �\���L�[�E )
		
		//�I���������(�E)�ɂ���
		s_nSelectTimeLimit = ((s_nSelectTimeLimit + 1) + TIMELIMIT_MAX) % TIMELIMIT_MAX;
	}

	int nTimeLimit = 0;

	switch (s_nSelectTimeLimit)
	{
	case TIMELIMIT_15:		//15�b
		nTimeLimit = 15;	//�ꎞ�ۑ�
		break;

	case TIMELIMIT_30:		//30�b
		nTimeLimit = 30;	//�ꎞ�ۑ�
		break;

	case TIMELIMIT_45:		//45�b
		nTimeLimit = 45;	//�ꎞ�ۑ�
		break;

	case TIMELIMIT_90:		//90�b
		nTimeLimit = 90;	//�ꎞ�ۑ�
		break;

	case TIMELIMIT_INFINITY:	//����(�������ԂȂ�)
		nTimeLimit = 99;		//�ꎞ�ۑ�(���ő���l)
		break;

	default:
		assert(false);
		break;
	}

	return nTimeLimit;	//�ݒ肵���l��Ԃ�
}

//--------------------------------------------
// �|�C���g���̑I��
//--------------------------------------------
static int SelectPoint(void)
{
	if (GetKeyboardTrigger(DIK_A) || GetJoypadTrigger(JOYKEY_LEFT, 0))
	{// �������( A�L�[ or �\���L�[�� )

		//�I��������O(��)�ɂ���
		s_nSelectPoint = ((s_nSelectPoint - 1) + POINTCOUNT_MAX) % POINTCOUNT_MAX;
	}
	else if (GetKeyboardTrigger(DIK_D) || GetJoypadTrigger(JOYKEY_RIGHT, 0))
	{// �E�����( D�L�[ or �\���L�[�E )
		
		//�I���������(�E)�ɂ���
		s_nSelectPoint = ((s_nSelectPoint + 1) + POINTCOUNT_MAX) % POINTCOUNT_MAX;
	}

	int nPoint = 0; 

	switch (s_nSelectPoint)
	{
	case POINTCOUNT_12:	//12�_
		nPoint = 12;	//�ꎞ�ۑ�
		break;

	case POINTCOUNT_15:	//15�_
		nPoint = 15;	//�ꎞ�ۑ�
		break;

	case POINTCOUNT_21:	//21�_
		nPoint = 21;	//�ꎞ�ۑ�
		break;

	default:
		assert(false);
		break;
	}

	return nPoint;	//�ݒ肵���l��Ԃ�
}

//--------------------------------------------
// �Z�b�g���̑I��
//--------------------------------------------
static int SelectSetCount(void)
{
	if (GetKeyboardTrigger(DIK_A) || GetJoypadTrigger(JOYKEY_LEFT, 0))
	{// �������( A�L�[ or �\���L�[�� )

		//�I��������O(��)�ɂ���
		s_nSelectSetCount = ((s_nSelectSetCount - 1) + SETCOUNT_MAX) % SETCOUNT_MAX;
	}
	else if (GetKeyboardTrigger(DIK_D) || GetJoypadTrigger(JOYKEY_RIGHT, 0))
	{// �E�����( D�L�[ or �\���L�[�E )
		
		//�I���������(�E)�ɂ���
		s_nSelectSetCount = ((s_nSelectSetCount + 1) + SETCOUNT_MAX) % SETCOUNT_MAX;
	}

	int nSetCount = 0;

	switch (s_nSelectSetCount)
	{
	case SETCOUNT_ONE:		//1�Z�b�g
		nSetCount = 1;		//�ꎞ�ۑ�
		break;

	case SETCOUNT_TWO:		//2�Z�b�g
		nSetCount = 2;		//�ꎞ�ۑ�
		break;

	case SETCOUNT_THREE:	//3�Z�b�g
		nSetCount = 3;		//�ꎞ�ۑ�
		break;

	default:
		assert(false);
		break;
	}

	return nSetCount;	//�ݒ肵���l��Ԃ�
}