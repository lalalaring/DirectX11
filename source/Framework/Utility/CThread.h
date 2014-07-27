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
	* @brief	スレッド作成
	* @note		インスタンスに付き1スレッドまでしか作成不可です
	* @author	N.Kaji
	* @date		2014/05/27
	* @param	threadFunc	スレッドにする関数
	* @param	arg			スレッドに渡す引数
	* @return	スレッドが作れたか否か　0:成功/それ以外:失敗
	------------------------------*/
	int	CreateThread( void *(__cdecl *threadFunc)(void*), void* arg);

	/**-----------------------------
	* @brief	スレッドのDetach
	* @note		pthread特有です。ググって
	* @author	N.Kaji
	* @date		2014/05/27
	* @return	Detachが成功したか否か　0:成功/それ以外:失敗
	------------------------------*/
	int DetachThread();

	/**-----------------------------
	* @brief	スレッドが終わるまでブロック
	* @note		タイムアウトなどはありません。ずっとブロックします。
	* @author	N.Kaji
	* @date		2014/05/27
	* @return	スレッドが終わったか否か　0:成功/それ以外:失敗
	------------------------------*/
	int	WaitThread();
};