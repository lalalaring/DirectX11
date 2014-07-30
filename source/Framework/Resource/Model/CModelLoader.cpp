#include "CModelLoader.h"
#include"Framework\Renderer\CD3D11Device.h"
#include"Framework\Resource\Texture\CTextureLoader.h"
#include"Framework\Utility\SafeDelete.h"
#include<clocale>

CModelLoader::CModelLoader()
{
}

CModelLoader::~CModelLoader()
{
}


bool	CModelLoader::LoadPMD( std::wstring _filePath, CModelData* _pOut)
{
	if( _filePath.empty() || _pOut == nullptr )
		return false;

	std::ifstream ifs( _filePath, std::ios::binary);
	if( !ifs )
		return false;

	_pOut->m_filePath = _filePath;
	_pOut->m_fileName = _filePath.substr( _filePath.find_last_of( L"/")+1);

	if(!this->LoadFromPMDFile( ifs, _pOut))
		return false;

	// シェーダもセットされていれば、CanRenderingをtrueに
//	if( _pOut->CheckShader() )
		_pOut->m_canRendering = true;

	return true;
}

bool	CModelLoader::LoadPMD( std::wstring _filePath, CAnimationModelData* _pOut)
{
	if( _filePath.empty() || _pOut == nullptr )
		return false;

	std::ifstream ifs( _filePath, std::ios::binary);
	if( !ifs )
		return false;

	if(!this->LoadFromPMDFile( ifs, _pOut))
		return false;

	// ボーン
	
	// シェーダもセットされていれば、CanRenderingをtrueに
	if( _pOut->CheckShader() )
		_pOut->m_canRendering = true;

	return true;
}

bool	CModelLoader::LoadFromPMDFile( std::ifstream& _ifs, CModelData* _pOut)
{
	PmdHeader pmdHeader;
	_ifs.read( (char*)&pmdHeader, sizeof( PmdHeader));

	if( strcmp( pmdHeader.magic, "Pmd") != 0 )
		return false;

	PmdVertex* pPmdVertices;
	USHORT* pPmdFaces;
	PmdMaterial* pPmdMaterials;

	// 頂点
	_ifs.read((char*)&_pOut->m_numVertex, sizeof( UINT));
	pPmdVertices = new PmdVertex[_pOut->m_numVertex];
	_ifs.read((char*)pPmdVertices, sizeof(PmdVertex) * _pOut->m_numVertex);
	
	// 面
	_ifs.read((char*)&_pOut->m_nunIndex, sizeof( UINT));
	pPmdFaces = new USHORT[_pOut->m_nunIndex];
	_ifs.read((char*)pPmdFaces, sizeof(USHORT) * _pOut->m_nunIndex);

	// マテリアル
	_ifs.read((char*)&_pOut->m_numMaterial, sizeof( UINT));
	pPmdMaterials = new PmdMaterial[_pOut->m_numMaterial];
	_ifs.read((char*)pPmdMaterials, sizeof(PmdMaterial) * _pOut->m_numMaterial);

	bool isCreate = this->CreateModelDataFromPMD( pPmdVertices, pPmdFaces, pPmdMaterials, _pOut);

	SafeDeleteArray( pPmdVertices);
	SafeDeleteArray( pPmdFaces);
	SafeDeleteArray( pPmdMaterials);

	return isCreate;
}

bool	CModelLoader::CreateModelDataFromPMD( PmdVertex* _pPmdVertices, USHORT* _pPmdFaces, PmdMaterial* _pPmdMaterials, CModelData* _pModelData)
{
	if( _pPmdVertices == nullptr || _pPmdFaces == nullptr || _pModelData == nullptr )
	{
		return false;
	}

	HRESULT hr;

	D3D11_BUFFER_DESC bufferDesc;
	D3D11_SUBRESOURCE_DATA	subResourceData;

	UINT* stride = new UINT[1];
	UINT* offset = new UINT[1];
	stride[0] = sizeof(PmdVertex);
	offset[0] = 0;
	_pModelData->m_pVertexStrides = stride;
	_pModelData->m_pVertexOffsets = offset;

	// S---------- Create Vertex buffer. ----------
	bufferDesc.ByteWidth = sizeof( PmdVertex) * _pModelData->m_numVertex;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	subResourceData.pSysMem = _pPmdVertices;
	subResourceData.SysMemPitch = 0;
	subResourceData.SysMemSlicePitch = 0;

	hr = CD3D11Device::GetInstance().GetD3dDevice()->CreateBuffer( &bufferDesc, &subResourceData, &_pModelData->m_pVertexBuffer);
	if( FAILED(hr) )
		return false;
	// E---------- Create Vertex buffer. ----------


	// S---------- Create index buffer. ----------
	bufferDesc.ByteWidth = sizeof( USHORT) * _pModelData->m_nunIndex;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	
	subResourceData.pSysMem = _pPmdFaces;

	hr = CD3D11Device::GetInstance().GetD3dDevice()->CreateBuffer( &bufferDesc, &subResourceData, &_pModelData->m_pIndexBuffer);
	if( FAILED(hr) )
		return false;

	_pModelData->m_indexFormat = DXGI_FORMAT_R16_UINT;
	// E---------- Create index buffer. ----------


	// S---------- Create material and index location. ----------
	CTextureLoader texLoader;
	std::wstring filePathTemp = _pModelData->m_filePath.substr( 0, _pModelData->m_filePath.find_last_of( L"/")+1 );
	std::wstring filePath;
	wchar_t textureFileName[20];

	_pModelData->m_materials.resize( _pModelData->m_numMaterial);
	_pModelData->m_startIndexLocations.resize( _pModelData->m_numMaterial);
	_pModelData->m_indexCounts.resize( _pModelData->m_numMaterial);
	for( UINT i=0; i<_pModelData->m_numMaterial; ++i ){
		filePath = filePathTemp;

		mbstowcs_s( nullptr, textureFileName, _pPmdMaterials[i].textureFileName, sizeof( textureFileName));
		filePath = filePath + textureFileName;
		filePath = filePath.substr( 0, filePath.find_last_of( L"*"));

		_pModelData->m_materials[i].m_srView.push_back( nullptr);
		hr = texLoader.Load( filePath, &_pModelData->m_materials[i].m_srView[0]);
		if( FAILED(hr) )
		{
			return false;
		}

		// Index location.
		if( i == 0 )
		{
			_pModelData->m_startIndexLocations[i] = 0;
			_pModelData->m_indexCounts[i] = _pPmdMaterials[i].faceVertCnt;
		}
		else
		{
			_pModelData->m_startIndexLocations[i] = _pPmdMaterials[i-1].faceVertCnt + _pModelData->m_startIndexLocations[i-1];
			_pModelData->m_indexCounts[i] = _pPmdMaterials[i].faceVertCnt;
		}
	}
	// E---------- Create material and index location. ----------
	

	return true;
}