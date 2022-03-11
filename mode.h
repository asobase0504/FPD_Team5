//==================================================
// 
// FPG���� ( mode.h )
// Author  : katsuki mizuki
// 
//==================================================
#ifndef _MODE_H_		// ���̃}�N������`����ĂȂ�������
#define _MODE_H_		// 2�d�C���N���[�h�h�~�̃}�N����`

//--------------------------------------------------
// �񋓌^
//--------------------------------------------------
typedef enum
{
	MODE_NONE = 0,
	MODE_TITLE,	// �^�C�g�����
	MODE_GAME,		// �Q�[�����
	MODE_RESULT,	// ���U���g���
	MODE_MAX,
}MODE;

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
void UninitMode(void);
void UpdateMode(void);
void DrawMode(void);
void SetMode(void);
MODE GetMode(void);
void ChangeMode(MODE mode);

#endif // !_MODE_H_
