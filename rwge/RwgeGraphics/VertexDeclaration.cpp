#include "VertexDeclaration.h"

#include "D3D9Device.h"
#include <d3dx9.h>

VertexDeclaration::VertexDeclaration() : 
	m_pDevice(nullptr), 
	m_uStreamCount(0), 
	m_pDeclarations(nullptr)
{

}


VertexDeclaration::~VertexDeclaration()
{
}

bool VertexDeclaration::AddVertexElement(const VertexElement& element, unsigned short uStreamID /* = 0 */)
{
	while (uStreamID >= m_tableVertexDeclaration.size())
	{
		m_tableVertexDeclaration.push_back(VertexElementList());
	}

	m_tableVertexDeclaration[uStreamID].push_back(element);

	m_uStreamCount = 0;	// 将m_uStreamCount置为0，说明m_pStreamDeclarations需要更新

	return true;
}

unsigned short VertexDeclaration::Enable(const D3D9Device& device) const
{
	if (m_uStreamCount == 0 || m_pDevice != device.GetDevicePtr())
	{
		UpdateD3DVertexDeclaration();
		m_pDevice = device.GetDevicePtr();
	}

	m_pDevice->SetVertexDeclaration(m_pDeclarations);

	return m_uStreamCount;
}

unsigned short VertexDeclaration::GetVertexStreamCount() const
{
	if (m_uStreamCount == 0)
	{
		UpdateD3DVertexDeclaration();
	}

	return m_uStreamCount;
}

unsigned short VertexDeclaration::GetStreamVertexSize(unsigned short uStreamID /* = 0 */) const
{
	if (m_uStreamCount == 0)
	{
		UpdateD3DVertexDeclaration();
	}

	return m_vecStreamVertexSize[uStreamID];
}

unsigned short VertexDeclaration::GetVertexSize() const
{
	return m_uVertexSize;
}

void VertexDeclaration::UpdateD3DVertexDeclaration() const
{
	m_uStreamCount = m_tableVertexDeclaration.size();

	unsigned int uElementCount = 0;
	for (unsigned int i = 0; i < m_tableVertexDeclaration.size(); ++i)
	{
		uElementCount += m_tableVertexDeclaration[i].size();
	}

	D3DVERTEXELEMENT9* pVertexElements = new D3DVERTEXELEMENT9[uElementCount + 1];
	m_vecStreamVertexSize.clear();
	m_uVertexSize = 0;

	unsigned int uElementIndex = 0;
	for (unsigned short uStreamID = 0; uStreamID < m_tableVertexDeclaration.size(); ++uStreamID)
	{
		const VertexElementList& streamElements = m_tableVertexDeclaration[uStreamID];
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

	delete []pVertexElements;
}
