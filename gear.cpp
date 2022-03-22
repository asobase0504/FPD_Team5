//================================//
//								  //
//       ���Ԃ̃t�@�C��	    	  //
//       Author: Ricci Alex		  //
//								  //
//================================//

//====================================
//�C���N���[�h�t�@�C��
//====================================
#include "gear.h"

//====================================
//�O���[�o���ϐ�
//====================================
static Gear g_aGear[MAX_GEAR];									//���Ԍ^�̃O���[�o���ϐ���錾����
static LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGear = NULL;			//���_�o�b�t�@�ւ̃|�C���^
static LPDIRECT3DTEXTURE9 g_apTexGear[5] = {};					//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLogo = NULL;			//���_�o�b�t�@�ւ̃|�C���^
static LPDIRECT3DTEXTURE9 g_apTexLogo = NULL;					//�e�N�X�`���ւ̃|�C���^

//====================================
//���Ԃ̏���������
//====================================
void InitGear(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X���̎擾
	VERTEX_2D*pVtx;									//���_���ւ̃|�C���^

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Gear.png",
		&g_apTexGear[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Gear2.png",
		&g_apTexGear[1]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Gear3.png",
		&g_apTexGear[2]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Gear4.png",
		&g_apTexGear[3]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Gear5.png",
		&g_apTexGear[4]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\TemporaryLogo.png",
		&g_apTexLogo);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_GEAR,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGear,
		NULL);

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffLogo,
		NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffGear->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntGear = 0; nCntGear < MAX_GEAR; nCntGear++)
	{
		ZeroMemory(&g_aGear[nCntGear], sizeof(Gear));

		//���_���W�̐ݒ�
		pVtx[(nCntGear * 4) + 0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[(nCntGear * 4) + 1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[(nCntGear * 4) + 2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[(nCntGear * 4) + 3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//rhw�̐ݒ�
		pVtx[(nCntGear * 4) + 0].rhw = 1.0f;
		pVtx[(nCntGear * 4) + 1].rhw = 1.0f;
		pVtx[(nCntGear * 4) + 2].rhw = 1.0f;
		pVtx[(nCntGear * 4) + 3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[(nCntGear * 4) + 0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[(nCntGear * 4) + 1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[(nCntGear * 4) + 2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[(nCntGear * 4) + 3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[(nCntGear * 4) + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[(nCntGear * 4) + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[(nCntGear * 4) + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[(nCntGear * 4) + 3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffGear->Unlock();

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffLogo->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

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
	g_pVtxBuffLogo->Unlock();
}

//====================================
//�͂���܂̏I������
//====================================
void UninitGear(void)
{
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffGear != NULL)
	{			  
		g_pVtxBuffGear->Release();
		g_pVtxBuffGear = NULL;
	}
	if (g_pVtxBuffLogo != NULL)
	{			 
		g_pVtxBuffLogo->Release();
		g_pVtxBuffLogo = NULL;
	}

	//�e�N�X�`���|�C���^�̔j��
	for (int nCntTex = 0; nCntTex < 5; nCntTex++)
	{
		if (g_apTexGear[nCntTex] != NULL)
		{			   
			g_apTexGear[nCntTex]->Release();
			g_apTexGear[nCntTex] = NULL;
		}
	}
	if (g_apTexLogo != NULL)
	{		  
		g_apTexLogo->Release();
		g_apTexLogo = NULL;
	}
}

//====================================
//���Ԃ̍X�V����
//====================================
void UpdateGear(void)
{
	VERTEX_2D *pVtx = NULL;						//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffGear->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntGear = 0; nCntGear < MAX_GEAR; nCntGear++)
	{
		if (g_aGear[nCntGear].bUse == true)
		{
			g_aGear[nCntGear].fAngle += g_aGear[nCntGear].fFrameRot;			//��]�p�x�̍X�V����

			//===========================================================
			//���K������
			if (g_aGear[nCntGear].fAngle > D3DX_PI * 2.0f)
			{
				g_aGear[nCntGear].fAngle += -D3DX_PI * 2.0f;
			}
			else if (g_aGear[nCntGear].fAngle < -D3DX_PI * 2.0f)
			{
				g_aGear[nCntGear].fAngle += D3DX_PI * 2.0f;
			}
			//===========================================================

			//==================================================================================================================================================================
			//���_���W�̍X�V����
			pVtx[(nCntGear * 4) + 0].pos.x = g_aGear[nCntGear].pos.x + sinf(g_aGear[nCntGear].rot + g_aGear[nCntGear].fAngle + (D3DX_PI * 0.75f)) * g_aGear[nCntGear].fLenght;
			pVtx[(nCntGear * 4) + 0].pos.y = g_aGear[nCntGear].pos.y + cosf(g_aGear[nCntGear].rot + g_aGear[nCntGear].fAngle + (D3DX_PI * 0.75f)) * g_aGear[nCntGear].fLenght;
			pVtx[(nCntGear * 4) + 0].pos.z = 0.0f;																													   
																																									   
			pVtx[(nCntGear * 4) + 1].pos.x = g_aGear[nCntGear].pos.x + sinf(g_aGear[nCntGear].rot + g_aGear[nCntGear].fAngle + (D3DX_PI * 0.25f)) * g_aGear[nCntGear].fLenght;
			pVtx[(nCntGear * 4) + 1].pos.y = g_aGear[nCntGear].pos.y + cosf(g_aGear[nCntGear].rot + g_aGear[nCntGear].fAngle + (D3DX_PI * 0.25f)) * g_aGear[nCntGear].fLenght;
			pVtx[(nCntGear * 4) + 1].pos.z = 0.0f;																													   
																																									   
			pVtx[(nCntGear * 4) + 2].pos.x = g_aGear[nCntGear].pos.x + sinf(g_aGear[nCntGear].rot + g_aGear[nCntGear].fAngle + (-D3DX_PI * 0.75f)) * g_aGear[nCntGear].fLenght;
			pVtx[(nCntGear * 4) + 2].pos.y = g_aGear[nCntGear].pos.y + cosf(g_aGear[nCntGear].rot + g_aGear[nCntGear].fAngle + (-D3DX_PI * 0.75f)) * g_aGear[nCntGear].fLenght;
			pVtx[(nCntGear * 4) + 2].pos.z = 0.0f;																													   
																																									   
			pVtx[(nCntGear * 4) + 3].pos.x = g_aGear[nCntGear].pos.x + sinf(g_aGear[nCntGear].rot + g_aGear[nCntGear].fAngle + (-D3DX_PI * 0.25f)) * g_aGear[nCntGear].fLenght;
			pVtx[(nCntGear * 4) + 3].pos.y = g_aGear[nCntGear].pos.y + cosf(g_aGear[nCntGear].rot + g_aGear[nCntGear].fAngle + (-D3DX_PI * 0.25f)) * g_aGear[nCntGear].fLenght;
			pVtx[(nCntGear * 4) + 3].pos.z = 0.0f;
			//==================================================================================================================================================================
		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffGear->Unlock();
}

//====================================
//���Ԃ̕`�揈��
//====================================
void DrawGear(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//�f�o�C�X�̎擾

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffGear, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntGear = 0; nCntGear < MAX_GEAR; nCntGear++)
	{
		if (g_aGear[nCntGear].bUse == true)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTexGear[g_aGear[nCntGear].nType]);

			//���Ԃ�`�悷��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntGear * 4, 2);
		}
	}

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffLogo, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_apTexLogo);

	//���S��`�悷��
	//pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//====================================
//�͂���܂̐ݒ菈��
//====================================
void SetGear(D3DXVECTOR3 pos, float size, float frameRot, int type)
{
	VERTEX_2D *pVtx = NULL;							//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffGear->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntGear = 0; nCntGear < MAX_GEAR; nCntGear++)
	{
		if (g_aGear[nCntGear].bUse == false)
		{	   
			g_aGear[nCntGear].pos = pos;										//���Ԃ̈ʒu�̐ݒ�
			g_aGear[nCntGear].fSize = size;										//���Ԃ̃T�C�Y�̐ݒ�

			//��]�p�̕ϐ��̐ݒ�
			g_aGear[nCntGear].fLenght = sqrtf((g_aGear[nCntGear].fSize * g_aGear[nCntGear].fSize) + (g_aGear[nCntGear].fSize * g_aGear[nCntGear].fSize)) * 0.5f;
			g_aGear[nCntGear].fAngle = atan2f(g_aGear[nCntGear].fSize * 0.5f, g_aGear[nCntGear].fSize * 0.5f);

			g_aGear[nCntGear].fFrameRot = frameRot;								//��t���[���̉�]�p�x�̐ݒ�
			g_aGear[nCntGear].nType = type;										//���Ԃ̎�ނ̐ݒ�
			
			//���_���W�̐ݒ�
			pVtx[(nCntGear * 4) + 0].pos = D3DXVECTOR3(pos.x - (size * 0.5f), pos.y - (size * 0.5f), 0.0f);
			pVtx[(nCntGear * 4) + 1].pos = D3DXVECTOR3(pos.x + (size * 0.5f), pos.y - (size * 0.5f), 0.0f);
			pVtx[(nCntGear * 4) + 2].pos = D3DXVECTOR3(pos.x - (size * 0.5f), pos.y + (size * 0.5f), 0.0f);
			pVtx[(nCntGear * 4) + 3].pos = D3DXVECTOR3(pos.x + (size * 0.5f), pos.y + (size * 0.5f), 0.0f);

			//���_�J���[�̐ݒ�
			pVtx[(nCntGear * 4) + 0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[(nCntGear * 4) + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[(nCntGear * 4) + 2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[(nCntGear * 4) + 3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			g_aGear[nCntGear].bUse = true;			//�g�p����Ă����Ԃɂ���

			break;
		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffGear->Unlock();
}