#include "RwgeD3d9ShaderManager.h"

#include <fstream>
#include <RwgeTime.h>
#include <RwgeAssert.h>
#include <RwgeLog.h>
#include "RwgeShaderCompilerEnv.h"
#include "RwgeD3dx9Extension.h"

using namespace std;
using namespace RwgeD3dx9Extension;

bool RD3d9ShaderManager::m_bRecompileShader = true;

RD3d9ShaderManager::RD3d9ShaderManager() :
	m_pSharedShader(nullptr)
{
	HRESULT hResult = D3DXCreateEffectPool(&m_pEffectPool);

	if (FAILED(hResult))
	{
		RwgeLog(TEXT("Create effect pool failed - ErrorCode : %s"), D3dErrorCodeToString(hResult));
		return;
	}
}

RD3d9ShaderManager::~RD3d9ShaderManager()
{

}

bool RD3d9ShaderManager::CompileShader(const RShaderKey& key)
{
	RShaderCompilerEnvironment compilerEnvironment;
	compilerEnvironment.SetShaderKey(key);

	// ====================== ���������ܵ���NamedPipe�������ڻ�ȡ����Log ======================
	SECURITY_ATTRIBUTES securityAttributes;
	securityAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
	securityAttributes.bInheritHandle = TRUE;
	securityAttributes.lpSecurityDescriptor = nullptr;

	// �������ý��̳�ʼ�����������ݽṹ�����а����󶨹ܵ��Ĳ���
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
		RwgeLog("Compile shader failed! Unable to create information pipe.");
		return false;
	}

	bResult = CreatePipe(&hReadErrorPipe, &infoStartup.hStdError, &securityAttributes, 0);
	if (bResult == FALSE)
	{
		RwgeLog("Compile shader failed! Unable to create error pipe.");
		return false;
	}

	// ������ɫ��������FXC���ӽ���
	PROCESS_INFORMATION infoProcess;
	ZeroMemory(&infoProcess, sizeof(PROCESS_INFORMATION));

	bResult = CreateProcess(
		nullptr,														// ��ִ���ļ�·�������������������У��˴���Ϊ�գ�
		const_cast<char*>(compilerEnvironment.GetCompilerCmdLine()),	// �����в���
		nullptr,														// ���̳е�ǰ���̾��
		nullptr,														// ���̳е�ǰ�߳̾��
		TRUE,															// �Ƿ�̳о��
		0,																// ��ʹ�ô�����־��Flag��
		nullptr,														// ʹ�ø����̵Ļ�����
		nullptr,														// ʹ�ø����̵ĳ���·��
		&infoStartup,													// ��ȡSTARTUPINFO
		&infoProcess);													// ��ȡPROCESS_INFORMATION

	CloseHandle(infoStartup.hStdOutput);
	CloseHandle(infoStartup.hStdError);

	if (bResult == FALSE)
	{
		RwgeLog(TEXT("Compile shader failed! Unable to create compilation process."));
		return false;
	}

	HANDLE waitHandles[] = { infoProcess.hProcess, hReadInfoPipe, hReadErrorPipe };

	const unsigned int u32BufferSize = 4096;
	TCHAR szBuffer[u32BufferSize];

	ofstream streamComplitationInfo(RShaderCompilerEnvironment::GetShaderDebugInfoPath(key), ios::app);

	streamComplitationInfo << "=================================================================" << endl;
	streamComplitationInfo << TEXT("[TIME] ") << GetCurrentDateTime(ETF_Standard) << endl;
	streamComplitationInfo << endl;

	// ������������ڼ��������ǰ�߳�
	while (true)
	{
		DWORD dwBytesRead, dwBytesAvailable;

		DWORD dwWaitResult = WaitForMultipleObjects(3, waitHandles, FALSE, 60000L);

		if (PeekNamedPipe(hReadInfoPipe, nullptr, 0, nullptr, &dwBytesAvailable, nullptr) && dwBytesAvailable)
		{
			streamComplitationInfo << TEXT("[INFO]") << endl;

			while (ReadFile(hReadInfoPipe, szBuffer, u32BufferSize - 1, &dwBytesRead, nullptr))
			{
				streamComplitationInfo << string(szBuffer, static_cast<size_t>(dwBytesRead));
			}

			streamComplitationInfo << endl;
		}

		if (PeekNamedPipe(hReadErrorPipe, nullptr, 0, nullptr, &dwBytesAvailable, nullptr) && dwBytesAvailable)
		{
			streamComplitationInfo << TEXT("[ERROR]") << endl;

			while (ReadFile(hReadErrorPipe, szBuffer, u32BufferSize - 1, &dwBytesRead, nullptr))
			{
				streamComplitationInfo << string(szBuffer, static_cast<size_t>(dwBytesRead));
			}

			streamComplitationInfo << endl;
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

RD3d9Shader* RD3d9ShaderManager::GetShader(const RShaderKey& key)
{
	ShaderMap::iterator itShader = m_mapShaders.find(key);

	// ����ҵ��˾ͷ�����ɫ��ָ��
	if (itShader != m_mapShaders.end())
	{
		return itShader->second;
	}

	if (m_bRecompileShader)
	{
		// ���±���Shader
		if (!CompileShader(key))
		{
			RwgeLog("Compile shader failed with shader key : %s!", key.ToHexString());
			return nullptr;
		}
	}
	
	// ���û���ҵ��ͳ��Դ��ļ��м���
	RD3d9Shader* pShader = new RD3d9Shader(key, m_pEffectPool);
	if (!pShader->IsSuccessLoaded())
	{
		RwgeLog("Load shader failed! Attempt to comiple and reload.");

		// �������ʧ�ܣ�������ٴγ��Լ���
		if (!CompileShader(key))
		{
			RwgeLog("Compile shader failed!");
			return nullptr;
		}

		delete pShader;
		pShader = new RD3d9Shader(key, m_pEffectPool);

		if (!pShader->IsSuccessLoaded())
		{
			RwgeLog("Load shader failed again!");
			delete pShader;
			return nullptr;
		}
	}

	RwgeAssert(m_mapShaders.insert(make_pair(key, pShader)).second);

	return pShader;
}

RD3d9Shader* RD3d9ShaderManager::GetSharedShader()
{
	if (m_pSharedShader == nullptr)
	{
		if (m_mapShaders.empty())
		{
			return nullptr;
		}

		m_pSharedShader = m_mapShaders.begin()->second;
	}

	return m_pSharedShader;
}
