/*--------------------------------------------------------------------------------------------------------------------*\
   【CREATE】	
	AUTH :	大便一箩筐																			   DATE : 2016-02-29
	DESC :	
	1.	Shader封装了D3D Effect，它仅能由ShaderManager进行创建与释放。
	2.	2016-03-02 ToDo：
		A.	目前Shader的设计不支持多Pass渲染，先将完整的渲染逻辑写完再考虑拓展。
		B.	目前的Shader拓展性还是不够高，需要动态生成material脚本和technique脚本才能解决：
			I.	纹理不使用固定的名字，由程序生成，并绑定到相应的GetXXX()函数中（较简单）
			II.	technique由程序动态生成，以便拆分渲染过程，支持多pass渲染（实现难度较大）
	3.	2016-04-21 ToDo：
		目前的Shader将Vertex Shader和Pixel Shader组合在了一起，以后需要考虑分开它们，以实现更好的拓展性和性能
\*--------------------------------------------------------------------------------------------------------------------*/


#pragma once

#include <d3dx9.h>
#include "RwgeTexture.h"
#include "RwgeShaderKey.h"

class RD3d9Device;
class ShaderType;
class RMaterial;
class Light;

struct SHCoefficients
{
	D3DXVECTOR4 R;
	D3DXVECTOR4 G;
	D3DXVECTOR4 B;
};

class RShader : public RObject
{
	friend class RShaderManager;

private:
	RShader(ShaderType* pShaderType);
	~RShader();

public:
	bool Load(const RD3d9Device* pDevice, LPD3DXEFFECTPOOL pEffectPool);

	void Begin() const;
	void End() const;
	void CommitChanges() const;

	void SetMaterial(RMaterial* pMaterial, RenderTarget* pRenderTarget);

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
	RShaderKey	m_ShaderKey;

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
