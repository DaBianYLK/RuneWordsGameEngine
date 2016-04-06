#include "MaterialFactory.h"

#include "MExpTexture2DSample.h"
#include "TextureManager.h"

Material* MaterialFactory::CreateWhiteMaterial()
{
	Material* pMaterial = new Material();

	pMaterial->m_BaseColor.SetConstant(FColorRGB(1.0f));
	pMaterial->m_EmissiveColor.SetConstant(FColorRGB(1.0f));
	pMaterial->m_Normal.SetConstant(D3DXVECTOR3(0.0f, 0.0f, 1.0f));
	pMaterial->m_Metallic.SetConstant(0.0f);
	pMaterial->m_Specular.SetConstant(1.0f);
	pMaterial->m_Roughness.SetConstant(1.0f);
	pMaterial->m_Opacity.SetConstant(1.0f);
	pMaterial->m_OpacityMask.SetConstant(1.0f);

	pMaterial->m_bTwoSided = false;
	pMaterial->m_fOpacityMaskClipValue = 1.0f;
	pMaterial->m_BlendMode = BM_Opaque;
	pMaterial->m_ShadingMode = SM_Default;

	return pMaterial;
}

Material* MaterialFactory::CreateWoodenBoxMaterial()
{
	Material* pMaterial = new Material();

	MExpTexture2DSample* pBaseColorExpression = new MExpTexture2DSample();
	pBaseColorExpression->SetTextureInfo(TextureManager::GetInstance().GetTextureInfo("textures/WoodenBox.jpg"));

	MExpTexture2DSample* pNormalExpression = new MExpTexture2DSample();
	pNormalExpression->SetTextureInfo(TextureManager::GetInstance().GetTextureInfo("textures/WoodenBox-Normal.png"));

	pMaterial->m_BaseColor.SetExpression(pBaseColorExpression);
	pMaterial->m_EmissiveColor.SetConstant(FColorRGB(0.0f));
	//pMaterial->m_Normal.SetConstant(D3DXVECTOR3(0.0f, 0.0f, 1.0f));
	pMaterial->m_Normal.SetExpression(pNormalExpression);
	pMaterial->m_Metallic.SetConstant(0.0f);
	pMaterial->m_Specular.SetConstant(0.0f);
	pMaterial->m_Roughness.SetConstant(0.8f);
	pMaterial->m_Opacity.SetConstant(1.0f);
	pMaterial->m_OpacityMask.SetConstant(1.0f);

	pMaterial->m_bTwoSided = false;
	pMaterial->m_fOpacityMaskClipValue = 1.0f;
	pMaterial->m_BlendMode = BM_Opaque;
	pMaterial->m_ShadingMode = SM_Default;

	return pMaterial;
}

Material* MaterialFactory::CreateMetalBoxMaterial()
{
	Material* pMaterial = new Material();

	MExpTexture2DSample* pBaseColorExpression = new MExpTexture2DSample();
	pBaseColorExpression->SetTextureInfo(TextureManager::GetInstance().GetTextureInfo("textures/WoodenBox.jpg"));

	MExpTexture2DSample* pNormalExpression = new MExpTexture2DSample();
	pNormalExpression->SetTextureInfo(TextureManager::GetInstance().GetTextureInfo("textures/WoodenBox-Normal.png"));

	pMaterial->m_BaseColor.SetExpression(pBaseColorExpression);
	pMaterial->m_EmissiveColor.SetConstant(FColorRGB(0.0f));
	//pMaterial->m_Normal.SetConstant(D3DXVECTOR3(0.0f, 0.0f, 1.0f));
	pMaterial->m_Normal.SetExpression(pNormalExpression);
	pMaterial->m_Metallic.SetConstant(0.2f);
	pMaterial->m_Specular.SetConstant(0.1f);
	pMaterial->m_Roughness.SetConstant(0.5f);
	pMaterial->m_Opacity.SetConstant(1.0f);
	pMaterial->m_OpacityMask.SetConstant(1.0f);

	pMaterial->m_bTwoSided = false;
	pMaterial->m_fOpacityMaskClipValue = 1.0f;
	pMaterial->m_BlendMode = BM_Opaque;
	pMaterial->m_ShadingMode = SM_Default;

	return pMaterial;
}

