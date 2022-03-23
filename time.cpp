//------------------------------
//���Ԑݒ�
//
//Author: goto yuuki
//
//------------------------------
#include "time.h"
#include "math.h"								//���𐶂ݏo���ϐ�

//�O���[�o���ϐ�
static LPDIRECT3DTEXTURE9 g_pTexture = NULL;			//�e�N�X�`���w�m�|�C���^
static LPDIRECT3DVERTEXBUFFER9 g_pVtxBuff = NULL;		//���_�o�b�t�@�ւ̃|�C���^
static TIME g_Time;										//�^�C���\����

//�^�C���̏���������
void InitTime(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/number000.png",
		&g_pTexture);

	bool bTimeFlag = false;									//�^�C�g���̈ړ��̃t���O
	g_Time.nTime =30;										//�^�C���̒l
	g_Time.nCntTime = 0;									//�^�C���J�E���^�[�l
	g_Time.nMinusTime =0;										//�^�C����������

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_CNT_TIME,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuff,
		NULL);

	VERTEX_2D*pVtx;									//���_���ւ̃|�C���^

	//�^�C�������炵�ĕ`�悷��ݒ�

	for (g_Time.nCntTime = 0; g_Time.nCntTime < MAX_CNT_TIME; g_Time.nCntTime++)
	{
		g_Time.pos[g_Time.nCntTime] = D3DXVECTOR3(594.0f + (20.0f + (20.0f * 2) * g_Time.nCntTime), 0.0f + 60.0f, 0.0f);
	}

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (g_Time.nCntTime = 0; g_Time.nCntTime < MAX_CNT_TIME; g_Time.nCntTime++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_Time.pos[g_Time.nCntTime].x - 20.0f, g_Time.pos[g_Time.nCntTime].y - 35.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Time.pos[g_Time.nCntTime].x + 20.0f, g_Time.pos[g_Time.nCntTime].y - 35.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Time.pos[g_Time.nCntTime].x - 20.0f, g_Time.pos[g_Time.nCntTime].y + 35.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Time.pos[g_Time.nCntTime].x + 20.0f, g_Time.pos[g_Time.nCntTime].y + 35.0f, 0.0f);

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
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;		//���_�f�[�^�̃|�C���^��4���i�߂�
	}
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuff->Unlock();
}
//�^�C���̏I������
void UninitTime(void)
{
	//�e�N�X�`���̔j��
	if (g_pTexture != NULL)
	{
		g_pTexture->Release();
		g_pTexture = NULL;
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuff != NULL)
	{
		g_pVtxBuff->Release();
		g_pVtxBuff = NULL;
	}
}

//�^�C���̍X�V����
void UpdateTime(void)
{
	int aPosTexU[2];			//�e���̐������i�[
	g_Time.nMinusTime++;				//nCntTime��������Ă���
	if (g_Time.nTime != 0)
	{

		if (g_Time.nMinusTime % 60 == 0)		//nCntTime��0��菬�����Ȃ�����
		{
			g_Time.nTime--;
			//g_nTime[s_nSelectTime]--;				//���Ԃ���b�������Ă���
		}
	}
	aPosTexU[0] = g_Time.nTime % 100 / 10;
	aPosTexU[1] = g_Time.nTime % 10;
	//aPosTexU[2] = g_Time.nTime % 10;

	//aPosTexU[0] = g_nTime[s_nSelectTime] % 1000 / 100;
	//aPosTexU[1] = g_nTime[s_nSelectTime] % 100 / 10;
	//aPosTexU[2] = g_nTime[s_nSelectTime] % 10;

	VERTEX_2D*pVtx;												//���_���ւ̃|�C���^

																//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (g_Time.nCntTime = 0; g_Time.nCntTime < MAX_CNT_TIME; g_Time.nCntTime++)
	{
		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f + (aPosTexU[g_Time.nCntTime] * 0.1f), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (aPosTexU[g_Time.nCntTime] * 0.1f), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (aPosTexU[g_Time.nCntTime] * 0.1f), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (aPosTexU[g_Time.nCntTime] * 0.1f), 1.0f);

		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuff->Unlock();

	/*if (g_nTime <= 0)
	{
	if (GetFade() == FADE_NONE)
	{
	ChangeMode(MODE_RESULT);
	}
	}*/

}

//�^�C���̕`�揈��
void DrawTime(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (g_Time.nCntTime = 0; g_Time.nCntTime < MAX_CNT_TIME; g_Time.nCntTime++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexture);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			4 * g_Time.nCntTime,
			2);
	}
}

//
// �ݒ�
//
void SetTime(int nTime)
{
	g_Time.nTime = nTime;
}

TIME *GetTime(void)
{
	return &g_Time;		//�u&�v���g���̂̓|�C���^���g�p���Ă邩��
}