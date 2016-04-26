/*--------------------------------------------------------------------------------------------------------------------*\
   【CREATE】	
	AUTH :	大便一箩筐																			   DATE : 2016-04-19
	DESC :	用于生成调用FXC.exe编译shader的命令行参数
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
	static std::string	m_strFxcPath;						// Fxc程序的路径
	static std::string	m_strTargetVersion;					// 需要编译Shader的版本
	static std::string	m_strDebugInfoPrefix;				// 包含输出目录的调试信息文件前缀
	static std::string	m_strDebugInfoExtension;			// 调试信息文件拓展名
	static std::string	m_strShaderBinaryPrefix;			// 包含输出目录的Shader二进制文件名前缀
	static std::string	m_strShaderBinaryExtension;			// Shader二进制文件拓展名
	static std::string	m_strShaderSourcePath;				// 待编译的Shader源文件路径	
	static bool			m_bEnableDebugInfo;					// 显示编译过程中的调试信息
	static bool			m_bEnableDx9Compiler;				// 使用DX9编译器编译

	RShaderKey			m_ShaderKey;
	std::map<std::string, std::string> m_mapDefinitions;	// Shader中使用的宏定义声明
	std::string			m_strCompilerCmdLine;				// 用于调用Fxc的命令行文本
};