#include <d3dx9.h>
#include <RwgeCoreDef.h>

namespace RwgeD3dx9Extension
{
	static const D3DCOLOR D3dColorBlack = D3DCOLOR_ARGB(255,   0,   0,   0);
	static const D3DCOLOR D3DColorGray	= D3DCOLOR_ARGB(255, 128, 128, 128);
	static const D3DCOLOR D3DColorWhite = D3DCOLOR_ARGB(255, 255, 255, 255);
	static const D3DCOLOR D3DColorRed	= D3DCOLOR_ARGB(255, 255,   0,   0);
	static const D3DCOLOR D3DColorGreen = D3DCOLOR_ARGB(255,   0, 255,   0);
	static const D3DCOLOR D3DColorBlue	= D3DCOLOR_ARGB(255,   0,   0, 255);


	const char* D3dFormatToString(D3DFORMAT format);
	const char* D3dErrorCodeToString(HRESULT hResult);

}