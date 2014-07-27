/**=============================
* @file			CDebugLog.h
* @brief		デバッグウィンドウ(コンソール)のクラスヘッダ
				TCHARに対応しています
* @attention	ONE_DEBUGがdefineされていないと何も起きないようにしています
* @warning		
* @note			モノステートで実装しています
* @author		N.Kaji
* @date			2014/03/07
==============================*/

#ifndef __CDebugLog_H__ONE__
#define __CDebugLog_H__ONE__

#include<Windows.h>
#include<assert.h>
#include<string>
#include"DebugResult.h"
#include"Debug.h"

// 各Result毎に文字の前景色と背景色を用意する
#define	DRESULT_SUCCESSED_COLOR	(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)
#define	DRESULT_FAILED_COLOR		(FOREGROUND_RED | FOREGROUND_INTENSITY)
#define	DRESULT_COMPLATED_COLOR	(BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE)


class CDebugLog
{
private:
	static HANDLE	m_hConsole;	// コンソールのハンドル

	// エラー回避のために、LPCTSTRではなくconst _TCHAR*を使っています。
	static UINT	CreateStr( std::wstring* pOut, RESULT_TYPE type, bool isSuccess, LPCWSTR str);
	static UINT	CreateStr( std::wstring* pOut, RESULT_TYPE type, HRESULT hr, LPCWSTR str);
	static UINT	CreateCompleteStr( std::wstring* pOut, LPCWSTR str);

public:
	 CDebugLog(void);
	 ~CDebugLog(void);

	static bool	Create();
	static bool	Destroy();

	// 結果の種類、成功したのか否か、何が
	static UINT Output( LPCWSTR str);
	static UINT	Output( RESULT_TYPE type, bool isSuccess, LPCWSTR str);
	static bool	CheckHrAndOutput( RESULT_TYPE type, HRESULT hr, LPCWSTR str);
};


// HRESULTの値をチェックし、デバックコンソールに成否結果を表示する。(戻り値bool)
#define CheckHRESULT( RESULT_TYPE, hr, str) CDebugLog::CheckHrAndOutput( RESULT_TYPE, hr, _T(str))
#define	CheckCreate( hr, str) CDebugLog::CheckHrAndOutput( RESULT_TYPE_CREATE, hr, _T(str))
// bool判定
#define CheckBoolResult( RESULT_TYPE, bool, str)	CDebugLog::Output( RESULT_TYPE, bool, _T(str))

// 単純に文字列表示
#define OutputConsole( str)	CDebugLog::Output( _T(str))

#endif // !__CDebugLog_H__ONE__
