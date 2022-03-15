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
#include "shadow.h"
#include "stage.h"

//-----------------------------------------
// �}�N����`
//-----------------------------------------
#define NUM_PLAYER		(2)								// �v���C���[�̐�
#define PLAYER_FILE		"data/player.txt"				// �v���C���[�ǂݍ��݃t�@�C��
#define PLAYER_TEX		"data/TEXTURE/player00.png"		// �v���C���[�̃e�N�X�`��
#define PLAYER_SIZ		(45.0f)							// �v���C���[�̑傫��
#define ZERO_VECTOR		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))	// �[���x�N�g��

//-----------------------------------------
// �X�^�e�B�b�N�ϐ�
//-----------------------------------------
static Player s_player[NUM_PLAYER] = {};
static Player s_playerType[PLAYERTYPE_MAX] = {};
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
static D3DXVECTOR3 InputMovePlayer(int nIdxPlayer);

//=========================================
// �v���C���[�̏���������
//=========================================
void InitPlayer(void)
{
	//������
	ZeroMemory(s_player, sizeof(s_player));
	ZeroMemory(s_playerType, sizeof(s_playerType));

	moveCurve = ZERO_VECTOR;
	LoadPlayer();	// �ǂݍ���

	// �v���C���[�̐ݒ�
	SetPlayer(D3DXVECTOR3(200.0f, SCREEN_HEIGHT * 0.5f, 0.0f), PLAYERTYPE_1);
	s_player[0].nIdxShadow = SetShadow(s_player[0].pos, PLAYER_SIZ * 3.0f);

	SetPlayer(D3DXVECTOR3(SCREEN_WIDTH - 200.0f, SCREEN_HEIGHT * 0.5f, 0.0f), PLAYERTYPE_2);
	s_player[1].nIdxShadow = SetShadow(s_player[1].pos, PLAYER_SIZ * 3.0f);
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
		SetPositionShadow(pPlayer->nIdxShadow, pPlayer->pos);

		if (pPlayer->bHaveDisk)
		{ //�f�B�X�N���������Ă�ꍇ
			pPlayer->move = ZERO_VECTOR;
			// ������
			ThrowPlayer(nIdxPlayer);

			if (pPlayer->jumpstate == JUMP_NOW && pPlayer->fVerticalSpeed > 0.0f)
			{
				pPlayer->fHeight += pPlayer->fVerticalSpeed;
				pPlayer->fVerticalSpeed -= 0.15f;
			}
		}
		else
		{
			// �󂯎��
			CatchDiscPlayer(nIdxPlayer);

			switch (pPlayer->jumpstate)
			{
			case JUMP_NONE:
				// �ړ�
				MovePlayer(nIdxPlayer);
				// ����
				JumpPlayer(nIdxPlayer);
				break;
			case JUMP_NOW:
				pPlayer->fHeight += pPlayer->fVerticalSpeed;
				pPlayer->fVerticalSpeed -= 0.15f;

				if (pPlayer->fHeight <= 0.0f)
				{
					pPlayer->fVerticalSpeed = 5.0f;
					pPlayer->fHeight = 0.0f;
					pPlayer->jumpstate = JUMP_NONE;
				}

				break;
			default:
				assert(false);
				break;
			}
		}

		// �ړ�����
		{
			STAGE_LENGTH stageLength;
			if (nIdxPlayer == 0)
			{
				stageLength = *GetP1StgLng();
			}
			else
			{
				stageLength = *GetP2StgLng();
			}

			if (stageLength.max.y <= pPlayer->pos.y + PLAYER_SIZ)
			{
				pPlayer->pos.y = stageLength.max.y - PLAYER_SIZ;
			}
			if (stageLength.max.x <= pPlayer->pos.x + PLAYER_SIZ)
			{
				pPlayer->pos.x = stageLength.max.x - PLAYER_SIZ;
			}
			if (stageLength.min.y >= pPlayer->pos.y - PLAYER_SIZ)
			{
				pPlayer->pos.y = stageLength.min.y + PLAYER_SIZ;
			}
			if (stageLength.min.x >= pPlayer->pos.x - PLAYER_SIZ)
			{
				pPlayer->pos.x = stageLength.min.x + PLAYER_SIZ;
			}
		}

		VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		pPlayer->pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(pPlayer->pos.x - (pPlayer->fSize * (1 + (pPlayer->fHeight * 0.015f))),
			pPlayer->pos.y - ((pPlayer->fHeight - 10.0f) * 0.5f) - (pPlayer->fSize * (1 + (pPlayer->fHeight * 0.015f))), 0.0f);

		pVtx[1].pos = D3DXVECTOR3(pPlayer->pos.x + (pPlayer->fSize * (1 + (pPlayer->fHeight * 0.015f))),
			pPlayer->pos.y - ((pPlayer->fHeight - 10.0f) * 0.5f) - (pPlayer->fSize * (1 + (pPlayer->fHeight * 0.015f))), 0.0f);

		pVtx[2].pos = D3DXVECTOR3(pPlayer->pos.x - (pPlayer->fSize * (1 + (pPlayer->fHeight * 0.015f))),
			pPlayer->pos.y - ((pPlayer->fHeight - 10.0f) * 0.5f) + (pPlayer->fSize * (1 + (pPlayer->fHeight * 0.015f))), 0.0f);

		pVtx[ 3].pos = D3DXVECTOR3(pPlayer->pos.x + (pPlayer->fSize * (1 + (pPlayer->fHeight * 0.015f))),
			pPlayer->pos.y - ((pPlayer->fHeight - 10.0f) * 0.5f) + (pPlayer->fSize * (1 + (pPlayer->fHeight * 0.015f))), 0.0f);

		// ���_�o�b�t�@���A�����b�N����
		pPlayer->pVtxBuff->Unlock();
	}
}

