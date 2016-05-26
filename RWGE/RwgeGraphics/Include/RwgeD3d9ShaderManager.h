/*--------------------------------------------------------------------------------------------------------------------*\
   【CREATE】	
	AUTH :	大便一箩筐																			   DATE : 2016-05-03
	DESC :	
	1.	这个类负责了着色器的编译与管理工作
	2.	关于着色器的管理
		A.	为了避免每次启动游戏时都重新编译着色器，使用FXC 执行离线编译，并将着色器二进制数据保存在资源文件中。
		B.	需要获取Shader时，首先根据材质和环境得到ShaderKey ，使用ShaderKey 在一个全局的<ShaderKey, Shader> hash表
			中查找，查找成功时返回Shader，否则根据ShaderKey 从资源文件中加载着色器，加载失败时再重新编译着色器。
	3.	着色器管理器必须指定D3D9Device，且一旦指定后就不能变更。因为所有的Shader都是与Device绑定的，更换Device意味着
		需要重新加载所有的Shader
\*--------------------------------------------------------------------------------------------------------------------*/


#pragma once

#include <RwgeCoreDef.h>
#include <RwgeSingleton.h>
#include <RwgeObject.h>
#include "RwgeShaderKey.h"
#include "RwgeD3d9Shader.h"

#define USING_HASH_MAP		1		// 是否使用hahs表储存Shader	

#if USING_HASH_MAP
#	include <unordered_map>
typedef std::unordered_map<RShaderKey, RD3d9Shader*, RShaderKey::Hash, RShaderKey::Equal> ShaderMap;
#else
#	include <map>
typedef std::map<RShaderKey, RShader*> ShaderMap;
#endif

class RD3d9ShaderManager : 
	public RObject,
	public Singleton<RD3d9ShaderManager>
{
public:
	RD3d9ShaderManager();
	~RD3d9ShaderManager();

	static bool CompileShader(const RShaderKey& key);
	RD3d9Shader* GetShader(const RShaderKey& key);

	RD3d9Shader* GetSharedShader();				// 返回着色器映射表中的第一个着色器，如果映射表为空则返回nullptr

private:
	RD3d9Shader*		m_pSharedShader;		// 用于设置共享常量的Shader
	ShaderMap			m_mapShaders;

	LPD3DXEFFECTPOOL	m_pEffectPool;			// 用于在Shader之间共享常量

	static bool			m_bRecompileShader;		// 是否在游戏启动时重新编译所有的Shader
};