Material* MaterialFactory::CreateWoodenBoxMaterialWithoutNormalMap()
{
	Material* pMaterial = new Material();

	MExpTexture2DSample* pBaseColorExpression = new MExpTexture2DSample();
	pBaseColorExpression->SetTextureInfo(TextureManager::GetInstance().GetTextureInfo("textures/WoodenBox.jpg"));

	pMaterial->m_BaseColor.SetExpression(pBaseColorExpression);
	pMaterial->m_EmissiveColor.SetConstant(FColorRGB(0.0f));
	pMaterial->m_Normal.SetConstant(D3DXVECTOR3(0.0f, 0.0f, 1.0f));
	pMaterial->m_Metallic.SetConstant(0.0f);
	pMaterial->m_Specular.SetConstant(0.0f);
	pMaterial->m_Roughness.SetConstant(0.8f);
	pMaterial->m_Opacity.SetConstant(1.0f);
	pMaterial->m_OpacityMask.SetConstant(1.0f);

	pMaterial->m_bTwoSided = false;
	pMaterial->m_fOpacityMaskClipValue = 1.0f;
	pMaterial->m_BlendMode = BM_Opaque;
	pMaterial->m_ShadingMode = SM_Default;

	return pMaterial;
}

Material* MaterialFactory::CreateMetalBoxMaterialWithoutNormalMap()
{
	Material* pMaterial = new Material();

	MExpTexture2DSample* pBaseColorExpression = new MExpTexture2DSample();
	pBaseColorExpression->SetTextureInfo(TextureManager::GetInstance().GetTextureInfo("textures/WoodenBox.jpg"));

	pMaterial->m_BaseColor.SetExpression(pBaseColorExpression);
	pMaterial->m_EmissiveColor.SetConstant(FColorRGB(0.0f));
	pMaterial->m_Normal.SetConstant(D3DXVECTOR3(0.0f, 0.0f, 1.0f));
	pMaterial->m_Metallic.SetConstant(0.2f);
	pMaterial->m_Specular.SetConstant(0.1f);
	pMaterial->m_Roughness.SetConstant(0.5f);
	pMaterial->m_Opacity.SetConstant(1.0f);
	pMaterial->m_OpacityMask.SetConstant(1.0f);

	pMaterial->m_bTwoSided = false;
	pMaterial->m_fOpacityMaskClipValue = 1.0f;
	pMaterial->m_BlendMode = BM_Opaque;
	pMaterial->m_ShadingMode = SM_Default;

	return pMaterial;
}

Material* MaterialFactory::CreateZhanHunBodyMaterial()
{
	Material* pMaterial = new Material();

	MExpTexture2DSample* pBaseColorExpression = new MExpTexture2DSample();
	pBaseColorExpression->SetTextureInfo(TextureManager::GetInstance().GetTextureInfo("textures/zhanhun1_cloth.bmp"));

	MExpTexture2DSample* pNormalExpression = new MExpTexture2DSample();
	pNormalExpression->SetTextureInfo(TextureManager::GetInstance().GetTextureInfo("textures/zhanhun1_cloth-Normal.png"));
	
	pMaterial->m_BaseColor.SetExpression(pBaseColorExpression);
	pMaterial->m_EmissiveColor.SetConstant(FColorRGB(0.0f));
	pMaterial->m_Normal.SetExpression(pNormalExpression);
	pMaterial->m_Metallic.SetConstant(0.2f);
	pMaterial->m_Specular.SetConstant(0.1f);
	pMaterial->m_Roughness.SetConstant(0.5f);
	pMaterial->m_Opacity.SetConstant(1.0f);
	pMaterial->m_OpacityMask.SetConstant(1.0f);

	pMaterial->m_bTwoSided = false;
	pMaterial->m_fOpacityMaskClipValue = 1.0f;
	pMaterial->m_BlendMode = BM_Opaque;
	pMaterial->m_ShadingMode = SM_Default;

	return pMaterial;
}


