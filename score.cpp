//------------------------------
//�X�R�A�ݒ�
//ACG000
//Author: goto yuuki
//
//------------------------------
#include"main.h"
#include"score.h"
#include"input.h"
//�O���[�o���ϐ�
static LPDIRECT3DTEXTURE9 g_pTextureScore = NULL;				//�e�N�X�`���ւ̃|�C���^
static SCORE	g_nScore[2];									//�X�R�A�̍\����
static int nPlayerIdx;

//------------------------------
//�X�R�A�̏���������
//------------------------------
void InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/gotou/TEXTURE/number000.png",
		&g_pTextureScore);

	for (int nScoreCnt = 0; nScoreCnt < 2; nScoreCnt++)
	{
		g_nScore[nScoreCnt].nScore[0] = 0;									//�l������������
		g_nScore[nScoreCnt].nScore[1] = 0;									//�l������������
		g_nScore[nScoreCnt].nCnt = 0;										//�l������������
		g_nScore[nScoreCnt].pos = D3DXVECTOR3(500.0f, 50.0f, 0.0f);			//�ʒu������������

		//���_�o�b�t�@�̐���
		pDevice->CreateVertexBuffer(
			(sizeof(VERTEX_2D) * 4 * NUM_SCORE),		//4�ŉ摜���
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&g_nScore[nScoreCnt].pVtxBuff,
			NULL);

		VERTEX_2D*pVtx;		//���_���ւ̃|�C���^

		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_nScore[nScoreCnt].pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		
		//�X�R�A�̏��̏�����
		for (int nPosCnt = 0; nPosCnt < NUM_SCORE; nPosCnt++)
		{
			//���_���W
			pVtx[0].pos = D3DXVECTOR3(g_nScore[nScoreCnt].pos.x - 30.0f + nPosCnt * 70.0f, g_nScore[nScoreCnt].pos.y - 30, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_nScore[nScoreCnt].pos.x + 30.0f + nPosCnt * 70.0f, g_nScore[nScoreCnt].pos.y - 30, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_nScore[nScoreCnt].pos.x - 30.0f + nPosCnt * 70.0f, g_nScore[nScoreCnt].pos.y + 30, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_nScore[nScoreCnt].pos.x + 30.0f + nPosCnt * 70.0f, g_nScore[nScoreCnt].pos.y + 30, 0.0f);

			if (nPosCnt == 1 || nPosCnt == 2)
			{
				g_nScore[nScoreCnt].pos += D3DXVECTOR3(60.0f, 0.0f, 0.0f);
				if (nPosCnt == 2)
				{
					g_nScore[nScoreCnt].pos -= D3DXVECTOR3(60.0f, 0.0f, 0.0f);
				}
			}

			//RHW�̐ݒ�
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//�e�N�X�`���̍��W�ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

			pVtx += 4; //���_�|�C���g���l�i��
		}
		//���_�o�b�t�@���A�����b�N����
		g_nScore[nScoreCnt].pVtxBuff->Unlock();
	}
}

//------------------------------
//�X�R�A�̏I������
//------------------------------
void UninitScore(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}

	for (int nScoreCnt = 0; nScoreCnt < 2; nScoreCnt++)
	{
		//���_�o�b�t�@�̔j��
		if (g_nScore[nScoreCnt].pVtxBuff != NULL)
		{
			g_nScore[nScoreCnt].pVtxBuff->Release();
			g_nScore[nScoreCnt].pVtxBuff = NULL;
		}
	}
}

//------------------------------
//�X�R�A�̍X�V����
//------------------------------
void UpdateScore(void)
{
}

//------------------------------
//�X�R�A�̕`�揈��
//------------------------------
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nScoreCnt = 0; nScoreCnt < 2; nScoreCnt++)
	{
		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_nScore[nScoreCnt].pVtxBuff, 0, sizeof(VERTEX_2D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		for (int nCntScore = 0; nCntScore < NUM_SCORE; nCntScore++)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureScore);

			//�|���S���̕`��
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
				nCntScore * 4,				//�`�悷��ŏ��̒��_�C���f�b�N�X
				2);							//�v���~�e�B�u�i�|���S���j�̐�
		}
	}
}

//------------------------------
//�X�R�A�̃Z�b�g����
//------------------------------
void SetScore(int nScore)//,int nPlayerIdx)
{
	int nCntScore;
	int aPosTexU[4];	//�e���̐������i�[

	g_nScore[2].nScore[2] = nScore;

	aPosTexU[0] = (g_nScore[0].nScore[0] % 100) / 10;
	aPosTexU[1] = (g_nScore[0].nScore[1] % 10) / 1;
	aPosTexU[2] = (g_nScore[1].nScore[0] % 100) / 10;
	aPosTexU[3] = (g_nScore[1].nScore[1] % 10) / 1;

	//aPosTexU[0] = (g_nScore[2].nScore % 100) / 10;
	//aPosTexU[0] = (g_nScore[3].nScore % 10) / 1;

	for (int nScoreCnt = 0; nScoreCnt < 2; nScoreCnt++)
	{
		VERTEX_2D*pVtx; //���_�ւ̃|�C���^	

		//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
		g_nScore[nScoreCnt].pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (nCntScore = 0; nCntScore < NUM_SCORE; nCntScore++)
		{
			//�e�N�X�`���̍��W�ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntScore], 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntScore] + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntScore], 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntScore] + 0.1f, 1.0f);
			pVtx += 4; //���_�|�C���g���l�i��
		}

		//���_�o�b�t�@���A�����b�N
		g_nScore[nScoreCnt].pVtxBuff->Unlock();
	}
}

//------------------------------
//�X�R�A�̉��Z����
//------------------------------
void AddScore(int nValse, int nPlayerIdx)
{
	int nCntScore;
	int aPosTexU[4]; //�e���̐������i�[

	g_nScore[nPlayerIdx].nScore[1] += nValse;

	//0�Ԗڂ̔z��ɁA1�Ԗڂ̔z��ɐ���������
	g_nScore[nPlayerIdx].nScore[0] = g_nScore[nPlayerIdx].nScore[1];

	aPosTexU[0] = (g_nScore[0].nScore[0] % 100) / 10;
	aPosTexU[1] = (g_nScore[0].nScore[1] % 10) / 1;
	aPosTexU[2] = (g_nScore[1].nScore[0] % 100) / 10;
	aPosTexU[3] = (g_nScore[1].nScore[1] % 10) / 1;

	for (int nScoreCnt = 0; nScoreCnt < 2; nScoreCnt++)
	{
		VERTEX_2D*pVtx; //���_�ւ̃|�C���^

		//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
		g_nScore[nScoreCnt].pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (nCntScore = 0; nCntScore < NUM_SCORE; nCntScore++)
		{
			//�e�N�X�`���̍��W�ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntScore], 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntScore] + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntScore], 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntScore] + 0.1f, 1.0f);
			pVtx += 4; //���_�|�C���g���l�i��
		}

		//���_�o�b�t�@���A�����b�N
		g_nScore[nScoreCnt].pVtxBuff->Unlock();
	}
}

//�X�R�A�̎擾
SCORE GetScore(void)
{
	return g_nScore[0];
}