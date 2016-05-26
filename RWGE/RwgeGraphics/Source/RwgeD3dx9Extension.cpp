#include "RwgeD3dx9Extension.h"

const char* RwgeD3dx9Extension::D3dFormatToString(D3DFORMAT format)
{
	RwgeSymbolToStringBegin(format)
	{
		RwgeSymbolToString(D3DFMT_UNKNOWN);
		RwgeSymbolToString(D3DFMT_R8G8B8);
		RwgeSymbolToString(D3DFMT_A8R8G8B8);
		RwgeSymbolToString(D3DFMT_X8R8G8B8);
		RwgeSymbolToString(D3DFMT_R5G6B5);
		RwgeSymbolToString(D3DFMT_X1R5G5B5);
		RwgeSymbolToString(D3DFMT_A1R5G5B5);
		RwgeSymbolToString(D3DFMT_A4R4G4B4);
		RwgeSymbolToString(D3DFMT_R3G3B2);
		RwgeSymbolToString(D3DFMT_A8);
		RwgeSymbolToString(D3DFMT_A8R3G3B2);
		RwgeSymbolToString(D3DFMT_X4R4G4B4);
		RwgeSymbolToString(D3DFMT_A2B10G10R10);
		RwgeSymbolToString(D3DFMT_A8B8G8R8);
		RwgeSymbolToString(D3DFMT_X8B8G8R8);
		RwgeSymbolToString(D3DFMT_G16R16);
		RwgeSymbolToString(D3DFMT_A2R10G10B10);
		RwgeSymbolToString(D3DFMT_A16B16G16R16);
		RwgeSymbolToString(D3DFMT_A8P8);
		RwgeSymbolToString(D3DFMT_P8);
		RwgeSymbolToString(D3DFMT_L8);
		RwgeSymbolToString(D3DFMT_A8L8);
		RwgeSymbolToString(D3DFMT_A4L4);
		RwgeSymbolToString(D3DFMT_V8U8);
		RwgeSymbolToString(D3DFMT_L6V5U5);
		RwgeSymbolToString(D3DFMT_X8L8V8U8);
		RwgeSymbolToString(D3DFMT_Q8W8V8U8);
		RwgeSymbolToString(D3DFMT_V16U16);
		RwgeSymbolToString(D3DFMT_A2W10V10U10);
		RwgeSymbolToString(D3DFMT_UYVY);
		RwgeSymbolToString(D3DFMT_R8G8_B8G8);
		RwgeSymbolToString(D3DFMT_YUY2);
		RwgeSymbolToString(D3DFMT_G8R8_G8B8);
		RwgeSymbolToString(D3DFMT_DXT1);
		RwgeSymbolToString(D3DFMT_DXT2);
		RwgeSymbolToString(D3DFMT_DXT3);
		RwgeSymbolToString(D3DFMT_DXT4);
		RwgeSymbolToString(D3DFMT_DXT5);
		RwgeSymbolToString(D3DFMT_D16_LOCKABLE);
		RwgeSymbolToString(D3DFMT_D32);
		RwgeSymbolToString(D3DFMT_D15S1);
		RwgeSymbolToString(D3DFMT_D24S8);
		RwgeSymbolToString(D3DFMT_D24X8);
		RwgeSymbolToString(D3DFMT_D24X4S4);
		RwgeSymbolToString(D3DFMT_D16);
		RwgeSymbolToString(D3DFMT_D32F_LOCKABLE);
		RwgeSymbolToString(D3DFMT_D24FS8);
		RwgeSymbolToString(D3DFMT_D32_LOCKABLE);
		RwgeSymbolToString(D3DFMT_S8_LOCKABLE);
		RwgeSymbolToString(D3DFMT_L16);
		RwgeSymbolToString(D3DFMT_VERTEXDATA);
		RwgeSymbolToString(D3DFMT_INDEX16);
		RwgeSymbolToString(D3DFMT_INDEX32);
		RwgeSymbolToString(D3DFMT_Q16W16V16U16);
		RwgeSymbolToString(D3DFMT_MULTI2_ARGB8);
		RwgeSymbolToString(D3DFMT_R16F);
		RwgeSymbolToString(D3DFMT_G16R16F);
		RwgeSymbolToString(D3DFMT_A16B16G16R16F);
		RwgeSymbolToString(D3DFMT_R32F);
		RwgeSymbolToString(D3DFMT_G32R32F);
		RwgeSymbolToString(D3DFMT_A32B32G32R32F);
		RwgeSymbolToString(D3DFMT_CxV8U8);
		RwgeSymbolToString(D3DFMT_A1);
		RwgeSymbolToString(D3DFMT_A2B10G10R10_XR_BIAS);
		RwgeSymbolToString(D3DFMT_BINARYBUFFER);

		RwgeSymbolToStringDefault();
	}
}

