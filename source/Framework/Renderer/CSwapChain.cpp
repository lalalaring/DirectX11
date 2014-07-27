#include "CSwapChain.h"
#include"Framework\Renderer\CD3D11Device.h"
#include"Framework\Debug\CDebugLog.h"
#include"Framework\Utility\SafeDelete.h"
#include<cassert>

CSwapChain::CSwapChain()
{
}


CSwapChain::~CSwapChain()
{
	this->Destroy();
}

RESULT	CSwapChain::Create( CD3D11Device* pDevice, const CWindow* pWnd)
{
	assert( pDevice->GetD3dDevice());
	assert( pWnd->GetHWnd());

	HRESULT hr;
	IDXGIFactory *pFactory;
	DXGI_SWAP_CHAIN_DESC sd;

	hr = CreateDXGIFactory( __uuidof(IDXGIFactory), (void**)&pFactory);
	if( !CheckHRESULT( RESULT_TYPE_CREATE, hr, "DXGI Factory") )
		return RESULT_FAILED_SYSTEM;
	
	m_clientWidth = pWnd->GetClientWidth();
	m_clientHeight = pWnd->GetClientHeight();

	// Create swap chain
	this->CreateSwapChainDesc( &sd, pWnd);
	hr = pFactory->CreateSwapChain( pDevice->GetD3dDevice(), &sd, &m_pSwapChain);
	if( !CheckHRESULT( RESULT_TYPE_CREATE, hr, "Swap chain") )
		return RESULT_FAILED_SYSTEM;

	return RESULT_OK;
}

void	CSwapChain::Destroy()
{
	SafeRelease( m_pSwapChain);
}

void	CSwapChain::CreateSwapChainDesc( DXGI_SWAP_CHAIN_DESC* pOut, const CWindow* pWnd)
{
	pOut->BufferCount = 1;
	pOut->BufferDesc.Width = m_clientWidth;
	pOut->BufferDesc.Height = m_clientHeight;
	pOut->BufferDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	pOut->BufferDesc.RefreshRate.Numerator = 60;
	pOut->BufferDesc.RefreshRate.Denominator = 1;
	pOut->BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	pOut->BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	pOut->BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	pOut->OutputWindow = pWnd->GetHWnd();
	pOut->SampleDesc.Count = 1;
	pOut->SampleDesc.Quality = 0;
	pOut->SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	pOut->Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	if( pWnd->IsFullscreen() )
		pOut->Windowed = FALSE;
	else
		pOut->Windowed = TRUE;
}

IDXGISwapChain*	CSwapChain::GetSwapChain()const
{
	return m_pSwapChain;
}

UINT	CSwapChain::GetWidth()const
{
	return m_clientWidth;
}

UINT	CSwapChain::GetHeight()const
{
	return m_clientHeight;
}