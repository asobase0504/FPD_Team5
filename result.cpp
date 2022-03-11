//================================
//
// ���U���g���
// Author:Teruto Sato
//Author:Kishimoto Eiji
//
//================================
#include "result.h"
#include "input.h"

//**************************************************
//�}�N����`
//**************************************************
#define MAX_WIN_LOSE		(2)			//�u�����v�u�s�k�v
#define WIN_LOSE_WIDTH		(160.0f)	//���sUI�̕�
#define WIN_LOSE_HEIGHT		(90.0f)		//���sUI�̍���
#define LEFTSIDE_POS_X		(300.0f)	//������UI��X���W
#define LEFTSIDE_POS_Y		(100.0f)	//������UI��Y���W
#define RIGHTSIDE_POS_X		(900.0f)	//�E����UI��X���W
#define RIGHTSIDE_POS_Y		(100.0f)	//�E����UI��Y���W

//**************************************************
//�X�^�e�B�b�N�ϐ�
//**************************************************
static LPDIRECT3DTEXTURE9		s_apTexture[MAX_WIN_LOSE];	//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffWinLose = NULL;		//���_�o�b�t�@�ւ̃|�C���^

//============================
// ���U���g�̏���������
//============================
void InitResult(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾
	VERTEX_2D *pVtx;							//���_���ւ̃|�C���^

	//�e�N�X�`���|�C���^�̏�����
	memset(s_apTexture, NULL, sizeof(s_apTexture));

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/��łȂ񂩓����.png",
								&s_apTexture[0]);

	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/��łȂ񂩓����.png",
								&s_apTexture[1]);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_WIN_LOSE,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&s_pVtxBuffWinLose,
								NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffWinLose->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_WIN_LOSE; i++)
	{
		D3DXVECTOR3 posWinLose;		//���sUI�̒��_���W�ݒ�p

		if (i == 0)
		{//1P��(��)
			posWinLose = D3DXVECTOR3(LEFTSIDE_POS_X, LEFTSIDE_POS_Y, 0.0f);
		}
		else if(i == 1)
		{//2P��(�E)
			posWinLose = D3DXVECTOR3(RIGHTSIDE_POS_X, RIGHTSIDE_POS_Y, 0.0f);
		}

		//���_���W�̐ݒ�
		float fLeft		= (posWinLose.x - WIN_LOSE_WIDTH);
		float fRight	= (posWinLose.x + WIN_LOSE_WIDTH);
		float fTop		= (posWinLose.y - WIN_LOSE_HEIGHT);
		float fBottom	= (posWinLose.y + WIN_LOSE_HEIGHT);

		pVtx[0].pos = D3DXVECTOR3(fLeft,  fTop,    0.0f);
		pVtx[1].pos = D3DXVECTOR3(fRight, fTop,    0.0f);
		pVtx[2].pos = D3DXVECTOR3(fLeft,  fBottom, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(fRight, fBottom, 0.0f);

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

		pVtx += 4;		//�|�C���^��i�߂�
	}

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuffWinLose->Unlock();
}

//============================
// ���U���g�̏I������
//============================
void UninitResult(void)
{
	for (int i = 0; i < MAX_WIN_LOSE; i++)
	{
		if (s_apTexture[i] != NULL)
		{//�e�N�X�`���̔j��
			s_apTexture[i]->Release();
			s_apTexture[i] = NULL;
		}
	}

	if (s_pVtxBuffWinLose != NULL)
	{//���_�o�b�t�@�̔j��
		s_pVtxBuffWinLose->Release();
		s_pVtxBuffWinLose = NULL;
	}
}

//============================
// ���U���g�̍X�V����
//============================
void UpdateResult(void)
{

}

//============================
// ���U���g�̕`�揈��
//============================
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffWinLose, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < MAX_WIN_LOSE; i++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, s_apTexture[i]);

		//�`���[�g���A���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
								i * 4,					//�`�悷��ŏ��̒��_�C���f�b�N�X
								2);						//�`�悷��v���~�e�B�u��
	}
}