/*--------------------------------------------------------------------------------------------------------------------*\
   【CREATE】
	AUTH :	大便一箩筐																			   DATE : 2016-05-05
	DESC :	以枚举值记录所有材质表达式的表达式ID，表达式ID的取值范围为[0, 255]
\*--------------------------------------------------------------------------------------------------------------------*/

#pragma once

enum EMaterialExpressionID
{
	EME_Constant			= 0,

	// 单通道2D纹理采样的表达式有多个，分别用于表示取纹理像素中不同通道的值作为采样的返回值
	EME_2dTextureSampleR	= 1,
	EME_2dTextureSampleG	= 2,
	EME_2dTextureSampleB	= 3,
	EME_2dTextureSampleA	= 4,
	
	// 3通道采样和单通道采样分别应用于不同基本类型的材质属性，它们的表达式ID值是互相独立的
	EME_2dTextureSampleRGB	= 1,

	EMaterialExpression_MAX
};