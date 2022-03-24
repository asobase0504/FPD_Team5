//================================//
//								  //
//      chain.cpp			 	  //
//       Author: Ricci Alex		  //
//								  //
//================================//

//====================================
//�C���N���[�h�t�@�C��
//====================================
#include "chain.h"

//====================================
//�O���[�o���ϐ�
//====================================
static Chain g_Chain;											//�z�ǌ^�̃O���[�o���ϐ���錾����
static LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffChain = NULL;			//���_�o�b�t�@�ւ̃|�C���^
static LPDIRECT3DTEXTURE9 g_pTexChain = NULL;					//�e�N�X�`���ւ̃|�C���^

//====================================
//���̏���������
//====================================
void InitChain(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X���̎擾
	VERTEX_2D*pVtx;									//���_���ւ̃|�C���^

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Chain.png",
		&g_pTexChain);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffChain,
		NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffChain->Lock(0, 0, (void**)&pVtx, 0);

	ZeroMemory(&g_Chain, sizeof(Chain));

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 2.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 2.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffChain->Unlock();
}

//====================================
//���̏I������
//====================================
void UninitChain(void)
{
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffChain != NULL)
	{
		g_pVtxBuffChain->Release();
		g_pVtxBuffChain = NULL;
	}

	//�e�N�X�`���|�C���^�̔j��
	if (g_pTexChain != NULL)
	{
		g_pTexChain->Release();
		g_pTexChain = NULL;
	}
}

//====================================
//���̍X�V����
//====================================
void UpdateChain(void)
{

}

//====================================
//���̕`�揈��
//====================================
void DrawChain(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//�f�o�C�X�̎擾

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffChain, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_Chain.bUse == true)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexChain);

		//���Ԃ�`�悷��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//====================================
//���̐ݒ菈��
//====================================
void SetChain(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	VERTEX_2D *pVtx = NULL;			//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffChain->Lock(0, 0, (void**)&pVtx, 0);

	if (g_Chain.bUse == false)
	{
		g_Chain.pos = pos;
		g_Chain.size = size;
		g_Chain.bUse = true;

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(pos.x - size.x, pos.y - size.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pos.x + size.x, pos.y - size.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pos.x - size.x, pos.y + size.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pos.x + size.x, pos.y + size.y, 0.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 2.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 2.0f);

		//���_�o�b�t�@���A�����b�N����
		g_pVtxBuffChain->Unlock();
	}
}