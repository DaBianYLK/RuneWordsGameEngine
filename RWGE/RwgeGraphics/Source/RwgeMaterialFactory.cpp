#include "RwgeMaterialFactory.h"

#include "RwgeMExp2dTextureSample.h"
#include "RwgeTextureManager.h"
#include "RwgeMExpConstant.h"

RMaterial* MaterialFactory::CreateWhiteMaterial()
{
	RMaterial* pMaterial = new RMaterial();

	pMaterial->SetEmissiveColorExpression(new MExpConstantColor(D3DXVECTOR3(1.0f, 1.0f, 1.0f)));
	pMaterial->m_ShadingMode = ESM_Unlit;

	pMaterial->Update();

	return pMaterial;
}

RMaterial* MaterialFactory::CreateWoodenBoxMaterial()
{
	RMaterial* pMaterial = new RMaterial();

	MExp2dTextureSampleRGB* pBaseColorExpression = new MExp2dTextureSampleRGB(RTextureManager::GetInstance().GetTexture("textures/WoodenBox.jpg"));
	MExp2dTextureSampleRGB* pNormalExpression = new MExp2dTextureSampleRGB(RTextureManager::GetInstance().GetTexture("textures/WoodenBox-Normal.png"));

	pMaterial->SetBaseColorExpression(pBaseColorExpression);
	pMaterial->SetEmissiveColorExpression(new MExpConstantColor(D3DXVECTOR3(0.0f, 0.0f, 0.0f)));
	pMaterial->SetNormalExpression(pNormalExpression);
	pMaterial->SetMetallicExpression(new MExpConstantScalar(0.0f));
	pMaterial->SetSpecularExpression(new MExpConstantScalar(0.0f));
	pMaterial->SetRoughnessExpression(new MExpConstantScalar(0.8f));
	pMaterial->SetOpacityExpression(new MExpConstantScalar(1.0f));
	pMaterial->SetOpacityMaskExpression(new MExpConstantScalar(1.0f));

	pMaterial->m_bTwoSided = false;
	pMaterial->m_fOpacityMaskClipValue = 1.0f;
	pMaterial->m_BlendMode = EBM_Opaque;
	pMaterial->m_ShadingMode = ESM_Default;

	pMaterial->Update();

	return pMaterial;
}

RMaterial* MaterialFactory::CreateMetalBoxMaterial()
{
	RMaterial* pMaterial = new RMaterial();

	MExp2dTextureSampleRGB* pBaseColorExpression = new MExp2dTextureSampleRGB(RTextureManager::GetInstance().GetTexture("textures/WoodenBox.jpg"));
	MExp2dTextureSampleRGB* pNormalExpression = new MExp2dTextureSampleRGB(RTextureManager::GetInstance().GetTexture("textures/WoodenBox-Normal.png"));

	pMaterial->SetBaseColorExpression(pBaseColorExpression);
	pMaterial->SetEmissiveColorExpression(new MExpConstantColor(D3DXVECTOR3(0.0f, 0.0f, 0.0f)));
	pMaterial->SetNormalExpression(pNormalExpression);
	pMaterial->SetMetallicExpression(new MExpConstantScalar(0.2f));
	pMaterial->SetSpecularExpression(new MExpConstantScalar(0.1f));
	pMaterial->SetRoughnessExpression(new MExpConstantScalar(0.5f));
	pMaterial->SetOpacityExpression(new MExpConstantScalar(1.0f));
	pMaterial->SetOpacityMaskExpression(new MExpConstantScalar(1.0f));

	pMaterial->m_bTwoSided = false;
	pMaterial->m_fOpacityMaskClipValue = 1.0f;
	pMaterial->m_BlendMode = EBM_Opaque;
	pMaterial->m_ShadingMode = ESM_Default;

	pMaterial->Update();

	return pMaterial;
}

