#include "RwgeShaderManager.h"

#include <fstream>
#include <RwgeTime.h>
#include <RwgeAssert.h>

using namespace std;

string ShaderManager::m_strFXCPath					= "shaders/fxc.exe";
string ShaderManager::m_strTargetVersion			= "fx_2_0";
string ShaderManager::m_strShaderBinaryPrefix		= "shaders/bin/Shader";
string ShaderManager::m_strShaderBinaryExtension	= ".bin";
string ShaderManager::m_strHLSLPath					= "shaders/src/BaseForwardShading.hlsl";
string ShaderManager::m_strCompilationLogPrefix		= "CompilationOutput";

ShaderManager::ShaderManager()
{

}

ShaderManager::~ShaderManager()
{

}

const unsigned long long MaskBit1 = 1;
const unsigned long long MaskBit2 = (MaskBit1 << 1) | 1;
const unsigned long long MaskBit3 = (MaskBit2 << 1) | 1;
const unsigned long long MaskBit4 = (MaskBit3 << 1) | 1;

const unsigned long long MaterialKeyBitCount = 40;

inline unsigned long long Uint64MaskAndLeftShit(unsigned long long number, unsigned long long mask, unsigned int shiftBitCount)
{
	return (number & mask) << shiftBitCount;
}

unsigned long long ShaderManager::GetMaterialKey(const Material* pMaterial)
{
	bool bMaterialNonmetal = pMaterial->m_Metallic.GetExpression() == nullptr && pMaterial->m_Metallic.GetConstant() == 0.0f;
	bool bMaterialFullyRough = pMaterial->m_Roughness.GetExpression() == nullptr && pMaterial->m_Roughness.GetConstant() == 1.0f;

	unsigned long long u64Key 
			= Uint64MaskAndLeftShit(pMaterial->m_BaseColor.GetExpressionID(),		MaskBit4,  0);
	u64Key |= Uint64MaskAndLeftShit(pMaterial->m_EmissiveColor.GetExpressionID(),	MaskBit4,  4);
	u64Key |= Uint64MaskAndLeftShit(pMaterial->m_Normal.GetExpressionID(),			MaskBit4,  8);
	u64Key |= Uint64MaskAndLeftShit(pMaterial->m_Metallic.GetExpressionID(),		MaskBit4, 12);
	u64Key |= Uint64MaskAndLeftShit(pMaterial->m_Specular.GetExpressionID(),		MaskBit4, 16);
	u64Key |= Uint64MaskAndLeftShit(pMaterial->m_Roughness.GetExpressionID(),		MaskBit4, 20);
	u64Key |= Uint64MaskAndLeftShit(pMaterial->m_Opacity.GetExpressionID(),			MaskBit4, 24);
	u64Key |= Uint64MaskAndLeftShit(pMaterial->m_OpacityMask.GetExpressionID(),		MaskBit4, 28);
	u64Key |= Uint64MaskAndLeftShit(pMaterial->m_BlendMode,							MaskBit3, 32);
	u64Key |= Uint64MaskAndLeftShit(pMaterial->m_ShadingMode,						MaskBit2, 35);
	u64Key |= Uint64MaskAndLeftShit(pMaterial->m_bTwoSided,							MaskBit1, 37);
	u64Key |= Uint64MaskAndLeftShit(bMaterialNonmetal,								MaskBit1, 38);
	u64Key |= Uint64MaskAndLeftShit(bMaterialFullyRough,							MaskBit1, 39);

	return u64Key;
}

unsigned long long ShaderManager::GetEnvironmentKey(const Light* pLight)
{
	unsigned long long u64Key = 0;

	// 无光照时，LightType = 0
	if (pLight != nullptr)
	{
		u64Key = Uint64MaskAndLeftShit(pLight->GetLightType(), MaskBit2, 0);
	}

	return u64Key;
}

unsigned long long ShaderManager::GetShaderKey(const Material* pMaterial, const Light* pLight)
{
	unsigned long long u64MaterialKey = pMaterial->GetMaterialKey();		// 如果材质中缓存了MaterialKey，可以避免重复计算
	unsigned long long u64EnvironmentKey = GetEnvironmentKey(pLight);		// 由于计算较简单，暂时不缓存EnvironmentKey

	return u64MaterialKey | (u64EnvironmentKey << MaterialKeyBitCount);
}

unsigned long long ShaderManager::GetShaderKey(unsigned long long u64MaterialKey, unsigned long long u64EnvironmentKey)
{
	return u64MaterialKey | (u64EnvironmentKey << MaterialKeyBitCount);
}

const string ShaderManager::GetShaderPath(unsigned long long u64Key)
{
	const unsigned int uBufferSize = 32;
	char cBuffer[uBufferSize];
	_ui64toa_s(u64Key, cBuffer, uBufferSize, 16);		// 使用16进制编码

	string strOutputPath = m_strShaderBinaryPrefix;
	strOutputPath.append(cBuffer);
	strOutputPath.append(m_strShaderBinaryExtension.c_str());

	return strOutputPath;
}

