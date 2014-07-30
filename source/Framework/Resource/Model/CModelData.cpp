#include "CModelData.h"
#include"Framework\Resource\Shader\CShaderManager.h"

CModelData::CModelData()
{
	m_pVertexBuffer = nullptr;
	m_pVertexStrides = nullptr;
	m_pVertexOffsets = nullptr;

	m_pIndexBuffer = nullptr;
	m_indexFormat = DXGI_FORMAT_R32_UINT;

	m_primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_canRendering = false;
}


CModelData::~CModelData()
{
}

bool	CModelData::SetVertexShaderIndex( UINT _materialIndex, UINT _shaderIndex)
{
	if( _materialIndex >= m_materials.size() )
		return false;

	m_materials[_materialIndex].m_vertexShaderIndex = _shaderIndex;

	return true;
}

bool	CModelData::SetPixelShaderIndex( UINT _materialIndex, UINT _shaderIndex)
{
	if( _materialIndex >= m_materials.size() )
		return false;

	m_materials[_materialIndex].m_pixelShaderIndex = _shaderIndex;

	return true;
}


bool	CModelData::SetInputLayoutIndex( UINT _materialIndex, UINT _inputLayoutIndex)
{
	if( _materialIndex >= m_materials.size() )
		return false;

	m_materials[_materialIndex].m_inputLayoutIndex = _inputLayoutIndex;

	return true;
}

ID3D11Buffer* const*	CModelData::GetVertexBuffer()const
{
	return &m_pVertexBuffer;
}

UINT*	CModelData::GetVertexStrides()const
{
	return m_pVertexStrides;
}

UINT*	CModelData::GetVertexOffsets()const
{
	return m_pVertexOffsets;
}

ID3D11Buffer*	CModelData::GetIndexBuffer()const
{
	return m_pIndexBuffer;
}

DXGI_FORMAT	CModelData::GetIndexFormat()const
{
	return m_indexFormat;
}

UINT	CModelData::GetIndexStartLocation( UINT _materialIndex)const
{
	if( _materialIndex >= m_startIndexLocations.size() )
		return 0;

	return m_startIndexLocations[_materialIndex];
}

UINT	CModelData::GetIndexCounts( UINT _materialIndex)const
{
	if( _materialIndex >= m_indexCounts.size() )
		return 0;

	return m_indexCounts[_materialIndex];
}

UINT	CModelData::GetNumMaterial()const
{
	return m_numMaterial;
}

ID3D11ShaderResourceView* const*	CModelData::GetShaderResourceView( UINT _materialIndex, UINT _textureIndex)const
{
	if( _materialIndex >= m_materials.size() )
		return nullptr;

	if( _textureIndex >= m_materials[_materialIndex].m_srView.size() )
		return nullptr;

	return &m_materials[_materialIndex].m_srView[_textureIndex];
}

UINT	CModelData::GetVertexShaderIndex( UINT _materialIndex, UINT _shaderIndex)const
{
	if( _materialIndex >= m_numMaterial )
		return NONE_SHADER_INDEX;

	return m_materials[_materialIndex].m_vertexShaderIndex;
}

UINT	CModelData::GetInputLayoutIndex( UINT _materialIndex, UINT _shaderIndex)const
{
	if( _materialIndex >= m_numMaterial )
		return NONE_SHADER_INDEX;

	return m_materials[_materialIndex].m_inputLayoutIndex;
}

UINT CModelData::GetPixelShaderIndex( UINT _materialIndex, UINT _shaderIndex)const
{
	if( _materialIndex >= m_numMaterial )
		return NONE_SHADER_INDEX;

	return m_materials[_materialIndex].m_pixelShaderIndex;
}

bool CModelData::CheckShader()
{
	for( auto ite = m_materials.begin(); ite != m_materials.end(); ++ite ){
		if( ite->m_vertexShaderIndex == NONE_SHADER_INDEX ||
			ite->m_pixelShaderIndex == NONE_SHADER_INDEX)
		{
			return false;
		}
	}

	return true;
}

D3D11_PRIMITIVE_TOPOLOGY	CModelData::GetPrimitiveTopology()const
{
	return m_primitiveTopology;
}

bool	CModelData::CanRendering()const
{
	return m_canRendering;
}