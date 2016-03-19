#pragma once

#include "Material.h"

class MaterialFactory
{
public:
	static Material* CreateWhiteMaterial();
	static Material* CreateWoodenBoxMaterial();
};
