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
	pBaseColorExpression->SetTextureInfo(TextureManager::GetInstance().GetTextureInfo("WoodenBox.jpg"));

	pMaterial->m_BaseColor.SetExpression(pBaseColorExpression);
	pMaterial->m_EmissiveColor.SetConstant(FColorRGB(0.0f));
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
