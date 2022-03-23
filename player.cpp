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
#include "landingPoint.h"
#include "effect.h"
#include "game.h"

//-----------------------------------------
// �}�N����`
//-----------------------------------------
#define NUM_PLAYER		(2)								// �v���C���[�̐�
#define PLAYER_FILE		"data/player.txt"				// �v���C���[�ǂݍ��݃t�@�C��
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
static void ThrowDisk(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 acc, DISK_TYPE type, int nIdxPlayer);
static void ChargePlayer(int nIdxPlayer);
static void CatchDiscPlayer(int nIdxPlayer);
static void MoveLimitPlayer(int nIdxPlayer);
static void UpdateVtxPosPlayer(int nIdxPlayer);
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

		// �e�̈ʒu�̒���
		SetPositionShadow(pPlayer->nIdxShadow, pPlayer->pos);

		if (!*GetResetScore())
		{ // ���Z�b�g���ł͂Ȃ�
			if (pPlayer->bHaveDisk)
			{ //�f�B�X�N���������Ă�ꍇ

				pPlayer->move -= pPlayer->move * pPlayer->fAttenuationSlidingSpead;	// �ړ��ʂ̌���

				// ������
				ThrowPlayer(nIdxPlayer);

				if (pPlayer->jumpstate == JUMP_NOW)
				{
					if (pPlayer->fVerticalSpeed > 0.0f)
					{
						pPlayer->fHeight += pPlayer->fVerticalSpeed;
						pPlayer->fVerticalSpeed -= 0.15f;
					}

					if (pPlayer->fThrowPower <= 4.0f)	// �����̏������͌㒲��
					{	// ��莞�Ԃ̌o�߂ŏ��ɖ߂�
						pPlayer->fHeight += pPlayer->fVerticalSpeed;
						pPlayer->fVerticalSpeed -= 0.15f;

						if (pPlayer->fHeight <= 0.0f)
						{
							pPlayer->fVerticalSpeed = 5.0f;
							pPlayer->fHeight = 0.0f;
							pPlayer->jumpstate = JUMP_NONE;
						}
					}
				}
			}
			else
			{
				switch (pPlayer->jumpstate)
				{
				case JUMP_NONE:
					// �ړ�
					MovePlayer(nIdxPlayer);
					// �`���[�W
					ChargePlayer(nIdxPlayer);
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
		}

		// �󂯎��
		CatchDiscPlayer(nIdxPlayer);

		// �ړ�����
		MoveLimitPlayer(nIdxPlayer);

		// ���_���W�̍X�V
		UpdateVtxPosPlayer(nIdxPlayer);
	}
}

