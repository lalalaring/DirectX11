#ifndef	__SAFE_DELETE_H__ONE__
#define	__SAFE_DELETE_H__ONE__

#include<Windows.h>

// Inst���AT���p�����Ă��邩�𔻒肷��
// (Java��instanceof�Ɠ��@�\)
template<typename T, typename Inst>
inline bool InstanceOf( const Inst& inst)
{
	return dynamic_cast<const T*>(inst) != 0;
}

#define	instanceof( super, inst)	InstanceOf<super>(inst)


// new���ꂽ���̂�delete����
template<typename T>
inline void	SafeDelete(T& ptr)
{
	if(ptr != NULL)
	{
		delete ptr;
		ptr = NULL;
	}
}

// new�����z���delete����
template<typename T>
inline void	SafeDeleteArray(T& ptr)
{
	if(ptr != NULL)
	{
		delete[] ptr;
		ptr = NULL;
	}
}

// malloc���ꂽ���̂�free����
template<typename T>
inline void  SafeFree(T& ptr)
{
	if(ptr != NULL)
	{
		free(ptr);
		ptr = NULL;
	}
}

// DirectX��COM�C���X�^���X��Release����
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


// HANDLE��close����
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