const string ShaderManager::GetFXCCommandLine(const Material* pMaterial, const Light* pLight)
{
	unsigned long long u64Key = GetShaderKey(pMaterial, pLight);

	return GetFXCCommandLine(u64Key);
}

const std::string ShaderManager::GetFXCCommandLine(unsigned long long u64Key)
{
	const unsigned int uBufferSize = 256;
	char cBuffer[uBufferSize];

	// FXC可执行文件路径
	sprintf_s(cBuffer, "\"%s\"", m_strFXCPath.c_str());
	string strCommand = cBuffer;

	// 编译着色器的版本
	sprintf_s(cBuffer, " /T %s ", m_strTargetVersion.c_str());
	strCommand.append(cBuffer);
	//strCommand.append(" /T ");
	//strCommand.append(m_strTargetVersion.c_str());

	// 输出着色器二进制文件路径
	sprintf_s(cBuffer, " /Fo %s ", GetShaderPath(u64Key).c_str());
	strCommand.append(cBuffer);

	// 开启调试信息
	strCommand.append(" /Zi ");

	// 使用遗留编译器（DX9编译器）编译
	strCommand.append(" /LD ");

	// 输入着色器参数
	sprintf_s(cBuffer, " /D BASE_COLOR_EXPRESSION=%u ",		static_cast<unsigned int>((u64Key >>  0) & MaskBit4));
	strCommand.append(cBuffer);
	sprintf_s(cBuffer, " /D EMISSIVE_COLOR_EXPRESSION=%u ", static_cast<unsigned int>((u64Key >>  4) & MaskBit4));
	strCommand.append(cBuffer);
	sprintf_s(cBuffer, " /D NORMAL_EXPRESSION=%u ",			static_cast<unsigned int>((u64Key >>  8) & MaskBit4));
	strCommand.append(cBuffer);
	sprintf_s(cBuffer, " /D METALLIC_EXPRESSION=%u ",		static_cast<unsigned int>((u64Key >> 12) & MaskBit4));
	strCommand.append(cBuffer);
	sprintf_s(cBuffer, " /D SPECULAR_EXPRESSION=%u ",		static_cast<unsigned int>((u64Key >> 16) & MaskBit4));
	strCommand.append(cBuffer);
	sprintf_s(cBuffer, " /D ROUGHNESS_EXPRESSION=%u ",		static_cast<unsigned int>((u64Key >> 20) & MaskBit4));
	strCommand.append(cBuffer);
	sprintf_s(cBuffer, " /D OPACITY_EXPRESSION=%u ",		static_cast<unsigned int>((u64Key >> 24) & MaskBit4));
	strCommand.append(cBuffer);
	sprintf_s(cBuffer, " /D OPACITY_MASK_EXPRESSION=%u ",	static_cast<unsigned int>((u64Key >> 28) & MaskBit4));
	strCommand.append(cBuffer);
	sprintf_s(cBuffer, " /D MATERIAL_BLEND_MODE=%u ",		static_cast<unsigned int>((u64Key >> 32) & MaskBit3));
	strCommand.append(cBuffer);
	sprintf_s(cBuffer, " /D MATERIAL_SHADING_MODE=%u ",		static_cast<unsigned int>((u64Key >> 35) & MaskBit2));
	strCommand.append(cBuffer);
	sprintf_s(cBuffer, " /D MATERIAL_TWO_SIDED=%u ",		static_cast<unsigned int>((u64Key >> 37) & MaskBit1));
	strCommand.append(cBuffer);
	sprintf_s(cBuffer, " /D MATERIAL_NONMETAL=%u ",			static_cast<unsigned int>((u64Key >> 38) & MaskBit1));
	strCommand.append(cBuffer);
	sprintf_s(cBuffer, " /D MATERIAL_FULLY_ROUGH=%u ",		static_cast<unsigned int>((u64Key >> 39) & MaskBit1));
	strCommand.append(cBuffer);
	sprintf_s(cBuffer, " /D LIGHT_TYPE=%u ",				static_cast<unsigned int>((u64Key >> 40) & MaskBit2));
	strCommand.append(cBuffer);

	// HLSL文件路径
	sprintf_s(cBuffer, "\"%s\"", m_strHLSLPath.c_str());
	strCommand.append(cBuffer);

	return strCommand;
}

bool ShaderManager::CompileShader(const Material* pMaterial, const Light* pLight)
{
	return CompileShader(GetShaderKey(pMaterial, pLight));
}

