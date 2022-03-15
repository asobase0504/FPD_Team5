//============================================
//
//�`�[������<�t���C���O�p���[�f�B�X�N>[select.cpp]
//Author:Kishimoto Eiji
//
//============================================

#include "select.h"
#include "option.h"
#include "input.h"

#include <assert.h>

//***********************************
//�}�N����`
//***********************************
#define MAX_TEXTURE		(3)			//�g�p����e�N�X�`���̍ő吔
#define MAX_TIMELIMIT	(5)			//�������Ԃ̑I�����̍ő吔
#define MAX_POINT		(4)			//�|�C���g���̑I�����̍ő吔
#define MAX_SETCOUNT	(4)			//�Z�b�g���̑I�����̍ő吔

#define TEXTURE_TIMELIMIT	("data/TEXTURE/��������")		//�������Ԃ̃e�N�X�`��
#define TEXTURE_POINT		("data/TEXTURE/�|�C���g��")		//�|�C���g���̃e�N�X�`��
#define TEXTURE_SETCOUNT	("data/TEXTURE/�Z�b�g��")		//�Z�b�g���̃e�N�X�`��

//***********************************
//�X�^�e�B�b�N�ϐ�
//***********************************
static LPDIRECT3DTEXTURE9		s_apTexture[MAX_TEXTURE];	//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuff = NULL;			//���_�o�b�t�@�ւ̃|�C���^

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

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
								TEXTURE_TIMELIMIT,
								&s_apTexture[0]);	//��������

	D3DXCreateTextureFromFile(pDevice,
								TEXTURE_POINT,
								&s_apTexture[1]);	//�|�C���g��

	D3DXCreateTextureFromFile(pDevice,
								TEXTURE_SETCOUNT,
								&s_apTexture[2]);	//�Z�b�g��

	//�ϐ��̏�����
	s_nSelectTimeLimit = 0;
	s_nSelectPoint = 0;
	s_nSelectSetCount = 0;
	s_nTimeLimit = 0;
	s_nPoint = 0;
	s_nSetCount = 0;

	VERTEX_2D * pVtx = NULL;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TEXTURE,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&s_pVtxBuff,
								NULL);

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i > MAX_TEXTURE; i++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

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

		//�e�N�X�`�����W�̐ݒ�------>��ŕς���
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;		//4���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//============================================
//�Z���N�g�̏I��
//============================================
void UninitSelect(void)
{
	for (int i = 0; i > MAX_TEXTURE; i++)
	{
		if (s_apTexture[i] != NULL)
		{//�e�N�X�`���̔j��
			s_apTexture[i]->Release();
			s_apTexture[i] = NULL;
		}
	}

	if (s_pVtxBuff != NULL)
	{//���_�o�b�t�@�̔j��
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
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
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i > MAX_TEXTURE; i++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, s_apTexture[i]);

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
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (nSelectMenu * 4);		//�w��̈ʒu�܂Ői�߂�

	float fDivid,fTexV;		//�e�N�X�`�����W�ݒ�p

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
	s_pVtxBuff->Unlock();
}