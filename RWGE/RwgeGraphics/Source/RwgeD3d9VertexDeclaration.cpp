#include "RwgeD3d9VertexDeclaration.h"

#include "RwgeD3d9Device.h"
#include "RwgeVertexDeclarationTemplate.h"
#include <RwgeLog.h>

using namespace std;

RD3d9VertexDeclaration::RD3d9VertexDeclaration(const RVertexDeclarationTemplate& declarationTemplate)
{
	unsigned char u8ElementCount = declarationTemplate.GetElementCount();
	unsigned char u8StreamCount = declarationTemplate.GetStreamCount();
	m_vecStreamVertexSize.clear();
	m_u32VertexSize = 0;

	D3DVERTEXELEMENT9* pVertexElements = new D3DVERTEXELEMENT9[u8ElementCount + 1];

	unsigned int u32ElementIndex = 0;
	for (unsigned char u8StreamID = 0; u8StreamID < u8StreamCount; ++u8StreamID)
	{
		const list<VertexElement>& streamElements = declarationTemplate.GetVertexElementListOfStream(u8StreamID);
		unsigned short u16Offset = 0;

		for (const VertexElement& element : streamElements)
		{
			pVertexElements[u32ElementIndex].Stream		= u8StreamID;
			pVertexElements[u32ElementIndex].Offset		= u16Offset;
			pVertexElements[u32ElementIndex].Type		= element.u8Type;
			pVertexElements[u32ElementIndex].Method		= element.u8Method;
			pVertexElements[u32ElementIndex].Usage		= element.u8Usage;
			pVertexElements[u32ElementIndex].UsageIndex	= element.u8UsageIndex;

			u16Offset += element.GetElementSize();
			++u32ElementIndex;
		}

		m_vecStreamVertexSize.push_back(u16Offset);
		m_u32VertexSize += u16Offset;
	}

	pVertexElements[u8ElementCount] = D3DDECL_END();

	HRESULT hResult = g_pD3d9Device->CreateVertexDeclaration(pVertexElements, &m_pD3dVertexDeclaration);
	if (FAILED(hResult))
	{
		RwgeLog(TEXT("Create vertex declaration failed : %X"), hResult);
	}

	delete[] pVertexElements;
}

RD3d9VertexDeclaration::~RD3d9VertexDeclaration()
{
	if (m_pD3dVertexDeclaration)
	{
		m_pD3dVertexDeclaration->Release();
	}
}