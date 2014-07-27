#pragma once

#include<DirectXTex.h>

#pragma comment( lib, "DirectXTex.lib")

class CTextureLoader
{
public:
	CTextureLoader();
	~CTextureLoader();

	HRESULT	Load( std::wstring _filePath, ID3D11ShaderResourceView** _ppOut);
};

