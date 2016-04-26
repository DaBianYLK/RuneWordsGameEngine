/*--------------------------------------------------------------------------------------------------------------------*\
   【CREATE】	
	AUTH :	大便一箩筐																			   DATE : 2016-03-01
	DESC :	
	1.	这个类负责了着色器的编译与管理工作
	2.	关于着色器的管理
		A.	为了避免每次启动游戏时都重新编译着色器，使用FXC 执行离线编译，并将着色器二进制数据保存在资源文件中。
		B.	需要获取Shader时，首先根据材质和环境得到ShaderKey ，使用ShaderKey 在一个全局的<ShaderKey, Shader> hash表
			中查找，查找成功时返回Shader，否则根据ShaderKey 从资源文件中加载着色器，加载失败时再重新编译着色器。
	3.	着色器管理器必须指定D3D9Device，且一旦指定后就不能变更。因为所有的Shader都是与Device绑定的，更换Device意味着
		需要重新加载所有的Shader。
\*--------------------------------------------------------------------------------------------------------------------*/


#pragma once

#include <string>
#include <hash_map>
#include <RwgeSingleton.h>
#include <RwgeObject.h>
#include "RwgeShaderKey.h"
#include "RwgeShader.h"
#include "RwgeMaterial.h"
#include "RwgeLight.h"
#include "RwgeD3d9Device.h"

class RShaderManager : 
	public RObject,
	public Singleton<RShaderManager>
{
public:
	RShaderManager();
	~RShaderManager();

	__forceinline static const MaterialKey& GetMaterialKey(const RMaterial* pMaterial)
	{
		return pMaterial->GetMaterialKey();
	};

	static const EnvironmentKey& GetEnvironmentKey(const Light* pLight);
	static RShaderKey GetShaderKey(const MaterialKey& matKey, const EnvironmentKey& envKey);

	static bool CompileShader(const RShaderKey& key);

private:
	std::hash_map<RShaderKey, RShader> m_hashShaders;
};

