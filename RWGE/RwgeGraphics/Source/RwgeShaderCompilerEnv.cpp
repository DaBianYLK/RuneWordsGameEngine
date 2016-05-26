#include "RwgeShaderCompilerEnv.h"

#include "RwgeMaterialExpressionID.h"
#include <RwgeLog.h>

using namespace std;
using namespace Rwge;

const TCHAR*	RShaderCompilerEnvironment::m_szFxcPath					= TEXT("shaders\\fxc.exe");
const TCHAR*	RShaderCompilerEnvironment::m_szTargetVersion			= TEXT("fx_2_0");						// fx_2_0 = Shader Model 3.0
const TCHAR*	RShaderCompilerEnvironment::m_szDebugInfoPrefix			= TEXT("shaders\\log\\CompilerInfo_");
const TCHAR*	RShaderCompilerEnvironment::m_szDebugInfoExtension		= TEXT(".log");
const TCHAR*	RShaderCompilerEnvironment::m_szShaderBinaryPrefix		= TEXT("shaders\\bin\\Shader_");
const TCHAR*	RShaderCompilerEnvironment::m_szShaderBinaryExtension	= TEXT(".bin");
const TCHAR*	RShaderCompilerEnvironment::m_szShaderAssemblyPrefix	= TEXT("shaders\\asm\\Shader_");;
const TCHAR*	RShaderCompilerEnvironment::m_szShaderAssemblyExtension = TEXT(".asm");
const TCHAR*	RShaderCompilerEnvironment::m_szShaderSourcePath		= TEXT("shaders\\src\\BaseForwardShading.hlsl");
bool			RShaderCompilerEnvironment::m_bEnableDebugInfo			= true;
bool			RShaderCompilerEnvironment::m_bEnableDx9Compiler		= true;
TCHAR			RShaderCompilerEnvironment::m_szBuffer128[128];
TCHAR			RShaderCompilerEnvironment::m_szBuffer1024[1024];

#ifdef _DEBUG
bool			RShaderCompilerEnvironment::m_bOutputAssemblyFile		= true;
#else
bool			RShaderCompilerEnvironment::m_bOutputAssemblyFile		= false;
#endif


RShaderCompilerEnvironment::RShaderCompilerEnvironment()
{
	
}

RShaderCompilerEnvironment::~RShaderCompilerEnvironment()
{

}

void RShaderCompilerEnvironment::EnableDebugInfo(bool bEnable)
{
	m_bEnableDebugInfo = bEnable;
}

void RShaderCompilerEnvironment::EnableDx9Compiler(bool bEnable)
{
	m_bEnableDx9Compiler = bEnable;
}

const TCHAR* RShaderCompilerEnvironment::GetShaderBinaryPath(const RShaderKey& inKey)
{
	_tcscpy_s(m_szBuffer128, m_szShaderBinaryPrefix);
	_tcscat_s(m_szBuffer128, inKey.ToHexString());
	_tcscat_s(m_szBuffer128, m_szShaderBinaryExtension);

	return m_szBuffer128;
}

const TCHAR* RShaderCompilerEnvironment::GetShaderDebugInfoPath(const RShaderKey& inKey)
{
	_tcscpy_s(m_szBuffer128, m_szDebugInfoPrefix);
	_tcscat_s(m_szBuffer128, inKey.ToHexString());
	_tcscat_s(m_szBuffer128, m_szDebugInfoExtension);

	return m_szBuffer128;
}

