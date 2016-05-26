#include "RwgeVertexDeclarationManager.h"

#include <d3dx9.h>
#include <RwgeVertexDeclarationTemplate.h>
#include <RwgeLog.h>

using namespace std;

const string strDefaultVertexDeclarationName = "DefaultVertexDeclaration";

RVertexDeclarationManager::RVertexDeclarationManager()
{
	GenerateDefaultVertexDeclaration();
}

RVertexDeclarationManager::~RVertexDeclarationManager()
{

}

RD3d9VertexDeclaration* RVertexDeclarationManager::GetDefaultVertexDeclaration()
{
	return m_mapVertexDeclarations[strDefaultVertexDeclarationName];
}

void RVertexDeclarationManager::GenerateDefaultVertexDeclaration()
{
	RVertexDeclarationTemplate declarationTemplate;

	// { Type, Method, Usage, UsageIndex }
	// 使用D3DDECLMETHOD_CROSSUV时顶点声明会创建失败，原因不明（DX的官方Sample中Tangent使用的Method也是Default）
	VertexElement position = { D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 };
	VertexElement texCoord = { D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 };
	VertexElement normal = { D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 };
	VertexElement tangent = { D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 };

	declarationTemplate.PushBackVertexElement(position);
	declarationTemplate.PushBackVertexElement(texCoord);
	declarationTemplate.PushBackVertexElement(normal);
	declarationTemplate.PushBackVertexElement(tangent);

	RD3d9VertexDeclaration* pVertexDeclaration = new RD3d9VertexDeclaration(declarationTemplate);

	if (!m_mapVertexDeclarations.insert(make_pair(strDefaultVertexDeclarationName, pVertexDeclaration)).second)
	{
		RwgeLog(TEXT("Insert default vertex declaration failed!"));
	}
}