//=========================================
// �v���C���[�̕`�揈��
//=========================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();;	// �f�o�C�X�ւ̃|�C���g
	Player *pPlayer = GetPlayer();

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

	// �������͏���
	D3DXVECTOR3 inputMove = InputMovePlayer(nIdxPlayer);

	if (IsJoyPadUse(nIdxPlayer))
	{ // JoyPad�̗L��
		if (GetJoypadTrigger(JOYKEY_A))
		{ //�X���C�f�B���O�̎g�p
			pPlayer->bUseSliding = true;
		}
	}
	else
	{
		if (GetKeyboardTrigger(DIK_B))
		{ //�X���C�f�B���O�̎g�p
			pPlayer->bUseSliding = true;
		}
	}

	if (pPlayer->bUseSliding)
	{ // �X���C�f�B���O
		if (pPlayer->nSlidingRigorCnt == 0)
		{
			D3DXVec3Normalize(&inputMove, &inputMove);	// �����̐��K��
			pPlayer->move = inputMove * pPlayer->fSlidingVolume;
			pPlayer->nSlidingRigorCnt++;
		}
		else if (pPlayer->nSlidingRigorCnt >= pPlayer->nSlidingRigorMax)
		{
			pPlayer->nSlidingRigorCnt = 0;
			pPlayer->bUseSliding = false;
		}
		else
		{
			pPlayer->move -= pPlayer->move * pPlayer->fAttenuationSlidingSpead;
			pPlayer->nSlidingRigorCnt++;
		}
	}
	else
	{
		if (D3DXVec3Length(&inputMove) > 0.0f)
		{
			pPlayer->move = inputMove * pPlayer->fMoveSpeed;
			D3DXVec3Normalize(&inputMove, &inputMove);	// �����̐��K��
		}

		// ��������
		pPlayer->move -= pPlayer->move * pPlayer->fAttenuationMoveSpead;
	}
}

//=========================================
// �v���C���[�̃W�����v����
//=========================================
void JumpPlayer(int nIdxPlayer)
{
	Player *pPlayer = &s_player[nIdxPlayer];

	if ((IsJoyPadUse(nIdxPlayer) && GetJoypadTrigger(JOYKEY_Y, nIdxPlayer))
		|| !IsJoyPadUse(nIdxPlayer) && GetKeyboardTrigger(DIK_J))
	{
		pPlayer->move = ZERO_VECTOR;
		pPlayer->jumpstate = JUMP_NOW;
	}
}

