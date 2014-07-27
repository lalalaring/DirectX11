#include "CThread.h"


CThread::CThread()
{
	m_thread.p = nullptr;
	m_thread.x = 0;
}


CThread::~CThread()
{
}

/**-----------------------------
* @brief	�X���b�h�쐬
* @note		�C���X�^���X�ɕt��1�X���b�h�܂ł����쐬�s�ł�
* @author	N.Kaji
* @date		2014/05/27
* @param	threadFunc	�X���b�h�ɂ���֐�
* @param	arg			�X���b�h�ɓn������
* @return	�X���b�h����ꂽ���ۂ��@0:����/����ȊO:���s
------------------------------*/
int	CThread::CreateThread( void *(__cdecl *threadFunc)(void*), void* arg)
{
	// ���łɃX���b�h������Ă���
	if( m_thread.p)
		return -1;

	return pthread_create( &m_thread, nullptr, threadFunc, arg);
}

/**-----------------------------
* @brief	�X���b�h��Detach
* @note		pthread���L�ł��B�O�O����
* @author	N.Kaji
* @date		2014/05/27
* @return	Detach�������������ۂ��@0:����/����ȊO:���s
------------------------------*/
int CThread::DetachThread()
{
	// �X���b�h������Ă��Ȃ�
	if( m_thread.p == nullptr )
		return -1;

	return pthread_detach( m_thread);
}

/**-----------------------------
* @brief	�X���b�h���I���܂Ńu���b�N
* @note		�^�C���A�E�g�Ȃǂ͂���܂���B�����ƃu���b�N���܂��B
* @author	N.Kaji
* @date		2014/05/27
* @return	�X���b�h���I��������ۂ��@0:����/����ȊO:���s
------------------------------*/
int	CThread::WaitThread()
{
	// �X���b�h������Ă��Ȃ�
	if( m_thread.p == nullptr )
		return -1;

	return pthread_join( m_thread, nullptr);
}