#pragma once

#include<d3d11.h>
#include<vector>
#include"Framework\Result.h"
#include"Framework\Debug\Debug.h"

// シングルトン実装

#pragma comment( lib, "d3d11.lib")


class CD3D11Device
{
private:
	ID3D11Device*		m_pD3dDevice;			// 3Dデバイス
	ID3D11DeviceContext*	m_pImmediateContext;	// デバイスコンテキスト(イミディエイト)
	//static std::vector<ID3D11DeviceContext*>	m_pDeferrdContext;	// デバイスコンテキスト動的配列(ディファード)
	D3D_FEATURE_LEVEL			m_featureLevel;
	D3D_DRIVER_TYPE				m_deviceType;
	D3D11_CREATE_DEVICE_FLAG	m_createDeviceFlags;

	CD3D11Device(void);
	CD3D11Device& operator =(CD3D11Device&);


public:
	//CD3D11Device(void);
	~CD3D11Device(void);

	static CD3D11Device&	GetInstance();

	RESULT	Create();
	void	Destroy();

	ID3D11Device*				GetD3dDevice()const;
	ID3D11DeviceContext*		GetImmediateContext()const;

};

