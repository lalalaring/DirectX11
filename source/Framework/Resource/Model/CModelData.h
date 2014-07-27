#pragma once

#include<d3d11.h>
#include<xnamath.h>
#include<vector>
#include<string>
#include"Framework\Utility\SafeDelete.h"
#include"Framework\Resource\\Shader\CShaderManager.h"

class CModelLoader;

// 定数バッファ用
struct s_cbMaterial
{
	XMFLOAT4	m_diffuse;
	XMFLOAT4	m_specular;
	float		m_power;
};

// マテリアルとシェーダが1対1とは限らん
struct s_material
{
	s_cbMaterial m_cbMaterial;
	UINT		m_vertexShaderIndex;
	UINT		m_pixelShaderIndex;
	UINT		m_inputLayoutIndex;

	std::vector<ID3D11ShaderResourceView*>	m_srView;


	s_material()
	{
		m_vertexShaderIndex = NONE_SHADER_INDEX;
		m_pixelShaderIndex = NONE_SHADER_INDEX;
		m_inputLayoutIndex = NONE_INPUTLAYOUT_INDEX;

	}

	~s_material()
	{
		for( auto ite = m_srView.begin(); ite != m_srView.end(); ++ite ){
			SafeRelease( *ite);
		}
	}
};

class CModelData
{
friend CModelLoader;

protected:
	UINT	m_numVertex;
	UINT	m_nunIndex;
	UINT	m_numMaterial;
	UINT*	m_pVertexStrides;
	UINT*	m_pVertexOffsets;

	std::wstring	m_filePath;
	std::wstring	m_fileName;
	std::vector<UINT>	m_startIndexLocations;
	std::vector<UINT>	m_indexCounts;

	std::vector<s_material>	m_materials;

	ID3D11Buffer*	m_pVertexBuffer;
	ID3D11Buffer*	m_pIndexBuffer;

	DXGI_FORMAT		m_indexFormat;
	D3D11_PRIMITIVE_TOPOLOGY	m_primitiveTopology;

	bool	m_canRendering;	// 正常に描画できるかのフラグ(ちゃんとロード出来たか)

public:
	CModelData();
	virtual ~CModelData();

	bool	SetVertexShaderIndex( UINT _materialIndex, UINT _shaderIndex);
	bool	SetPixelShaderIndex( UINT _materialIndex, UINT _shaderIndex);
	bool	SetInputLayoutIndex( UINT _materialIndex, UINT _inputLayoutIndex);

	// Vertex buffer.
	ID3D11Buffer* const*	GetVertexBuffer()const;
	UINT*	GetVertexStrides()const;
	UINT*	GetVertexOffsets()const;

	// Index buffer.
	ID3D11Buffer*	GetIndexBuffer()const;
	DXGI_FORMAT		GetIndexFormat()const;
	UINT			GetIndexStartLocation( UINT _materialIndex)const;
	UINT			GetIndexCounts( UINT _materialIndex)const;

	// Material.
	UINT			GetNumMaterial()const;
	ID3D11ShaderResourceView*	GetShaderResourceView( UINT _materialIndex, UINT _textureIndex)const;

	// Shader.
	UINT	GetVertexShaderIndex( UINT _materialIndex, UINT _shaderIndex)const;
	UINT	GetPixelShaderIndex( UINT _materialIndex, UINT _shaderIndex)const;
	bool	CheckShader();

	// Other.
	UINT	GetInputLayoutIndex( UINT _materialIndex, UINT _shadrIndex)const;
	D3D11_PRIMITIVE_TOPOLOGY	GetPrimitiveTopology()const;
	bool	CanRendering()const;
};
