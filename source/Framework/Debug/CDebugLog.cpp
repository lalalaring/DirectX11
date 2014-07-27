/**=============================
* @file			CDebugLog.cpp
* @brief		デバッグウィンドウ(コンソール)のクラス実装部
* @attention	ONE_DEBUGがdefineされていないと何も起きないようにしています
* @warning		
* @note			モノステートで実装しています
* @author		N.Kaji
* @date			2014/03/07
==============================*/

#include "CDebugLog.h"
#include "Framework\Utility\SafeDelete.h"

HANDLE	CDebugLog::m_hConsole = 0;

CDebugLog::CDebugLog(void)
{
}


CDebugLog::~CDebugLog(void)
{
	this->Destroy();
}

/**-----------------------------
* @brief	コンソールの作成
* @note
* @author	N.Kaji
* @date		2014/03/07
------------------------------*/
bool	CDebugLog::Create()
{
#ifdef ONE_DEBUG

	// Create.
	if( AllocConsole() == 0 )
		return false;

	HMENU hMenu;

	// Get handle.
	m_hConsole = GetStdHandle( STD_OUTPUT_HANDLE);
	if( m_hConsole == INVALID_HANDLE_VALUE )
	{
		m_hConsole = 0;
		return false;
	}

	// Disable close button.
	hMenu = GetSystemMenu(GetConsoleWindow(), FALSE);
	RemoveMenu(hMenu, SC_CLOSE, MF_BYCOMMAND);

	return true;
#else
	return false;
#endif // ONE_DEBUG
}

/**-----------------------------
* @brief	終了処理
* @note
* @author	N.Kaji
* @date		2014/03/07
------------------------------*/
bool	CDebugLog::Destroy()
{
	FreeConsole();
	return true;
}

UINT	CDebugLog::Output( LPCWSTR str)
{
#ifdef ONE_DEBUG

	std::wstring dispStr( str);
	DWORD	strLen;

	SetConsoleTextAttribute( m_hConsole, DRESULT_SUCCESSED_COLOR);

	dispStr += L"\n";

	WriteConsole( m_hConsole, (void*)dispStr.c_str(), dispStr.size(), &strLen, nullptr);

	return (UINT)strLen;

#else
	return 0;

#endif // ONE_DEBUG
}

/**-----------------------------
* @brief	結果出力
* @note	
* @return	文字数
* @author	N.Kaji
* @date		2014/03/07
------------------------------*/
UINT	CDebugLog::Output( RESULT_TYPE type, bool isSuccess, LPCWSTR str)
{
#ifdef ONE_DEBUG

	DWORD	strLen;
	BOOL	ok;
	std::wstring	dispStr;

	strLen = CreateStr( &dispStr, type, isSuccess, str);

	// 出力
	ok = WriteConsole( m_hConsole, (void*)dispStr.c_str(), dispStr.size(), &strLen, nullptr);

	return (UINT)strLen;
#else
	return 0;

#endif // ONE_DEBUG
}

/**-----------------------------
* @brief	HRESULTとRESULT_TYPEを見て、コンソールに出力
* @note	
* @author	N.Kaji
* @date		2014/03/07
------------------------------*/
bool	CDebugLog::CheckHrAndOutput( RESULT_TYPE type, HRESULT hr, LPCWSTR str)
{
#ifdef ONE_DEBUG

	DWORD		strLen;
	std::wstring	dispStr;

	if( SUCCEEDED(hr) )
	{
		strLen = CreateStr( &dispStr, type, true, str);
		WriteConsole( m_hConsole, (void*)dispStr.c_str(), dispStr.size(), &strLen, nullptr);
		return true;
	}
	else
	{
		strLen = CreateStr( &dispStr, type, false, str);
		WriteConsole( m_hConsole, (void*)dispStr.c_str(), dispStr.size(), &strLen, nullptr);
		return false;
	}

#else
	if( SUCCEEDED(hr))
		return true;
	else
		return false;

#endif // ONE_DEBUG
}


/**-----------------------------
* @brief	出力文字列の作成
* @note
* @author	N.Kaji
* @date		2014/03/07
------------------------------*/
UINT	CDebugLog::CreateStr( std::wstring* pOut, RESULT_TYPE type, bool isSuccess, LPCWSTR str)
{
	if( type == RESULT_TYPE_COMPLATE )
		CreateCompleteStr( pOut, str);

	else
	{
		// 成功した？
		if( isSuccess )
		{
			SetConsoleTextAttribute( m_hConsole, DRESULT_SUCCESSED_COLOR);
			*pOut = L"SUCCESSED ";
		}
		else
		{
			SetConsoleTextAttribute( m_hConsole, DRESULT_FAILED_COLOR);
			*pOut = L"FAILED ";
		}

		// どんな結果？(typeを文字列に変換できれば楽になるんだが・・・)
		switch( type)
		{
		case RESULT_TYPE_INITIALIZE:
			*pOut += L"Initialize";
			break;

		case RESULT_TYPE_FINALIZE:
			*pOut += L"Finalize";
			break;

		case RESULT_TYPE_CREATE:
			*pOut += L"Create";
			break;

		case RESULT_TYPE_MAP:
			*pOut += L"Map";
			break;

		case RESULT_TYPE_LOAD:
			*pOut += L"Load";
			break;

		case RESULT_TYPE_COMPILE:
			*pOut += L"Compile";
			break;

		case RESULT_TYPE_SET:
			*pOut += L"Set";
			break;
		}
		*pOut += L" ";

		// 何が
		*pOut += '\"';
		*pOut += str;
		*pOut += L"\"\n";
	}

	return pOut->size();
}

/**-----------------------------
* @brief	COMPLETE文字列の作成
* @note
* @author	N.Kaji
* @date		2014/03/07
------------------------------*/
UINT	CDebugLog::CreateCompleteStr( std::wstring* pOut, LPCWSTR str)
{
	SetConsoleTextAttribute( m_hConsole, DRESULT_COMPLATED_COLOR);

	*pOut = L"COMPLETED ";
	*pOut += str;
	*pOut += '\n';

	return 0;
}