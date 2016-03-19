#include "VertexDeclaration.h"

#include "D3D9Device.h"
#include "VertexDeclarationType.h"
#include <AssertUtil.h>

VertexDeclaration::VertexDeclaration(VertexDeclarationType* pVertexDeclaration) :
	m_pVertexDeclarationType(pVertexDeclaration), 
	m_pD3DVertexDeclaration(nullptr)
{
}

VertexDeclaration::VertexDeclaration(VertexDeclaration&& declaration) :
	m_pVertexDeclarationType(declaration.m_pVertexDeclarationType),
	m_pD3DVertexDeclaration(declaration.m_pD3DVertexDeclaration)
{
	declaration.m_pVertexDeclarationType = nullptr;
	declaration.m_pD3DVertexDeclaration = nullptr;
}

VertexDeclaration::~VertexDeclaration()
{
}

bool VertexDeclaration::Load(const D3D9Device* pDevice)
{
	unsigned int uElementCount = m_pVertexDeclarationType->GetElementCount();
	unsigned int uStreamCount = m_pVertexDeclarationType->GetStreamCount();

	D3DVERTEXELEMENT9* pVertexElements = new D3DVERTEXELEMENT9[uElementCount + 1];

	unsigned int uElementIndex = 0;
	for (unsigned short uStreamID = 0; uStreamID < uStreamCount; ++uStreamID)
	{
		const VertexElementList& streamElements = m_pVertexDeclarationType->m_VertexStreamElements[uStreamID];
		unsigned short uOffset = 0;

		VertexElementList::const_iterator it = streamElements.begin();
		while (it != streamElements.end())
		{
			pVertexElements[uElementIndex] = { uStreamID, uOffset, it->uType, it->uMethod, it->uUsage, it->uUsageIndex };

			uOffset += it->uSize;
			++it;
			++uElementIndex;
		}
	}

	pVertexElements[uElementCount] = D3DDECL_END();

	HRESULT hResult = pDevice->GetDevicePtr()->CreateVertexDeclaration(pVertexElements, &m_pD3DVertexDeclaration);

	if (FAILED(hResult))
	{
		ErrorBox("Create vertex declaration failed : %X", hResult);
	}

	delete[]pVertexElements;
	

	return true;
}