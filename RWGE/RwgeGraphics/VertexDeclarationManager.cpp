#include "VertexDeclarationManager.h"

using namespace std;

VertexDeclarationManager::VertexDeclarationManager(D3D9Device* pDevice) : m_pDevice(pDevice)
{

}

VertexDeclarationManager::~VertexDeclarationManager()
{

}

VertexDeclaration* VertexDeclarationManager::GetVertexDeclaration(const string& strDeclarationName)
{
	map<string, VertexDeclaration*>::iterator it = m_mapVertexDeclarations.find(strDeclarationName);
	if (it != m_mapVertexDeclarations.end())
	{
		return it->second;
	}

	return nullptr;
}
