#pragma once

#include "Texture.h"
#include <d3dx9.h>

class D3D9Device;
class ShaderType;
class Material;
class Light;

/*
Shader仅能由RenderTarget创建和释放。

2016-03-02 ToDo：
1.	目前Shader的设计不支持多Pass渲染，先将完整的渲染逻辑写完再考虑拓展。
2.	加入Effect Pool的常量共享机制
3.	目前的Shader拓展性还是不够高，需要动态生成material脚本和technique脚本才能解决：
	A.	纹理不使用固定的名字，由程序生成，并绑定到相应的GetXXX()函数中（较简单）
	B.	technique由程序动态生成，以便拆分渲染过程，支持多pass渲染（实现难度较大）

目前的Shader是绑定于Device的，这样一来增加一个Device，所有的Shader就会冗余一次，所以需要对Shader进行拆分：
1.	定义一个ShaderType类，该类与Device无关，它与具体的Material对应，定义了一个shader的实现，相当于对应了一个Shader的二进制文件
2.	定义一个Shader类，该类绑定于某一个ShaderType，它的对象是一个ShaderType的具体实例，绑定于某一个Device
3.	维护一个ShaderTypeMap，用于生成与管理ShaderType
4.	每个RenderTarget中维护一个hash_map<ShaderType*, Shader>，通过ShaderType指针就可以获取相应的Shader

Shader由RenderTarget管理而不由ShaderType管理的原因：
如果一个RenderTarget被释放掉，相应的Shader也需要被一并释放，如果由ShaderType管理Shader，释放过程效率较低
*/

struct SHCoefficients
{
	D3DXVECTOR4 R;
	D3DXVECTOR4 G;
	D3DXVECTOR4 B;
};

class Shader
{
	friend class RenderTarget;

private:
	Shader(ShaderType* pShaderType);

public:
	Shader(Shader&& shader);
	~Shader();

	bool Load(const D3D9Device* pDevice, LPD3DXEFFECTPOOL pEffectPool);

	void Begin() const;
	void End() const;
	void CommitChanges() const;

	void SetMaterial(Material* pMaterial, RenderTarget* pRenderTarget);

	void SetTransform(const D3DXMATRIX* pWorld, const D3DXMATRIX* pWVP);
	void SetViewOppositeDirection(const void* pDirection);
	void SetMaterialConstant(const void* pMaterialConstant, unsigned int uSize);
	void SetBaseColorTexture(const Texture* pTexture);
	void SetEmissiveColorTexture(const Texture* pTexture);
	void SetNormalTexture(const Texture* pTexture);
	void SetMetallicTexture(const Texture* pTexture);
	void SetSpecularTexture(const Texture* pTexture);
	void SetRoughnessTexture(const Texture* pTexture);
	void SetOpacityTexture(const Texture* pTexture);
	void SetOpacityMaskTexture(const Texture* pTexture);
	void SetLight(const void* pLight, unsigned int uSize);
	void SetSHCoefficients(const SHCoefficients* pCoefficients);

private:
	ShaderType*	m_pShaderType;

	LPD3DXEFFECT m_pEffect;

	D3DXHANDLE m_hWorldTransform;
	D3DXHANDLE m_hWVPTransform;
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
	D3DXHANDLE m_hSHCoefficients;
};