const char* RwgeD3dx9Extension::D3dErrorCodeToString(HRESULT hResult)
{
	RwgeSymbolToStringBegin(hResult)
	{
		RwgeSymbolToString(D3D_OK);

		RwgeSymbolToString(D3DERR_WRONGTEXTUREFORMAT);
		RwgeSymbolToString(D3DERR_UNSUPPORTEDCOLOROPERATION);
		RwgeSymbolToString(D3DERR_UNSUPPORTEDCOLORARG);
		RwgeSymbolToString(D3DERR_UNSUPPORTEDALPHAOPERATION);
		RwgeSymbolToString(D3DERR_UNSUPPORTEDALPHAARG);
		RwgeSymbolToString(D3DERR_TOOMANYOPERATIONS);
		RwgeSymbolToString(D3DERR_CONFLICTINGTEXTUREFILTER);
		RwgeSymbolToString(D3DERR_UNSUPPORTEDFACTORVALUE);
		RwgeSymbolToString(D3DERR_CONFLICTINGRENDERSTATE);
		RwgeSymbolToString(D3DERR_UNSUPPORTEDTEXTUREFILTER);
		RwgeSymbolToString(D3DERR_CONFLICTINGTEXTUREPALETTE);
		RwgeSymbolToString(D3DERR_DRIVERINTERNALERROR);
		RwgeSymbolToString(D3DERR_NOTFOUND);
		RwgeSymbolToString(D3DERR_DEVICELOST);
		RwgeSymbolToString(D3DERR_DEVICENOTRESET);
		RwgeSymbolToString(D3DERR_NOTAVAILABLE);
		RwgeSymbolToString(D3DERR_OUTOFVIDEOMEMORY);
		RwgeSymbolToString(D3DERR_INVALIDDEVICE);
		RwgeSymbolToString(D3DERR_INVALIDCALL);
		RwgeSymbolToString(D3DERR_DRIVERINVALIDCALL);
		RwgeSymbolToString(D3DERR_WASSTILLDRAWING);
		RwgeSymbolToString(D3DOK_NOAUTOGEN);
		RwgeSymbolToString(D3DERR_DEVICEREMOVED);
		RwgeSymbolToString(S_NOT_RESIDENT);
		RwgeSymbolToString(S_RESIDENT_IN_SHARED_MEMORY);
		RwgeSymbolToString(S_PRESENT_MODE_CHANGED);
		RwgeSymbolToString(S_PRESENT_OCCLUDED);
		RwgeSymbolToString(D3DERR_DEVICEHUNG);
		RwgeSymbolToString(D3DERR_UNSUPPORTEDOVERLAY);
		RwgeSymbolToString(D3DERR_UNSUPPORTEDOVERLAYFORMAT);
		RwgeSymbolToString(D3DERR_CANNOTPROTECTCONTENT);
		RwgeSymbolToString(D3DERR_UNSUPPORTEDCRYPTO);
		RwgeSymbolToString(D3DERR_PRESENT_STATISTICS_DISJOINT);

		RwgeSymbolToStringDefault();
	}
}