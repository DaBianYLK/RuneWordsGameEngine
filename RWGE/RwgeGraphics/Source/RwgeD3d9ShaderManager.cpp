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

	// ====================== 创建有名管道（NamedPipe），用于获取编译Log ======================
	SECURITY_ATTRIBUTES securityAttributes;
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
		RwgeLog("Compile shader failed! Unable to create information pipe.");
		return false;
	}

	bResult = CreatePipe(&hReadErrorPipe, &infoStartup.hStdError, &securityAttributes, 0);
	if (bResult == FALSE)
	{
		RwgeLog("Compile shader failed! Unable to create error pipe.");
		return false;
	}

	// 创建着色器编译器FXC的子进程
	PROCESS_INFORMATION infoProcess;
	ZeroMemory(&infoProcess, sizeof(PROCESS_INFORMATION));

	bResult = CreateProcess(
		nullptr,														// 可执行文件路径（包含在了命令行中，此处设为空）
		const_cast<char*>(compilerEnvironment.GetCompilerCmdLine()),	// 命令行参数
		nullptr,														// 不继承当前进程句柄
		nullptr,														// 不继承当前线程句柄
		TRUE,															// 是否继承句柄
		0,																// 不使用创建标志（Flag）
		nullptr,														// 使用父进程的环境块
		nullptr,														// 使用父进程的程序路径
		&infoStartup,													// 获取STARTUPINFO
		&infoProcess);													// 获取PROCESS_INFORMATION

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

	// 编译程序运行期间会阻塞当前线程
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

	// 如果找到了就返回着色器指针
	if (itShader != m_mapShaders.end())
	{
		return itShader->second;
	}

	if (m_bRecompileShader)
	{
		// 重新编译Shader
		if (!CompileShader(key))
		{
			RwgeLog("Compile shader failed with shader key : %s!", key.ToHexString());
			return nullptr;
		}
	}
	
	// 如果没有找到就尝试从文件中加载
	RD3d9Shader* pShader = new RD3d9Shader(key, m_pEffectPool);
	if (!pShader->IsSuccessLoaded())
	{
		RwgeLog("Load shader failed! Attempt to comiple and reload.");

		// 如果加载失败，编译后再次尝试加载
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
