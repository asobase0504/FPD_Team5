//------------------------------
//�ݒ�
//ACG000
//Author: goto yuuki
//
//------------------------------

#include "option.h"
#include "sound.h"
#include "fade.h"

//------------------------------
//�O���[�o���ϐ�
//------------------------------

LPDIRECT3DTEXTURE9 g_pTextureOption[MAX_OPTION] = {};		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffOption = NULL;			//���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_OptionPos;									//�I�v�V�����̍��W
static int s_nSelectTime;
static int *p_nSelectTime;
void InitOption(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�ւ̃|�C���^

									//�f�o�C�X�̎擾
	pDevice = GetDevice();

	s_nSelectTime = 0;
	*p_nSelectTime;
	p_nSelectTime = &s_nSelectTime;

	//�O���[�o���ϐ��̏�����
	g_OptionPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"TEXTURE/�w�i",
		&g_pTextureOption[0]);

	D3DXCreateTextureFromFile(pDevice,
		"TEXTURE/�^�C�}�[",
		&g_pTextureOption[1]);

	D3DXCreateTextureFromFile(pDevice,
		"TEXTURE/�|�C���g��",
		&g_pTextureOption[2]);

	D3DXCreateTextureFromFile(pDevice,
		"TEXTURE/�Z�b�g��",
		&g_pTextureOption[3]);

	VERTEX_2D * pVtx;		//���_���ւ̃|�C���^

							//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 16,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffOption,
		NULL);

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffOption->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	pVtx[4].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 - 500.0f, 50.0f, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 + 500.0f, 50.0f, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 - 500.0f, 50.0f + 190.0f, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 + 500.0f, 50.0f + 190.0f, 0.0f);

	pVtx[8].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 - 500.0f, 240.0f, 0.0f);
	pVtx[9].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 + 500.0f, 240.0f, 0.0f);
	pVtx[10].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 - 500.0f, 240.0f + 190.0f, 0.0f);
	pVtx[11].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 + 500.0f, 240.0f + 190.0f, 0.0f);

	pVtx[12].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 - 500.0f, 430.0f, 0.0f);
	pVtx[13].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 + 500.0f, 430.0f, 0.0f);
	pVtx[14].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 - 500.0f, 620.0f, 0.0f);
	pVtx[15].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 + 500.0f, 620.0f, 0.0f);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[4].rhw = 1.0f;
	pVtx[5].rhw = 1.0f;
	pVtx[6].rhw = 1.0f;
	pVtx[7].rhw = 1.0f;

	pVtx[8].rhw = 1.0f;
	pVtx[9].rhw = 1.0f;
	pVtx[10].rhw = 1.0f;
	pVtx[11].rhw = 1.0f;

	pVtx[12].rhw = 1.0f;
	pVtx[13].rhw = 1.0f;
	pVtx[14].rhw = 1.0f;
	pVtx[15].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	pVtx[8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[9].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[10].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[11].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	pVtx[12].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[13].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[14].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[15].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx[8].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[9].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[10].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[11].tex = D3DXVECTOR2(1.0f, 1.0f);


	pVtx[12].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[13].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[14].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[15].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffOption->Unlock();

	//�T�E���h�̍Đ�
	//PlaySound(SOUND_LABEL_BGM000);
}
void UninitOption(void)
{
	//�T�E���h�̒�~
	//StopSound();

	for (int nCntOption = 0; nCntOption < MAX_OPTION; nCntOption++)
	{//�e�N�X�`���̔j��
		if (g_pTextureOption[nCntOption] != NULL)
		{
			g_pTextureOption[nCntOption]->Release();
			g_pTextureOption[nCntOption] = NULL;
		}
	}

	if (g_pVtxBuffOption != NULL)
	{//���_�o�b�t�@�̔j��
		g_pVtxBuffOption->Release();
		g_pVtxBuffOption = NULL;
	}
}
void UpdateOption(void)
{
	if (GetKeyboardTrigger(DIK_A)|| GetKeyboardTrigger(DIK_LEFT)
	&& (s_nSelectTime > 0 && s_nSelectTime <= 5))
	{
	s_nSelectTime--;
	&s_nSelectTime;
	}
	if (GetKeyboardTrigger(DIK_D) || GetKeyboardTrigger(DIK_RIGHT)
	&& s_nSelectTime <= 0 && s_nSelectTime < 5)
	{
	s_nSelectTime++;
	&s_nSelectTime;
	}
}
void DrawOption(void)
{

	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�ւ̃|�C���^

									//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffOption, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureOption[0]);

	//�|���S���̕`��       
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,	//�|���S���̌`
		0,						//���_�̊J�n�ꏊ
		2);						//�v���~�e�B�u�̐�

								//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureOption[1]);

	//�|���S���̕`��       
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,	//�|���S���̌`
		4,						//���_�̊J�n�ꏊ
		2);						//�v���~�e�B�u�̐�

								//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureOption[2]);

	//�|���S���̕`��       
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,	//�|���S���̌`
		8,						//���_�̊J�n�ꏊ
		2);						//�v���~�e�B�u�̐�

								//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureOption[3]);

	//�|���S���̕`��       
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,	//�|���S���̌`
		12,						//���_�̊J�n�ꏊ
		2);						//�v���~�e�B�u�̐�


}
void SetOption(void)
{

}