//================================//
//								  //
//       effect.cpp				  //
//       Author: Ricci Alex		  //
//								  //
//================================//

//====================================
//�C���N���[�h�t�@�C��
//====================================
#include "effect.h"
#include "input.h"

//====================================
//�O���[�o���ϐ�
//====================================
static Effect g_aEffect[MAX_EFFECT];							//�G�t�F�N�g�^�̔z��
static LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;			//���_�o�b�t�@�ւ̃|�C���^
static LPDIRECT3DTEXTURE9 g_apTexEffect = NULL;					//�e�N�X�`���ւ̃|�C���^

//====================================
//�G�t�F�N�g�̏���������
//====================================
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X���̎擾
	VERTEX_2D*pVtx;									//���_���ւ̃|�C���^

													//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\shadow000.jpg",
		&g_apTexEffect);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EFFECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		ZeroMemory(&g_aEffect[nCntEffect], sizeof(Effect));

		//���_���W�̐ݒ�
		pVtx[(nCntEffect * 4) + 0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[(nCntEffect * 4) + 1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[(nCntEffect * 4) + 2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[(nCntEffect * 4) + 3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//rhw�̐ݒ�
		pVtx[(nCntEffect * 4) + 0].rhw = 1.0f;
		pVtx[(nCntEffect * 4) + 1].rhw = 1.0f;
		pVtx[(nCntEffect * 4) + 2].rhw = 1.0f;
		pVtx[(nCntEffect * 4) + 3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[(nCntEffect * 4) + 0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[(nCntEffect * 4) + 1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[(nCntEffect * 4) + 2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[(nCntEffect * 4) + 3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[(nCntEffect * 4) + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[(nCntEffect * 4) + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[(nCntEffect * 4) + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[(nCntEffect * 4) + 3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEffect->Unlock();
}

//====================================
//�G�t�F�N�g�̏I������
//====================================
void UninitEffect(void)
{
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}

	//�e�N�X�`���|�C���^�̔j��
	if (g_apTexEffect != NULL)
	{
		g_apTexEffect->Release();
		g_apTexEffect = NULL;
	}
}

//====================================
//�G�t�F�N�g�̍X�V����
//====================================
void UpdateEffect(void)
{
	VERTEX_2D *pVtx = NULL;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{
			g_aEffect[nCntEffect].pos += g_aEffect[nCntEffect].move;
			g_aEffect[nCntEffect].move += g_aEffect[nCntEffect].acceleration;
			g_aEffect[nCntEffect].fSize -= g_aEffect[nCntEffect].fDeltaSize;
			g_aEffect[nCntEffect].col -= g_aEffect[nCntEffect].fDeltaCol;
			g_aEffect[nCntEffect].nLife--;

			if (g_aEffect[nCntEffect].nLife <= 0)
			{
				g_aEffect[nCntEffect].bUse = false;
			}
			if (g_aEffect[nCntEffect].fSize.x <= 0.0f || g_aEffect[nCntEffect].fSize.y <= 0.0f)
			{
				g_aEffect[nCntEffect].bUse = false;
			}

			pVtx[(nCntEffect * 4) + 0].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fSize.x, g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fSize.y, 0.0f);
			pVtx[(nCntEffect * 4) + 1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fSize.x, g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fSize.y, 0.0f);
			pVtx[(nCntEffect * 4) + 2].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fSize.x, g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fSize.y, 0.0f);
			pVtx[(nCntEffect * 4) + 3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fSize.x, g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fSize.y, 0.0f);

			pVtx[(nCntEffect * 4) + 0].col = g_aEffect[nCntEffect].col;
			pVtx[(nCntEffect * 4) + 1].col = g_aEffect[nCntEffect].col;
			pVtx[(nCntEffect * 4) + 2].col = g_aEffect[nCntEffect].col;
			pVtx[(nCntEffect * 4) + 3].col = g_aEffect[nCntEffect].col;
		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEffect->Unlock();
}

//====================================
//�G�t�F�N�g�̕`�揈��
//====================================
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���Z�����̐ݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTexEffect);

			//�f�B�X�N��`�悷��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * 4, 2);
		}
	}

	//�ݒ�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pDevice->SetTexture(0, NULL);
}

//====================================
//�G�t�F�N�g�̐ݒ菈��
//====================================
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 acceleration,
	D3DXVECTOR3 Size, D3DXVECTOR3 DeltaSize, D3DXCOLOR col, D3DXCOLOR Delta, int Life, EFFECT_TYPE Type)
{
	VERTEX_2D *pVtx = NULL;
	
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == false)
		{
			g_aEffect[nCntEffect].pos = pos;
			g_aEffect[nCntEffect].move = move;
			g_aEffect[nCntEffect].acceleration = acceleration;
			g_aEffect[nCntEffect].fSize = Size;
			g_aEffect[nCntEffect].fDeltaSize = DeltaSize;
			g_aEffect[nCntEffect].col = col;
			g_aEffect[nCntEffect].fDeltaCol = Delta;
			g_aEffect[nCntEffect].nLife = Life;
			g_aEffect[nCntEffect].type = Type;
			g_aEffect[nCntEffect].bUse = true;

			pVtx[(nCntEffect * 4) + 0].pos = D3DXVECTOR3(pos.x - Size.x, pos.y - Size.y, 0.0f);
			pVtx[(nCntEffect * 4) + 1].pos = D3DXVECTOR3(pos.x + Size.x, pos.y - Size.y, 0.0f);
			pVtx[(nCntEffect * 4) + 2].pos = D3DXVECTOR3(pos.x - Size.x, pos.y + Size.y, 0.0f);
			pVtx[(nCntEffect * 4) + 3].pos = D3DXVECTOR3(pos.x + Size.x, pos.y + Size.y, 0.0f);

			pVtx[(nCntEffect * 4) + 0].col = col;
			pVtx[(nCntEffect * 4) + 1].col = col;
			pVtx[(nCntEffect * 4) + 2].col = col;
			pVtx[(nCntEffect * 4) + 3].col = col;

			switch (Type)
			{
			case EFFECT_TYPE_WALL_IMPACT:

				pVtx[(nCntEffect * 4) + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[(nCntEffect * 4) + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[(nCntEffect * 4) + 2].tex = D3DXVECTOR2(0.0f, 0.5f);
				pVtx[(nCntEffect * 4) + 3].tex = D3DXVECTOR2(1.0f, 0.5f);

				break;
			}

			break;
		}
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEffect->Unlock();
}