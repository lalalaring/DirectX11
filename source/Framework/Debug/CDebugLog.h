/**=============================
* @file			CDebugLog.h
* @brief		�f�o�b�O�E�B���h�E(�R���\�[��)�̃N���X�w�b�_
				TCHAR�ɑΉ����Ă��܂�
* @attention	ONE_DEBUG��define����Ă��Ȃ��Ɖ����N���Ȃ��悤�ɂ��Ă��܂�
* @warning		
* @note			���m�X�e�[�g�Ŏ������Ă��܂�
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

// �eResult���ɕ����̑O�i�F�Ɣw�i�F��p�ӂ���
#define	DRESULT_SUCCESSED_COLOR	(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)
#define	DRESULT_FAILED_COLOR		(FOREGROUND_RED | FOREGROUND_INTENSITY)
#define	DRESULT_COMPLATED_COLOR	(BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE)


class CDebugLog
{
private:
	static HANDLE	m_hConsole;	// �R���\�[���̃n���h��

	// �G���[����̂��߂ɁALPCTSTR�ł͂Ȃ�const _TCHAR*���g���Ă��܂��B
	static UINT	CreateStr( std::wstring* pOut, RESULT_TYPE type, bool isSuccess, LPCWSTR str);
	static UINT	CreateStr( std::wstring* pOut, RESULT_TYPE type, HRESULT hr, LPCWSTR str);
	static UINT	CreateCompleteStr( std::wstring* pOut, LPCWSTR str);

public:
	 CDebugLog(void);
	 ~CDebugLog(void);

	static bool	Create();
	static bool	Destroy();

	// ���ʂ̎�ށA���������̂��ۂ��A����
	static UINT Output( LPCWSTR str);
	static UINT	Output( RESULT_TYPE type, bool isSuccess, LPCWSTR str);
	static bool	CheckHrAndOutput( RESULT_TYPE type, HRESULT hr, LPCWSTR str);
};


// HRESULT�̒l���`�F�b�N���A�f�o�b�N�R���\�[���ɐ��ی��ʂ�\������B(�߂�lbool)
#define CheckHRESULT( RESULT_TYPE, hr, str) CDebugLog::CheckHrAndOutput( RESULT_TYPE, hr, _T(str))
#define	CheckCreate( hr, str) CDebugLog::CheckHrAndOutput( RESULT_TYPE_CREATE, hr, _T(str))
// bool����
#define CheckBoolResult( RESULT_TYPE, bool, str)	CDebugLog::Output( RESULT_TYPE, bool, _T(str))

// �P���ɕ�����\��
#define OutputConsole( str)	CDebugLog::Output( _T(str))

#endif // !__CDebugLog_H__ONE__