bool ShaderManager::CompileShader(unsigned long long u64Key)
{
	string strCommandLine = GetFXCCommandLine(u64Key);

	// ====================== 创建有名管道（NamedPipe），用于获取编译Log ======================
	SECURITY_ATTRIBUTES   securityAttributes;
	securityAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
	securityAttributes.bInheritHandle = TRUE;
	securityAttributes.lpSecurityDescriptor = nullptr;

	// 用于设置进程初始化参数的数据结构，其中包括绑定管道的参数
	STARTUPINFO infoStartup;
	ZeroMemory(&infoStartup, sizeof(STARTUPINFO));
	infoStartup.cb = sizeof(infoStartup);
	GetStartupInfo(&infoStartup);
	infoStartup.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	infoStartup.wShowWindow = SW_HIDE;

	HANDLE hReadInfoPipe;
	HANDLE hReadErrorPipe;
	BOOL bResult = CreatePipe(&hReadInfoPipe, &infoStartup.hStdOutput, &securityAttributes, 0);
	if (bResult == FALSE)
	{
		ErrorBox("Compile shader failed! Unable to create information pipe.");
		return false;
	}

	bResult = CreatePipe(&hReadErrorPipe, &infoStartup.hStdError, &securityAttributes, 0);
	if (bResult == FALSE)
	{
		ErrorBox("Compile shader failed! Unable to create error pipe.");
		return false;
	}

	// 创建着色器编译器FXC的子进程
	PROCESS_INFORMATION infoProcess;
	ZeroMemory(&infoProcess, sizeof(PROCESS_INFORMATION));

	bResult = CreateProcess(nullptr,									// 可执行文件路径（包含在了命令行中，此处设为空）
							const_cast<char*>(strCommandLine.c_str()),	// 命令行参数
							nullptr,									// 不继承当前进程句柄
							nullptr,									// 不继承当前线程句柄
							TRUE,										// 是否继承句柄
							0,											// 不使用创建标志（Flag）
							nullptr,									// 使用父进程的环境块
							nullptr,									// 使用父进程的程序路径
							&infoStartup,								// 获取STARTUPINFO
							&infoProcess);								// 获取PROCESS_INFORMATION

	CloseHandle(infoStartup.hStdOutput);
	CloseHandle(infoStartup.hStdError);

	if (bResult == FALSE)
	{
		ErrorBox("Compile shader failed! Unable to create compilation process.");
		return false;
	}

	HANDLE waitHandles[] = { infoProcess.hProcess, hReadInfoPipe, hReadErrorPipe };

	const DWORD BUFSIZE = 4096;
	char buffer[BUFSIZE];

	sprintf_s(buffer, "%s_%s_%s.log", GetShaderPath(u64Key).c_str(), m_strCompilationLogPrefix.c_str(), GetCurrentDateTime(TF_DigitalWithUnderline).c_str());
	ofstream streamComplitationInfo(buffer, ios::out);

	// 编译程序运行期间会阻塞当前线程
	while (true)
	{
		DWORD dwBytesRead, dwBytesAvailable;

		DWORD dwWaitResult = WaitForMultipleObjects(3, waitHandles, FALSE, 60000L);

		if (PeekNamedPipe(hReadInfoPipe, nullptr, 0, nullptr, &dwBytesAvailable, nullptr) && dwBytesAvailable)
		{
			streamComplitationInfo << "[INFO]===========================================================" << endl;

			while (ReadFile(hReadInfoPipe, buffer, BUFSIZE - 1, &dwBytesRead, nullptr))
			{
				streamComplitationInfo << string(buffer, static_cast<size_t>(dwBytesRead));
			}

			streamComplitationInfo << "=================================================================" << endl;
		}

		if (PeekNamedPipe(hReadErrorPipe, nullptr, 0, nullptr, &dwBytesAvailable, nullptr) && dwBytesAvailable)
		{
			streamComplitationInfo << "[ERROR]==========================================================" << endl;

			while (ReadFile(hReadErrorPipe, buffer, BUFSIZE - 1, &dwBytesRead, nullptr))
			{
				streamComplitationInfo << string(buffer, static_cast<size_t>(dwBytesRead));
			}

			streamComplitationInfo << "=================================================================" << endl;
		}

		// Process is done, or we timed out:
		if (dwWaitResult == WAIT_OBJECT_0 || dwWaitResult == WAIT_TIMEOUT)
		{
			break;
		}
	}

	CloseHandle(hReadInfoPipe);
	CloseHandle(hReadErrorPipe);

	return true;
}

ShaderType* ShaderManager::GetShaderType(unsigned long long u64Key)
{
	hash_map<unsigned long long, ShaderType>::iterator it = m_hashShaderTypes.find(u64Key);

	// 如果找到了就返回指针
	if (it != m_hashShaderTypes.end())
	{
		return &it->second;
	}

	// 如果没有找到就编译一个
	CompileShader(u64Key);

	// 将ShaderType加入hash表
	ShaderType shaderType(u64Key);
	m_hashShaderTypes.insert(make_pair(u64Key, shaderType));

	return &m_hashShaderTypes.find(u64Key)->second;
}