//=========================================
// 
// UI����
// Author YudaKaito
// 
//=========================================
//------------------------------------
// include
//------------------------------------
#include "ui.h"
#include "score.h"
#include "time.h"
#include "pop.h"
#include "point.h"

//------------------------------------
// �ÓI�ϐ�
//------------------------------------
static LPDIRECT3DTEXTURE9		s_pTextureScoreAg;			//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffScoreAg = NULL;	//���_�o�b�t�@�ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffScoreBg = NULL;	//���_�o�b�t�@�ւ̃|�C���^

//------------------------------------
// �v���g�^�C�v�錾
//------------------------------------
static void InitScoreDesign(void);
static void UninitScoreDesign(void);
static void UpdateScoreDesign(void);
static void DrawAgScoreDesign(void);
static void DrawBgScoreDesign(void);

//=========================================
// ������
//=========================================
void InitUI(void)
{
	InitScore();		// �X�R�A
	InitTime();			// �^�C��
	InitPop();			// ���_
	InitScoreDesign();	// �X�R�A�f�U�C��
	InitPoint();		// �|�C���g
}

//=========================================
// �I��
//=========================================
void UninitUI(void)
{
	UninitScore();			// �X�R�A
	UninitTime();			// �^�C��
	UninitPop();			// ���_
	UninitScoreDesign();	// �X�R�A�f�U�C��
	UninitPoint();			// �|�C���g
}

//=========================================
// �X�V
//=========================================
void UpdateUI(void)
{
	UpdateScore();			// �X�R�A
	UpdateTime();			// �^�C��
	UpdatePop();			// ���_
	UpdateScoreDesign();	// �X�R�A�f�U�C��
	UpdatePoint();			// �|�C���g
}

//=========================================
// �`��
//=========================================
void DrawUI()
{
	DrawPop();				// ���_
	DrawBgScoreDesign();	// �X�R�A�f�U�C��
	DrawTime();				// �^�C��
	DrawScore();			// �X�R�A
	DrawAgScoreDesign();	// �X�R�A�f�U�C��
	DrawPoint();			// �|�C���g
}

//=========================================
// �X�R�A�f�U�C���̏�����
//=========================================
void InitScoreDesign(void)
{
	{
		//�e�N�X�`���[�̓ǂݍ���
		D3DXCreateTextureFromFile(GetDevice(), "data\\TEXTURE\\ScorePaling.png", &s_pTextureScoreAg);

		// ���_�o�b�t�@�̐���
		GetDevice()->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&s_pVtxBuffScoreAg,
			NULL);

		VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuffScoreAg->Lock(0, 0, (void**)&pVtx, 0);

		D3DXVECTOR3 pos(SCREEN_WIDTH * 0.5f, 30.0f, 0.0f);
		D3DXVECTOR2 size(512.0f * 0.4f, 256.0f * 0.4f);

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(pos.x - size.x, pos.y - size.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pos.x + size.x, pos.y - size.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pos.x - size.x, pos.y + size.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pos.x + size.x, pos.y + size.y, 0.0f);

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// ���_�o�b�t�@���A�����b�N����
		s_pVtxBuffScoreAg->Unlock();
	}

	{
		// ���_�o�b�t�@�̐���
		GetDevice()->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&s_pVtxBuffScoreBg,
			NULL);

		VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuffScoreBg->Lock(0, 0, (void**)&pVtx, 0);

		D3DXVECTOR3 pos(SCREEN_WIDTH * 0.5f, 30.0f, 0.0f);
		D3DXVECTOR2 size(512.0f * 0.375f, 256.0f * 0.35f);

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(pos.x - size.x, pos.y - size.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pos.x + size.x, pos.y - size.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pos.x - size.x + 27.0f, pos.y + size.y - 10.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pos.x + size.x - 27.0f, pos.y + size.y - 10.0f, 0.0f);

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// ���_�o�b�t�@���A�����b�N����
		s_pVtxBuffScoreBg->Unlock();
	}
}

//=========================================
// �X�R�A�f�U�C���̏I��
//=========================================
void UninitScoreDesign(void)
{
	// �e�N�X�`���̔j��
	if (s_pTextureScoreAg != NULL)
	{
		s_pTextureScoreAg->Release();
		s_pTextureScoreAg = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (s_pVtxBuffScoreAg != NULL)
	{
		s_pVtxBuffScoreAg->Release();
		s_pVtxBuffScoreAg = NULL;
	}
}

//=========================================
// �X�R�A�f�U�C���̍X�V
//=========================================
void UpdateScoreDesign(void)
{

}

//=========================================
// �X�R�A�f�U�C���̕`��
//=========================================
void DrawAgScoreDesign(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���g

												// ���_�o�b�t�@���f�[�^�X�g���[���ݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffScoreAg, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTextureScoreAg);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

}

//=========================================
// �X�R�A�f�U�C���̕`��
//=========================================
void DrawBgScoreDesign(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���g

												// ���_�o�b�t�@���f�[�^�X�g���[���ݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffScoreBg, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

}
