#include "CD3D11Device.h"
#include "Framework\Utility\SafeDelete.h"

CD3D11Device::CD3D11Device()
{
}

CD3D11Device::~CD3D11Device(void)
{
	this->Destroy();
}

CD3D11Device&	CD3D11Device::GetInstance()
{
	static CD3D11Device obj;

	return obj;
}

RESULT	CD3D11Device::Create()
{
	HRESULT hr;

// デバイスの種類をdefineで変えている
#ifdef ONE_DEBUG
	m_createDeviceFlags = D3D11_CREATE_DEVICE_DEBUG;
	//m_deviceType = D3D_DRIVER_TYPE_REFERENCE;	// くそおもいｗｗ
	m_deviceType = D3D_DRIVER_TYPE_HARDWARE;
#else
	m_createDeviceFlags = D3D11_CREATE_DEVICE_SWITCH_TO_REF;
	m_deviceType = D3D_DRIVER_TYPE_HARDWARE;
#endif // ONE_DEBUG

	// デバイス作成
	hr = D3D11CreateDevice( nullptr,
							m_deviceType,
							nullptr, 
							m_createDeviceFlags,
							nullptr,
							0,
							D3D11_SDK_VERSION,
							&m_pD3dDevice,
							&m_featureLevel,
							&m_pImmediateContext);

	if( FAILED(hr) )
		return RESULT_FAILED_SYSTEM;
	
	// マルチスレッドをサポートしているかチェック
	//D3D11_FEATURE_DATA_THREADING featureThread;
	//hr = m_pD3dDevice->CheckFeatureSupport( D3D11_FEATURE_THREADING, &featureThread, sizeof( featureThread));

	//if( featureThread.DriverConcurrentCreates )



	return RESULT_OK;
}

void	CD3D11Device::Destroy()
{
	if( m_pImmediateContext )
		m_pImmediateContext->ClearState();
	SafeRelease( m_pImmediateContext);

	SafeRelease( m_pD3dDevice);
}

ID3D11Device*	CD3D11Device::GetD3dDevice()const
{
	return m_pD3dDevice;
}

ID3D11DeviceContext*	CD3D11Device::GetImmediateContext()const
{
	return m_pImmediateContext;
}
