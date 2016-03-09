#pragma once

#include "Texture.h"
#include <string>
#include <d3dx9.h>

class D3D9Device;

/*
ShaderProgram������ShaderManager�������ͷš�

2016-03-02 ToDo��
1.	ĿǰShader����Ʋ�֧�ֶ�Pass��Ⱦ���Ƚ���������Ⱦ�߼�д���ٿ�����չ��
2.	����Effect Pool�ĳ����������
3.	Ŀǰ��Shader��չ�Ի��ǲ����ߣ���Ҫ��̬����material�ű���technique�ű����ܽ����
	A.	����ʹ�ù̶������֣��ɳ������ɣ����󶨵���Ӧ��GetXXX()�����У��ϼ򵥣�
	B.	technique�ɳ���̬���ɣ��Ա�����Ⱦ���̣�֧�ֶ�pass��Ⱦ��ʵ���ѶȽϴ�
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