Material* MaterialFactory::CreateZhanHunBodyMaterialWithoutNormalMap()
{
	Material* pMaterial = new Material();

	MExpTexture2DSample* pBaseColorExpression = new MExpTexture2DSample();
	pBaseColorExpression->SetTextureInfo(TextureManager::GetInstance().GetTextureInfo("textures/zhanhun1_cloth.bmp"));

	pMaterial->m_BaseColor.SetExpression(pBaseColorExpression);
	pMaterial->m_EmissiveColor.SetConstant(FColorRGB(0.0f));
	pMaterial->m_Normal.SetConstant(D3DXVECTOR3(0.0f, 0.0f, 1.0f));
	pMaterial->m_Metallic.SetConstant(0.2f);
	pMaterial->m_Specular.SetConstant(0.1f);
	pMaterial->m_Roughness.SetConstant(0.5f);
	pMaterial->m_Opacity.SetConstant(1.0f);
	pMaterial->m_OpacityMask.SetConstant(1.0f);

	pMaterial->m_bTwoSided = false;
	pMaterial->m_fOpacityMaskClipValue = 1.0f;
	pMaterial->m_BlendMode = BM_Opaque;
	pMaterial->m_ShadingMode = SM_Default;

	return pMaterial;
}

Material* MaterialFactory::CreateZhanHunShoulderMaterial()
{
	Material* pMaterial = new Material();

	MExpTexture2DSample* pBaseColorExpression = new MExpTexture2DSample();
	pBaseColorExpression->SetTextureInfo(TextureManager::GetInstance().GetTextureInfo("textures/alpha_zhanhun1_cloth.tga"));

	MExpTexture2DSample* pOpacityExpression = new MExpTexture2DSample();
	pOpacityExpression->SetTextureInfo(TextureManager::GetInstance().GetTextureInfo("textures/alpha_zhanhun1_cloth.tga"));

	pMaterial->m_BaseColor.SetExpression(pBaseColorExpression);
	pMaterial->m_EmissiveColor.SetConstant(FColorRGB(0.0f));
	pMaterial->m_Normal.SetConstant(D3DXVECTOR3(0.0f, 0.0f, 1.0f));
	pMaterial->m_Metallic.SetConstant(0.0f);
	pMaterial->m_Specular.SetConstant(0.1f);
	pMaterial->m_Roughness.SetConstant(0.8f);
	pMaterial->m_Opacity.SetExpression(pOpacityExpression);
	pMaterial->m_OpacityMask.SetConstant(1.0f);

	pMaterial->m_bTwoSided = false;
	pMaterial->m_fOpacityMaskClipValue = 1.0f;
	pMaterial->m_BlendMode = BM_Translucent;
	pMaterial->m_ShadingMode = SM_Default;

	return pMaterial;
}

Material* MaterialFactory::CreateZhanHunHeadMaterial()
{
	Material* pMaterial = new Material();

	MExpTexture2DSample* pBaseColorExpression = new MExpTexture2DSample();
	pBaseColorExpression->SetTextureInfo(TextureManager::GetInstance().GetTextureInfo("textures/man_head_a.bmp"));

	pMaterial->m_BaseColor.SetExpression(pBaseColorExpression);
	pMaterial->m_EmissiveColor.SetConstant(FColorRGB(0.0f));
	pMaterial->m_Normal.SetConstant(D3DXVECTOR3(0.0f, 0.0f, 1.0f));
	pMaterial->m_Metallic.SetConstant(0.0f);
	pMaterial->m_Specular.SetConstant(0.1f);
	pMaterial->m_Roughness.SetConstant(0.8f);
	pMaterial->m_Opacity.SetConstant(1.0f);
	pMaterial->m_OpacityMask.SetConstant(1.0f);

	pMaterial->m_bTwoSided = false;
	pMaterial->m_fOpacityMaskClipValue = 1.0f;
	pMaterial->m_BlendMode = BM_Opaque;
	pMaterial->m_ShadingMode = SM_Default;

	return pMaterial;
}

