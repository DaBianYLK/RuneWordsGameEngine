#include "RwgeTexture.h"

#include <d3dx9.h>
#include "RwgeD3d9Device.h"
#include "RwgeTextureInfo.h"
#include <RwgeAssert.h>

Texture::Texture(TextureInfo* pInfo) :
	m_pInfo(pInfo), 
	m_pD3DTexture(nullptr)
{

}

Texture::~Texture()
{

}

void Texture::Load(const RD3d9Device& device)
{
	HRESULT hResult = D3DXCreateTextureFromFile(device.GetD3dDevice(), m_pInfo->GetTextureFilePath().c_str(), &m_pD3DTexture);

	if (FAILED(hResult))
	{
		RwgeErrorBox("Create texture failed : %X", hResult);
	}
}
