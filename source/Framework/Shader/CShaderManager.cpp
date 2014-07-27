#include "CShaderManager.h"
#include "Framework\Renderer\CD3D11Device.h"

CShaderManager::CShaderManager()
{
}


CShaderManager::~CShaderManager()
{
	for( auto ite = m_vertexShaders.begin(); ite != m_vertexShaders.end(); ++ite ){
		SafeDelete( *ite);
	}

	for( auto ite = m_pixelShaders.begin(); ite != m_pixelShaders.end(); ++ite ){
		SafeRelease( *ite);
	}

	for( auto ite = m_inputLayouts.begin(); ite != m_inputLayouts.end(); ++ite ){
		SafeRelease( *ite);
	}
}

HRESULT	CShaderManager::CompileVS( std::wstring _filePath, std::string _entryPoint, std::string _profile)
{
	if( _filePath.empty() || _entryPoint.empty() || _profile.empty() )
		return E_FAIL;

	ID3D11Device* pD3dDevice = CD3D11Device::GetInstance().GetD3dDevice();
	if( pD3dDevice == nullptr )
		return E_FAIL;

	HRESULT hr;
	s_VS*	pVs = new s_VS;

	hr = this->Compile( _filePath, _entryPoint, _profile, &pVs->m_pCompile);
	if( FAILED(hr) )
	{
		return hr;
	}

	hr = pD3dDevice->CreateVertexShader( pVs->m_pCompile->GetBufferPointer(),
										 pVs->m_pCompile->GetBufferSize(),
										 nullptr,
										 &pVs->m_pShader);
	if( FAILED(hr) )
	{
		return hr;
	}

	m_vertexShaders.push_back( pVs);

	return hr;
}

HRESULT	CShaderManager::CompilePS( std::wstring _filePath, std::string _entryPoint, std::string _profile)
{
	if( _filePath.empty() || _entryPoint.empty() || _profile.empty() )
		return E_FAIL;

	ID3D11Device* pD3dDevice = CD3D11Device::GetInstance().GetD3dDevice();
	if( pD3dDevice == nullptr )
		return E_FAIL;

	HRESULT hr;
	ID3D11PixelShader*	pShader;
	ID3DBlob* pCompile;

	hr = this->Compile( _filePath, _entryPoint, _profile, &pCompile);
	if( FAILED(hr) )
	{
		return hr;
	}


	hr = pD3dDevice->CreatePixelShader( pCompile->GetBufferPointer(),
										pCompile->GetBufferSize(),
										nullptr,
										&pShader);
	if( FAILED(hr) )
	{
		return hr;
	}

	SafeRelease( pCompile);
	m_pixelShaders.push_back( pShader);

	return hr;
}

HRESULT	CShaderManager::CreateInputLayout( UINT _vsIndex, D3D11_INPUT_ELEMENT_DESC* _pElement, UINT _numElement)
{
	if( _vsIndex >= m_vertexShaders.size() || _pElement == nullptr )
		return E_FAIL;

	ID3D11Device* pD3dDevice = CD3D11Device::GetInstance().GetD3dDevice();
	if( pD3dDevice == nullptr )
		return E_FAIL;

	HRESULT hr;
	ID3D11InputLayout* pInputLayout;

	hr = pD3dDevice->CreateInputLayout( _pElement, _numElement,
										m_vertexShaders[_vsIndex]->m_pCompile->GetBufferPointer(),
										m_vertexShaders[_vsIndex]->m_pCompile->GetBufferSize(),
										&pInputLayout);


	if( FAILED(hr) )
		return hr;

	m_inputLayouts.push_back( pInputLayout);
	//m_vertexShaders[_vsIndex]->m_inputLayoutIndex = m_inputLayouts.size()-1;

	return hr;
}

HRESULT	CShaderManager::Compile( std::wstring _filePath, std::string _entryPoint, std::string _profile, ID3DBlob** _ppOut)
{
	if( _ppOut == nullptr )
		return E_FAIL;

	HRESULT hr;
	ID3DBlob* pError = nullptr;

	hr = D3DCompileFromFile( _filePath.c_str(),
							 nullptr, nullptr,
							 _entryPoint.c_str(), _profile.c_str(),
							 0, 0,
							 _ppOut, &pError);

	if( FAILED(hr) )
	{
		if( pError )
		{
			MessageBoxA( nullptr, (char*)pError->GetBufferPointer(), "ppVs error", MB_OK);
		}
		return hr;
	}

	SafeRelease( pError);

	return hr;
}


CShaderManager&	CShaderManager::GetInstance()
{
	static CShaderManager obj;
	return obj;
}

ID3D11VertexShader*	CShaderManager::GetVertexShader( UINT _index)const
{
	if( _index >= m_vertexShaders.size() )
		return nullptr;

	return m_vertexShaders[_index]->m_pShader;
}

ID3D11PixelShader* CShaderManager::GetPixelShader( UINT _index)const
{
	if( _index >= m_pixelShaders.size() )
		return nullptr;

	return m_pixelShaders[_index];
}

ID3D11InputLayout*	CShaderManager::GetInputLayout( UINT _index)const
{
	if( _index >= m_inputLayouts.size() )
		return nullptr;

	return m_inputLayouts[_index];
}