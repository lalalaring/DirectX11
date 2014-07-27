#pragma once

#include"CModelData.h"

class CModelLoader;

class CAnimationModelData
	:public CModelData
{
friend CModelLoader;

public:
	CAnimationModelData();
	~CAnimationModelData();
};

