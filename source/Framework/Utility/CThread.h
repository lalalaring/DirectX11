#pragma once

#include"pthread.h"

#pragma comment( lib, "pthreadVC2.lib")

class CThread
{
private:
	pthread_t	m_thread;
	
public:
	CThread();
	~CThread();

	/**-----------------------------
	* @brief	�X���b�h�쐬
	* @note		�C���X�^���X�ɕt��1�X���b�h�܂ł����쐬�s�ł�
	* @author	N.Kaji
	* @date		2014/05/27
	* @param	threadFunc	�X���b�h�ɂ���֐�
	* @param	arg			�X���b�h�ɓn������
	* @return	�X���b�h����ꂽ���ۂ��@0:����/����ȊO:���s
	------------------------------*/
	int	CreateThread( void *(__cdecl *threadFunc)(void*), void* arg);

	/**-----------------------------
	* @brief	�X���b�h��Detach
	* @note		pthread���L�ł��B�O�O����
	* @author	N.Kaji
	* @date		2014/05/27
	* @return	Detach�������������ۂ��@0:����/����ȊO:���s
	------------------------------*/
	int DetachThread();

	/**-----------------------------
	* @brief	�X���b�h���I���܂Ńu���b�N
	* @note		�^�C���A�E�g�Ȃǂ͂���܂���B�����ƃu���b�N���܂��B
	* @author	N.Kaji
	* @date		2014/05/27
	* @return	�X���b�h���I��������ۂ��@0:����/����ȊO:���s
	------------------------------*/
	int	WaitThread();
};