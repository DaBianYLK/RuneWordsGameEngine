#pragma once

#include "Texture.h"
#include <d3dx9.h>

class D3D9Device;
class ShaderType;
class Material;
class Light;

/*
Shader������RenderTarget�������ͷš�

2016-03-02 ToDo��
1.	ĿǰShader����Ʋ�֧�ֶ�Pass��Ⱦ���Ƚ���������Ⱦ�߼�д���ٿ�����չ��
2.	����Effect Pool�ĳ����������
3.	Ŀǰ��Shader��չ�Ի��ǲ����ߣ���Ҫ��̬����material�ű���technique�ű����ܽ����
	A.	����ʹ�ù̶������֣��ɳ������ɣ����󶨵���Ӧ��GetXXX()�����У��ϼ򵥣�
	B.	technique�ɳ���̬���ɣ��Ա�����Ⱦ���̣�֧�ֶ�pass��Ⱦ��ʵ���ѶȽϴ�

Ŀǰ��Shader�ǰ���Device�ģ�����һ������һ��Device�����е�Shader�ͻ�����һ�Σ�������Ҫ��Shader���в�֣�
1.	����һ��ShaderType�࣬������Device�޹أ���������Material��Ӧ��������һ��shader��ʵ�֣��൱�ڶ�Ӧ��һ��Shader�Ķ������ļ�
2.	����һ��Shader�࣬�������ĳһ��ShaderType�����Ķ�����һ��ShaderType�ľ���ʵ��������ĳһ��Device
3.	ά��һ��ShaderTypeMap���������������ShaderType
4.	ÿ��RenderTarget��ά��һ��hash_map<ShaderType*, Shader>��ͨ��ShaderTypeָ��Ϳ��Ի�ȡ��Ӧ��Shader

Shader��RenderTarget���������ShaderType�����ԭ��
���һ��RenderTarget���ͷŵ�����Ӧ��ShaderҲ��Ҫ��һ���ͷţ������ShaderType����Shader���ͷŹ���Ч�ʽϵ�
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
