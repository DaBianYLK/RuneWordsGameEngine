/*--------------------------------------------------------------------------------------------------------------------*\
   ��CREATE��
	AUTH :	���һ���																			   DATE : 2016-05-24
	DESC :	
	1.	���������ģ��
\*--------------------------------------------------------------------------------------------------------------------*/


#pragma once

#include "RwgeModel.h"

class ModelFactory
{
public:
	static RModel* CreateTriangle();
	static RModel* CreatePanel();
	static RModel* CreateBox();

	static RMesh*  LoadMesh(const std::string& strPath);
	static RModel* CreateZhanHun();

	static RModel* LoadModel(const std::string& strPath);
};
