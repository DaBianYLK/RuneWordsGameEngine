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
