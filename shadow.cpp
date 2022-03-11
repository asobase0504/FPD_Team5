//================================//
//								  //
//       �e����(shadow.cpp)		  //
//       Author: Ricci Alex		  //
//								  //
//================================//

//====================================
//�C���N���[�h�t�@�C��
//====================================
#include "shadow.h"

//====================================
//�O���[�o���ϐ�
//====================================
static Shadow g_aShadow[MAX_SHADOW];
static LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;			//���_�o�b�t�@�ւ̃|�C���^
static LPDIRECT3DTEXTURE9 g_apTexShadow = NULL;					//�e�N�X�`���ւ̃|�C���^

//====================================
//�e�̏���������
//====================================
void InitShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X���̎擾
	VERTEX_2D*pVtx;									//���_���ւ̃|�C���^

													//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\shadow000.jpg",
		&g_apTexShadow);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_SHADOW,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffShadow,
		NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		ZeroMemory(&g_aShadow[nCntShadow], sizeof(Shadow));

		//���_���W�̐ݒ�
		pVtx[(nCntShadow * 4) + 0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[(nCntShadow * 4) + 1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[(nCntShadow * 4) + 2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[(nCntShadow * 4) + 3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//rhw�̐ݒ�
		pVtx[(nCntShadow * 4) + 0].rhw = 1.0f;
		pVtx[(nCntShadow * 4) + 1].rhw = 1.0f;
		pVtx[(nCntShadow * 4) + 2].rhw = 1.0f;
		pVtx[(nCntShadow * 4) + 3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[(nCntShadow * 4) + 0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[(nCntShadow * 4) + 1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[(nCntShadow * 4) + 2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[(nCntShadow * 4) + 3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[(nCntShadow * 4) + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[(nCntShadow * 4) + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[(nCntShadow * 4) + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[(nCntShadow * 4) + 3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffShadow->Unlock();

}

//====================================
//�e�̏I������
//====================================
void UninitShadow(void)
{
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffShadow != NULL)
	{
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}

	//�e�N�X�`���|�C���^�̔j��
	if (g_apTexShadow != NULL)
	{
		g_apTexShadow->Release();
		g_apTexShadow = NULL;
	}
}

//====================================
//�e�̍X�V����
//====================================
void UpdateShadow(void)
{
	VERTEX_2D*pVtx = NULL;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_aShadow[nCntShadow].bUse == true)
		{
			pVtx[(nCntShadow * 4) + 0].pos = D3DXVECTOR3(g_aShadow[nCntShadow].pos.x - (g_aShadow[nCntShadow].fSize * 0.5f),
				g_aShadow[nCntShadow].pos.y - (g_aShadow[nCntShadow].fSize * 0.5f), 0.0f);

			pVtx[(nCntShadow * 4) + 1].pos = D3DXVECTOR3(g_aShadow[nCntShadow].pos.x + (g_aShadow[nCntShadow].fSize * 0.5f),
				g_aShadow[nCntShadow].pos.y - (g_aShadow[nCntShadow].fSize * 0.5f), 0.0f);

			pVtx[(nCntShadow * 4) + 2].pos = D3DXVECTOR3(g_aShadow[nCntShadow].pos.x - (g_aShadow[nCntShadow].fSize * 0.5f),
				g_aShadow[nCntShadow].pos.y + (g_aShadow[nCntShadow].fSize * 0.5f), 0.0f);

			pVtx[(nCntShadow * 4) + 3].pos = D3DXVECTOR3(g_aShadow[nCntShadow].pos.x + (g_aShadow[nCntShadow].fSize * 0.5f),
				g_aShadow[nCntShadow].pos.y + (g_aShadow[nCntShadow].fSize * 0.5f), 0.0f);
		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffShadow->Unlock();
}

//====================================
//�e�̕`�揈��
//====================================
void DrawShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���Z�����̐ݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntWall = 0; nCntWall < MAX_SHADOW; nCntWall++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_apTexShadow);

		//�f�B�X�N��`�悷��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntWall * 4, 2);
	}

	//�ݒ�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pDevice->SetTexture(0, NULL);
}

//====================================
//�e�̐ݒ菈��
//====================================
int SetShadow(D3DXVECTOR3 pos, float size)
{
	VERTEX_2D*pVtx = NULL;
	int nCntShadow;										//�e���J�E���g����ϐ���錾����

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_aShadow[nCntShadow].bUse == false)
		{//�g�p����Ă��Ȃ��ꍇ

			size -= 2.0f;

			g_aShadow[nCntShadow].pos = pos;			//�e�̈ʒu�̐ݒ�
			g_aShadow[nCntShadow].pos.y += 7.0f;		
			g_aShadow[nCntShadow].fSize = size;			//�e�̑傫���̐ݒ�
			g_aShadow[nCntShadow].bUse = true;			//�g�p����Ă����Ԃɂ���

			

			pVtx[(nCntShadow * 4) + 0].pos = D3DXVECTOR3(g_aShadow[nCntShadow].pos.x - (size * 0.5f), g_aShadow[nCntShadow].pos.y - (size * 0.5f), 0.0f);
			pVtx[(nCntShadow * 4) + 1].pos = D3DXVECTOR3(g_aShadow[nCntShadow].pos.x + (size * 0.5f), g_aShadow[nCntShadow].pos.y - (size * 0.5f), 0.0f);
			pVtx[(nCntShadow * 4) + 2].pos = D3DXVECTOR3(g_aShadow[nCntShadow].pos.x - (size * 0.5f), g_aShadow[nCntShadow].pos.y + (size * 0.5f), 0.0f);
			pVtx[(nCntShadow * 4) + 3].pos = D3DXVECTOR3(g_aShadow[nCntShadow].pos.x + (size * 0.5f), g_aShadow[nCntShadow].pos.y + (size * 0.5f), 0.0f);

			break;
		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffShadow->Unlock();

	return nCntShadow;									//nCntShadow��Ԃ�
}

//====================================
//�e�̈ʒu�̍X�V����
//====================================
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos)
{
	//�e�̈ʒu�̍X�V
	g_aShadow[nIdxShadow].pos = pos;
	g_aShadow[nIdxShadow].pos.y += 7.0f;		
}
