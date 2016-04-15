#pragma once

#include "RwgeModel.h"

class ModelFactory
{
public:
	static Model* CreateTriangle();
	static Model* CreatePanel();
	static Model* CreateBox();

	static Mesh*  LoadMesh(const std::string& strPath);
	static Model* CreateZhanHun();

	static Model* LoadModel(const std::string& strPath);
};
