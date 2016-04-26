/*--------------------------------------------------------------------------------------------------------------------*\
   ��CREATE��	
	AUTH :	���һ���																			   DATE : 2016-02-29
	DESC :	
	1.	Shader��װ��D3D Effect����������ShaderManager���д������ͷš�
	2.	2016-03-02 ToDo��
		A.	ĿǰShader����Ʋ�֧�ֶ�Pass��Ⱦ���Ƚ���������Ⱦ�߼�д���ٿ�����չ��
		B.	Ŀǰ��Shader��չ�Ի��ǲ����ߣ���Ҫ��̬����material�ű���technique�ű����ܽ����
			I.	����ʹ�ù̶������֣��ɳ������ɣ����󶨵���Ӧ��GetXXX()�����У��ϼ򵥣�
			II.	technique�ɳ���̬���ɣ��Ա�����Ⱦ���̣�֧�ֶ�pass��Ⱦ��ʵ���ѶȽϴ�
	3.	2016-04-21 ToDo��
		Ŀǰ��Shader��Vertex Shader��Pixel Shader�������һ���Ժ���Ҫ���Ƿֿ����ǣ���ʵ�ָ��õ���չ�Ժ�����
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
