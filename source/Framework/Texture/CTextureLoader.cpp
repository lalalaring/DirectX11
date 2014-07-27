#include "CTextureLoader.h"
#include"Framework\Renderer\CD3D11Device.h"

CTextureLoader::CTextureLoader()
{
}


CTextureLoader::~CTextureLoader()
{
}

HRESULT	CTextureLoader::Load( std::wstring _filePath, ID3D11ShaderResourceView** _ppOut)
{
	if( _filePath.empty() || _ppOut == nullptr )
		return E_FAIL;

	ID3D11Device* pDevice = CD3D11Device::GetInstance().GetD3dDevice();
	if( pDevice == nullptr )
		return E_FAIL;

	HRESULT hr;
	std::wstring fileExtension = _filePath.substr( _filePath.find_last_of( L".")+1);
	DirectX::TexMetadata metaData;
	DirectX::ScratchImage scratchImage;

	if( fileExtension == L"tga" )
	{
		hr = DirectX::LoadFromTGAFile( _filePath.c_str(), &metaData, scratchImage);
		if( FAILED(hr) )
			return E_FAIL;
	}
	else if( fileExtension == L"dds" )
	{
		hr = DirectX::LoadFromDDSFile( _filePath.c_str(), 0, &metaData, scratchImage);
		if( FAILED(hr) )
			return E_FAIL;
	}
	else
	{
		hr = DirectX::LoadFromWICFile( _filePath.c_str(), 0, &metaData, scratchImage);
		if( FAILED(hr) )
			return E_FAIL;
	}

	hr = DirectX::CreateShaderResourceView( pDevice,
											scratchImage.GetImages(),
											scratchImage.GetImageCount(),
											metaData,
											_ppOut);
	
	return hr;
}