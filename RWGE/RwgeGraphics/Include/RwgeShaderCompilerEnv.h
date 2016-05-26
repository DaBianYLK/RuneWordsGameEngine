/*--------------------------------------------------------------------------------------------------------------------*\
   ��CREATE��	
	AUTH :	���һ���																			   DATE : 2016-05-02
	DESC :	�������ɵ���FXC.exe����shader�������в���
\*--------------------------------------------------------------------------------------------------------------------*/

#pragma once

#include <RwgeObject.h>
#include <map>
#include <tchar.h>
#include <RwgeTString.h>
#include "RwgeShaderKey.h"

class RShaderCompilerEnvironment : public RObject
{
public:
	RShaderCompilerEnvironment();
	~RShaderCompilerEnvironment();

	static void EnableDebugInfo(bool bEnable);
	static void EnableDx9Compiler(bool bEnable);
	
	static const TCHAR* GetShaderBinaryPath(const RShaderKey& inKey);		// �����ַ����洢�ھ�̬�����У����̰߳�ȫ
	static const TCHAR* GetShaderDebugInfoPath(const RShaderKey& inKey);	// �����ַ����洢�ھ�̬�����У����̰߳�ȫ

	void SetShaderKey(const RShaderKey& key);

	void SetDefine(const TCHAR* pDefine, const TCHAR* pValue);
	void SetDefine(const TCHAR* pDefine, unsigned int u32Value); 
	void SetDefine(const TCHAR* pDefine, float f32Value);
	void SetDefine(const TCHAR* pDefine, unsigned char u8Value);
	void SetDefine(const TCHAR* pDefine, bool bValue);

	const TCHAR* GetCompilerCmdLine();										// �����ַ����洢�ھ�̬�����У����̰߳�ȫ

private:
	static const TCHAR* GetDefinitionText(const std::pair<Rwge::tstring, Rwge::tstring>& pairDefinition);

private:
	static const TCHAR*		m_szFxcPath;							// Fxc�����·��
	static const TCHAR*		m_szTargetVersion;						// ��Ҫ����Shader�İ汾
	static const TCHAR*		m_szDebugInfoPrefix;					// �������Ŀ¼�ĵ�����Ϣ�ļ�ǰ׺
	static const TCHAR*		m_szDebugInfoExtension;					// ������Ϣ�ļ���չ��
	static const TCHAR*		m_szShaderBinaryPrefix;					// �������Ŀ¼��Shader�������ļ���ǰ׺
	static const TCHAR*		m_szShaderBinaryExtension;				// Shader�������ļ���չ��
	static const TCHAR*		m_szShaderAssemblyPrefix;				// �������Ŀ¼��Shader����ļ���ǰ׺
	static const TCHAR*		m_szShaderAssemblyExtension;			// Shader����ļ���չ��
	static const TCHAR*		m_szShaderSourcePath;					// �������ShaderԴ�ļ�·��	
	static bool				m_bEnableDebugInfo;						// ��ʾ��������еĵ�����Ϣ
	static bool				m_bEnableDx9Compiler;					// ʹ��DX9����������
	static bool				m_bOutputAssemblyFile;					// �������ʽ��Shader�����������Ż��������ɫ����


	RShaderKey								m_ShaderKey;
	std::map<Rwge::tstring, Rwge::tstring>	m_mapDefinitions;		// Shader��ʹ�õĺ궨������
	std::string								m_strCompilerCmdLine;	// ���ڵ���Fxc���������ı�

	// ���ڴ洢�ַ�������ʱ������
	static TCHAR			m_szBuffer128[128];					
	static TCHAR			m_szBuffer1024[1024];
};