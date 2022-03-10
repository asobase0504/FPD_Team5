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
#include <stdio.h>

//-----------------------------------------
// �}�N����`
//-----------------------------------------
#define NUM_PLAYER		(2)								// �v���C���[�̐�
#define NUM_TYPE_PLAYER	(25)							// �v���C���[�̎��
#define PLAYER_FILE		"data/player.txt"				// �v���C���[�ǂݍ��݃t�@�C��
#define PLAYER_TEX		"data/TEXTURE/player00.png"		// �v���C���[�̃e�N�X�`��
#define PLAYER_SIZ		(35.0f)							// �v���C���[�̕�

//-----------------------------------------
// �X�^�e�B�b�N�ϐ�
//-----------------------------------------
static Player s_player[NUM_PLAYER] = {};
static Player s_playerType[NUM_TYPE_PLAYER] = {};
static D3DXVECTOR3 moveCurve;
static bool s_bCurveInput;

//-----------------------------------------
// �v���g�^�C�v�錾
//-----------------------------------------
static void MovePlayer(int nIdxPlayer);
static void JumpPlayer(int nIdxPlayer);
static void ThrowPlayer(int nIdxPlayer);
static void CatchDiscPlayer(int nIdxPlayer);
static void LoadPlayer(void);
static bool CollisionCircle(D3DXVECTOR3 Pos1, float fRadius1, D3DXVECTOR3 Pos2, float fRadius2);	// �~���m�̓ǂݍ��ݏ���

