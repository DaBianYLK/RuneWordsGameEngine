#include "RwgeShaderCompilerEnv.h"

using namespace std;

string	RShaderCompilerEnvironment::m_strFxcPath				= "shaders\\fxc.exe";
string	RShaderCompilerEnvironment::m_strTargetVersion			= "fxc_2_0";						// fxc_2_0 = Shader Model 3.0
string	RShaderCompilerEnvironment::m_strDebugInfoPrefix		= "shaders\\log\\CompilerInfo_";
string	RShaderCompilerEnvironment::m_strDebugInfoExtension		= ".log";
string	RShaderCompilerEnvironment::m_strShaderBinaryPrefix		= "shaders\\bin\\Shader_";
string	RShaderCompilerEnvironment::m_strShaderBinaryExtension	= ".bin";
string	RShaderCompilerEnvironment::m_strShaderSourcePath		= "shaders\\src\\BaseForwardingShading.hlsl";
bool	RShaderCompilerEnvironment::m_bEnableDebugInfo			= true;
bool	RShaderCompilerEnvironment::m_bEnableDx9Compiler		= true;

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

string RShaderCompilerEnvironment::GetShaderBinaryPath(const RShaderKey& key)
{
	char arrayBuffer[128];

	arrayBuffer[0] = '\0';
	strcat_s(arrayBuffer, m_strShaderBinaryPrefix.c_str());
	strcat_s(arrayBuffer, key.ToHexString());
	strcat_s(arrayBuffer, m_strShaderBinaryExtension.c_str());

	return string(arrayBuffer);
}

string RShaderCompilerEnvironment::GetShaderDebugInfoPath(const RShaderKey& key)
{
	char arrayBuffer[128];

	arrayBuffer[0] = '\0';
	strcat_s(arrayBuffer, m_strDebugInfoPrefix.c_str());
	strcat_s(arrayBuffer, key.ToHexString());
	strcat_s(arrayBuffer, m_strDebugInfoExtension.c_str());

	return string(arrayBuffer);
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
	SetDefine("MATERIAL_FULLY_ROUGH",		key.GetFullyRoughKey());
	SetDefine("LIGHT_TYPE",					key.GetLightTypeKey());
}

void RShaderCompilerEnvironment::SetDefine(const char* pDefine, const char* pValue)
{
	m_mapDefinitions[pDefine] = pValue;
}

void RShaderCompilerEnvironment::SetDefine(const char* pDefine, unsigned u32Value)
{
	char arrayBuffer[16];
	sprintf_s(arrayBuffer, "%u", u32Value);
	m_mapDefinitions[pDefine] = arrayBuffer;
}

void RShaderCompilerEnvironment::SetDefine(const char* pDefine, float f32Value)
{
	char arrayBuffer[16];
	sprintf_s(arrayBuffer, "%f", f32Value);
	m_mapDefinitions[pDefine] = arrayBuffer;
}

void RShaderCompilerEnvironment::SetDefine(const char* pDefine, unsigned char u8Value)
{
	SetDefine(pDefine, static_cast<unsigned int>(u8Value));
}

void RShaderCompilerEnvironment::SetDefine(const char* pDefine, bool bValue)
{
	SetDefine(pDefine, static_cast<unsigned int>(bValue));
}

string RShaderCompilerEnvironment::GetCompilerCmdLine()
{
	char arrayCommandBuffer[128];
	char arrayCmdLineBuffer[1024];

	// FXC可执行文件路径
	sprintf_s(arrayCmdLineBuffer, "\"%s\"", m_strFxcPath.c_str());

	// 编译着色器的版本
	sprintf_s(arrayCommandBuffer, " /T %s ", m_strTargetVersion.c_str());
	strcat_s(arrayCmdLineBuffer, arrayCommandBuffer);

	// 输出着色器二进制文件路径
	sprintf_s(arrayCommandBuffer, " /Fo %s%s%s ", m_strShaderBinaryPrefix.c_str(), m_ShaderKey.ToHexString(), m_strShaderBinaryExtension.c_str());
	strcat_s(arrayCmdLineBuffer, arrayCommandBuffer);

	if (m_bEnableDebugInfo)
	{
		strcat_s(arrayCmdLineBuffer, " /Zi ");	// 开启调试信息	
	}
	
	if (m_bEnableDx9Compiler)
	{
		strcat_s(arrayCmdLineBuffer, " /LD ");	// 使用遗留编译器（DX9编译器）编译
	}
	
	for (pair<const string, string>& pairDefinition : m_mapDefinitions)
	{
		strcat_s(arrayCmdLineBuffer, GetDefinitionText(pairDefinition));
	}

	// HLSL文件路径
	sprintf_s(arrayCommandBuffer, "\"%s\"", m_strShaderSourcePath.c_str());
	strcat_s(arrayCmdLineBuffer, arrayCommandBuffer);

	return string(arrayCmdLineBuffer);
}

const char* RShaderCompilerEnvironment::GetDefinitionText(const std::pair<std::string, std::string>& pairDefinition)
{
	static char arrayBuffer[64];

	sprintf_s(arrayBuffer, " /D %s=%s ", pairDefinition.first.c_str(), pairDefinition.second.c_str());

	return arrayBuffer;
}
