/*--------------------------------------------------------------------------------------------------------------------*\
   【CREATE】
	AUTH :	大便一箩筐																			   DATE : 2016-05-24
	DESC :	
	1.	生成与管理材质
\*--------------------------------------------------------------------------------------------------------------------*/


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