RMaterial* MaterialFactory::CreateWoodenBoxMaterialWithoutNormalMap()
{
	RMaterial* pMaterial = new RMaterial();

	MExp2dTextureSampleRGB* pBaseColorExpression = new MExp2dTextureSampleRGB(RTextureManager::GetInstance().GetTexture("textures/WoodenBox.jpg"));

	pMaterial->SetBaseColorExpression(pBaseColorExpression);
	pMaterial->SetEmissiveColorExpression(new MExpConstantColor(D3DXVECTOR3(0.0f, 0.0f, 0.0f)));
	pMaterial->SetNormalExpression(new MExpConstantVector(D3DXVECTOR3(0.0f, 0.0f, 1.0f)));
	pMaterial->SetMetallicExpression(new MExpConstantScalar(0.0f));
	pMaterial->SetSpecularExpression(new MExpConstantScalar(0.0f));
	pMaterial->SetRoughnessExpression(new MExpConstantScalar(0.8f));
	pMaterial->SetOpacityExpression(new MExpConstantScalar(1.0f));
	pMaterial->SetOpacityMaskExpression(new MExpConstantScalar(1.0f));

	pMaterial->m_bTwoSided = false;
	pMaterial->m_fOpacityMaskClipValue = 1.0f;
	pMaterial->m_BlendMode = EBM_Opaque;
	pMaterial->m_ShadingMode = ESM_Default;

	pMaterial->Update();

	return pMaterial;
}

RMaterial* MaterialFactory::CreateMetalBoxMaterialWithoutNormalMap()
{
	RMaterial* pMaterial = new RMaterial();

	MExp2dTextureSampleRGB* pBaseColorExpression = new MExp2dTextureSampleRGB(RTextureManager::GetInstance().GetTexture("textures/WoodenBox.jpg"));

	pMaterial->SetBaseColorExpression(pBaseColorExpression);
	pMaterial->SetEmissiveColorExpression(new MExpConstantColor(D3DXVECTOR3(0.0f, 0.0f, 0.0f)));
	pMaterial->SetNormalExpression(new MExpConstantVector(D3DXVECTOR3(0.0f, 0.0f, 1.0f)));
	pMaterial->SetMetallicExpression(new MExpConstantScalar(0.2f));
	pMaterial->SetSpecularExpression(new MExpConstantScalar(0.1f));
	pMaterial->SetRoughnessExpression(new MExpConstantScalar(0.5f));
	pMaterial->SetOpacityExpression(new MExpConstantScalar(1.0f));
	pMaterial->SetOpacityMaskExpression(new MExpConstantScalar(1.0f));

	pMaterial->m_bTwoSided = false;
	pMaterial->m_fOpacityMaskClipValue = 1.0f;
	pMaterial->m_BlendMode = EBM_Opaque;
	pMaterial->m_ShadingMode = ESM_Default;

	pMaterial->Update();

	return pMaterial;
}

RMaterial* MaterialFactory::CreateZhanHunBodyMaterial()
{
	RMaterial* pMaterial = new RMaterial();

	MExp2dTextureSampleRGB* pBaseColorExpression = new MExp2dTextureSampleRGB(RTextureManager::GetInstance().GetTexture("textures/zhanhun1_cloth.bmp"));
	MExp2dTextureSampleRGB* pNormalExpression = new MExp2dTextureSampleRGB(RTextureManager::GetInstance().GetTexture("textures/zhanhun1_cloth-Normal.png"));
	
	pMaterial->SetBaseColorExpression(pBaseColorExpression);
	pMaterial->SetEmissiveColorExpression(new MExpConstantColor(D3DXVECTOR3(0.0f, 0.0f, 0.0f)));
	pMaterial->SetNormalExpression(pNormalExpression);
	pMaterial->SetMetallicExpression(new MExpConstantScalar(0.2f));
	pMaterial->SetSpecularExpression(new MExpConstantScalar(0.1f));
	pMaterial->SetRoughnessExpression(new MExpConstantScalar(0.5f));
	pMaterial->SetOpacityExpression(new MExpConstantScalar(1.0f));
	pMaterial->SetOpacityMaskExpression(new MExpConstantScalar(1.0f));

	pMaterial->m_bTwoSided = false;
	pMaterial->m_fOpacityMaskClipValue = 1.0f;
	pMaterial->m_BlendMode = EBM_Opaque;
	pMaterial->m_ShadingMode = ESM_Default;

	pMaterial->Update();

	return pMaterial;
}


