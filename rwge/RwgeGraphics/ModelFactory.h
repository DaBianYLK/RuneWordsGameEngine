#pragma once

#include "Model.h"

class ModelFactory
{
public:
	static Model* CreateTriangle();
	static Model* CreateBox();

	static Model* LoadModel(std::string strPath);
};
