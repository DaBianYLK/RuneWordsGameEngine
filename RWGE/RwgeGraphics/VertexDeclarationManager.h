#pragma once

#include "VertexDeclaration.h"
#include <string>
#include <map>

class VertexDeclarationManager
{
public:
	VertexDeclarationManager(D3D9Device* pDevice);
	~VertexDeclarationManager();

	VertexDeclaration* GetVertexDeclaration(const std::string& strDeclarationName);

private:
	D3D9Device* m_pDevice;

	std::map<std::string, VertexDeclaration*>	m_mapVertexDeclarations;	// <顶点声明名称，顶点声明>
};