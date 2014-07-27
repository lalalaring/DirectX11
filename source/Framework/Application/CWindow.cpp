#include "CWindow.h"
#include"Framework\Debug\CDebugLog.h"


CWindow::CWindow(void)
{
	m_hWnd = 0;
	m_hInstance = 0;
	m_wndWidth = 0;
	m_wndHeight = 0;
	m_clientWidth = 0;
	m_clientHeight = 0;
	m_isFullscreen = false;
}

CWindow::~CWindow(void)
{
}

// ������
bool	CWindow::Create( HINSTANCE hInstance, int nCmdShow, LPCWSTR wndName, WINDOW_TYPE wndType, DISPLAY_MODE dispMode)
{
	WNDCLASSEX wc;

	this->SetWndClass( &wc, hInstance);

	lstrcpyW( m_wndName, wndName);

	// �f�o�b�O���[�h�ł��邱�Ƃ��E�B���h�E�ɖ��L����
#ifdef ONE_DEBUG
	lstrcatW( m_wndName, L"[Debug]");
#endif // ONE_DEBUG


	m_hInstance = hInstance;
	m_dispMode = dispMode;

	// �t���X�N���[�����ۂ��Ŕ���
	if( wndType == WINDOW_TYPE_WINDOW)
	{
		m_isFullscreen = false;

		switch( m_dispMode )
		{
		case DISPLAY_MODE_VGA_640x480:
			m_wndWidth = 640;
			m_wndHeight = 480;
			break;

		case DISPLAY_MODE_SVGA_800x600:
			m_wndWidth = 800;
			m_wndHeight = 600;
			break;
		}

		// �w�肳�ꂽ�E�B���h�E�T�C�Y�ō쐬
		m_hWnd = this->CreateNonFullscreenWindow( hInstance, &wc);
	}
	else
	{
		// PC�̃X�N���[���̕��ƍ����ŃE�B���h�E������Ă���
		m_wndWidth = GetSystemMetrics(SM_CXSCREEN);
		m_wndHeight = GetSystemMetrics(SM_CYSCREEN);
		m_isFullscreen = true;
		m_hWnd = CreateWindow(wc.lpszClassName,
				m_wndName,
				WS_VISIBLE | WS_POPUP,
				0,
				0,
				m_wndWidth,
				m_wndHeight,
				nullptr,
				nullptr,
				hInstance,
				nullptr
			);
	}

	// �`�F�b�N
	if( !m_hWnd )
	{
		CheckBoolResult( RESULT_TYPE_CREATE, false, "Window");
		return false;
	}
	else
	{
		CheckBoolResult( RESULT_TYPE_CREATE, true, "Window");
	}

	ShowWindow( m_hWnd, nCmdShow);
	UpdateWindow( m_hWnd);

	// �N���C�A���g�̈�ݒ�
	this->SetClientSize();

	ValidateRect( m_hWnd, nullptr);	// WM_PAINT���Ă΂�Ȃ��悤�ɂ���

	return true;
}

// �t���X�N���[���ł͂Ȃ��E�B���h�E�����
HWND	CWindow::CreateNonFullscreenWindow( HINSTANCE hInstance, WNDCLASSEX* pWc)
{
	HWND hWnd;

	hWnd = CreateWindow(pWc->lpszClassName,
			m_wndName,
			WS_SYSMENU,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			m_wndWidth,
			m_wndHeight,
			nullptr,
			nullptr,
			hInstance,
			nullptr
		);

	return hWnd;
}

// �N���C�A���g���W�����
void	CWindow::SetClientSize()
{
	RECT clientSize;
	GetClientRect( m_hWnd, &clientSize);
	m_clientWidth = clientSize.right;
	m_clientHeight = clientSize.bottom;
}

// �E�B���h�E�N���X�̍쐬
void	CWindow::SetWndClass( WNDCLASSEX* pOut, HINSTANCE hInstance)
{
	if( pOut == nullptr )
		throw RESULT_TYPE_CREATE,false;

	pOut->cbSize = sizeof(WNDCLASSEX);
	pOut->style = CS_HREDRAW | CS_VREDRAW;
	pOut->hInstance = hInstance;
	pOut->lpszClassName = CLASS_NAME;
	pOut->lpfnWndProc = (WNDPROC)WndProc;
	pOut->hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	pOut->hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
	pOut->hCursor = LoadCursor(nullptr, IDC_ARROW);
	pOut->hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	pOut->lpszMenuName = nullptr;
	pOut->cbClsExtra = 0;
	pOut->cbWndExtra = 0;

	if( !(RegisterClassEx(pOut)) )
	{	// �݌v����Windows�ɒ�o
		CheckBoolResult( RESULT_TYPE_CREATE, false, "MainWindow");
		throw RESULT_TYPE_CREATE,false;
	}
}


// �E�B���h�E�̕\���ݒ�
void	CWindow::SetShowWindow(int nCmdShow)
{
	ShowWindow( m_hWnd, nCmdShow);
}


// ���b�Z�[�W�̑��o
bool	CWindow::DispatchedWMQuitMessage(void)
{
	if( PeekMessage( &m_msg, nullptr, 0, 0, PM_REMOVE) )
	{
		if( m_msg.message == WM_QUIT )
			return true;

		TranslateMessage(&m_msg);
		DispatchMessage(&m_msg);
	}
	return false;
}


// �E�B���h�E�v���V�[�W��
LRESULT	CWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch( message ){

#ifdef ONE_DEBUG
		case WM_KEYDOWN:
			if( wParam == VK_ESCAPE )
				DestroyWindow( hWnd);
			break;
#endif

		case WM_CLOSE:
			if( IDYES == MessageBox( hWnd, _T("�I�����܂����H"), _T("�m�F"), MB_YESNO) )
				DestroyWindow( hWnd);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;	
}

// ��������ȉ�Getter���\�b�h
HWND	CWindow::GetHWnd()const
{
	return m_hWnd;
}

HINSTANCE	CWindow::GetHInstance()const
{
	return m_hInstance;
}

DISPLAY_MODE	CWindow::GetDisplayMode()const
{
	return m_dispMode;
}

int	CWindow::GetWndWidth()const
{
	return m_wndWidth;
}

int	CWindow::GetWndHeight()const
{
	return m_wndHeight;
}

int	CWindow::GetClientWidth()const
{
	return m_clientWidth;
}

int CWindow::GetClientHeight()const
{
	return m_clientHeight;
}

bool	CWindow::IsFullscreen()const
{
	return m_isFullscreen;
}

