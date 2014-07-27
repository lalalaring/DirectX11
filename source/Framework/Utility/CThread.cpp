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
* @brief	スレッド作成
* @note		インスタンスに付き1スレッドまでしか作成不可です
* @author	N.Kaji
* @date		2014/05/27
* @param	threadFunc	スレッドにする関数
* @param	arg			スレッドに渡す引数
* @return	スレッドが作れたか否か　0:成功/それ以外:失敗
------------------------------*/
int	CThread::CreateThread( void *(__cdecl *threadFunc)(void*), void* arg)
{
	// すでにスレッドが作られている
	if( m_thread.p)
		return -1;

	return pthread_create( &m_thread, nullptr, threadFunc, arg);
}

/**-----------------------------
* @brief	スレッドのDetach
* @note		pthread特有です。ググって
* @author	N.Kaji
* @date		2014/05/27
* @return	Detachが成功したか否か　0:成功/それ以外:失敗
------------------------------*/
int CThread::DetachThread()
{
	// スレッドが作られていない
	if( m_thread.p == nullptr )
		return -1;

	return pthread_detach( m_thread);
}

/**-----------------------------
* @brief	スレッドが終わるまでブロック
* @note		タイムアウトなどはありません。ずっとブロックします。
* @author	N.Kaji
* @date		2014/05/27
* @return	スレッドが終わったか否か　0:成功/それ以外:失敗
------------------------------*/
int	CThread::WaitThread()
{
	// スレッドが作られていない
	if( m_thread.p == nullptr )
		return -1;

	return pthread_join( m_thread, nullptr);
}