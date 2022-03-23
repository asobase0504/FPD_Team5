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
#include "point.h"
#include "score.h"

//------------------------------------
// �}�N����`
//------------------------------------
#define NUM_POINT	(2)
#define DIGIT_POINT	(2)

//------------------------------------
// �ÓI�ϐ�
//------------------------------------
static LPDIRECT3DTEXTURE9 g_pTextureNumber = NULL;			//�e�N�X�`���ւ̃|�C���^
static Point s_aPoint[NUM_POINT];
static bool s_bPointUse;
static int s_nUseTime;

//------------------------------------
// �v���g�^�C�v�錾
//------------------------------------
static void DestroyPoint(void);

//=========================================
// ������
//=========================================
void InitPoint(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number.png", &g_pTextureNumber);

	Point* pPoint = s_aPoint;

	for (int nCntScore = 0; nCntScore < NUM_POINT; nCntScore++, pPoint++)
	{
		//���_�o�b�t�@�̐���
		pDevice->CreateVertexBuffer(
			(sizeof(VERTEX_2D) * 4 * 2),		//4�ŉ摜���
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&pPoint->pVtxBuff,
			NULL);

		//pos.x = ((��ʔ���) - (������ * (���� + 2��))) + ((������ * (�\���� + ���� + 1.5��)) * ����)
		pPoint->pos = D3DXVECTOR3(((SCREEN_WIDTH / 2) - (POINT_WIDTH * (DIGIT_POINT + 1.5))) + ((POINT_WIDTH * (NUM_POINT + DIGIT_POINT + 1)) * nCntScore),
			SCREEN_HEIGHT * 0.5f, 0.0f);
		pPoint->bUse = false;
		s_bPointUse = false;
		s_nUseTime = 0;

		VERTEX_2D*pVtx;		//���_���ւ̃|�C���^

		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		pPoint->pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntDigit = 0; nCntDigit < DIGIT_POINT; nCntDigit++ , pVtx += 4)
		{
			//pVtx.pos.x = (�ʒu.x - ������ + (���� * ������ * ����))
			pVtx[0].pos = D3DXVECTOR3(pPoint->pos.x - POINT_WIDTH + (nCntDigit * POINT_WIDTH * DIGIT_POINT), pPoint->pos.y - POINT_HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pPoint->pos.x + POINT_WIDTH + (nCntDigit * POINT_WIDTH * DIGIT_POINT), pPoint->pos.y - POINT_HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pPoint->pos.x - POINT_WIDTH + (nCntDigit * POINT_WIDTH * DIGIT_POINT), pPoint->pos.y + POINT_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pPoint->pos.x + POINT_WIDTH + (nCntDigit * POINT_WIDTH * DIGIT_POINT), pPoint->pos.y + POINT_HEIGHT, 0.0f);

			//RHW�̐ݒ�
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 0.5f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 0.5f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 0.5f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 0.5f, 1.0f);

			//�e�N�X�`���̍��W�ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
		}

		//���_�o�b�t�@���A�����b�N����
		pPoint->pVtxBuff->Unlock();
	}
}

//=========================================
// �I��
//=========================================
void UninitPoint(void)
{
}

//=========================================
// �X�V
//=========================================
void UpdatePoint(void)
{
	if (s_bPointUse)
	{
		s_nUseTime++;

		if (s_nUseTime >= 120)
		{
			s_nUseTime = 0;
			s_bPointUse = false;
		}
	}
}

//=========================================
// �`��
//=========================================
void DrawPoint(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^

	Point* pPoint = s_aPoint;

	for (int nCntPoint = 0; nCntPoint < NUM_POINT; nCntPoint++, pPoint++)
	{
		if (!s_bPointUse)
		{
			continue;
		}

		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, pPoint->pVtxBuff, 0, sizeof(VERTEX_2D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureNumber);

		for (int nCntDigit = 0; nCntDigit < DIGIT_POINT; nCntDigit++)
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
void SetPoint(void)
{
	SCORE* pScore = GetScore();
	Point* pPoint = s_aPoint;
	s_bPointUse = true;

	for (int i = 0; i < NUM_POINT; i++, pScore++, pPoint++)
	{
		pPoint->nScore = pScore->nScore[0];

		int aNumDigit = pPoint->nScore;

		VERTEX_2D*pVtx;		//���_���ւ̃|�C���^

		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		pPoint->pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		//�e�N�X�`���̍��W�ݒ�
		int aData = aNumDigit * 0.1f;
		pVtx[0].tex = D3DXVECTOR2(aData % 10 * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2((aData + 1) % 10 * 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(aData % 10 * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2((aData + 1) % 10 * 0.1f, 1.0f);

		pVtx[4].tex = D3DXVECTOR2(aNumDigit % 10 * 0.1f, 0.0f);
		pVtx[5].tex = D3DXVECTOR2((aNumDigit + 1) % 10 * 0.1f, 0.0f);
		pVtx[6].tex = D3DXVECTOR2(aNumDigit % 10 * 0.1f, 1.0f);
		pVtx[7].tex = D3DXVECTOR2((aNumDigit + 1) % 10 * 0.1f, 1.0f);

		//���_�o�b�t�@���A�����b�N����
		pPoint->pVtxBuff->Unlock();
	}
}

//=========================================
// �j��
//=========================================
void DestroyPoint(void)
{
	Point* pPoint = s_aPoint;
}
