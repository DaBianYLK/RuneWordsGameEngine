/*--------------------------------------------------------------------------------------------------------------------*\
   【CREATE】
	AUTH :	大便一箩筐																			   DATE : 2016-05-05
	DESC :	
	1.	每个表达式类型都对应了一个ID，这个ID与着色器文件中材质表达式的宏定义一一对应
	2.	表达式用于将着色器输入与着色器输出进行连接
	3.	纹理取样可以看做一个材质表达式，它的输入参数是一张纹理贴图，输出是一个常量
	4.	由于不同表达式的输入数据类型、类型数量都有可能不同，所以只能通过继承与多态实现兼容
	5.	所有材质表达式的常量都是由float 类型组成的，bool、int 等类型都需要转化为float 存储，再传递给着色器
		注：这么做的依据是着色器中所有的变量都是按float格式储存的，所以CPU 端也保持同步

	ToDo:
	2016-05-05
		材质表达式可能会用到顶点声明中的数据作为参数，因此需要加入相应的标志字，让引擎可以判断材质与顶点声明是否兼容
\*--------------------------------------------------------------------------------------------------------------------*/

#pragma once

#include <RwgeCoreDef.h>
#include <RwgeEnumAsByte.h>
#include "RwgeMaterialExpressionID.h"

class RD3d9Texture;

template<typename ReturnType>
class MaterialExpression
{
public:
	MaterialExpression() {};
	virtual ~MaterialExpression() {};

	virtual unsigned char GetExpressionID() const = 0;

	virtual unsigned short GetConstantCount() { return 0; };
	virtual float* GetConstants() { return nullptr; };			// 表达式不使用常量时返回nullptr

	virtual unsigned char GetTextureCount() { return 0; };
	virtual RD3d9Texture** GetTextures() { return nullptr; };	// 表达式不使用纹理时返回nullptr
};

template<typename ReturnType, EMaterialExpressionID ExpressionID>
class MaterialExpressionWithID : public MaterialExpression<ReturnType>
{
public:
	MaterialExpressionWithID() {};
	virtual ~MaterialExpressionWithID() {};

	virtual unsigned char GetExpressionID() const override { return m_ExpressionID; };

private:
	static const EMaterialExpressionID m_ExpressionID = ExpressionID;
};