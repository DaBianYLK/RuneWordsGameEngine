#include "VertexDeclaration.h"

#include "D3D9Device.h"
#include <d3dx9.h>

VertexDeclaration::VertexDeclaration() : 
	m_pDevice(nullptr),
    m_uStreamCount(0),
	m_uVertexSize(0),
	m_pDeclarations(nullptr)
{

}

VertexDeclaration::~VertexDeclaration()
{
}

bool VertexDeclaration::AddVertexElement(const VertexElement& element, unsigned short uStreamID /* = 0 */)
{
	while (uStreamID >= m_VertexStreamElements.size())
	{
		m_VertexStreamElements.push_back(VertexElementList());
	}

	m_VertexStreamElements[uStreamID].push_back(element);

	m_uStreamCount = 0;	// 将m_uStreamCount置为0，说明m_pStreamDeclarations需要更新

	return true;
}

void VertexDeclaration::UpdateD3DDeclaration(const D3D9Device* pDevice)
{
	m_pDevice = pDevice->GetDevicePtr();

	m_uStreamCount = m_VertexStreamElements.size();

	unsigned int uElementCount = 0;
	for (unsigned int i = 0; i < m_VertexStreamElements.size(); ++i)
	{
		uElementCount += m_VertexStreamElements[i].size();
	}

	D3DVERTEXELEMENT9* pVertexElements = new D3DVERTEXELEMENT9[uElementCount + 1];
	m_vecStreamVertexSize.clear();
	m_uVertexSize = 0;

	unsigned int uElementIndex = 0;
	for (unsigned short uStreamID = 0; uStreamID < m_VertexStreamElements.size(); ++uStreamID)
	{
		const VertexElementList& streamElements = m_VertexStreamElements[uStreamID];
		unsigned short uOffset = 0;

		VertexElementList::const_iterator it = streamElements.begin();
		while (it != streamElements.end())
		{
			pVertexElements[uElementIndex] = { uStreamID, uOffset, it->uType, it->uMethod, it->uUsage, it->uUsageIndex };

			uOffset += it->uSize;
			++it;
			++uElementIndex;
		}

		m_vecStreamVertexSize.push_back(uOffset);
		m_uVertexSize += uOffset;
	}

	pVertexElements[uElementCount] = D3DDECL_END();

	m_pDevice->CreateVertexDeclaration(pVertexElements, &m_pDeclarations);

	delete[]pVertexElements;
}

void VertexDeclaration::Enable() const
{
	if (m_pDevice == nullptr || m_pDeclarations == nullptr)
	{
		return;
	}

	m_pDevice->SetVertexDeclaration(m_pDeclarations);
}

unsigned short VertexDeclaration::GetVertexStreamCount() const
{
	return m_uStreamCount;
}

unsigned short VertexDeclaration::GetStreamVertexSize(unsigned short uStreamID /* = 0 */) const
{
	return m_vecStreamVertexSize[uStreamID];
}

unsigned short VertexDeclaration::GetVertexSize() const
{
	return m_uVertexSize;
}