//=========================================
// �v���C���[�̕`�揈��
//=========================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���g
	Player *pPlayer = GetPlayer();

	for (int i = 0; i < NUM_PLAYER; i++, pPlayer++)
	{
		if (!pPlayer->bUse)
		{
			continue;
		}

		// ���_�o�b�t�@���f�[�^�X�g���[���ݒ�
		pDevice->SetStreamSource(0, pPlayer->pVtxBuff, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, pPlayer->pTexture);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
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
			pPlayer->move -= pPlayer->move * pPlayer->fAttenuationSlidingSpead;	// �ړ��ʂ̌���
			pPlayer->nSlidingRigorCnt++;

			// �G�t�F�N�g�̐���
			for (int i = 0; i < 25; i++)
			{
				SetEffect(pPlayer->pos, 0.0f, EFFECT_TYPE_SLIDING_IMPACT_2);
			}
			for (int i = 0; i < 10; i++)
			{
				SetEffect(pPlayer->pos, 0.0f, EFFECT_TYPE_SLIDING_IMPACT_3);
			}
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

	// ����v���C���[�����Ƀx�N�g������������
	D3DXVECTOR3 inputVec = ZERO_VECTOR;

	// ���͏���
	inputVec += InputMovePlayer(nIdxPlayer);

	if (D3DXVec3Length(&inputVec) >= 0.9f)
	{
		if (!s_bCurveInput)
		{
			// �ǂꂭ�炢�Ȃ��邩�̓���
			moveCurve.y = -inputVec.y;
		}
		s_bCurveInput = true;
	}
	else
	{
		moveCurve = ZERO_VECTOR;
		s_bCurveInput = false;
	}

	inputVec.x += (nIdxPlayer == 0) ? 2.0f : -2.0f;

	D3DXVECTOR3 move = inputVec * pPlayer->fThrowPower;

	if (pPlayer->fThrowPower >= 2.5f)
	{
		// �З͂̌���
		pPlayer->fThrowPower -= pPlayer->fThrowPower * 0.005f;
	}
	else
	{
		// �����r�o
		ThrowDisk(pPlayer->pos, move, ZERO_VECTOR, DISK_TYPE_NORMAL, nIdxPlayer);
	}

	if (IsJoyPadUse(nIdxPlayer))
	{ // JoyPad����
		switch (pPlayer->jumpstate)
		{
		case JUMP_NONE:
			if (GetJoypadTrigger(JOYKEY_A, nIdxPlayer))
			{
				if (pPlayer->nSpecialSkillCnt <= 100)
				{
					ThrowDisk(pPlayer->pos, move, moveCurve, DISK_TYPE_NORMAL, nIdxPlayer);
				}
				else
				{
					ThrowDisk(pPlayer->pos, move, ZERO_VECTOR, DISK_TYPE_SPECIAL_0, nIdxPlayer);
				}
			}
			else if (GetJoypadTrigger(JOYKEY_B, nIdxPlayer))
			{
				ThrowDisk(pPlayer->pos, move, ZERO_VECTOR, DISK_TYPE_LOB, nIdxPlayer);
			}
			break;
		case JUMP_NOW:
			if (GetJoypadTrigger(JOYKEY_A, nIdxPlayer))
			{
				ThrowDisk(pPlayer->pos, move, moveCurve, DISK_TYPE_JUMP, nIdxPlayer);
			}
			break;
		case JUMP_MAX:
		default:
			assert(false);
			break;
		}
	}
	else
	{ // �L�[�{�[�h����
		switch (pPlayer->jumpstate)
		{
		case JUMP_NONE:
			if (GetKeyboardTrigger(DIK_RETURN))
			{
				if (pPlayer->nSpecialSkillCnt <= 100)
				{
					ThrowDisk(pPlayer->pos, move, ZERO_VECTOR, DISK_TYPE_NORMAL, nIdxPlayer);
				}
				else
				{
					ThrowDisk(pPlayer->pos, move, ZERO_VECTOR, DISK_TYPE_SPECIAL_4, nIdxPlayer);
				}
			}
			else if (GetKeyboardTrigger(DIK_SPACE))
			{
				ThrowDisk(pPlayer->pos, move, ZERO_VECTOR, DISK_TYPE_LOB, nIdxPlayer);
			}
			break;
		case JUMP_NOW:
			if (GetKeyboardTrigger(DIK_RETURN))
			{
				ThrowDisk(pPlayer->pos, move, moveCurve, DISK_TYPE_JUMP, nIdxPlayer);
			}
			break;
		case JUMP_MAX:
		default:
			assert(false);
			break;
		}
	}
}

//=========================================
// �v���C���[���f�B�X�N�𓊂���
//=========================================
void ThrowDisk(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 acc, DISK_TYPE type, int nIdxPlayer)
{
	Player *pPlayer = &s_player[nIdxPlayer];
	SetDisk(pos, move, acc, type, nIdxPlayer, 90.0f);
	pPlayer->bHaveDisk = false;
	s_bCurveInput = false;
}

//=========================================
// �K�E�Z���`���[�W����
//=========================================
void ChargePlayer(int nIdxPlayer)
{
	Player *pPlayer = &s_player[nIdxPlayer];
	LandingMark *pMark = GetLandingMark();

	if (CollisionCircle(pPlayer->pos, pPlayer->fSize, pMark->pos,pMark->fSize * 0.5f) && pPlayer->jumpstate == JUMP_NONE && pMark->bUse)
	{
		pPlayer->nSpecialSkillCnt++;
	}
	else
	{
		pPlayer->nSpecialSkillCnt = 0;
	}
}

//=========================================
// �f�B�X�N���󂯎�鏈��
//=========================================
void CatchDiscPlayer(int nIdxPlayer)
{
	Player *pPlayer = &s_player[nIdxPlayer];
	Disk* pDisk = GetDisk();

	// �f�B�X�N�ƃv���C���[�̓����蔻��
	if ((CollisionCircle(pPlayer->pos, pPlayer->fSize * (1 + (pPlayer->fHeight * 0.005f)), pDisk->pos, (pDisk->fSize * 0.5f))) && (pDisk->nPlayer != nIdxPlayer))
	{
		// ���Ŏ擾
		if ((pDisk->type != DISK_TYPE_LOB || (pDisk->type == DISK_TYPE_LOB && pDisk->fHeight <= 0.0f)) && pPlayer->jumpstate == JUMP_NONE)
		{
			pPlayer->move += D3DXVECTOR3(pDisk->move.x, -pDisk->move.y, pDisk->move.z);
			DestroyDisk();	// �f�B�X�N�̔j��
			pPlayer->bHaveDisk = true;
			pPlayer->fThrowPower = pPlayer->fMaxThrowPower;
		}

		// �󒆂Ŏ擾
		if (pDisk->type == DISK_TYPE_LOB && pPlayer->jumpstate == JUMP_NOW)
		{
			DestroyDisk();	// �f�B�X�N�̔j��
			pPlayer->bHaveDisk = true;
			pPlayer->fThrowPower = pPlayer->fMaxThrowPower;
		}
	}
}