Material* MaterialFactory::CreateZhanHunHandMaterial()
{
	Material* pMaterial = new Material();

	MExpTexture2DSample* pBaseColorExpression = new MExpTexture2DSample();
	pBaseColorExpression->SetTextureInfo(TextureManager::GetInstance().GetTextureInfo("textures/zhanhun1_hand.bmp"));

	pMaterial->m_BaseColor.SetExpression(pBaseColorExpression);
	pMaterial->m_EmissiveColor.SetConstant(FColorRGB(0.0f));
	pMaterial->m_Normal.SetConstant(D3DXVECTOR3(0.0f, 0.0f, 1.0f));
	pMaterial->m_Metallic.SetConstant(0.2f);
	pMaterial->m_Specular.SetConstant(0.1f);
	pMaterial->m_Roughness.SetConstant(0.5f);
	pMaterial->m_Opacity.SetConstant(1.0f);
	pMaterial->m_OpacityMask.SetConstant(1.0f);

	pMaterial->m_bTwoSided = false;
	pMaterial->m_fOpacityMaskClipValue = 1.0f;
	pMaterial->m_BlendMode = BM_Opaque;
	pMaterial->m_ShadingMode = SM_Default;

	return pMaterial;
}

Material* MaterialFactory::CreateZhanHunHairMaterial()
{
	Material* pMaterial = new Material();

	MExpTexture2DSample* pBaseColorExpression = new MExpTexture2DSample();
	pBaseColorExpression->SetTextureInfo(TextureManager::GetInstance().GetTextureInfo("textures/alphat_zhanhun1_hair.tga"));

	pMaterial->m_BaseColor.SetExpression(pBaseColorExpression);
	pMaterial->m_EmissiveColor.SetConstant(FColorRGB(0.0f));
	pMaterial->m_Normal.SetConstant(D3DXVECTOR3(0.0f, 0.0f, 1.0f));
	pMaterial->m_Metallic.SetConstant(0.2f);
	pMaterial->m_Specular.SetConstant(0.1f);
	pMaterial->m_Roughness.SetConstant(0.5f);
	pMaterial->m_Opacity.SetConstant(1.0f);
	pMaterial->m_OpacityMask.SetConstant(1.0f);

	pMaterial->m_bTwoSided = false;
	pMaterial->m_fOpacityMaskClipValue = 1.0f;
	pMaterial->m_BlendMode = BM_Opaque;
	pMaterial->m_ShadingMode = SM_Default;

	return pMaterial;
}

Material* MaterialFactory::CreateBackgroundMaterial()
{
	Material* pMaterial = new Material();

	MExpTexture2DSample* pEmissiveColorExpression = new MExpTexture2DSample();
	pEmissiveColorExpression->SetTextureInfo(TextureManager::GetInstance().GetTextureInfo("textures/Background.png"));

	pMaterial->m_BaseColor.SetConstant(FColorRGB(0.0f));
	pMaterial->m_EmissiveColor.SetExpression(pEmissiveColorExpression);
	pMaterial->m_Normal.SetConstant(D3DXVECTOR3(0.0f, 0.0f, 1.0f));
	pMaterial->m_Metallic.SetConstant(0.0f);
	pMaterial->m_Specular.SetConstant(0.1f);
	pMaterial->m_Roughness.SetConstant(1.0f);
	pMaterial->m_Opacity.SetConstant(1.0f);
	pMaterial->m_OpacityMask.SetConstant(1.0f);

	pMaterial->m_bTwoSided = false;
	pMaterial->m_fOpacityMaskClipValue = 1.0f;
	pMaterial->m_BlendMode = BM_Opaque;
	pMaterial->m_ShadingMode = SM_Unlit;

	return pMaterial;
}
