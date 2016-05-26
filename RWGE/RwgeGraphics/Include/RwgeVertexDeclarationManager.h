/*--------------------------------------------------------------------------------------------------------------------*\
   【CREATE】	
	AUTH :	大便一箩筐																			   DATE : 2016-05-18
	DESC :	
	1.	用于生成顶点声明

	ToDo：
	2016-05-18
		暂时只支持通过VertexDeclarationTemplate生成顶点声明，还需要提供从文件加载顶点声明的功能
\*--------------------------------------------------------------------------------------------------------------------*/


#pragma once

#include <string>
#include <map>
#include <RwgeObject.h>
#include <RwgeSingleton.h>
#include "RwgeD3d9VertexDeclaration.h"

class RVertexDeclarationManager : 
	public RObject,
	public Singleton<RVertexDeclarationManager>
{
public:
	RVertexDeclarationManager();
	~RVertexDeclarationManager();

	RD3d9VertexDeclaration* GetDefaultVertexDeclaration();

private:
	void GenerateDefaultVertexDeclaration();

private:
	std::map<std::string, RD3d9VertexDeclaration*> m_mapVertexDeclarations;
};