//=========================================
// �v���C���[�̏���������
//=========================================
void InitPlayer(void)
{
	Player *pPlayer = s_player;

	ZeroMemory(s_player, sizeof(s_player));
	ZeroMemory(s_playerType, sizeof(s_playerType));

	LoadPlayer();	// �ǂݍ���

	SetPlayer(D3DXVECTOR3(200.0f, SCREEN_HEIGHT * 0.5f, 0.0f), PLAYERTYPE_1);
	SetPlayer(D3DXVECTOR3(SCREEN_WIDTH - 200.0f, SCREEN_HEIGHT * 0.5f, 0.0f), PLAYERTYPE_2);
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

		JumpPlayer(nIdxPlayer);			// ����
		MovePlayer(nIdxPlayer);			// �ړ�
		ThrowPlayer(nIdxPlayer);		// ������
		CatchDiscPlayer(nIdxPlayer);	// �󂯎��

		VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		pPlayer->pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(pPlayer->pos.x - pPlayer->fSize, pPlayer->pos.y - pPlayer->fSize, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pPlayer->pos.x + pPlayer->fSize, pPlayer->pos.y - pPlayer->fSize, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pPlayer->pos.x - pPlayer->fSize, pPlayer->pos.y + pPlayer->fSize, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pPlayer->pos.x + pPlayer->fSize, pPlayer->pos.y + pPlayer->fSize, 0.0f);

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
	Player *pPlayer = &s_player[nIdxPlayer];
	D3DXVECTOR3 inputMove(0.0f, 0.0f, 0.0f);	// ���͕���
	float moveLength = 0.0f;					// ���͂̒���

	if (IsJoyPadUse(nIdxPlayer))
	{ // JoyPad����
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
	{ // �L�[�{�[�h����
		if (GetKeyboardPress(DIK_W))
		{
			inputMove.y -= 1.0f;
			moveLength = 1.0f;
		}
		if (GetKeyboardPress(DIK_A))
		{
			inputMove.x -= 1.0f;
			moveLength = 1.0f;
		}
		if (GetKeyboardPress(DIK_S))
		{
			inputMove.y += 1.0f;
			moveLength = 1.0f;
		}
		if (GetKeyboardPress(DIK_D))
		{
			inputMove.x += 1.0f;
			moveLength = 1.0f;
		}
	}

	if (moveLength > 0.0f)
	{
		D3DXVec3Normalize(&inputMove, &inputMove);
	}

	pPlayer->move = inputMove * 0.9f * pPlayer->fSpeed;
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

	if (!pPlayer->bHaveDisk)
	{
		return;
	}

	D3DXVECTOR3 inputVec(0.0f, 0.0f, 0.0f);	// ���͕���
	float fVecLength = 0.0f;				// ���͂̒���

	if (IsJoyPadUse(nIdxPlayer))
	{ // JoyPad����
		inputVec = GetJoypadStick(JOYKEY_LEFT_STICK, nIdxPlayer);

		if (GetJoypadTrigger(JOYKEY_B, nIdxPlayer))
		{
			SetDisk(pPlayer->pos, inputVec * 1.25f, moveCurve, DISK_TYPE_NORMAL, 40.0f);
			pPlayer->bHaveDisk = false;
		}
		if (GetJoypadTrigger(JOYKEY_A, nIdxPlayer))
		{
			SetDisk(pPlayer->pos, inputVec * 1.25f, moveCurve, DISK_TYPE_LOB, 40.0f);
			pPlayer->bHaveDisk = false;
		}

		fVecLength = D3DXVec3Length(&inputVec);

		if (fVecLength >= 1.0f)
		{
			if (!s_bCurveInput)
			{
				moveCurve = inputVec * 0.01f;
			}
			s_bCurveInput = true;
		}
		else
		{
			moveCurve = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			s_bCurveInput = false;
		}
	}
	else
	{ // �L�[�{�[�h����
		if (GetKeyboardPress(DIK_W))
		{
			inputVec.y -= 1.0f;
		}
		if (GetKeyboardPress(DIK_A))
		{
			inputVec.x -= 1.0f;
		}
		if (GetKeyboardPress(DIK_S))
		{
			inputVec.y += 1.0f;
		}
		if (GetKeyboardPress(DIK_D))
		{
			inputVec.x += 1.0f;
		}

		if (GetKeyboardPress(DIK_RETURN))
		{
			SetDisk(pPlayer->pos, inputVec, D3DXVECTOR3(0.0f, 0.0f, 0.0f), DISK_TYPE_NORMAL, 40.0f);
			pPlayer->bHaveDisk = false;
		}
		if (GetKeyboardPress(DIK_SPACE))
		{
			SetDisk(pPlayer->pos, inputVec, D3DXVECTOR3(0.0f, 0.0f, 0.0f), DISK_TYPE_LOB, 40.0f);
			pPlayer->bHaveDisk = false;
		}
	}
}

//=========================================
// �f�B�X�N���󂯎�鏈��
//=========================================
void CatchDiscPlayer(int nIdxPlayer)
{
	Player *pPlayer = &s_player[nIdxPlayer];
	Disk* pDisk = GetDisk();

	if (CollisionCircle(pPlayer->pos, pPlayer->fSize, pDisk->pos, pDisk->fSize))
	{
		pDisk->bUse = false;
		pPlayer->bHaveDisk = true;
	}
}

//=========================================
// �ݒ�
//=========================================
void SetPlayer(const D3DXVECTOR3& pos, PLAYERTYPE type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���g

	Player *pPlayer = s_player;

	for (int i = 0; i < NUM_PLAYER; i++, pPlayer++)
	{
		if (pPlayer->bUse)
		{
			continue;
		}

		// �f�[�^�̏�����
		ZeroMemory(&s_player[i],sizeof(s_player[0]));

		s_player[i] = s_playerType[type];

		pPlayer->pos = pos; 			// �ʒu��������
		pPlayer->fSize = PLAYER_SIZ;	// �v���C���[�̑傫��
		pPlayer->bUse = true;			// �v���C���[�̕\���̗L��

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
		pVtx[0].pos = D3DXVECTOR3(pPlayer->pos.x - pPlayer->fSize, pPlayer->pos.y - pPlayer->fSize, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pPlayer->pos.x + pPlayer->fSize, pPlayer->pos.y - pPlayer->fSize, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pPlayer->pos.x - pPlayer->fSize, pPlayer->pos.y + pPlayer->fSize, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pPlayer->pos.x + pPlayer->fSize, pPlayer->pos.y + pPlayer->fSize, 0.0f);

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

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

		break;
	}

}

//=========================================
// �v���C���[�̎擾����
//=========================================
Player* GetPlayer(void)
{
	return s_player;
}

//=========================================
// �v���C���[�̓ǂݍ��ݏ���
//=========================================
void LoadPlayer(void)
{
	char s_aString[256];	// �ǂݍ��ݗp������
	int NumType = 0;		// �ǂݍ��񂾃^�C�v��

	//�t�@�C�����J��
	FILE* pFile = fopen(PLAYER_FILE, "r");

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ
		fscanf(pFile, "%s", &s_aString);

		while (strncmp(&s_aString[0], "SCRIPT", 6) != 0)
		{//�X�^�[�g����܂ŋ󔒓ǂݍ���
			s_aString[0] = {};
			fscanf(pFile, "%s", &s_aString[0]);
		}

		while (strncmp(&s_aString[0], "END_SCRIPT", 10) != 0)
		{// ������̏������Ɠǂݍ���// ������̏������Ɠǂݍ���

			fscanf(pFile, "%s", &s_aString[0]);

			if (strncmp(&s_aString[0], "#", 1) == 0)
			{//����̂��ƃR�����g
				fgets(&s_aString[0], sizeof(s_aString), pFile);
				continue;
			}

			if (strcmp(&s_aString[0], "TYPESTATE") == 0)
			{// �����񂪈�v�����ꍇ
				while (strncmp(&s_aString[0], "ENDSTATE", 8) != 0)
				{
					fscanf(pFile, "%s", &s_aString[0]);// ���ǂݍ��ނ��

					if (strcmp(&s_aString[0], "TEXTURE") == 0)
					{
						fscanf(pFile, "%s", &s_aString[0]);	// ���ǂݍ��ނ��
						fscanf(pFile, "%s", &s_aString[0]);
						//�e�N�X�`���̓ǂݍ���
						D3DXCreateTextureFromFile(GetDevice(), &s_aString[0], &s_playerType[NumType].pTexture);
					}
					if (strcmp(&s_aString[0], "MOVESPEED") == 0)
					{
						fscanf(pFile, "%s", &s_aString[0]);	// ���ǂݍ��ނ��
						fscanf(pFile, "%f", &s_playerType[NumType].fSpeed);
					}
				}
				NumType++;
			}
		}
	}
	//�t�@�C�������
	fclose(pFile);
}


//-----------------------------------------------------------------------------
//�~�̓����蔻��
//-----------------------------------------------------------------------------
static bool CollisionCircle(D3DXVECTOR3 Pos1, float fRadius1, D3DXVECTOR3 Pos2, float fRadius2)
{
	//�Q���̕��̂̔��a���m�̘a
	float fDiff = fRadius1 + fRadius2;

	//X�̍���
	float fCalculationX = Pos1.x - Pos2.x;
	//Y�̍���
	float fCalculationY = Pos1.y - Pos2.y;

	//���݂̂Q�_�̋���
	float fLength = sqrtf(fCalculationX * fCalculationX + fCalculationY * fCalculationY);

	return fDiff >= fLength;
}