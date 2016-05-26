/*--------------------------------------------------------------------------------------------------------------------*\
   ��CREATE��	
	AUTH :	���һ���																			   DATE : 2016-05-04
	DESC :	
	1.	Shader��װ��D3D Effect����������ShaderManager���д������ͷš�

	2.	��CPU ��Shader��������ʱ����Ҫע��Shader�����е����ݶ�����float4Ϊ��λ���洢�ģ�Ҳ����˵�����Shader�ж�����
		һ��float3������ʵ��������ռ��һ��float4�Ĵ�����Ҳ����16�ֽڵĿռ䡣�������һ������2 ��float3�����Ľṹ�壬
		��ʵ���ϻ�ռ��2 ��float4�Ĵ�������ʱ��ʹ��SetRawValue ��ýṹ���ʵ���������ݣ��ᵼ�����ݴ�λ����SetValue��
		��ִ�����ݶ��룬������ɫ���������ݡ���ˣ����SetValue��˵��SetRawValue ʡȥ�����ݶ��������Ч��Ҳ����ߡ�

	3.	HLSL�еľ���Ĭ���ǰ�Column-Major����ģ����ʹ��SetRawValue ���ݾ���ʱ��Ҫ�Ƚ�����ת��
	
	ToDo��
	2016-03-02 
		A.	ĿǰShader����Ʋ�֧�ֶ�Pass��Ⱦ���Ƚ���������Ⱦ�߼�д���ٿ�����չ��
		B.	Ŀǰ��Shader��չ�Ի��ǲ����ߣ���Ҫ��̬����material�ű���technique�ű����ܽ����
			I.	����ʹ�ù̶������֣��ɳ������ɣ����󶨵���Ӧ��GetXXX()�����У��ϼ򵥣�
			II.	technique �ɳ���̬���ɣ��Ա�����Ⱦ���̣�֧�ֶ�pass��Ⱦ��ʵ���ѶȽϴ�
	
	2016-04-21
		Ŀǰ��Shader��Vertex Shader��Pixel Shader �������һ���Ժ���Ҫ���Ƿֿ����ǣ���ʵ�ָ��õ���չ�Ժ�����

	2016-05-05
		Ŀǰ��Shaderʹ����ͨ�õĿ�ܣ��Ժ���ܻ��в���Ч��ʹ�õ�Shader�����������ܣ���ʱ���Խ�Shader������һ����
		ͬ�Ļ��࣬��ͨ���̳�����չ��ͬ��Shader

	2016-05-24
		���ò��ʳ����Ŀ���ԼΪ�л�Shader������һ�룬���߲�ಢ�����ر����ʱ�����һ�½����ʽ����ֱ����Ϊ�궨�崫
		�ݸ�Shader�����л��Ƶ�Ч�ʶԱȡ�ֱ�ӽ�������Ϊ�궨�崫�ݸ�Shader���Խ����ּ����ڱ�����ִ�У�����Ⱦ��Ϊƿ��ʱ
		����Ч�ʸ��ߣ��������ַ�ʽ�����Խ�ʡ���ֲ�������Ŀ��������ڳ����궨����ShaderKey �ļ������⣬���Գ���ʹ����
		����TextureHashKey�ķ�ʽ�����
\*--------------------------------------------------------------------------------------------------------------------*/


#pragma once

#include <d3dx9.h>
#include "RwgeShaderKey.h"
#include <RwgeTString.h>

class RD3d9Device;
class ShaderType;
class RMaterial;
class RLight;
class RD3d9Texture;

struct PrimitiveTransform
{
	D3DXMATRIX world;
	D3DXMATRIX worldViewProj;
};

struct SHCoefficients
{
	D3DXVECTOR4 R;
	D3DXVECTOR4 G;
	D3DXVECTOR4 B;
};

class RD3d9Shader : public RObject
{
	friend class RD3d9ShaderManager;

private:
	RD3d9Shader(const RShaderKey& key, LPD3DXEFFECTPOOL pEffectPool = nullptr);
	~RD3d9Shader();

public:
	void Begin();
	void End();
	void CommitChanges() const;

	void SetOppositeView(const D3DXVECTOR3* pDirection);
	void SetLight(const RLight* pLight);
	void SetMaterial(const RMaterial* pMaterial);
	void SetTexture(unsigned int u32Index, const RD3d9Texture* pTexture);
	void SetTransform(const D3DXMATRIX* pWorld, const D3DXMATRIX* pViewProjection);

	FORCE_INLINE bool IsSuccessLoaded() const { return m_bSuccessLoaded; };

private:
	void ClearBoundingTextures();

private:
	Rwge::tstring			m_strBinaryFilePath;
	RShaderKey				m_ShaderKey;

	LPD3DXEFFECT			m_pEffect;

	D3DXHANDLE				m_hOppositeView;
	D3DXHANDLE				m_hLight;
	D3DXHANDLE				m_hMaterial;
	D3DXHANDLE				m_hPrimitiveTransform;

	bool					m_bSuccessLoaded;
	unsigned char			m_u8TextureCount;
	D3DXHANDLE*				m_aryTextureHandles;
	RD3d9Texture**			m_aryBoundingTextures;		// ��ǰ�󶨵���������
};
