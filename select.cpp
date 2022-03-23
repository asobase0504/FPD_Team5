//============================================
//
//�`�[������<�t���C���O�p���[�f�B�X�N>[select.cpp]
//Author:Kishimoto Eiji
//
//============================================

#include "select.h"
#include "option.h"
#include "menu.h"
#include "input.h"

#include <assert.h>

//***********************************
//�}�N����`
//***********************************
#define MAX_SELECT		(3)			//�I�����̍ő吔
#define SELECT_SIZE		(35.0f)		//�I�𕔕��̃T�C�Y
#define SELECT_POS_X	(200.0f)	//�I�����̈ʒu���猩���I�𕔕���X���W

#define MAX_TIMELIMIT	(5)			//�������Ԃ̑I�����̍ő吔
#define MAX_POINT		(4)			//�|�C���g���̑I�����̍ő吔
#define MAX_SETCOUNT	(4)			//�Z�b�g���̑I�����̍ő吔

#define MAX_ARROW		(6)			//���̍ő吔
#define ARROW_SIZE		(25.0f)		//���̃T�C�Y
#define ARROW_POS_X		(100.0f)	//�I�𕔕����猩������X���W

#define TEXTURE_TIMELIMIT	("data/TEXTURE/WORD/FPD_SelectUI_TimeLimit_Re.png")	//�������Ԃ̃e�N�X�`��
#define TEXTURE_POINT		("data/TEXTURE/WORD/FPD_SelectUI_Point_Re.png")		//�|�C���g���̃e�N�X�`��
#define TEXTURE_SETCOUNT	("data/TEXTURE/WORD/FPD_SelectUI_SetCount_Re.png")		//�Z�b�g���̃e�N�X�`��
#define TEXTURE_ARROW		("data/TEXTURE/WORD/FPD_SelectUI_Arrow_Re.png")		//���̃e�N�X�`��

//***********************************
//�X�^�e�B�b�N�ϐ�
//***********************************
static LPDIRECT3DTEXTURE9		s_apTextureSelect[MAX_SELECT];	//�e�N�X�`���ւ̃|�C���^(�I�𕔕�)
static LPDIRECT3DTEXTURE9		s_pTextureArrow;				//�e�N�X�`���ւ̃|�C���^(���)
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffSelect = NULL;		//���_�o�b�t�@�ւ̃|�C���^(�I�𕔕�)
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffArrow = NULL;			//���_�o�b�t�@�ւ̃|�C���^(���)

static int s_nSelectTimeLimit;	//�I�΂�Ă��鐧������
static int s_nSelectPoint;		//�I�΂�Ă���|�C���g��
static int s_nSelectSetCount;	//�I�΂�Ă���Z�b�g��

static int s_nTimeLimit;	//�������Ԃ�ۑ�����
static int s_nPoint;		//�|�C���g����ۑ�����
static int s_nSetCount;		//�Z�b�g����ۑ�����

//***********************************
//�v���g�^�C�v�錾
//***********************************

static void ChangeTexture(int nSelectMenu,int nSelectOption);

