#pragma once

#include<d3d11.h>
#include"Framework\Application\CWindow.h"
#include"Framework\Result.h"

#pragma comment( lib, "DXGI.lib")

class CD3D11Device;

class CSwapChain
{
private:
	IDXGISwapChain*	m_pSwapChain;

	UINT	m_clientWidth;
	UINT	m_clientHeight;

	void	CreateSwapChainDesc( DXGI_SWAP_CHAIN_DESC* pOut, const CWindow* pWnd);

public:
	CSwapChain();
	~CSwapChain();

	RESULT	Create( CD3D11Device* pDevice, const CWindow* pWnd);
	void	Destroy();

	IDXGISwapChain*	GetSwapChain()const;
	UINT	GetWidth()const;
	UINT	GetHeight()const;
};
