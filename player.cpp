//=========================================
// 
// �v���C���[�̏���
// Author YudaKaito
// 
//=========================================

//-----------------------------------------
// include
//-----------------------------------------
#include "main.h"
#include "input.h"
#include "player.h"
#include <assert.h>

//-----------------------------------------
// �X�^�e�B�b�N�ϐ�
//-----------------------------------------
static LPDIRECT3DTEXTURE9 s_pTexture = NULL;		// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
static Player s_player[2] = {};

//-----------------------------------------
// �v���g�^�C�v�錾
//-----------------------------------------
static void MovePlayer(void);
static void JumpPlayer(void);

//=========================================
// �v���C���[�̏���������
//=========================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���g
	Player *pPlayer = s_player;

	// �e�N�X�`���̓Ǎ�
	D3DXCreateTextureFromFile(pDevice,PLAYER_TEX,&s_pTexture);

	pPlayer->pos = D3DXVECTOR3(600.0f, SCREEN_HEIGHT - 50.0f, 0.0f);	// �ʒu��������
	pPlayer->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						// �ړ��ʂ�������
	pPlayer->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						// ������������
	pPlayer->fWidth = PLAYER_WIDTH;										// �v���C���[�̕�
	pPlayer->fHeigth = PLAYER_HEIGTH;									// �v���C���[�̍���
	pPlayer->bUse = true;												// �v���C���[�̕\���̗L��
	pPlayer->state = PLAYERSTATE_APPEAR;								// �v���C���[�̃X�e�[�^�X
	pPlayer->RevivalInterval = 0;										// �����̃C���^�[�o��

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(pPlayer->pos.x - pPlayer->fWidth, pPlayer->pos.y - pPlayer->fHeigth, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pPlayer->pos.x + pPlayer->fWidth, pPlayer->pos.y - pPlayer->fHeigth, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pPlayer->pos.x - pPlayer->fWidth, pPlayer->pos.y + pPlayer->fHeigth, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pPlayer->pos.x + pPlayer->fWidth, pPlayer->pos.y + pPlayer->fHeigth, 0.0f);

	// ���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(0.4f, 0.71f, 0.63f, 1.0f);
	pVtx[1].col = D3DXCOLOR(0.4f, 0.71f, 0.63f, 1.0f);
	pVtx[2].col = D3DXCOLOR(0.4f, 0.71f, 0.63f, 1.0f);
	pVtx[3].col = D3DXCOLOR(0.4f, 0.71f, 0.63f, 1.0f);

	// rhw�̐ݒ�
	pVtx[0].rhw	= 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//=========================================
// �v���C���[�̏I������
//=========================================
void UninitPlayer(void)
{
	// �e�N�X�`���̔j��
	if (s_pTexture != NULL)
	{
		s_pTexture->Release();
		s_pTexture = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (s_pVtxBuff != NULL)
	{
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//=========================================
// �v���C���[�̍X�V����
//=========================================
void UpdatePlayer(void)
{
	VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^
	Player *pPlayer = s_player;

	switch (pPlayer->state)
	{
	case PLAYERSTATE_APPEAR:	// �v���C���[���o����
		pPlayer->state = PLAYERSTATE_NORMAL;
		pPlayer->bUse = true;
		pPlayer->move.y = 0.0f;
		pPlayer->rot.z = 0.0f;
		pPlayer->pos = D3DXVECTOR3(600.0f, SCREEN_HEIGHT - 50.0f, 0.0f);	// �ʒu��������
		break;
	case PLAYERSTATE_NORMAL:	// �v���C���[��������
		// �d�͂̉��Z
		pPlayer->move.y += WOARD_GRAVITY;

		// �O��̍��W���X�V
		pPlayer->posOld = pPlayer->pos;

		JumpPlayer();
		MovePlayer();

		break;
	default:
		assert(false);
		break;
	}

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//=========================================
// �v���C���[�̕`�揈��
//=========================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���g
	Player *pPlayer = GetPlayer();

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (pPlayer->bUse)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, s_pTexture);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//=========================================
// �v���C���[�̈ړ�����
//=========================================
void MovePlayer(void)
{
	Player *pPlayer = s_player;
}

//=========================================
// �v���C���[�̃W�����v����
//=========================================
void JumpPlayer(void)
{
	Player *pPlayer = s_player;
}

//=========================================
// �v���C���[�̎擾����
//=========================================
Player* GetPlayer(void)
{
	return s_player;
}
