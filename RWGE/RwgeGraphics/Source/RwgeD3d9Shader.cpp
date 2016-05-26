#include "RwgeD3d9Shader.h"

#include <RwgeAssert.h>
#include <RwgeLog.h>
#include "RwgeGraphics.h"
#include "RwgeD3d9Device.h"
#include "RwgeLight.h"
#include "RwgeMaterial.h"
#include "RwgeD3d9Texture.h"
#include "RwgeShaderCompilerEnv.h"
#include "RwgeShaderKey.h"
#include <RwgeMath.h>

using namespace std;

RD3d9Shader::RD3d9Shader(const RShaderKey& key, LPD3DXEFFECTPOOL pEffectPool /* = nullptr */)
{
	m_strBinaryFilePath = RShaderCompilerEnvironment::GetShaderBinaryPath(key);
	m_ShaderKey = key;

	LPD3DXBUFFER pErrorBuffer = nullptr;
	HRESULT hResult = D3DXCreateEffectFromFile(
		g_pD3d9Device,								// D3D Deviceָ��
		m_strBinaryFilePath.c_str(),				// Shader��Դ�ļ�·��
		nullptr,									// �궨���������������ļ�����Ҫ��
		nullptr,									// Include�������������ļ�����Ҫ��
		0,											// Flag
		pEffectPool,								// EffectPoolָ��
		&m_pEffect,									// Effectָ���ָ��
		&pErrorBuffer);								// ����Error��Ϣָ��

	if (FAILED(hResult))
	{
		RwgeLog(TEXT("Create effect failed : %X, Error info : %s"), hResult, pErrorBuffer);
		m_bSuccessLoaded = false;
		return;
	}

	m_hOppositeView = m_pEffect->GetParameterByName(nullptr, "g_vecOppositeView");
	m_hLight = m_pEffect->GetParameterByName(nullptr, "g_Light");
	m_hMaterial = m_pEffect->GetParameterByName(nullptr, "g_Material");
	m_hPrimitiveTransform = m_pEffect->GetParameterByName(nullptr, "g_Transform");

	m_u8TextureCount = key.GetTextureCountKey();
	if (m_u8TextureCount)
	{
		m_aryTextureHandles = new D3DXHANDLE[m_u8TextureCount];
		m_aryBoundingTextures = new RD3d9Texture*[m_u8TextureCount];

		TCHAR szTextureName[16];
		for (unsigned int i = 0; i < m_u8TextureCount; ++i)
		{
			_stprintf_s(szTextureName, TEXT("g_Texture_%u"), i);
			m_aryTextureHandles[i] = m_pEffect->GetParameterByName(nullptr, szTextureName);

			m_aryBoundingTextures[i] = nullptr;
		}
	}

	m_bSuccessLoaded = true;
}

RD3d9Shader::~RD3d9Shader()
{
	RwgeSafeRelease(m_pEffect);
}

void RD3d9Shader::Begin()
{
	unsigned int uPassCount;
	m_pEffect->Begin(&uPassCount, 0);
	m_pEffect->BeginPass(0);		// ��ʱ�����Ƕ�Pass���ٶ�����Technique���ǵ�Pass
}

void RD3d9Shader::End()
{
	m_pEffect->EndPass();
	m_pEffect->End();

	ClearBoundingTextures();	// ��յ�ǰ�Ѱ󶨵���������
}

void RD3d9Shader::CommitChanges() const
{
	m_pEffect->CommitChanges();
}

void RD3d9Shader::SetOppositeView(const D3DXVECTOR3* pDirection)
{
	RwgeAssert(pDirection);

	m_pEffect->SetRawValue(m_hOppositeView, pDirection, 0, sizeof(float) * 3);
}

void RD3d9Shader::SetLight(const RLight* pLight)
{
	RwgeAssert(pLight);

	m_pEffect->SetValue(m_hLight, pLight->GetConstants(), pLight->GetConstantCount() * sizeof(float));
}

void RD3d9Shader::SetMaterial(const RMaterial* pMaterial)
{
	RwgeAssert(pMaterial);

	// ������Ⱦ״̬
	if (pMaterial->GetBlendMode() == EBM_Masked)
	{
		RwgeAssert(pMaterial->GetOpacityMaskClipValue() <= 1.0f && pMaterial->GetOpacityMaskClipValue() >= 0.0f);

		/* ������Ⱦ״̬��������Effect */
		// 1.	����Ŀǰû������VertexShader��PixelShader����������Shader������Ϊ����Effect
		//		ʵ����Effect��ͨ���ű�������Device������Ⱦ״̬�ģ�������Ⱦ״̬�����ÿ�����Ϊ������Shader�Ĺ���
		// 2.	����һ��ʵ�ַ�ʽ�ǽ�MaskClipValue��ֵͨ���궨�崫�ݸ�Shader��
		//		�������˵Ч�ʽϵͣ�����D3DRS_ALPHAREF�Ŀ���ԼΪ500��CPUʱ�����ڣ��л�Shader�Ŀ���ͨ����5000ʱ����������
		g_pD3d9Device->SetRenderState(D3DRS_ALPHAREF, static_cast<unsigned long>(pMaterial->GetOpacityMaskClipValue() * RwgeMath::u8Max));
	}

	// �󶨳���
	m_pEffect->SetValue(m_hMaterial, pMaterial->GetConstants(), pMaterial->GetConstantCount() * sizeof(float));

	// ������
	RwgeAssert(m_u8TextureCount == pMaterial->GetTextureCount());
	RD3d9Texture** pTextureAry = pMaterial->GetTextures();
	for (unsigned char i = 0; i < m_u8TextureCount; ++i)
	{
		if (pTextureAry[i] != m_aryBoundingTextures[i])
		{
			SetTexture(i, pTextureAry[i]);
		}
	}
}

void RD3d9Shader::SetTexture(unsigned u32Index, const RD3d9Texture* pTexture)
{
	RwgeAssert(pTexture);
	RwgeAssert(u32Index < 16);

	m_pEffect->SetTexture(m_aryTextureHandles[u32Index], pTexture->GetD3DTexture());
	m_aryBoundingTextures[u32Index] = const_cast<RD3d9Texture*>(pTexture);		// Shader����ı�Texture�������ת����Ϊ�˱��淽��
}

void RD3d9Shader::SetTransform(const D3DXMATRIX* pWorld, const D3DXMATRIX* pViewProjection)
{
	RwgeAssert(pWorld);
	RwgeAssert(pViewProjection);

	static PrimitiveTransform transform;
	D3DXMatrixTranspose(&transform.world, pWorld);
	D3DXMatrixMultiplyTranspose(&transform.worldViewProj, pWorld, pViewProjection);

	m_pEffect->SetRawValue(m_hPrimitiveTransform, &transform, 0, sizeof(PrimitiveTransform));
}

void RD3d9Shader::ClearBoundingTextures()
{
	for (unsigned int i = 0; i < m_u8TextureCount; ++i)
	{
		m_aryBoundingTextures[i] = nullptr;
	}
}