//============================================
//�Z���N�g�̏�����
//============================================
void InitSelect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾

	//�e�N�X�`���̓ǂݍ���(�I�𕔕�)
	D3DXCreateTextureFromFile(pDevice,
								TEXTURE_TIMELIMIT,
								&s_apTextureSelect[0]);	//��������

	D3DXCreateTextureFromFile(pDevice,
								TEXTURE_POINT,
								&s_apTextureSelect[1]);	//�|�C���g��

	D3DXCreateTextureFromFile(pDevice,
								TEXTURE_SETCOUNT,
								&s_apTextureSelect[2]);	//�Z�b�g��

	//�e�N�X�`���̓ǂݍ���(���)
	D3DXCreateTextureFromFile(pDevice,
								TEXTURE_ARROW,
								&s_pTextureArrow);

	//�ϐ��̏�����
	s_nSelectTimeLimit = 0;
	s_nSelectPoint = 0;
	s_nSelectSetCount = 0;
	s_nTimeLimit = 0;
	s_nPoint = 0;
	s_nSetCount = 0;

	VERTEX_2D * pVtx = NULL;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_SELECT,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&s_pVtxBuffSelect,
								NULL);

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ARROW,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&s_pVtxBuffArrow,
								NULL);

	/**************** �I�𕔕� ****************/

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	s_pVtxBuffSelect->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 posSelect;					//�I�𕔕��̈ʒu�ݒ�p
	D3DXVECTOR3 aPosArrow[MAX_SELECT];		//���̈ʒu�ݒ�p

	for (int i = 0; i < MAX_SELECT; i++)
	{
		//���j���[�̈ʒu���n�_�ɁA�I�𕔕��̈ʒu��ݒ肷��
		posSelect.x = (GetMenu()->Option[i].pos.x) + SELECT_POS_X;
		posSelect.y = GetMenu()->Option[i].pos.y;
		posSelect.z = 0.0f;

		aPosArrow[i] = posSelect;	//���̐ݒ莞�Ɏg�p���邽�߈ꎞ�ۑ�

		//�ϐ��ɂ܂Ƃ߂ĒZ������
		float fLeft		= (posSelect.x - SELECT_SIZE);
		float fRight	= (posSelect.x + SELECT_SIZE);
		float fTop		= (posSelect.y - SELECT_SIZE);
		float fBottom	= (posSelect.y + SELECT_SIZE);

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(fLeft,  fTop,		0.0f);
		pVtx[1].pos = D3DXVECTOR3(fRight, fTop,		0.0f);
		pVtx[2].pos = D3DXVECTOR3(fLeft,  fBottom,	0.0f);
		pVtx[3].pos = D3DXVECTOR3(fRight, fBottom,	0.0f);

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
		pVtx[2].tex = D3DXVECTOR2(0.0f, 0.25f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.25f);

		pVtx += 4;		//4���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffSelect->Unlock();

	/**************** ��� ****************/

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	s_pVtxBuffArrow->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_ARROW; i++)
	{
		float fPosX, fPosY;		//�ʒu�ݒ�p
		
		if ((i >= 0) && (i < (MAX_ARROW / 2)))
		{//���̏c��
			//�ۑ����Ă��������W����A�����̖��̈ʒu�����߂�
			fPosX = aPosArrow[i].x - ARROW_POS_X;
			fPosY = aPosArrow[i].y;
		}
		else if ((i >= (MAX_ARROW / 2)) && (i < MAX_ARROW))
		{//�E�̏c��
			//�ۑ����Ă��������W����A�E���̖��̈ʒu�����߂�
			fPosX = aPosArrow[i % MAX_SELECT].x + ARROW_POS_X;
			fPosY = aPosArrow[i % MAX_SELECT].y;
		}

		//�ϐ��ɂ܂Ƃ߂ĒZ������
		float fLeft		= (fPosX - ARROW_SIZE);
		float fRight	= (fPosX + ARROW_SIZE);
		float fTop		= (fPosY - ARROW_SIZE);
		float fBottom	= (fPosY + ARROW_SIZE);

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(fLeft,  fTop,		0.0f);
		pVtx[1].pos = D3DXVECTOR3(fRight, fTop,		0.0f);
		pVtx[2].pos = D3DXVECTOR3(fLeft,  fBottom,	0.0f);
		pVtx[3].pos = D3DXVECTOR3(fRight, fBottom,	0.0f);

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

		pVtx += 4;		//4���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffArrow->Unlock();
}

//============================================
//�Z���N�g�̏I��
//============================================
void UninitSelect(void)
{
	/****** �e�N�X�`���̔j�� ******/

	for (int i = 0; i < MAX_SELECT; i++)
	{
		if (s_apTextureSelect[i] != NULL)
		{//�I�𕔕�
			s_apTextureSelect[i]->Release();
			s_apTextureSelect[i] = NULL;
		}
	}

	if (s_pTextureArrow != NULL)
	{//���
		s_pTextureArrow->Release();
		s_pTextureArrow = NULL;
	}

	/****** ���_�o�b�t�@�̔j�� ******/

	if (s_pVtxBuffSelect != NULL)
	{//�I�𕔕�
		s_pVtxBuffSelect->Release();
		s_pVtxBuffSelect = NULL;
	}

	if (s_pVtxBuffArrow != NULL)
	{//���
		s_pVtxBuffArrow->Release();
		s_pVtxBuffArrow = NULL;
	}
}

//============================================
//�Z���N�g�̍X�V
//============================================
void UpdateSelect(void)
{
	
}

