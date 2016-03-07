#pragma once

#include "ShaderProgram.h"
#include "Material.h"
#include "Light.h"
#include "D3D9Device.h"
#include <string>
#include <hash_map>

/*
关于着色器的管理
1.	为了避免每次重启游戏时都要重新编译着色器，需要使用FXC对着色器执行离线编译，并保存在一个统一的着色器管理器中。
2.	定义一个规则，将离线编译得到的着色器文件的路径与一个64位无符号整型的Key进行对应。
3.	将所有可能用到的Key存储到资源文件中，每次游戏启动时加载该资源文件，根据Key计算得到路径，再根据路径获取着色器二进制编码，生成一个<Key，ShaderProgram>hash表。
4.	当程序需要用到着色器时，管理器根据材质等参数计算出64位的Key，找到对应的ShaderProgram并返回指针。

64位Key的生成规则：
位范围		位数	说明
MaterialKey
[ 0,  3]	4		BaseColor表达式编号
[ 4,  7]	4		EmissiveColor表达式编号
[ 8, 11]	4		Normal表达式编号
[12, 15]	4		Metallic表达式编号
[16, 19]	4		Specular表达式编号
[20, 23]	4		Roughness表达式编号
[24, 27]	4		Opacity表达式编号
[28, 31]	4		Opacity表达式编号
[32, 34]	3		材质混合模式编号
[35, 36]	2		材质着色模式编号
[37, 37]	1		双面着色标志
[38, 38]	1		材质是否为非金属的标志
[39, 39]	1		材质是否完全粗糙的标志

EnvironmentKey
[40, 41]	2		方向光类型编号
//[40, 40]	1		着色器计算骨骼动画标志
其他		23		保留字段


1.	一个着色器管理器必须对应一个D3D9Device，且一旦指定后就不能变更。
	因为所有的ShaderProgram都是与Device绑定的，更换Device意味着需要重新加载所有的Shader。
2.	由于D3D9Device的关系，ShaderManager与RenderTarget是绑定的，不能够独立存在，所以它只能由RenderTarget创建和释放。
*/

class ShaderManager
{
	friend class RenderManager;
	friend class RenderTarget;

private:
	ShaderManager(D3D9Device* pDevice);
	~ShaderManager();

public:
	static unsigned long long GetMaterialKey(const Material* pMaterial);
	static unsigned long long GetEnvironmentKey(const Light* pLight);
	static unsigned long long GetShaderProgramKey(const Material* pMaterial, const Light* pLight);
	static unsigned long long GetShaderProgramKey(const unsigned long long u64MaterialKey, const unsigned long long u64EnvironmentKey);

	static const std::string& GetFXCCommandLine(const Material* pMaterial, const Light* pLight);
	static const std::string& GetFXCCommandLine(const unsigned long long u64Key);

	static bool CompileShader(const Material* pMaterial, const Light* pLight);
	static bool CompileShader(const unsigned long long u64Key);

	static ShaderProgram* LoadShaderByKey(const unsigned long long u64Key, const D3D9Device* pDevice, LPD3DXEFFECTPOOL pEffectPool);

	ShaderProgram* GetShaderProgramByKey(const unsigned long long u64Key);

private:
	static std::string m_strFXCPath;
	static std::string m_strTargetVersion;
	static std::string m_strShaderBinaryPrefix;
	static std::string m_strShaderBinaryExtension;
	static std::string m_strHLSLPath;
	static std::string m_strCompilationLogPrefix;

	D3D9Device* m_pDevice;
	std::hash_map<unsigned long long, ShaderProgram*> m_hashShaders;

	LPD3DXEFFECTPOOL	m_pEffectPool;		// 用于在Shader之间共享常量
};