RMaterial* MaterialFactory::CreateZhanHunBodyMaterialWithoutNormalMap()
{
	RMaterial* pMaterial = new RMaterial();

	MExp2dTextureSampleRGB* pBaseColorExpression = new MExp2dTextureSampleRGB(RTextureManager::GetInstance().GetTexture("textures/zhanhun1_cloth.bmp"));

	pMaterial->SetBaseColorExpression(pBaseColorExpression);
	pMaterial->SetEmissiveColorExpression(new MExpConstantColor(D3DXVECTOR3(0.0f, 0.0f, 0.0f)));
	pMaterial->SetNormalExpression(new MExpConstantVector(D3DXVECTOR3(0.0f, 0.0f, 1.0f)));
	pMaterial->SetMetallicExpression(new MExpConstantScalar(0.2f));
	pMaterial->SetSpecularExpression(new MExpConstantScalar(0.1f));
	pMaterial->SetRoughnessExpression(new MExpConstantScalar(0.5f));
	pMaterial->SetOpacityExpression(new MExpConstantScalar(1.0f));
	pMaterial->SetOpacityMaskExpression(new MExpConstantScalar(1.0f));

	pMaterial->m_bTwoSided = false;
	pMaterial->m_fOpacityMaskClipValue = 1.0f;
	pMaterial->m_BlendMode = EBM_Opaque;
	pMaterial->m_ShadingMode = ESM_Default;

	pMaterial->Update();

	return pMaterial;
}

RMaterial* MaterialFactory::CreateZhanHunShoulderMaterial()
{
	RMaterial* pMaterial = new RMaterial();

	MExp2dTextureSampleRGB* pBaseColorExpression = new MExp2dTextureSampleRGB(RTextureManager::GetInstance().GetTexture("textures/alpha_zhanhun1_cloth.tga"));
	MExp2dTextureSampleA* pOpacityExpression = new MExp2dTextureSampleA(RTextureManager::GetInstance().GetTexture("textures/alpha_zhanhun1_cloth.tga"));

	pMaterial->SetBaseColorExpression(pBaseColorExpression);
	pMaterial->SetEmissiveColorExpression(new MExpConstantColor(D3DXVECTOR3(0.0f, 0.0f, 0.0f)));
	pMaterial->SetNormalExpression(new MExpConstantVector(D3DXVECTOR3(0.0f, 0.0f, 1.0f)));
	pMaterial->SetMetallicExpression(new MExpConstantScalar(0.0f));
	pMaterial->SetSpecularExpression(new MExpConstantScalar(0.1f));
	pMaterial->SetRoughnessExpression(new MExpConstantScalar(0.8f));
	pMaterial->SetOpacityExpression(pOpacityExpression);
	pMaterial->SetOpacityMaskExpression(new MExpConstantScalar(1.0f));

	pMaterial->m_bTwoSided = false;
	pMaterial->m_fOpacityMaskClipValue = 1.0f;
	pMaterial->m_BlendMode = EBM_Translucent;
	pMaterial->m_ShadingMode = ESM_Default;

	pMaterial->Update();

	return pMaterial;
}

RMaterial* MaterialFactory::CreateZhanHunHeadMaterial()
{
	RMaterial* pMaterial = new RMaterial();

	MExp2dTextureSampleRGB* pBaseColorExpression = new MExp2dTextureSampleRGB(RTextureManager::GetInstance().GetTexture("textures/man_head_a.bmp"));

	pMaterial->SetBaseColorExpression(pBaseColorExpression);
	pMaterial->SetEmissiveColorExpression(new MExpConstantColor(D3DXVECTOR3(0.0f, 0.0f, 0.0f)));
	pMaterial->SetNormalExpression(new MExpConstantVector(D3DXVECTOR3(0.0f, 0.0f, 1.0f)));
	pMaterial->SetMetallicExpression(new MExpConstantScalar(0.0f));
	pMaterial->SetSpecularExpression(new MExpConstantScalar(0.1f));
	pMaterial->SetRoughnessExpression(new MExpConstantScalar(0.8f));
	pMaterial->SetOpacityExpression(new MExpConstantScalar(1.0f));
	pMaterial->SetOpacityMaskExpression(new MExpConstantScalar(1.0f));

	pMaterial->m_bTwoSided = false;
	pMaterial->m_fOpacityMaskClipValue = 1.0f;
	pMaterial->m_BlendMode = EBM_Opaque;
	pMaterial->m_ShadingMode = ESM_Default;

	pMaterial->Update();

	return pMaterial;
}

