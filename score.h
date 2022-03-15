//------------------------------
//�X�R�A�ݒ�̃w�b�_�[
//ACG000
//Author: goto yuuki
//
//------------------------------
#ifndef _SCORE_H_
#define _SCORE_H_

//�}�N����`
#define NUM_SCORE		(4)								//����
#define MAX_CNT_SCORE	(2)								//�ő�J�E���^�[��

//�X�R�A�̍\����
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;			//���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3 pos;				//���݂̈ʒu
	D3DXVECTOR3 aPosTexU[MAX_CNT_SCORE];
	int nScore;											//�X�R�A�̒l
	int nCnt;										//�X�R�A�̃J�E���^�[
}SCORE;

//�v���g�^�C�v�錾
void InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);
void SetScore(int nScore);
void AddScore(int nValse);
SCORE GetScore(void);

#endif
