//================================//
//								  //
//       �z�ǂ̃t�@�C��	    	  //
//       Author: Ricci Alex		  //
//								  //
//================================//

//====================================
//�C���N���[�h�t�@�C��
//====================================
#include "pipe.h"
#include "effect.h"

//====================================
//�O���[�o���ϐ�
//====================================
static Pipe g_aPipe[MAX_PIPE];									//�z�ǌ^�̃O���[�o���ϐ���錾����
static LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPipe = NULL;			//���_�o�b�t�@�ւ̃|�C���^
static LPDIRECT3DTEXTURE9 g_apTexPipe[PIPE_TYPE_MAX] = {};		//�e�N�X�`���ւ̃|�C���^

//�z�ǂ̏���������
void InitPipe(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X���̎擾
	VERTEX_2D*pVtx;									//���_���ւ̃|�C���^

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Pipes\\SteamPipe1.png",
		&g_apTexPipe[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Pipes\\SteamPipeVertical1.png",
		&g_apTexPipe[1]);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_PIPE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPipe,
		NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPipe->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPipe = 0; nCntPipe < MAX_PIPE; nCntPipe++)
	{
		ZeroMemory(&g_aPipe[nCntPipe], sizeof(Pipe));

		//���_���W�̐ݒ�
		pVtx[(nCntPipe * 4) + 0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[(nCntPipe * 4) + 1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[(nCntPipe * 4) + 2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[(nCntPipe * 4) + 3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//rhw�̐ݒ�
		pVtx[(nCntPipe * 4) + 0].rhw = 1.0f;
		pVtx[(nCntPipe * 4) + 1].rhw = 1.0f;
		pVtx[(nCntPipe * 4) + 2].rhw = 1.0f;
		pVtx[(nCntPipe * 4) + 3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[(nCntPipe * 4) + 0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[(nCntPipe * 4) + 1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[(nCntPipe * 4) + 2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[(nCntPipe * 4) + 3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[(nCntPipe * 4) + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[(nCntPipe * 4) + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[(nCntPipe * 4) + 2].tex = D3DXVECTOR2(0.0f, 0.95f);
		pVtx[(nCntPipe * 4) + 3].tex = D3DXVECTOR2(1.0f, 0.95f);
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPipe->Unlock();
}

//�z�ǂ̏I������
void UninitPipe(void)
{
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffPipe != NULL)
	{
		g_pVtxBuffPipe->Release();
		g_pVtxBuffPipe = NULL;
	}

	//�e�N�X�`���|�C���^�̔j��
	for (int nCntTex = 0; nCntTex < PIPE_TYPE_MAX; nCntTex++)
	{
		if (g_apTexPipe[nCntTex] != NULL)
		{
			g_apTexPipe[nCntTex]->Release();
			g_apTexPipe[nCntTex] = NULL;
		}
	}
}

//�z�ǂ̍X�V����
void UpdatePipe(void)
{
	VERTEX_2D *pVtx = NULL;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPipe->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPipe = 0; nCntPipe < MAX_PIPE; nCntPipe++)
	{
		if (g_aPipe[nCntPipe].bUse == true)
		{
			g_aPipe[nCntPipe].nCntAnim++;
			g_aPipe[nCntPipe].nCntMove++;

			if (g_aPipe[nCntPipe].nType == PIPE_TYPE_VERTICAL)
			{
				g_aPipe[nCntPipe].pos.y = (3.0f - (g_aPipe[nCntPipe].fRand * 2.0f)) * 10.0f * sin(g_aPipe[nCntPipe].fRand * (g_aPipe[nCntPipe].nCntMove + g_aPipe[nCntPipe].pos.x) * 0.025f) + 730.0f;

				if (g_aPipe[nCntPipe].nCntAnim > g_aPipe[nCntPipe].nDelay - 30)
				{
					for (int nCntEffect = 0; nCntEffect < 30; nCntEffect++)
					{
						SetEffect(D3DXVECTOR3(g_aPipe[nCntPipe].pos.x, g_aPipe[nCntPipe].pos.y - g_aPipe[nCntPipe].size.y + 5.0f, 0.0f), 0.0f, EFFECT_TYPE_SMOKE_VERTICAL);
					}

					if (g_aPipe[nCntPipe].nCntAnim > g_aPipe[nCntPipe].nDelay)
					{
						g_aPipe[nCntPipe].nCntAnim = 0;
						g_aPipe[nCntPipe].nDelay = ((rand() % 1001) + 300);
					}
				}
			}
			else
			{
				if (g_aPipe[nCntPipe].pos.x <= SCREEN_WIDTH * 0.5f)
				{
					g_aPipe[nCntPipe].pos.x = 30.0f * sin((g_aPipe[nCntPipe].nCntMove + g_aPipe[nCntPipe].pos.y) * 0.03f) + 460.0f;

					if (g_aPipe[nCntPipe].nCntAnim > g_aPipe[nCntPipe].nDelay - 60)
					{
						for (int nCntEffect = 0; nCntEffect < 50; nCntEffect++)
						{
							SetEffect(D3DXVECTOR3(g_aPipe[nCntPipe].pos.x - g_aPipe[nCntPipe].size.x + 5.0f, g_aPipe[nCntPipe].pos.y + g_aPipe[nCntPipe].size.y - 12.0f, 0.0f), 0.0f, EFFECT_TYPE_SMOKE);
						}

						if (g_aPipe[nCntPipe].nCntAnim > g_aPipe[nCntPipe].nDelay)
						{
							g_aPipe[nCntPipe].nCntAnim = 0;
							g_aPipe[nCntPipe].nDelay = ((rand() % 601) + 300);
						}
					}
				}
				else
				{
					g_aPipe[nCntPipe].pos.x = 15.0f * sin((g_aPipe[nCntPipe].nCntMove + g_aPipe[nCntPipe].pos.y) * 0.03f) + 800.0f;

					if (g_aPipe[nCntPipe].nCntAnim > g_aPipe[nCntPipe].nDelay - 60)
					{
						for (int nCntEffect = 0; nCntEffect < 50; nCntEffect++)
						{
							SetEffect(D3DXVECTOR3(g_aPipe[nCntPipe].pos.x + g_aPipe[nCntPipe].size.x - 5.0f, g_aPipe[nCntPipe].pos.y + g_aPipe[nCntPipe].size.y - 12.0f, 0.0f), 0.0f, EFFECT_TYPE_SMOKE);
						}

						if (g_aPipe[nCntPipe].nCntAnim > g_aPipe[nCntPipe].nDelay)
						{
							g_aPipe[nCntPipe].nCntAnim = 0;
							g_aPipe[nCntPipe].nDelay = ((rand() % 601) + 300);
						}
					}
				}
			}

			//���_���W�̍X�V
			if (g_aPipe[nCntPipe].nType == PIPE_TYPE_VERTICAL)
			{
				pVtx[(nCntPipe * 4) + 0].pos = D3DXVECTOR3(g_aPipe[nCntPipe].pos.x - g_aPipe[nCntPipe].size.x, g_aPipe[nCntPipe].pos.y - g_aPipe[nCntPipe].size.y, 0.0f);
				pVtx[(nCntPipe * 4) + 1].pos = D3DXVECTOR3(g_aPipe[nCntPipe].pos.x + g_aPipe[nCntPipe].size.x, g_aPipe[nCntPipe].pos.y - g_aPipe[nCntPipe].size.y, 0.0f);
				pVtx[(nCntPipe * 4) + 2].pos = D3DXVECTOR3(g_aPipe[nCntPipe].pos.x - (g_aPipe[nCntPipe].size.x - (g_aPipe[nCntPipe].pos.y * 0.01f)), g_aPipe[nCntPipe].pos.y + g_aPipe[nCntPipe].size.y, 0.0f);
				pVtx[(nCntPipe * 4) + 3].pos = D3DXVECTOR3(g_aPipe[nCntPipe].pos.x + (g_aPipe[nCntPipe].size.x - (g_aPipe[nCntPipe].pos.y * 0.01f)), g_aPipe[nCntPipe].pos.y + g_aPipe[nCntPipe].size.y, 0.0f);
			}
			else
			{
				pVtx[(nCntPipe * 4) + 0].pos = D3DXVECTOR3(g_aPipe[nCntPipe].pos.x - g_aPipe[nCntPipe].size.x, g_aPipe[nCntPipe].pos.y - g_aPipe[nCntPipe].size.y, 0.0f);
				pVtx[(nCntPipe * 4) + 1].pos = D3DXVECTOR3(g_aPipe[nCntPipe].pos.x + g_aPipe[nCntPipe].size.x, g_aPipe[nCntPipe].pos.y - g_aPipe[nCntPipe].size.y, 0.0f);
				pVtx[(nCntPipe * 4) + 2].pos = D3DXVECTOR3(g_aPipe[nCntPipe].pos.x - g_aPipe[nCntPipe].size.x, g_aPipe[nCntPipe].pos.y + g_aPipe[nCntPipe].size.y, 0.0f);
				pVtx[(nCntPipe * 4) + 3].pos = D3DXVECTOR3(g_aPipe[nCntPipe].pos.x + g_aPipe[nCntPipe].size.x, g_aPipe[nCntPipe].pos.y + g_aPipe[nCntPipe].size.y, 0.0f);
			}
		}
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPipe->Unlock();
}

//�z�ǂ̕`�揈��
void DrawPipe(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//�f�o�C�X�̎擾

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPipe, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntPipe = 0; nCntPipe < MAX_PIPE; nCntPipe++)
	{
		if (g_aPipe[nCntPipe].bUse == true)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTexPipe[g_aPipe[nCntPipe].nType]);

			//���Ԃ�`�悷��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPipe * 4, 2);
		}
	}
}

//�z�ǂ̐ݒ菈��
void SetPipe(D3DXVECTOR3 pos, D3DXVECTOR3 size, PIPE_TYPE type)
{
	VERTEX_2D *pVtx = NULL;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPipe->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPipe = 0; nCntPipe < MAX_PIPE; nCntPipe++)
	{
		if (g_aPipe[nCntPipe].bUse == false)
		{
			g_aPipe[nCntPipe].pos = pos;
			g_aPipe[nCntPipe].nCntAnim = 0;
			g_aPipe[nCntPipe].size = size;
			g_aPipe[nCntPipe].nType = type;
			g_aPipe[nCntPipe].nCntMove = 0;
			g_aPipe[nCntPipe].nDelay = ((rand() % 601) + 300);
			g_aPipe[nCntPipe].fRand = ((rand() % 701) + 300) * 0.001;

			//���_���W�̐ݒ�
			pVtx[(nCntPipe * 4) + 0].pos = D3DXVECTOR3(g_aPipe[nCntPipe].pos.x - g_aPipe[nCntPipe].size.x, g_aPipe[nCntPipe].pos.y - g_aPipe[nCntPipe].size.y, 0.0f);
			pVtx[(nCntPipe * 4) + 1].pos = D3DXVECTOR3(g_aPipe[nCntPipe].pos.x + g_aPipe[nCntPipe].size.x, g_aPipe[nCntPipe].pos.y - g_aPipe[nCntPipe].size.y, 0.0f);
			pVtx[(nCntPipe * 4) + 2].pos = D3DXVECTOR3(g_aPipe[nCntPipe].pos.x - g_aPipe[nCntPipe].size.x, g_aPipe[nCntPipe].pos.y + g_aPipe[nCntPipe].size.y, 0.0f);
			pVtx[(nCntPipe * 4) + 3].pos = D3DXVECTOR3(g_aPipe[nCntPipe].pos.x + g_aPipe[nCntPipe].size.x, g_aPipe[nCntPipe].pos.y + g_aPipe[nCntPipe].size.y, 0.0f);

			//���_�J���[�̐ݒ�
			pVtx[(nCntPipe * 4) + 0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[(nCntPipe * 4) + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[(nCntPipe * 4) + 2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[(nCntPipe * 4) + 3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			switch (type)
			{
			default:
				break;

			case PIPE_TYPE_STEAM:

				//�e�N�X�`�����W�̐ݒ�
				pVtx[(nCntPipe * 4) + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[(nCntPipe * 4) + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[(nCntPipe * 4) + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[(nCntPipe * 4) + 3].tex = D3DXVECTOR2(1.0f, 1.0f);

				break;

			case PIPE_TYPE_VERTICAL:

				g_aPipe[nCntPipe].nDelay = ((rand() % 1001) + 300);

				pVtx[(nCntPipe * 4) + 0].tex = D3DXVECTOR2(0.0f, 0.05f);
				pVtx[(nCntPipe * 4) + 1].tex = D3DXVECTOR2(1.0f, 0.05f);
				pVtx[(nCntPipe * 4) + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[(nCntPipe * 4) + 3].tex = D3DXVECTOR2(1.0f, 1.0f);

				break;
			}

			g_aPipe[nCntPipe].bUse = true;

			break;
		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPipe->Unlock();
}