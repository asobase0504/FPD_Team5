//------------------------------
//�X�R�A�ݒ�̃w�b�_�[
//ACG000
//Author: goto yuuki
//
//------------------------------
#ifndef _SCORE_H_
#define _SCORE_H_

//�X�R�A�̍\����
typedef struct
{
	D3DXVECTOR3 pos;		//���݂̈ʒu
	int nScore;				//�X�R�A�̒l
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
