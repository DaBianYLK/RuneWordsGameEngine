#include "ShaderManager.h"

#include <fstream>
#include <TimeUtil.h>
#include <AssertUtil.h>

using namespace std;

string ShaderManager::m_strFXCPath					= "fxc.exe";
string ShaderManager::m_strTargetVersion			= "fx_2_0";
string ShaderManager::m_strShaderBinaryPrefix		= "Shader";
string ShaderManager::m_strShaderBinaryExtension	= ".bin";
string ShaderManager::m_strHLSLPath					= "BaseForwardShading.hlsl";
string ShaderManager::m_strCompilationLogPrefix		= "ShaderCompilationOutput";

ShaderManager::ShaderManager(D3D9Device* pDevice) : 
	m_pDevice(pDevice),
	m_pEffectPool(nullptr)
{
	HRESULT result = D3DXCreateEffectPool(&m_pEffectPool);

	ASSERT(result == D3D_OK);
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
	bool bMaterialNonmetal   = pMaterial->m_Metallic.pExpression  == nullptr && pMaterial->m_Metallic.constant  == 0.0f;
	bool bMaterialFullyRough = pMaterial->m_Roughness.pExpression == nullptr && pMaterial->m_Roughness.constant == 1.0f;

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
	unsigned long long u64Key = Uint64MaskAndLeftShit(pLight->GetLightType(), MaskBit2, 40);

	return u64Key;
}

unsigned long long ShaderManager::GetShaderProgramKey(const Material* pMaterial, const Light* pLight)
{
	unsigned long long u64MaterialKey = pMaterial->GetMaterialKey();		// ��������л�����MaterialKey�����Ա����ظ�����
	unsigned long long u64EnvironmentKey = GetEnvironmentKey(pLight);		// ���ڼ���ϼ򵥣���ʱ������EnvironmentKey

	return u64MaterialKey | (u64EnvironmentKey << MaterialKeyBitCount);
}

unsigned long long ShaderManager::GetShaderProgramKey(unsigned long long u64MaterialKey, unsigned long long u64EnvironmentKey)
{
	return u64MaterialKey | (u64EnvironmentKey << MaterialKeyBitCount);
}

const string& ShaderManager::GetFXCCommandLine(const Material* pMaterial, const Light* pLight)
{
	unsigned long long u64Key = GetShaderProgramKey(pMaterial, pLight);

	return GetFXCCommandLine(u64Key);
}

const std::string& ShaderManager::GetFXCCommandLine(unsigned long long u64Key)
{
	const unsigned int uBufferSize = 256;
	char cBuffer[uBufferSize];

	// FXC��ִ���ļ�·��
	string strCommand = m_strFXCPath;

	// ������ɫ���İ汾
	sprintf_s(cBuffer, " /T %s ", m_strTargetVersion.c_str());
	strCommand.append(cBuffer);
	//strCommand.append(" /T ");
	//strCommand.append(m_strTargetVersion.c_str());

	// �����ɫ���������ļ�·��
	sprintf_s(cBuffer, " /Fo %s%ulld%s ", m_strShaderBinaryPrefix.c_str(), u64Key, m_strShaderBinaryExtension.c_str());
	strCommand.append(cBuffer);
	//_ui64toa_s(uKey, cBuffer, uBufferSize, 16);		// ʹ��16���Ʊ���
	//strCommand.append(" /Fo ");
	//strCommand.append(m_strOutputPrefix.c_str());
	//strCommand.append(cBuffer);
	//strCommand.append(m_strOutputExtension.c_str());

	// ����������Ϣ
	strCommand.append(" /Zi ");

	// ʹ��������������DX9������������
	strCommand.append(" /LD ");

	// ������ɫ������
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

	// HLSL�ļ�·��
	strCommand.append(m_strHLSLPath.c_str());

	return strCommand;
}

bool ShaderManager::CompileShader(const Material* pMaterial, const Light* pLight)
{
	unsigned long long u64Key = GetShaderProgramKey(pMaterial, pLight);
	
	return CompileShader(u64Key);
}

