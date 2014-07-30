#pragma once

#include"Framework\Renderer\CD3D11Device.h"

struct s_cBStruct
{};

class CConstantBuffer
{
private:
	UINT	m_srotNum;
	size_t	m_size;
	ID3D11Buffer*	m_pCbuffer;

public:
	CConstantBuffer( size_t _bufferDataSize, UINT _srotNum)
	{
		m_srotNum = _srotNum;
		m_size = _bufferDataSize;
		m_pCbuffer = nullptr;
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
	HRESULT	Update( s_cBStruct* _pBufferData)
	{
		ID3D11Device*	pD3dDevice = CD3D11Device::GetInstance().GetD3dDevice();
		ID3D11DeviceContext* pImmediate = CD3D11Device::GetInstance().GetImmediateContext();
		if( pD3dDevice == nullptr || pImmediate == nullptr )
			return E_FAIL;

		HRESULT hr;
		D3D11_BUFFER_DESC bufferDesc;
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		bufferDesc.ByteWidth = m_size;
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;

		hr = pD3dDevice->CreateBuffer( &bufferDesc, nullptr, &m_pCbuffer);
		if( FAILED(hr) )
			return E_FAIL;

		hr = pImmediate->Map( m_pCbuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if( FAILED(hr) )
			return E_FAIL;
		CopyMemory( mappedResource.pData, _pBufferData, m_size);
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
	ID3D11Buffer* const*	GetConstantBuffer()const
	{
		return &m_pCbuffer;
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

