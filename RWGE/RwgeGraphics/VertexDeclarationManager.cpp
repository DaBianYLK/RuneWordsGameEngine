#include "VertexDeclarationManager.h"

#include <d3dx9.h>

using namespace std;

char* VertexDeclarationManager::m_DefaultVertexDeclarationName = "DefaultVertexDeclaration";

VertexDeclarationManager::VertexDeclarationManager()
{
	GenerateDefaultVertexDeclaration();
}

VertexDeclarationManager::~VertexDeclarationManager()
{

}

void VertexDeclarationManager::GenerateDefaultVertexDeclaration()
{
	VertexDeclarationType* pDeclaration = new VertexDeclarationType();

	// { Size, Type, Method, Usage, UsageIndex }
	// 使用D3DDECLMETHOD_CROSSUV时顶点声明会创建失败，原因不明（DX的官方Sample中Tangent使用的Method也是Default）
	VertexElement position	= { 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 };
	VertexElement texCoord	= {  8, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 };
	VertexElement normal	= { 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0 };
	VertexElement tangent	= { 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT,  0 };

	pDeclaration->AddVertexElement(position);
	pDeclaration->AddVertexElement(texCoord);
	pDeclaration->AddVertexElement(normal);
	pDeclaration->AddVertexElement(tangent);

	pDeclaration->Update();

	m_mapVertexDeclarations.insert(make_pair(m_DefaultVertexDeclarationName, pDeclaration));
}

VertexDeclarationType* VertexDeclarationManager::GetDefaultVertexDeclaration()
{
	return GetVertexDeclaration(m_DefaultVertexDeclarationName);
}

VertexDeclarationType* VertexDeclarationManager::GetVertexDeclaration(const string& strDeclarationName)
{
	map<string, VertexDeclarationType*>::iterator it = m_mapVertexDeclarations.find(strDeclarationName);
	if (it != m_mapVertexDeclarations.end())
	{
		return it->second;
	}

	return nullptr;
}