bool ShaderManager::CompileShader(unsigned long long u64Key)
{
	string strCommandLine = GetFXCCommandLine(u64Key);

	// ������ɫ��������FXC���ӽ���
	STARTUPINFO			infoStartup;
	PROCESS_INFORMATION infoProcess;
	ZeroMemory(&infoStartup, sizeof(infoStartup));
	infoStartup.cb = sizeof(infoStartup);
	ZeroMemory(&infoStartup, sizeof(infoStartup));

	BOOL result = !CreateProcess(nullptr,									// ��ִ���ļ�·�������������������У��˴���Ϊ�գ�
								 const_cast<char*>(strCommandLine.c_str()),	// �����в���
								 nullptr,									// ���̳е�ǰ���̾��
								 nullptr,									// ���̳е�ǰ�߳̾��
								 FALSE,										// ���þ���̳�
								 0,											// ��ʹ�ô�����־��Flag��
								 nullptr,									// ʹ�ø����̵Ļ�����
								 nullptr,									// ʹ�ø����̵ĳ���·��
								 &infoStartup,								// ��ȡSTARTUPINFO
								 &infoProcess);								// ��ȡPROCESS_INFORMATION

	if (result == FALSE)
	{
		ErrorBox("Compile shader failed! Unable to create compilation process.");
		return false;
	}

	// ͨ�������ܵ���NamedPipe����ȡ����Log����������ǰ�̣߳�
	HANDLE waitHandles[] = { infoProcess.hProcess, infoStartup.hStdOutput, infoStartup.hStdError };

	const DWORD BUFSIZE = 4096;
	char buffer[BUFSIZE];

	sprintf_s(buffer, "%s_%ulld_%s.log", m_strCompilationLogPrefix.c_str(), u64Key, GetCurrentDateTime().c_str());
	ofstream streamComplitationInfo(buffer, ios::out);

	while (1)
	{
		DWORD dwBytesRead, dwBytesAvailable;

		DWORD dwWaitResult = WaitForMultipleObjects(3, waitHandles, FALSE, 60000L);

		streamComplitationInfo << "Compilation Information:" << endl;
		while (PeekNamedPipe(infoStartup.hStdOutput, nullptr, 0, nullptr, &dwBytesAvailable, nullptr) && dwBytesAvailable)
		{
			ReadFile(infoStartup.hStdOutput, buffer, BUFSIZE - 1, &dwBytesRead, nullptr);
			streamComplitationInfo << string(buffer, static_cast<size_t>(dwBytesRead));
		}
		streamComplitationInfo << endl;

		streamComplitationInfo << "Compilation Error:" << endl;
		while (PeekNamedPipe(infoStartup.hStdError, nullptr, 0, nullptr, &dwBytesAvailable, nullptr) && dwBytesAvailable)
		{
			ReadFile(infoStartup.hStdError, buffer, BUFSIZE - 1, &dwBytesRead, nullptr);
			streamComplitationInfo << string(buffer, static_cast<size_t>(dwBytesRead));
		}
		streamComplitationInfo << endl;

		// Process is done, or we timed out:
		if (dwWaitResult == WAIT_OBJECT_0 || dwWaitResult == WAIT_TIMEOUT)
		{
			break;
		}
	}

	return true;
}

ShaderProgram* ShaderManager::LoadShader(unsigned long long u64Key)
{
	const unsigned int uBufferSize = 256;
	char cBuffer[uBufferSize];
	sprintf_s(cBuffer, "%s%ulld%s", m_strShaderBinaryPrefix.c_str(), u64Key, m_strShaderBinaryExtension.c_str());

	ShaderProgram* pShader = new ShaderProgram(cBuffer);
	pShader->Load(m_pDevice, m_pEffectPool);

	return pShader;
}

ShaderProgram* ShaderManager::GetShaderProgram(unsigned long long u64Key)
{
	hash_map<unsigned long long, ShaderProgram*>::iterator it = m_hashShaders.find(u64Key);

	// ����ҵ��˾ͷ�����ɫ��ָ��
	if (it != m_hashShaders.end())
	{
		return it->second;
	}

	// ���û���ҵ��ͱ���һ��
	CompileShader(u64Key);

	// ����������
	ShaderProgram* pShader = LoadShader(u64Key);

	// �����غõ���ɫ�������ϣ��
	m_hashShaders.insert(make_pair(u64Key, pShader));

	return pShader;
}

ShaderProgram* ShaderManager::GetFirstShaderProgram()
{
	if (m_hashShaders.empty())
	{
		return nullptr;
	}

	return m_hashShaders.begin()->second;
}
