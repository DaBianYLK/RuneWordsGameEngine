/*--------------------------------------------------------------------------------------------------------------------*\
   ��CREATE��	
	AUTH :	���һ���																			   DATE : 2016-04-19
	DESC :	�������ɵ���FXC.exe����shader�������в���
\*--------------------------------------------------------------------------------------------------------------------*/

#pragma once

#include <RwgeObject.h>
#include <map>
#include <string>
#include "RwgeShaderKey.h"

class RShaderCompilerEnvironment : public RObject
{
public:
	RShaderCompilerEnvironment();
	~RShaderCompilerEnvironment();

	static void EnableDebugInfo(bool bEnable);
	static void EnableDx9Compiler(bool bEnable);
	
	static std::string GetShaderBinaryPath(const RShaderKey& key);
	static std::string GetShaderDebugInfoPath(const RShaderKey& key);

	void SetShaderKey(const RShaderKey& key);

	void SetDefine(const char* pDefine, const char* pValue);
	void SetDefine(const char* pDefine, unsigned int u32Value);
	void SetDefine(const char* pDefine, float f32Value);
	void SetDefine(const char* pDefine, unsigned char u8Value);
	void SetDefine(const char* pDefine, bool bValue);

	std::string GetCompilerCmdLine();

private:
	static const char* GetDefinitionText(const std::pair<std::string, std::string>& pairDefinition);

private:
	static std::string	m_strFxcPath;						// Fxc�����·��
	static std::string	m_strTargetVersion;					// ��Ҫ����Shader�İ汾
	static std::string	m_strDebugInfoPrefix;				// �������Ŀ¼�ĵ�����Ϣ�ļ�ǰ׺
	static std::string	m_strDebugInfoExtension;			// ������Ϣ�ļ���չ��
	static std::string	m_strShaderBinaryPrefix;			// �������Ŀ¼��Shader�������ļ���ǰ׺
	static std::string	m_strShaderBinaryExtension;			// Shader�������ļ���չ��
	static std::string	m_strShaderSourcePath;				// �������ShaderԴ�ļ�·��	
	static bool			m_bEnableDebugInfo;					// ��ʾ��������еĵ�����Ϣ
	static bool			m_bEnableDx9Compiler;				// ʹ��DX9����������

	RShaderKey			m_ShaderKey;
	std::map<std::string, std::string> m_mapDefinitions;	// Shader��ʹ�õĺ궨������
	std::string			m_strCompilerCmdLine;				// ���ڵ���Fxc���������ı�
};