RMaterial* MaterialFactory::CreateZhanHunHandMaterial()
{
	RMaterial* pMaterial = new RMaterial();

	MExp2dTextureSampleRGB* pBaseColorExpression = new MExp2dTextureSampleRGB(RTextureManager::GetInstance().GetTexture("textures/zhanhun1_hand.bmp"));

	pMaterial->SetBaseColorExpression(pBaseColorExpression);
	pMaterial->SetEmissiveColorExpression(new MExpConstantColor(D3DXVECTOR3(0.0f, 0.0f, 0.0f)));
	pMaterial->SetNormalExpression(new MExpConstantVector(D3DXVECTOR3(0.0f, 0.0f, 1.0f)));
	pMaterial->SetMetallicExpression(new MExpConstantScalar(0.2f));
	pMaterial->SetSpecularExpression(new MExpConstantScalar(0.1f));
	pMaterial->SetRoughnessExpression(new MExpConstantScalar(0.5f));
	pMaterial->SetOpacityExpression(new MExpConstantScalar(1.0f));
	pMaterial->SetOpacityMaskExpression(new MExpConstantScalar(1.0f));

	pMaterial->m_bTwoSided = false;
	pMaterial->m_fOpacityMaskClipValue = 1.0f;
	pMaterial->m_BlendMode = EBM_Opaque;
	pMaterial->m_ShadingMode = ESM_Default;

	pMaterial->Update();

	return pMaterial;
}

RMaterial* MaterialFactory::CreateZhanHunHairMaterial()
{
	RMaterial* pMaterial = new RMaterial();

	MExp2dTextureSampleRGB* pBaseColorExpression = new MExp2dTextureSampleRGB(RTextureManager::GetInstance().GetTexture("textures/alphat_zhanhun1_hair.tga"));

	pMaterial->SetBaseColorExpression(pBaseColorExpression);
	pMaterial->SetEmissiveColorExpression(new MExpConstantColor(D3DXVECTOR3(0.0f, 0.0f, 0.0f)));
	pMaterial->SetNormalExpression(new MExpConstantVector(D3DXVECTOR3(0.0f, 0.0f, 1.0f)));
	pMaterial->SetMetallicExpression(new MExpConstantScalar(0.2f));
	pMaterial->SetSpecularExpression(new MExpConstantScalar(0.1f));
	pMaterial->SetRoughnessExpression(new MExpConstantScalar(0.5f));
	pMaterial->SetOpacityExpression(new MExpConstantScalar(1.0f));
	pMaterial->SetOpacityMaskExpression(new MExpConstantScalar(1.0f));

	pMaterial->m_bTwoSided = false;
	pMaterial->m_fOpacityMaskClipValue = 1.0f;
	pMaterial->m_BlendMode = EBM_Opaque;
	pMaterial->m_ShadingMode = ESM_Default;

	pMaterial->Update();

	return pMaterial;
}

RMaterial* MaterialFactory::CreateBackgroundMaterial()
{
	RMaterial* pMaterial = new RMaterial();

	MExp2dTextureSampleRGB* pEmissiveColorExpression = new MExp2dTextureSampleRGB(RTextureManager::GetInstance().GetTexture("textures/Background.png"));

	pMaterial->SetEmissiveColorExpression(pEmissiveColorExpression);
	pMaterial->m_ShadingMode = ESM_Unlit;

	pMaterial->Update();

	return pMaterial;
}
