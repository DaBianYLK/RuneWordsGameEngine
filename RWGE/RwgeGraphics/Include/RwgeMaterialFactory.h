#pragma once

#include "RwgeMaterial.h"

class MaterialFactory
{
public:
	static RMaterial* CreateWhiteMaterial();
	static RMaterial* CreateWoodenBoxMaterial();
	static RMaterial* CreateMetalBoxMaterial();
	static RMaterial* CreateWoodenBoxMaterialWithoutNormalMap();
	static RMaterial* CreateMetalBoxMaterialWithoutNormalMap();

	static RMaterial* CreateZhanHunBodyMaterialWithoutNormalMap();
	static RMaterial* CreateZhanHunShoulderMaterial();
	static RMaterial* CreateZhanHunBodyMaterial();
	static RMaterial* CreateZhanHunHeadMaterial();
	static RMaterial* CreateZhanHunHandMaterial();
	static RMaterial* CreateZhanHunHairMaterial();

	static RMaterial* CreateBackgroundMaterial();
};