void RShaderCompilerEnvironment::SetShaderKey(const RShaderKey& key)
{
	m_ShaderKey = key;

	SetDefine("BASE_COLOR_EXPRESSION",		key.GetBaseColorKey());
	SetDefine("EMISSIVE_COLOR_EXPRESSION",	key.GetEmissiveColorKey());
	SetDefine("NORMAL_EXPRESSION",			key.GetNormalKey());
	SetDefine("METALLIC_EXPRESSION",		key.GetMetallicKey());
	SetDefine("SPECULAR_EXPRESSION",		key.GetSpecularKey());
	SetDefine("ROUGHNESS_EXPRESSION",		key.GetRoughnessKey());
	SetDefine("OPACITY_EXPRESSION",			key.GetOpacityKey());
	SetDefine("OPACITY_MASK_EXPRESSION",	key.GetOpacityMaskKey());
	SetDefine("MATERIAL_BLEND_MODE",		key.GetBlendModeKey());
	SetDefine("MATERIAL_SHADING_MODE",		key.GetShadingModeKey());
	SetDefine("MATERIAL_TWO_SIDED",			key.GetTwoSidedKey());
	SetDefine("MATERIAL_NONMETAL",			key.GetNonMetalKey());
	SetDefine("TEXTURE_COUNT",				key.GetTextureCountKey());
	SetDefine("MATERIAL_FULLY_ROUGH",		key.GetFullyRoughKey());
	SetDefine("LIGHT_TYPE",					key.GetLightTypeKey());

	const RTexturesToTextureUnitsMap* pTextureMap = RShaderKey::GetTexturesToTextureUnitsMap(key.GetTextureMapHashKey());
	if (pTextureMap != nullptr)
	{
		const RTexturesToTextureUnitsMap& texturesToTextureUnitsMap = *pTextureMap;
		unsigned short u16TextureIndex = 0;

		// BaseColor ==================================================
		if (key.GetBaseColorKey() == EME_2dTextureSampleRGB)
		{
			SetDefine("BASE_COLOR_TEXTURE_ID", static_cast<unsigned int>(texturesToTextureUnitsMap[u16TextureIndex]));
			++u16TextureIndex;
		}

		// EmissiveColor ==================================================
		if (key.GetEmissiveColorKey() == EME_2dTextureSampleRGB)
		{
			SetDefine("EMISSIVE_COLOR_TEXTURE_ID", static_cast<unsigned int>(texturesToTextureUnitsMap[u16TextureIndex]));
			++u16TextureIndex;
		}

		// Normal ==================================================
		if (key.GetNormalKey() == EME_2dTextureSampleRGB)
		{
			SetDefine("NORMAL_TEXTURE_ID", static_cast<unsigned int>(texturesToTextureUnitsMap[u16TextureIndex]));
			++u16TextureIndex;
		}

		// Metallic ==================================================
		if (key.GetMetallicKey() >= EME_2dTextureSampleR && key.GetMetallicKey() <= EME_2dTextureSampleA)
		{
			SetDefine("METALLIC_TEXTURE_ID", static_cast<unsigned int>(texturesToTextureUnitsMap[u16TextureIndex]));
			++u16TextureIndex;
		}

		// Specular ==================================================
		if (key.GetSpecularKey() >= EME_2dTextureSampleR && key.GetSpecularKey() <= EME_2dTextureSampleA)
		{
			SetDefine("SPECULAR_TEXTURE_ID", static_cast<unsigned int>(texturesToTextureUnitsMap[u16TextureIndex]));
			++u16TextureIndex;
		}

		// Roughness ==================================================
		if (key.GetRoughnessKey() >= EME_2dTextureSampleR && key.GetRoughnessKey() <= EME_2dTextureSampleA)
		{
			SetDefine("ROUGHNESS_TEXTURE_ID", static_cast<unsigned int>(texturesToTextureUnitsMap[u16TextureIndex]));
			++u16TextureIndex;
		}

		// Opacity ==================================================
		if (key.GetOpacityKey() >= EME_2dTextureSampleR && key.GetOpacityKey() <= EME_2dTextureSampleA)
		{
			SetDefine("OPACITY_TEXTURE_ID", static_cast<unsigned int>(texturesToTextureUnitsMap[u16TextureIndex]));
			++u16TextureIndex;
		}

		// OpacityMask ==================================================
		if (key.GetOpacityMaskKey() >= EME_2dTextureSampleR && key.GetOpacityMaskKey() <= EME_2dTextureSampleA)
		{
			SetDefine("OPACITY_MASK_TEXTURE_ID", static_cast<unsigned int>(texturesToTextureUnitsMap[u16TextureIndex]));
			RwgeAssert(++u16TextureIndex == texturesToTextureUnitsMap.GetTextureCount());
		}
	}
}

void RShaderCompilerEnvironment::SetDefine(const TCHAR* pDefine, const TCHAR* pValue)
{
	m_mapDefinitions[pDefine] = pValue;
}

void RShaderCompilerEnvironment::SetDefine(const TCHAR* pDefine, unsigned u32Value)
{
	_stprintf_s(m_szBuffer128, "%u", u32Value);
	m_mapDefinitions[pDefine] = m_szBuffer128;
}

void RShaderCompilerEnvironment::SetDefine(const TCHAR* pDefine, float f32Value)
{
	_stprintf_s(m_szBuffer128, "%f", f32Value);
	m_mapDefinitions[pDefine] = m_szBuffer128;
}

void RShaderCompilerEnvironment::SetDefine(const TCHAR* pDefine, unsigned char u8Value)
{
	SetDefine(pDefine, static_cast<unsigned int>(u8Value));
}

void RShaderCompilerEnvironment::SetDefine(const TCHAR* pDefine, bool bValue)
{
	SetDefine(pDefine, static_cast<unsigned int>(bValue));
}

const TCHAR* RShaderCompilerEnvironment::GetCompilerCmdLine()
{
	// FXC可执行文件路径
	_stprintf_s(m_szBuffer1024, TEXT("\"%s\""), m_szFxcPath);

	// 编译着色器的版本
	_stprintf_s(m_szBuffer128, TEXT(" /T %s "), m_szTargetVersion);
	_tcscat_s(m_szBuffer1024, m_szBuffer128);

	// 输出着色器二进制文件路径
	_stprintf_s(m_szBuffer128, TEXT(" /Fo %s%s%s "), m_szShaderBinaryPrefix, m_ShaderKey.ToHexString(), m_szShaderBinaryExtension);
	_tcscat_s(m_szBuffer1024, m_szBuffer128);

	// 输出着色器汇编文件路径
	if (m_bOutputAssemblyFile)
	{
		_stprintf_s(m_szBuffer128, TEXT(" /Fh %s%s%s "), m_szShaderAssemblyPrefix, m_ShaderKey.ToHexString(), m_szShaderAssemblyExtension);
		_tcscat_s(m_szBuffer1024, m_szBuffer128);
	}

	if (m_bEnableDebugInfo)
	{
		_tcscat_s(m_szBuffer1024, TEXT(" /Zi "));	// 开启调试信息	
	}
	
	if (m_bEnableDx9Compiler)
	{
		_tcscat_s(m_szBuffer1024, TEXT(" /LD "));	// 使用遗留编译器（DX9编译器）编译
	}
	
	for (pair<const tstring, tstring>& pairDefinition : m_mapDefinitions)
	{
		_tcscat_s(m_szBuffer1024, GetDefinitionText(pairDefinition));
	}

	// HLSL文件路径
	_stprintf_s(m_szBuffer128, TEXT("\"%s\""), m_szShaderSourcePath);
	_tcscat_s(m_szBuffer1024, m_szBuffer128);

	return m_szBuffer1024;
}

const TCHAR* RShaderCompilerEnvironment::GetDefinitionText(const std::pair<tstring, tstring>& pairDefinition)
{
	_stprintf_s(m_szBuffer128, TEXT(" /D %s=%s "), pairDefinition.first.c_str(), pairDefinition.second.c_str());

	return m_szBuffer128;
}
