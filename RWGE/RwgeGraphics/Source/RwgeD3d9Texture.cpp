#include "RwgeD3d9Texture.h"

#include <d3dx9.h>
#include "RwgeD3d9Device.h"
#include "RwgeGraphics.h"
#include <RwgeAssert.h>

RD3d9Texture::RD3d9Texture() : m_pD3DTexture(nullptr)
{

}

RD3d9Texture::~RD3d9Texture()
{
	RwgeSafeRelease(m_pD3DTexture);
}

bool RD3d9Texture::Load(const TCHAR* szPath)
{
	m_strFilePath = szPath;

	HRESULT hResult = D3DXCreateTextureFromFile(g_pD3d9Device, szPath, &m_pD3DTexture);

	if (FAILED(hResult))
	{
		RwgeErrorBox(TEXT("Create texture failed : %X, Texture path : %s"), hResult, szPath);
		return false;
	}

	return true;
}