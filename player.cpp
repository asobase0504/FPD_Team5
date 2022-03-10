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
#include "disk.h"
#include <assert.h>

//-----------------------------------------
// �}�N����`
//-----------------------------------------
#define NUM_PLAYER	(2)

//-----------------------------------------
// �X�^�e�B�b�N�ϐ�
//-----------------------------------------
static Player s_player[NUM_PLAYER] = {};

//-----------------------------------------
// �v���g�^�C�v�錾
//-----------------------------------------
static void MovePlayer(int nIdxPlayer);
static void JumpPlayer(int nIdxPlayer);
static void ThrowPlayer(int nIdxPlayer);

//=========================================
// �v���C���[�̏���������
//=========================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���g
	Player *pPlayer = s_player;

	for (int i = 0; i < NUM_PLAYER; i++, pPlayer++)
	{
		// �e�N�X�`���̓Ǎ�
		D3DXCreateTextureFromFile(pDevice, PLAYER_TEX, &pPlayer->pTexture);

		pPlayer->pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f + i * 200.0f, SCREEN_HEIGHT * 0.5f, 0.0f);	// �ʒu��������
		pPlayer->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						// �ړ��ʂ�������
		pPlayer->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						// ������������
		pPlayer->fWidth = PLAYER_WIDTH;										// �v���C���[�̕�
		pPlayer->fHeigth = PLAYER_HEIGTH;									// �v���C���[�̍���
		pPlayer->bUse = true;												// �v���C���[�̕\���̗L��
		pPlayer->RevivalInterval = 0;										// �����̃C���^�[�o��

		// ���_�o�b�t�@�̐���
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&pPlayer->pVtxBuff,
			NULL);

		VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		pPlayer->pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

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
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// ���_�o�b�t�@���A�����b�N����
		pPlayer->pVtxBuff->Unlock();
	}
}

//=========================================
// �v���C���[�̏I������
//=========================================
void UninitPlayer(void)
{
	Player *pPlayer = s_player;

	for (int i = 0; i < NUM_PLAYER; i++, pPlayer++)
	{
		// �e�N�X�`���̔j��
		if (pPlayer->pTexture != NULL)
		{
			pPlayer->pTexture->Release();
			pPlayer->pTexture = NULL;
		}

		// ���_�o�b�t�@�̔j��
		if (pPlayer->pVtxBuff != NULL)
		{
			pPlayer->pVtxBuff->Release();
			pPlayer->pVtxBuff = NULL;
		}
	}
}

//=========================================
// �v���C���[�̍X�V����
//=========================================
void UpdatePlayer(void)
{
	Player *pPlayer = s_player;

	for (int nIdxPlayer = 0; nIdxPlayer < NUM_PLAYER; nIdxPlayer++, pPlayer++)
	{
		pPlayer->pos += pPlayer->move;

		// �O��̍��W���X�V
		pPlayer->posOld = pPlayer->pos;

		JumpPlayer(nIdxPlayer);		// ����
		MovePlayer(nIdxPlayer);		// �ړ�
		ThrowPlayer(nIdxPlayer);	// ������

		VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		pPlayer->pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(pPlayer->pos.x - pPlayer->fWidth, pPlayer->pos.y - pPlayer->fHeigth, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pPlayer->pos.x + pPlayer->fWidth, pPlayer->pos.y - pPlayer->fHeigth, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pPlayer->pos.x - pPlayer->fWidth, pPlayer->pos.y + pPlayer->fHeigth, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pPlayer->pos.x + pPlayer->fWidth, pPlayer->pos.y + pPlayer->fHeigth, 0.0f);

		// ���_�o�b�t�@���A�����b�N����
		pPlayer->pVtxBuff->Unlock();
	}
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

	for (int i = 0; i < NUM_PLAYER; i++, pPlayer++)
	{
		// ���_�o�b�t�@���f�[�^�X�g���[���ݒ�
		pDevice->SetStreamSource(0, pPlayer->pVtxBuff, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		if (pPlayer->bUse)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, pPlayer->pTexture);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		}
	}
}

//=========================================
// �v���C���[�̈ړ�����
//=========================================
void MovePlayer(int nIdxPlayer)
{
	D3DXVECTOR3 inputMove(0.0f,0.0f,0.0f);
	float moveLength = 0.0f;
	Player *pPlayer = &s_player[nIdxPlayer];

	if (IsJoyPadUse(nIdxPlayer))
	{
		inputMove = GetJoypadStick(JOYKEY_LEFT_STICK, nIdxPlayer);

		if (inputMove.x != 0.0f || inputMove.y != 0.0f)
		{
			moveLength = D3DXVec3Length(&inputMove);

			if (moveLength > 1.0f)
			{
				moveLength = 1.0f;
			}
		}
	}
	else
	{
		if (GetKeyboardPress(DIK_W))
		{
			pPlayer->move.y -= 1.0f;
		}
		if (GetKeyboardPress(DIK_A))
		{
			pPlayer->move.x -= 1.0f;
		}
		if (GetKeyboardPress(DIK_S))
		{
			pPlayer->move.y += 1.0f;
		}
		if (GetKeyboardPress(DIK_D))
		{
			pPlayer->move.x += 1.0f;
		}
	}

	if (moveLength > 0.0f)
	{
		D3DXVec3Normalize(&inputMove, &inputMove);
	}

	pPlayer->move = inputMove * 0.9f * 5.0f;
}

//=========================================
// �v���C���[�̃W�����v����
//=========================================
void JumpPlayer(int nIdxPlayer)
{
	Player *pPlayer = &s_player[nIdxPlayer];

}

//=========================================
// �v���C���[�̓����鏈��
//=========================================
void ThrowPlayer(int nIdxPlayer)
{
	Player *pPlayer = &s_player[nIdxPlayer];

	D3DXVECTOR3 vec(2.0f,0.0f,0.0f);


	if (IsJoyPadUse(nIdxPlayer))
	{
		vec = GetJoypadStick(JOYKEY_LEFT_STICK, nIdxPlayer);

		if (GetJoypadTrigger(JOYKEY_B, nIdxPlayer))
		{
			SetDisk(pPlayer->pos, vec, D3DXVECTOR3(0.0f, 0.0f, 0.0f), DISK_TYPE_NORMAL, 40.0f);
		}
		if (GetJoypadTrigger(JOYKEY_A, nIdxPlayer))
		{
			SetDisk(pPlayer->pos, vec, D3DXVECTOR3(0.0f, 0.0f, 0.0f), DISK_TYPE_LOB, 40.0f);
		}
	}
	else
	{
		if (GetKeyboardPress(DIK_W))
		{
			vec.y -= 1.0f;
		}
		if (GetKeyboardPress(DIK_A))
		{
			vec.x -= 1.0f;
		}
		if (GetKeyboardPress(DIK_S))
		{
			vec.y += 1.0f;
		}
		if (GetKeyboardPress(DIK_D))
		{
			vec.x += 1.0f;
		}

		if (GetKeyboardPress(DIK_RETURN))
		{
			SetDisk(pPlayer->pos, vec, D3DXVECTOR3(0.0f, 0.0f, 0.0f), DISK_TYPE_NORMAL, 40.0f);
		}
		if (GetKeyboardPress(DIK_SPACE))
		{
			SetDisk(pPlayer->pos, vec, D3DXVECTOR3(0.0f, 0.0f, 0.0f), DISK_TYPE_LOB, 40.0f);
		}
	}
}

//=========================================
// �ݒ�
//=========================================
void SetPlayer(void)
{
}

//=========================================
// �v���C���[�̎擾����
//=========================================
Player* GetPlayer(void)
{
	return s_player;
}
