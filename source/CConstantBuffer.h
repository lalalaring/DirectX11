#pragma once

#include"Framework\Renderer\CD3D11Device.h"

template<typename T>
class CConstantBuffer
{
private:
	UINT	m_srotNum;
	size_t	m_size;
	ID3D11Buffer*	m_pCbuffer;

public:
	CConstantBuffer( UINT _srotNum)
	{
		m_srotNum = _srotNum;
		m_pCbuffer = nullptr;
		m_size = sizeof( T);
	}
	~CConstantBuffer()
	{
		SafeRelease( m_pCbuffer);
	}


	/*------------------------------
	* @brief	
	* @param
	* @note
	* @author   N.Kaji
	* @date		2014/07/27
	------------------------------*/
	HRESULT	Update( T buffer)
	{
		ID3D11Device*	pD3dDevice = CD3D11Device::GetInstance().GetD3dDevice();
		ID3D11DeviceContext* pImmediate = CD3D11Device::GetInstance().GetImmediateContext();
		if( pD3dDevice == nullptr || pImmediate == nullptr )
			return E_FAIL;

		HRESULT hr;
		D3D11_BUFFER_DESC bufferDesc;
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		bufferDesc.ByteWidth = sizeof( T);
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;

		hr = pD3dDevice->CreateBuffer( &bufferDesc, nullptr, &m_pCbuffer);
		if( FAILED(hr) )
			return E_FAIL;

		pImmediate->Map( m_pCbuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if( FAILED(hr) )
			return E_FAIL;
		CopyMemory( mappedResource.pData, &buffer, sizeof( T));
		pImmediate->Unmap( m_pCbuffer, 0);

		return hr;
	}

	/*------------------------------
	* @brief
	* @param
	* @note
	* @author   N.Kaji
	* @date
	------------------------------*/
	ID3D11Buffer*	GetConstantBuffer()const
	{
		return m_pCbuffer;
	}

	/*------------------------------
	* @brief
	* @param
	* @note
	* @author   N.Kaji
	* @date		
	------------------------------*/
	UINT GetSrotNum()const
	{
		return m_srotNum;
	}
};

