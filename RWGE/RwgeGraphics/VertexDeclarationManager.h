#pragma once

#include "VertexDeclarationType.h"
#include <string>
#include <map>
#include <Singleton.h>

class VertexDeclarationManager : public Singleton<VertexDeclarationManager>
{
public:
	VertexDeclarationManager();
	~VertexDeclarationManager();

	void GenerateDefaultVertexDeclaration();
	VertexDeclarationType* GetDefaultVertexDeclaration();
	VertexDeclarationType* GetVertexDeclaration(const std::string& strDeclarationName);

private:
	static char* m_DefaultVertexDeclarationName;

	std::map<std::string, VertexDeclarationType*>	m_mapVertexDeclarations;	// <顶点声明名称，顶点声明>
};
