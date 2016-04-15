#pragma once

#include "RwgeMaterial.h"

class MaterialFactory
{
public:
	static Material* CreateWhiteMaterial();
	static Material* CreateWoodenBoxMaterial();
	static Material* CreateMetalBoxMaterial();
	static Material* CreateWoodenBoxMaterialWithoutNormalMap();
	static Material* CreateMetalBoxMaterialWithoutNormalMap();

	static Material* CreateZhanHunBodyMaterialWithoutNormalMap();
	static Material* CreateZhanHunShoulderMaterial();
	static Material* CreateZhanHunBodyMaterial();
	static Material* CreateZhanHunHeadMaterial();
	static Material* CreateZhanHunHandMaterial();
	static Material* CreateZhanHunHairMaterial();

	static Material* CreateBackgroundMaterial();
};