//============================================
//�Z���N�g�̕`��
//============================================
void DrawSelect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffSelect, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < MAX_SELECT; i++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, s_apTextureSelect[i]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�|���S���̌`
								i * 4,				//���_�̊J�n�ꏊ
								2);					//�v���~�e�B�u�̐�
	}

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffArrow, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < MAX_ARROW; i++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, s_pTextureArrow);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�|���S���̌`
								i * 4,				//���_�̊J�n�ꏊ
								2);					//�v���~�e�B�u�̐�
	}
}

//============================================
// �������Ԃ̑I��
// int nSelectMenu ---> �I�����Ă��郁�j���[
//============================================
int SelectTimeLimit(int nSelectMenu)
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

	int nTimeLimit = 0;		//�l��Ԃ��p

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

	//�e�N�X�`���ʒu�̐؂�ւ�
	ChangeTexture(nSelectMenu, s_nSelectTimeLimit);

	return nTimeLimit;	//�ݒ肵���l��Ԃ�
}

//============================================
// �|�C���g���̑I��
// int nSelectMenu ---> �I�����Ă��郁�j���[
//============================================
int SelectPoint(int nSelectMenu)
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

	int nPoint = 0; 	//�l��Ԃ��p

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

	case POINTCOUNT_25:	//25�_
		nPoint = 25;	//�ꎞ�ۑ�
		break;

	default:
		assert(false);
		break;
	}

	//�e�N�X�`���ʒu�̐؂�ւ�
	ChangeTexture(nSelectMenu, s_nSelectPoint);

	return nPoint;	//�ݒ肵���l��Ԃ�
}

//============================================
// �Z�b�g���̑I��
// int nSelectMenu ---> �I�����Ă��郁�j���[
//============================================
int SelectSetCount(int nSelectMenu)
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

	int nSetCount = 0;	//�l��Ԃ��p

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

	case SETCOUNT_FOUR:		//4�Z�b�g
		nSetCount = 4;		//�ꎞ�ۑ�
		break;

	default:
		assert(false);
		break;
	}

	//�e�N�X�`���ʒu�̐؂�ւ�
	ChangeTexture(nSelectMenu, s_nSelectSetCount);

	return nSetCount;	//�ݒ肵���l��Ԃ�
}

//--------------------------------------------
// �e�N�X�`����؂�ւ���
// int nSelectMenu -----> �I�����Ă��郁�j���[
// int nSelectOption ---> �I�����Ă���I�v�V����
//--------------------------------------------
static void ChangeTexture(int nSelectMenu, int nSelectOption)
{
	VERTEX_2D * pVtx = NULL;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	s_pVtxBuffSelect->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (nSelectMenu * 4);		//�w��̈ʒu�܂Ői�߂�

	float fDivid = 0,fTexV = 0;		//�e�N�X�`�����W�ݒ�p

	switch (nSelectMenu)
	{
	case OPTION_TIMELIMIT:		//��������

		fDivid = (1.0f / MAX_TIMELIMIT);	//�e�N�X�`���ɍ��킹�ĕ���
		fTexV = fDivid * nSelectOption;		//�I�����ɉ������e�N�X�`���ʒu�ɂ���
		break;

	case OPTION_POINT:		//�|�C���g��

		fDivid = (1.0f / MAX_POINT);		//�e�N�X�`���ɍ��킹�ĕ���
		fTexV = fDivid * nSelectOption;		//�I�����ɉ������e�N�X�`���ʒu�ɂ���
		break;

	case OPTION_SETCOUNT:	//�Z�b�g��

		fDivid = (1.0f / MAX_SETCOUNT);		//�e�N�X�`���ɍ��킹�ĕ���
		fTexV = fDivid * nSelectOption;		//�I�����ɉ������e�N�X�`���ʒu�ɂ���
		break;

	default:
		assert(false);
		break;
	}

	//�e�N�X�`�����W�̐ݒ�------>��ŕς���
	pVtx[0].tex = D3DXVECTOR2(0.0f, fTexV);
	pVtx[1].tex = D3DXVECTOR2(1.0f, fTexV);
	pVtx[2].tex = D3DXVECTOR2(0.0f, fTexV + fDivid);
	pVtx[3].tex = D3DXVECTOR2(1.0f, fTexV + fDivid);

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffSelect->Unlock();
}