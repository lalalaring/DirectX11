#ifndef	__SAFE_DELETE_H__ONE__
#define	__SAFE_DELETE_H__ONE__

#include<Windows.h>

// Instが、Tを継承しているかを判定する
// (Javaのinstanceofと同機能)
template<typename T, typename Inst>
inline bool InstanceOf( const Inst& inst)
{
	return dynamic_cast<const T*>(inst) != 0;
}

#define	instanceof( super, inst)	InstanceOf<super>(inst)


// newされたものをdeleteする
template<typename T>
inline void	SafeDelete(T& ptr)
{
	if(ptr != NULL)
	{
		delete ptr;
		ptr = NULL;
	}
}

// newした配列をdeleteする
template<typename T>
inline void	SafeDeleteArray(T& ptr)
{
	if(ptr != NULL)
	{
		delete[] ptr;
		ptr = NULL;
	}
}

// mallocされたものをfreeする
template<typename T>
inline void  SafeFree(T& ptr)
{
	if(ptr != NULL)
	{
		free(ptr);
		ptr = NULL;
	}
}

// DirectXのCOMインスタンスをReleaseする
template<typename T>
inline void SafeRelease(T& ptr)
{
	if( instanceof( IUnknown, ptr) )
	{
		if(ptr != NULL)
		{
			ptr->Release();
			ptr = NULL;
		}
	}
}


// HANDLEをcloseする
//template<typename T>
//inline BOOL	SafeCloseHandle(T& handle)
//{
//	BOOL ok = false;
//
//	if( handle != 0 )
//	{
//		ok = CloseHandle(handle);
//		if( ok )
//			handle = 0;
//		else
//			return false;
//	}
//
//	return ok;
//}

#endif