//=========================================
// �ړ�����
//=========================================
void MoveLimitPlayer(int nIdxPlayer)
{
	Player* pPlayer = &s_player[nIdxPlayer];

	STAGE_LENGTH stageLength = nIdxPlayer == 0 ? *GetP1StgLng() : *GetP2StgLng();

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

//=========================================
// ���_���W�̍X�V
//=========================================
void UpdateVtxPosPlayer(int nIdxPlayer)
{
	Player* pPlayer = &s_player[nIdxPlayer];
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

	pVtx[3].pos = D3DXVECTOR3(pPlayer->pos.x + (pPlayer->fSize * (1 + (pPlayer->fHeight * 0.015f))),
		pPlayer->pos.y - ((pPlayer->fHeight - 10.0f) * 0.5f) + (pPlayer->fSize * (1 + (pPlayer->fHeight * 0.015f))), 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	pPlayer->pVtxBuff->Unlock();
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
		if (i == 0)
		{
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
		else
		{
			pVtx[0].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.0f, 1.0f);
		}

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
						fscanf(pFile, "%f", &s_playerType[nNumType].fMaxThrowPower);
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
					else if (strcmp(s_aString, "SPECIALSKILL_TYPE") == 0)
					{
						fscanf(pFile, "%s", s_aString);	// =�̓ǂݍ���
						fscanf(pFile, "%d", &s_playerType[nNumType].SpecialSkillType);
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
// �J�n�ʒu�ɖ߂�
//-----------------------------------------------------------------------------
bool ResetPosPlayer(void)
{
	D3DXVECTOR3 posDist;
	D3DXVECTOR3 vec;

	for (int nIdxPlayer = 0; nIdxPlayer < 2; nIdxPlayer++)
	{
		if (nIdxPlayer == 0)
		{
			posDist = D3DXVECTOR3(200.0f, SCREEN_HEIGHT * 0.5f, 0.0f);
		}
		else
		{
			posDist = D3DXVECTOR3(SCREEN_WIDTH - 200.0f, SCREEN_HEIGHT * 0.5f, 0.0f);
		}

		Player* pPlayer = &s_player[nIdxPlayer];

		posDist -= pPlayer->pos;

		D3DXVec3Normalize(&vec, &posDist);

		pPlayer->move = vec * 5.0f;

		if (D3DXVec3Length(&posDist) < D3DXVec3Length(&pPlayer->move))
		{
			pPlayer->move = posDist;
		}
	}

	return (D3DXVec3Length(&posDist) == D3DXVec3Length(&s_player[0].move)) && (D3DXVec3Length(&posDist) == D3DXVec3Length(&s_player[1].move));
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
// ����
//-----------------------------------------------------------------------------
D3DXVECTOR3 InputMovePlayer(int nIdxPlayer)
{
	D3DXVECTOR3 inputVec = ZERO_VECTOR;

	if (IsJoyPadUse(nIdxPlayer))
	{ // JoyPad����

		inputVec = GetJoypadStick(JOYKEY_LEFT_STICK, nIdxPlayer);

		if (GetJoypadPress(JOYKEY_UP))
		{
			inputVec.y -= 1.0f;
		}
		if (GetJoypadPress(JOYKEY_LEFT))
		{
			inputVec.x -= 1.0f;
		}
		if (GetJoypadPress(JOYKEY_DOWN))
		{
			inputVec.y += 1.0f;
		}
		if (GetJoypadPress(JOYKEY_RIGHT))
		{
			inputVec.x += 1.0f;
		}
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
	}

	D3DXVec3Normalize(&inputVec, &inputVec);
	return inputVec;
}

//=========================================
// �v���C���[�̎擾����
//=========================================
Player* GetPlayer(void)
{
	return s_player;
}