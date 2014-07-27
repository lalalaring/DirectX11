#pragma once

#include<fstream>
#include"CAnimationModelData.h"
#include"MmdData.h"

class CModelLoader
{
private:
	bool	LoadFromPMDFile( std::ifstream& _ifs, CModelData* _pOut);

public:
	CModelLoader();
	~CModelLoader();

	bool	LoadPMD( std::wstring _filePath, CModelData* _pOut);
	bool	LoadPMD( std::wstring _filePath, CAnimationModelData* _pOut);

	bool	CreateModelDataFromPMD( PmdVertex* _pPmdVertices, USHORT* _pPmdFaces, PmdMaterial* _pPmdMaterials, CModelData* _pModelData);
};

