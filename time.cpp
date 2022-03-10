//------------------------------
//���Ԑݒ�
//ACG000
//Author: goto yuuki
//
//------------------------------
#include "time.h"
#include "math.h"								//���𐶂ݏo���ϐ�

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureTime = NULL;			//�e�N�X�`���w�m�|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTime = NULL;		//���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_posTime[3];							//�^�C���̈ʒu
bool bTimeFlag;										//�^�C�g���̈ړ��̃t���O
static int g_nTime[MAX_TIME];						//�^�C���̒l
int g_nCntTime;										//�^�C���̃J�E���^�[
static int s_nSelectTime;

//�^�C���̏���������
void InitTime(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntTime;
	s_nSelectTime = 0;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"TEXTURE/number000.png",
		&g_pTextureTime);

	bool bTimeFlag = false;									//�^�C�g���̈ړ��̃t���O
	g_nTime[MAX_TIME] =  15,30,45,90, INFINITY;				//�^�C���̒l
	g_nCntTime = 0;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 3,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTime,
		NULL);

	VERTEX_2D*pVtx;									//���_���ւ̃|�C���^

	for (nCntTime = 0; nCntTime < 3; nCntTime++)
	{
		g_posTime[nCntTime] = D3DXVECTOR3(600.0f + (20.0f + (20.0f * 2) * nCntTime), 0.0f + 60.0f, 0.0f);
	}

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntTime = 0; nCntTime < 3; nCntTime++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_posTime[nCntTime].x - 20.0f, g_posTime[nCntTime].y - 35.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posTime[nCntTime].x + 20.0f, g_posTime[nCntTime].y - 35.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posTime[nCntTime].x - 20.0f, g_posTime[nCntTime].y + 35.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posTime[nCntTime].x + 20.0f, g_posTime[nCntTime].y + 35.0f, 0.0f);

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
	g_pVtxBuffTime->Unlock();

}
//�^�C���̏I������
void UninitTime(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureTime != NULL)
	{
		g_pTextureTime->Release();
		g_pTextureTime = NULL;
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffTime != NULL)
	{
		g_pVtxBuffTime->Release();
		g_pVtxBuffTime = NULL;
	}
}
//�^�C���̍X�V����
void UpdateTime(int *p_nSelectTime)
{
	int aPosTexU[3];			//�e���̐������i�[

	g_nCntTime++;				//nCntTime��������Ă���
	if (g_nCntTime % 60 == 0)		//nCntTime��0��菬�����Ȃ�����
	{
		g_nTime[s_nSelectTime]--;				//���Ԃ���b�������Ă���
	}

	aPosTexU[0] = g_nTime[s_nSelectTime] % 1000 / 100;
	aPosTexU[1] = g_nTime[s_nSelectTime] % 100 / 10;
	aPosTexU[2] = g_nTime[s_nSelectTime] % 10;

	VERTEX_2D*pVtx;												//���_���ւ̃|�C���^

																//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTime = 0; nCntTime < 3; nCntTime++)
	{
		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f + (aPosTexU[nCntTime] * 0.1f), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (aPosTexU[nCntTime] * 0.1f), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (aPosTexU[nCntTime] * 0.1f), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (aPosTexU[nCntTime] * 0.1f), 1.0f);

		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffTime->Unlock();

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
	int nCntTime;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTime, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntTime = 0; nCntTime < 3; nCntTime++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureTime);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			4 * nCntTime,
			2);
	}
}
void SetTime(void)
{

}