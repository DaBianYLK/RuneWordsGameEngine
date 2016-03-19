#pragma once

class TextureInfo;

/*
1.	每个类型的表达式都对应了一个ID，这个ID与着色器文件中的某个宏定义一一对应
2.	BaseMaterial的每一个材质属性都可以对应一个材质表达式
3.	纹理取样可以看做一个材质表达式，它的输入参数是一张纹理贴图，输出是一个常量
*/

class MaterialExpression
{
public:
	MaterialExpression();
	virtual ~MaterialExpression();

	virtual unsigned char GetExpressionID() const = 0;
	virtual unsigned int GetConstantParamSize() const = 0;

	virtual void GetConstantParamData(void*& pParam, unsigned char& uSize) const = 0;
	virtual unsigned char AddConstantParamToBuffer(void* pBuffer) const = 0;

	virtual TextureInfo* GetTextureInfo() = 0;		// 表达式不使用纹理时返回nullptr，不考虑一个表达式包含多张纹理的情况（通常这种表达式都可以预计算，将多张纹理合成为一张）
};

