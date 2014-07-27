#pragma once

#include<d3d11.h>
#include<d3dcompiler.h>
#include<string>
#include<vector>
#include"Framework\Utility\SafeDelete.h"

#pragma comment( lib, "d3dcompiler.lib")


const UINT NONE_SHADER_INDEX = 999999;
const UINT NONE_INPUTLAYOUT_INDEX = 999999;

// インプットレイアウトの為に頂点シェーダだけ構造体に
struct s_VS
{
	//UINT		m_inputLayoutIndex;
	ID3DBlob*	m_pCompile;
	ID3D11VertexShader*	m_pShader;

	s_VS()
	{
		//m_inputLayoutIndex = NONE_INPUTLAYOUT_INDEX;	// マテリアルに持たせることに(どっちがいいのやら)
		m_pCompile = nullptr;
		m_pShader = nullptr;
	}

	~s_VS()
	{
		SafeRelease( m_pCompile);
		SafeRelease( m_pShader);
	}
};

class CShaderManager
{
private:
	std::vector<s_VS*>	m_vertexShaders;
	std::vector<ID3D11PixelShader*> m_pixelShaders;
	std::vector<ID3D11InputLayout*> m_inputLayouts;

	CShaderManager();
	CShaderManager& operator=(CShaderManager&);

	HRESULT Compile( std::wstring _filePath, std::string _entryPoint, std::string _profile, ID3DBlob** _pOut);

public:
	~CShaderManager();

	static CShaderManager& GetInstance();

	HRESULT	CompileVS( std::wstring _filePath, std::string _entryPoint, std::string _profile);
	HRESULT CompilePS( std::wstring _filePath, std::string _entryPoint, std::string _profile);
	HRESULT	CreateInputLayout( UINT _vsIndex, D3D11_INPUT_ELEMENT_DESC* _pElement, UINT _numElement);

	UINT	Reset();	// シェーダを消す(シーン切り替えの時とかに呼ぶ。戻り値は残ったシェーダの数。今は全て消すので0)

	ID3D11VertexShader*	GetVertexShader( UINT _index)const;
	ID3D11PixelShader*	GetPixelShader( UINT _index)const;
	ID3D11InputLayout*	GetInputLayout( UINT _index)const;

};

