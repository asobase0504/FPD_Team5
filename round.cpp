//=========================================
// 
// �|�C���g�̕\��
// Author YudaKaito
// Author Tanimoto
// 
//=========================================
//------------------------------------
// include
//------------------------------------
#include "round.h"
#include "score.h"

//------------------------------------
// �}�N����`
//------------------------------------
#define NUM_ROUND		(4)
#define DIGIT_ROUND		(2)
#define ROUND_WIDTH		(128)
#define ROUND_HEIGHT	(128)

//------------------------------------
// �ÓI�ϐ�
//------------------------------------
static LPDIRECT3DTEXTURE9 s_pTexture = NULL;	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff;				//���_�o�b�t�@�ւ̃|�C���^
static Round s_aRound[NUM_ROUND];
static bool s_bRoundUse;
static int s_nUseTime;

//------------------------------------
// �v���g�^�C�v�錾
//------------------------------------
static void DestroyRound(void);

//=========================================
// ������
//=========================================
void InitRound(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/SetPoint.png", &s_pTexture);

	Round* pRound = s_aRound;

	for (int nCntScore = 0; nCntScore < NUM_ROUND; nCntScore++, pRound++)
	{
		//���_�o�b�t�@�̐���
		pDevice->CreateVertexBuffer(
			(sizeof(VERTEX_2D) * 4 * 2),		//4�ŉ摜���
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&pRound->pVtxBuff,
			NULL);

		//pos.x = ((��ʔ���) - (������ * (���� + 2��))) + ((������ * (�\���� + ���� + 1.5��)) * ����)
		pRound->pos = D3DXVECTOR3(((SCREEN_WIDTH / 2) - (ROUND_WIDTH * (DIGIT_ROUND + 1.5))) + ((ROUND_WIDTH * (NUM_ROUND + DIGIT_ROUND + 1)) * nCntScore),
			SCREEN_HEIGHT * 0.5f, 0.0f);
		pRound->bUse = false;
		s_bRoundUse = false;
		s_nUseTime = 0;

		VERTEX_2D*pVtx;		//���_���ւ̃|�C���^

							//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		pRound->pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntDigit = 0; nCntDigit < DIGIT_ROUND; nCntDigit++, pVtx += 4)
		{
			//pVtx.pos.x = (�ʒu.x - ������ + (���� * ������ * ����))
			pVtx[0].pos = D3DXVECTOR3(pRound->pos.x - ROUND_WIDTH + (nCntDigit * ROUND_WIDTH * DIGIT_ROUND), pRound->pos.y - ROUND_HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pRound->pos.x + ROUND_WIDTH + (nCntDigit * ROUND_WIDTH * DIGIT_ROUND), pRound->pos.y - ROUND_HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pRound->pos.x - ROUND_WIDTH + (nCntDigit * ROUND_WIDTH * DIGIT_ROUND), pRound->pos.y + ROUND_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pRound->pos.x + ROUND_WIDTH + (nCntDigit * ROUND_WIDTH * DIGIT_ROUND), pRound->pos.y + ROUND_HEIGHT, 0.0f);

			//RHW�̐ݒ�
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 0.5f, 1.0f);
			pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 0.5f, 1.0f);
			pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 0.5f, 1.0f);
			pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 0.5f, 1.0f);

			//�e�N�X�`���̍��W�ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
		}

		//���_�o�b�t�@���A�����b�N����
		pRound->pVtxBuff->Unlock();
	}
}

//=========================================
// �I��
//=========================================
void UninitRound(void)
{
}

//=========================================
// �X�V
//=========================================
void UpdateRound(void)
{
	if (s_bRoundUse)
	{
		s_nUseTime++;

		if (s_nUseTime >= 120)
		{
			s_nUseTime = 0;
			s_bRoundUse = false;
		}
	}
}

//=========================================
// �`��
//=========================================
void DrawRound(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^

	Round* pRound = s_aRound;

	for (int nCntRound = 0; nCntRound < NUM_ROUND; nCntRound++, pRound++)
	{
		if (!s_bRoundUse)
		{
			continue;
		}

		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, pRound->pVtxBuff, 0, sizeof(VERTEX_2D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, s_pTexture);

		for (int nCntDigit = 0; nCntDigit < DIGIT_ROUND; nCntDigit++)
		{
			//�|���S���̕`��
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
				nCntDigit * 4,				//�`�悷��ŏ��̒��_�C���f�b�N�X
				2);							//�v���~�e�B�u�i�|���S���j�̐�
		}
	}
}

//=========================================
// �ݒ�
//=========================================
void SetRound(void)
{
	SCORE* pScore = GetScore();
	Round* pRound = s_aRound;
	s_bRoundUse = true;

	for (int i = 0; i < NUM_ROUND; i++, pScore++, pRound++)
	{
	}
}

//=========================================
// �j��
//=========================================
void DestroyRound(void)
{
	Round* pRound = s_aRound;
}
