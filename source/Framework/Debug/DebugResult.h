#ifndef	__DEBUG_RESULT_H__ONE__
#define	__DEBUG_RESULT_H__ONE__

// �ǂ�Ȍ��ʂɂ��Ă�
enum	RESULT_TYPE{
	RESULT_TYPE_INITIALIZE,	// ������
	RESULT_TYPE_FINALIZE,	// �I��
	RESULT_TYPE_CREATE,		// �쐬(�Ȃ�ł�)
	RESULT_TYPE_MAP,		// �}�b�s���O
	RESULT_TYPE_LOAD,		// �ǂݍ���
	RESULT_TYPE_UPDATE,		// �X�V(����Ȃ�����)
	RESULT_TYPE_COMPILE,	// �R���p�C��
	RESULT_TYPE_SET,		// �ݒ�
	RESULT_TYPE_DELETE,		// �폜
	RESULT_TYPE_COMPLATE	// ���̏����Q������������
};

//struct	_RESULT
//{
//	RESULT_TYPE	type;
//	bool		isSuccess;	// �������������s������
//}

#endif