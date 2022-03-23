//==========================================//
//										    //
//       landingPoint.cpp				    //
//       Author: Ricci Alex				    //
//										    //
//==========================================//

//====================================
//�C���N���[�h�t�@�C��
//====================================
#include "landingPoint.h"

//====================================
//�O���[�o���ϐ�
//====================================
static LandingMark g_aLandingMark;										
static LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLandingMark = NULL;			//���_�o�b�t�@�ւ̃|�C���^
static LPDIRECT3DTEXTURE9 g_apTexLandingMark = NULL;					//�e�N�X�`���ւ̃|�C���^

void InitLandingMark(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X���̎擾
	VERTEX_2D*pVtx;									//���_���ւ̃|�C���^

													//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\target.png",
		&g_apTexLandingMark);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffLandingMark,
		NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffLandingMark->Lock(0, 0, (void**)&pVtx, 0);

	ZeroMemory(&g_aLandingMark, sizeof(LandingMark));

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
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffLandingMark->Unlock();
}

void UninitLandingMark(void)
{
	//�e�N�X�`���|�C���^�̔j��
	if (g_apTexLandingMark != NULL)
	{
		g_apTexLandingMark->Release();
		g_apTexLandingMark = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffLandingMark != NULL)
	{
		g_pVtxBuffLandingMark->Release();
		g_pVtxBuffLandingMark = NULL;
	}
}

void UpdateLandingMark(void)
{
	VERTEX_2D *pVtx = NULL;			//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffLandingMark->Lock(0, 0, (void**)&pVtx, 0);

	if (g_aLandingMark.bUse == true)
	{
		g_aLandingMark.nCntAnim++;

		if (g_aLandingMark.nCntAnim % 7 == 6)
		{
			g_aLandingMark.nAnimPattern ^= 1;
		}

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f * (1 - g_aLandingMark.nAnimPattern), 1.0f * g_aLandingMark.nAnimPattern, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f * (1 - g_aLandingMark.nAnimPattern), 1.0f * g_aLandingMark.nAnimPattern, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f * (1 - g_aLandingMark.nAnimPattern), 1.0f * g_aLandingMark.nAnimPattern, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f * (1 - g_aLandingMark.nAnimPattern), 1.0f * g_aLandingMark.nAnimPattern, 1.0f);
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffLandingMark->Unlock();
}

void DrawLandingMark(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffLandingMark, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_aLandingMark.bUse == true)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_apTexLandingMark);

		//�f�B�X�N��`�悷��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,0, 2);
	}
}

void SetLandingMark(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fHeight, float fSpeed, float fAcc, float fSize)
{
	VERTEX_2D *pVtx = NULL;			//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffLandingMark->Lock(0, 0, (void**)&pVtx, 0);

	if (g_aLandingMark.bUse == false)
	{
		while (fHeight > 0.0f)
		{
			fHeight += fSpeed;
			fSpeed += fAcc;
			pos += move;
		}

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(pos.x - (fSize * 0.25f), pos.y - (fSize * 0.25f), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pos.x + (fSize * 0.25f), pos.y - (fSize * 0.25f), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pos.x - (fSize * 0.25f), pos.y + (fSize * 0.25f), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pos.x + (fSize * 0.25f), pos.y + (fSize * 0.25f), 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		g_aLandingMark.pos = pos;				//�ʒu�̐ݒ�
		g_aLandingMark.nCntAnim = 0;			//�A�j���[�V�����J�E���^�[�̐ݒ�
		g_aLandingMark.nAnimPattern = 0;		//�A�j���[�V�����p�̕ϐ��̐ݒ�
		g_aLandingMark.bUse = true;				//�g�p����Ă����Ԃɂ���
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffLandingMark->Unlock();
}

void DestroyLandingMark(void)
{
	g_aLandingMark.bUse = false;
}

LandingMark* GetLandingMark(void)
{
	return &g_aLandingMark;
}
