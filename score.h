//------------------------------
//�X�R�A�ݒ�̃w�b�_�[
//ACG000
//Author: goto yuuki
//
//------------------------------
#ifndef _SCORE_H_
#define _SCORE_H_

//�}�N����`
#define NUM_SCORE		(2)								//����
#define MAX_CNT_SCORE	(2)								//�ő�J�E���^�[��

//�X�R�A�̍\����
typedef struct
{
	D3DXVECTOR3 posScore[MAX_CNT_SCORE];				//���݂̈ʒu
	int nScore;											//�X�R�A�̒l
	int nCnt;										//�X�R�A�̃J�E���^�[
}SCORE;

typedef struct
{
	D3DXVECTOR3 c_posScore[MAX_CNT_SCORE];				//���݂̈ʒu
	int cScore;											//�X�R�A�̒l
	int cCnt;										//�X�R�A�̃J�E���^�[
}SCORE2;

//�v���g�^�C�v�錾
void InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);
void SetScore(int nScore,int cScore);
void AddScore(int nValse,int cValse);
SCORE GetScore(void);

#endif
