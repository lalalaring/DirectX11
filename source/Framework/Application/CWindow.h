#ifndef __C_WINDOW_H__ONE__
#define __C_WINDOW_H__ONE__

#include<Windows.h>
#include"WindowDefine.h"
#include"Framework/Debug/DebugResult.h"


class CWindow
{
private:
	HWND		m_hWnd;
	HINSTANCE	m_hInstance;
	MSG			m_msg;

	wchar_t		m_wndName[32];

	DISPLAY_MODE	m_dispMode;
	int				m_wndWidth;
	int				m_wndHeight;
	int				m_clientWidth;
	int				m_clientHeight;

	bool		m_isFullscreen;	// フルスクリーンフラグ

private:
	void	SetWndClass( WNDCLASSEX* pOut, HINSTANCE hInstance);
	static LRESULT CALLBACK	WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void	SetClientSize();

	HWND	CreateNonFullscreenWindow( HINSTANCE hInstance, WNDCLASSEX* pWc);

public:
	CWindow(void);
	virtual		~CWindow(void);

	bool		Create(HINSTANCE hInstance, int nCmdShow, LPCWSTR wndName, WINDOW_TYPE wndType, DISPLAY_MODE dispMode);
	int			Exit(){return (int)m_msg.wParam;}
	void		SetShowWindow(int nCmdShow);
	bool		DispatchedWMQuitMessage(void);


	HWND			GetHWnd()const;
	HINSTANCE		GetHInstance()const;
	DISPLAY_MODE	GetDisplayMode()const;
	int		GetWndWidth()const;
	int		GetWndHeight()const;
	int		GetClientWidth()const;
	int		GetClientHeight()const;
	bool	IsFullscreen()const;



};
#endif // !__C_WINDOW_H__ONE__
