#pragma once

#include "Texture.h"
#include <string>
#include <d3dx9.h>

class D3D9Device;

/*
ShaderProgram仅能由ShaderManager创建和释放。

2016-03-02 ToDo：
1.	目前Shader的设计不支持多Pass渲染，先将完整的渲染逻辑写完再考虑拓展。
2.	加入Effect Pool的常量共享机制
3.	目前的Shader拓展性还是不够高，需要动态生成material脚本和technique脚本才能解决：
	A.	纹理不使用固定的名字，由程序生成，并绑定到相应的GetXXX()函数中（较简单）
	B.	technique由程序动态生成，以便拆分渲染过程，支持多pass渲染（实现难度较大）

2016-03-11 ToDo:
目前的ShaderProgram是绑定于Device的，这样一来增加一个Device，所有的ShaderProgram就会冗余一次。可以借鉴UE4的方法进行优化
UE4中的做法是：
1.	定义一个ShaderType类，该类与Device无关，它与具体的Material对应，定义了一个shader的实现，相当于对应了一个Shader的二进制文件
2.	定义一个Shader类，该类绑定于某一个ShaderType，它的对象是一个ShaderType的具体实例，绑定于某一个Device
3.	维护一个ShaderTypeMap，用于生成与管理ShaderType
4.	每个ShaderType中维护一个vector<Shader*>，为每个RenderTarget从0到1设置编号，当某一个RenderTarget激活时，从ShaderType中获取相应编号的Shader
*/

class ShaderProgram
{
	friend class ShaderManager;

protected:
	ShaderProgram(const std::string& strPath);
	~ShaderProgram();

	bool Load(const D3D9Device* pDevice, LPD3DXEFFECTPOOL pEffectPool);

public:
	void Begin() const;
	void End() const;

	void SetTransform(const void* pTransform);
	void SetViewOppositeDirection(const void* pDirection);
	void SetMaterial(const void* pMaterialConstant, unsigned int uSize);
	void SetBaseColorTexture(const Texture* pTexture);
	void SetEmissiveColorTexture(const Texture* pTexture);
	void SetNormalTexture(const Texture* pTexture);
	void SetMetallicTexture(const Texture* pTexture);
	void SetSpecularTexture(const Texture* pTexture);
	void SetRoughnessTexture(const Texture* pTexture);
	void SetOpacityTexture(const Texture* pTexture);
	void SetOpacityMaskTexture(const Texture* pTexture);
	void SetLight(const void* pLight, unsigned int uSize);

private:
	std::string m_strFilePath;

	LPD3DXEFFECT m_pEffect;

	D3DXHANDLE m_hTransform;
	D3DXHANDLE m_hViewOppositeDirection;
	D3DXHANDLE m_hMaterial;
	D3DXHANDLE m_hBaseColorTexture;
	D3DXHANDLE m_hEmissiveColorTexture;
	D3DXHANDLE m_hNormalTexture;
	D3DXHANDLE m_hMetallicTexture;
	D3DXHANDLE m_hSpecularTexture;
	D3DXHANDLE m_hRoughnessTexture;
	D3DXHANDLE m_hOpacityTexture;
	D3DXHANDLE m_hOpacityMaskTexture;
	D3DXHANDLE m_hDirectionalLight;
	D3DXHANDLE m_hPointLight;
};
