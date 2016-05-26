/*--------------------------------------------------------------------------------------------------------------------*\
   【CREATE】	
	AUTH :	大便一箩筐																			   DATE : 2016-05-02
	DESC :	用于生成调用FXC.exe编译shader的命令行参数
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
	
	static const TCHAR* GetShaderBinaryPath(const RShaderKey& inKey);		// 返回字符串存储在静态缓冲中，非线程安全
	static const TCHAR* GetShaderDebugInfoPath(const RShaderKey& inKey);	// 返回字符串存储在静态缓冲中，非线程安全

	void SetShaderKey(const RShaderKey& key);

	void SetDefine(const TCHAR* pDefine, const TCHAR* pValue);
	void SetDefine(const TCHAR* pDefine, unsigned int u32Value); 
	void SetDefine(const TCHAR* pDefine, float f32Value);
	void SetDefine(const TCHAR* pDefine, unsigned char u8Value);
	void SetDefine(const TCHAR* pDefine, bool bValue);

	const TCHAR* GetCompilerCmdLine();										// 返回字符串存储在静态缓冲中，非线程安全

private:
	static const TCHAR* GetDefinitionText(const std::pair<Rwge::tstring, Rwge::tstring>& pairDefinition);

private:
	static const TCHAR*		m_szFxcPath;							// Fxc程序的路径
	static const TCHAR*		m_szTargetVersion;						// 需要编译Shader的版本
	static const TCHAR*		m_szDebugInfoPrefix;					// 包含输出目录的调试信息文件前缀
	static const TCHAR*		m_szDebugInfoExtension;					// 调试信息文件拓展名
	static const TCHAR*		m_szShaderBinaryPrefix;					// 包含输出目录的Shader二进制文件名前缀
	static const TCHAR*		m_szShaderBinaryExtension;				// Shader二进制文件拓展名
	static const TCHAR*		m_szShaderAssemblyPrefix;				// 包含输出目录的Shader汇编文件名前缀
	static const TCHAR*		m_szShaderAssemblyExtension;			// Shader汇编文件拓展名
	static const TCHAR*		m_szShaderSourcePath;					// 待编译的Shader源文件路径	
	static bool				m_bEnableDebugInfo;						// 显示编译过程中的调试信息
	static bool				m_bEnableDx9Compiler;					// 使用DX9编译器编译
	static bool				m_bOutputAssemblyFile;					// 输出汇编格式的Shader（可以用于优化与调试着色器）


	RShaderKey								m_ShaderKey;
	std::map<Rwge::tstring, Rwge::tstring>	m_mapDefinitions;		// Shader中使用的宏定义声明
	std::string								m_strCompilerCmdLine;	// 用于调用Fxc的命令行文本

	// 用于存储字符串的临时缓冲区
	static TCHAR			m_szBuffer128[128];					
	static TCHAR			m_szBuffer1024[1024];
};