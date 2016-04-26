#include "RwgeShaderManager.h"

#include <fstream>
#include <RwgeTime.h>
#include <RwgeAssert.h>
#include "RwgeShaderCompilerEnv.h"

using namespace std;

RShaderManager::RShaderManager()
{

}

RShaderManager::~RShaderManager()
{

}

const EnvironmentKey& RShaderManager::GetEnvironmentKey(const Light* pLight)
{
	static EnvironmentKey envKey;

	envKey.SetLightTypeKey(pLight->GetLightType());
	envKey.SetShaderSkinKey(false);

	return envKey;
}

RShaderKey RShaderManager::GetShaderKey(const MaterialKey& matKey, const EnvironmentKey& envKey)
{
	RShaderKey shaderKey;

	shaderKey.SetMaterialKey(matKey);
	shaderKey.SetEnvironmentKey(envKey);

	return shaderKey;
}

bool RShaderManager::CompileShader(const RShaderKey& key)
{
	RShaderCompilerEnvironment compilerEnvironment;
	compilerEnvironment.SetShaderKey(key);

	string strCommandLine = compilerEnvironment.GetCompilerCmdLine();

	// ====================== ���������ܵ���NamedPipe�������ڻ�ȡ����Log ======================
	SECURITY_ATTRIBUTES   securityAttributes;
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
		RwgeErrorBox("Compile shader failed! Unable to create information pipe.");
		return false;
	}

	bResult = CreatePipe(&hReadErrorPipe, &infoStartup.hStdError, &securityAttributes, 0);
	if (bResult == FALSE)
	{
		RwgeErrorBox("Compile shader failed! Unable to create error pipe.");
		return false;
	}

	// ������ɫ��������FXC���ӽ���
	PROCESS_INFORMATION infoProcess;
	ZeroMemory(&infoProcess, sizeof(PROCESS_INFORMATION));

	bResult = CreateProcess(nullptr,									// ��ִ���ļ�·�������������������У��˴���Ϊ�գ�
							const_cast<char*>(strCommandLine.c_str()),	// �����в���
							nullptr,									// ���̳е�ǰ���̾��
							nullptr,									// ���̳е�ǰ�߳̾��
							TRUE,										// �Ƿ�̳о��
							0,											// ��ʹ�ô�����־��Flag��
							nullptr,									// ʹ�ø����̵Ļ�����
							nullptr,									// ʹ�ø����̵ĳ���·��
							&infoStartup,								// ��ȡSTARTUPINFO
							&infoProcess);								// ��ȡPROCESS_INFORMATION

	CloseHandle(infoStartup.hStdOutput);
	CloseHandle(infoStartup.hStdError);

	if (bResult == FALSE)
	{
		RwgeErrorBox("Compile shader failed! Unable to create compilation process.");
		return false;
	}

	HANDLE waitHandles[] = { infoProcess.hProcess, hReadInfoPipe, hReadErrorPipe };

	const unsigned int u32BufferSize = 4096;
	char arrayBuffer[u32BufferSize];

	sprintf_s(arrayBuffer, "%s_%s.log",
		RShaderCompilerEnvironment::GetShaderDebugInfoPath(key),
		GetCurrentDateTime(TF_DigitalWithUnderline).c_str());
	ofstream streamComplitationInfo(arrayBuffer, ios::out);

	// ������������ڼ��������ǰ�߳�
	while (true)
	{
		DWORD dwBytesRead, dwBytesAvailable;

		DWORD dwWaitResult = WaitForMultipleObjects(3, waitHandles, FALSE, 60000L);

		if (PeekNamedPipe(hReadInfoPipe, nullptr, 0, nullptr, &dwBytesAvailable, nullptr) && dwBytesAvailable)
		{
			streamComplitationInfo << "[INFO]===========================================================" << endl;

			while (ReadFile(hReadInfoPipe, arrayBuffer, u32BufferSize - 1, &dwBytesRead, nullptr))
			{
				streamComplitationInfo << string(arrayBuffer, static_cast<size_t>(dwBytesRead));
			}

			streamComplitationInfo << "=================================================================" << endl;
		}

		if (PeekNamedPipe(hReadErrorPipe, nullptr, 0, nullptr, &dwBytesAvailable, nullptr) && dwBytesAvailable)
		{
			streamComplitationInfo << "[ERROR]==========================================================" << endl;

			while (ReadFile(hReadErrorPipe, arrayBuffer, u32BufferSize - 1, &dwBytesRead, nullptr))
			{
				streamComplitationInfo << string(arrayBuffer, static_cast<size_t>(dwBytesRead));
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