//=========================================
// �v���C���[�̓����鏈��
//=========================================
void ThrowPlayer(int nIdxPlayer)
{
	Player *pPlayer = &s_player[nIdxPlayer];
	D3DXVECTOR3 inputVec;

	if (nIdxPlayer == 0)
	{
		inputVec = D3DXVECTOR3(2.0f, 0.0f, 0.0f);	// ���͕���
	}
	else
	{
		inputVec = D3DXVECTOR3(-2.0f, 0.0f, 0.0f);	// ���͕���
	}

	// �������͏���
	inputVec += InputMovePlayer(nIdxPlayer);

	if (IsJoyPadUse(nIdxPlayer))
	{ // JoyPad����

		float fVecLength = D3DXVec3Length(&inputVec);

		if (fVecLength >= 3.0f)
		{
			if (!s_bCurveInput)
			{
				// �ǂꂭ�炢�Ȃ��邩�̓���
				moveCurve.y = -inputVec.y * pPlayer->fThrowCurvePower;
			}
			s_bCurveInput = true;
		}
		else
		{
			moveCurve = ZERO_VECTOR;
			s_bCurveInput = false;
		}

		if (GetJoypadTrigger(JOYKEY_B, nIdxPlayer))
		{
			SetDisk(pPlayer->pos, inputVec * pPlayer->fThrowPower, moveCurve, DISK_TYPE_NORMAL, nIdxPlayer, 40.0f);
			pPlayer->bHaveDisk = false;
		}
		else if (GetJoypadTrigger(JOYKEY_A, nIdxPlayer))
		{
			SetDisk(pPlayer->pos, inputVec * pPlayer->fThrowPower, ZERO_VECTOR, DISK_TYPE_LOB, nIdxPlayer, 40.0f);
			pPlayer->bHaveDisk = false;
		}
		else if (GetJoypadTrigger(JOYKEY_X, nIdxPlayer))
		{
			SetDisk(pPlayer->pos, inputVec * pPlayer->fThrowPower, ZERO_VECTOR, DISK_TYPE_SPECIAL_0, nIdxPlayer, 40.0f);
			pPlayer->bHaveDisk = false;
		}

	}
	else
	{ // �L�[�{�[�h����
		if (GetKeyboardPress(DIK_RETURN))
		{
			SetDisk(pPlayer->pos, inputVec * pPlayer->fThrowPower, moveCurve, DISK_TYPE_NORMAL, nIdxPlayer, 40.0f);
			pPlayer->bHaveDisk = false;
		}
		else if (GetKeyboardPress(DIK_SPACE))
		{
			SetDisk(pPlayer->pos, inputVec * pPlayer->fThrowPower, ZERO_VECTOR, DISK_TYPE_LOB, nIdxPlayer, 40.0f);
			pPlayer->bHaveDisk = false;
		}
		else if (GetKeyboardPress(DIK_M))
		{
			SetDisk(pPlayer->pos, inputVec * pPlayer->fThrowPower, ZERO_VECTOR, DISK_TYPE_SPECIAL_0, nIdxPlayer, 40.0f);
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
	Shadow *pShadow = GetShadow();

	if ((CollisionCircle(pPlayer->pos, pPlayer->fSize * (1 + (pPlayer->fHeight * 0.005)), pDisk->pos, pDisk->fSize)) && (pDisk->nPlayer != nIdxPlayer))
	{
		if ((pDisk->type != DISK_TYPE_LOB || (pDisk->type == DISK_TYPE_LOB && pDisk->fHeight <= 0.0f)) && pPlayer->jumpstate == JUMP_NONE)
		{
			DestroyDisk();	// �f�B�X�N�̔j��
			pPlayer->bHaveDisk = true;
		}

		if (pDisk->type == DISK_TYPE_LOB && pPlayer->jumpstate == JUMP_NOW)
		{
			DestroyDisk();	// �f�B�X�N�̔j��
			pPlayer->bHaveDisk = true;
		}
	}
}

//=========================================
// �ݒ�
//=========================================
void SetPlayer(const D3DXVECTOR3& pos, PLAYERTYPE type)
{
	Player *pPlayer = s_player;

	for (int i = 0; i < NUM_PLAYER; i++, pPlayer++)
	{
		if (pPlayer->bUse)
		{
			continue;
		}

		// �f�[�^�̏�����
		ZeroMemory(&s_player[i],sizeof(s_player[0]));

		// ��ޕʂ̃f�[�^�̑��
		s_player[i] = s_playerType[type];

		pPlayer->pos = pos; 			// �ʒu��������
		pPlayer->fSize = PLAYER_SIZ;	// �v���C���[�̑傫��
		pPlayer->bUse = true;			// �v���C���[�̕\���̗L��

		// ���_�o�b�t�@�̐���
		GetDevice()->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
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
// �v���C���[�̓ǂݍ��ݏ���
//=========================================
void LoadPlayer(void)
{
	char s_aString[256];	// �ǂݍ��ݗp������
	int nNumType = 0;		// �ǂݍ��񂾃^�C�v��

	//�t�@�C�����J��
	FILE* pFile = fopen(PLAYER_FILE, "r");

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ
		fscanf(pFile, "%s", s_aString);

		while (strncmp(s_aString, "SCRIPT", 6) != 0)
		{//�X�^�[�g����܂ŋ󔒓ǂݍ���
			s_aString[0] = {};
			fscanf(pFile, "%s", s_aString);
		}

		while (strncmp(s_aString, "END_SCRIPT", 10) != 0)
		{// ������̏������Ɠǂݍ���

			fscanf(pFile, "%s", s_aString);

			if (strncmp(s_aString, "#", 1) == 0)
			{//����̂��ƃR�����g
				fgets(s_aString, sizeof(s_aString), pFile);
				continue;
			}

			if (strcmp(s_aString, "TYPESTATE") == 0)
			{// �����񂪈�v�����ꍇ

				assert(PLAYERTYPE_MAX != nNumType);	// �z���葽���t�@�C���ǂݍ���

				while (strncmp(s_aString, "ENDSTATE", 8) != 0)
				{
					fscanf(pFile, "%s", s_aString);	// =�̓ǂݍ���

					if (strcmp(s_aString, "TEXTURE") == 0)
					{
						fscanf(pFile, "%s", s_aString);	// =�̓ǂݍ���
						fscanf(pFile, "%s", s_aString);

						//�e�N�X�`���̓ǂݍ���
						D3DXCreateTextureFromFile(GetDevice(), s_aString, &s_playerType[nNumType].pTexture);
					}
					else if (strcmp(s_aString, "MOVESPEED") == 0)
					{
						fscanf(pFile, "%s", s_aString);	// =�̓ǂݍ���
						fscanf(pFile, "%f", &s_playerType[nNumType].fMoveSpeed);
					}
					else if (strcmp(s_aString, "MOVE_ATTENUATION") == 0)
					{
						fscanf(pFile, "%s", s_aString);	// =�̓ǂݍ���
						fscanf(pFile, "%f", &s_playerType[nNumType].fAttenuationMoveSpead);
					}
					else if (strcmp(s_aString, "SLIDING_ATTENUATION") == 0)
					{
						fscanf(pFile, "%s", s_aString);	// =�̓ǂݍ���
						fscanf(pFile, "%f", &s_playerType[nNumType].fAttenuationSlidingSpead);
					}
					else if (strcmp(s_aString, "THROW_POWER") == 0)
					{
						fscanf(pFile, "%s", s_aString);	// =�̓ǂݍ���
						fscanf(pFile, "%f", &s_playerType[nNumType].fThrowPower);
					}
					else if (strcmp(s_aString, "THROW_CURVE") == 0)
					{
						fscanf(pFile, "%s", s_aString);	// =�̓ǂݍ���
						fscanf(pFile, "%f", &s_playerType[nNumType].fThrowCurvePower);
					}
					else if (strcmp(s_aString, "SLIDING_VOLUME") == 0)
					{
						fscanf(pFile, "%s", s_aString);	// =�̓ǂݍ���
						fscanf(pFile, "%f", &s_playerType[nNumType].fSlidingVolume);
					}
					else if (strcmp(s_aString, "SLIDING_RIGOR") == 0)
					{
						fscanf(pFile, "%s", s_aString);	// =�̓ǂݍ���
						fscanf(pFile, "%d", &s_playerType[nNumType].nSlidingRigorMax);
					}
				}

				nNumType++;
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
	float fDiff = fRadius1 + fRadius2;		//2�̕��̂̔��a���m�̘a
	float fCalculationX = Pos1.x - Pos2.x;	//X�̍���
	float fCalculationY = Pos1.y - Pos2.y;	//Y�̍���

	//���݂̂Q�_�̋���
	float fLength = sqrtf(fCalculationX * fCalculationX + fCalculationY * fCalculationY);

	return fDiff >= fLength;
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
D3DXVECTOR3 InputMovePlayer(int nIdxPlayer)
{
	D3DXVECTOR3 inputVec = ZERO_VECTOR;

	if (IsJoyPadUse(nIdxPlayer))
	{ // JoyPad����
		inputVec = GetJoypadStick(JOYKEY_LEFT_STICK, nIdxPlayer);
	}
	else
	{ // �L�[�{�[�h����
		switch (nIdxPlayer)
		{
		case 0:
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
			break;
		case 1:
			if (GetKeyboardPress(DIK_UP))
			{
				inputVec.y -= 1.0f;
			}
			if (GetKeyboardPress(DIK_LEFT))
			{
				inputVec.x -= 1.0f;
			}
			if (GetKeyboardPress(DIK_DOWN))
			{
				inputVec.y += 1.0f;
			}
			if (GetKeyboardPress(DIK_RIGHT))
			{
				inputVec.x += 1.0f;
			}
			break;
		default:
			break;
		}

		D3DXVec3Normalize(&inputVec, &inputVec);

	}

	return inputVec;
}

//=========================================
// �v���C���[�̎擾����
//=========================================
Player* GetPlayer(void)
{
	return s_player;
}