#ifndef __SH__
#define __SH__

// 环境贴图取样并与一阶球谐函数相乘的结果（常数项已经预计算将结果存在常数系数中）,
// 直接与像素法向量求点积就可以得到像素点所接受的环境光照
// 其中，每个颜色通道R/G/B需要分别计算
struct OneBandSHCoefficients
{
	float4 R;
	float4 G;
	float4 B;
};

OneBandSHCoefficients g_OneBandSHCoefficients;

half3 DotSH(OneBandSHCoefficients coefficients, half3 normal)
{
	half3 result = 0;
	half4 homogeneousNormal = half4(normal, 1);
	result.r = dot(coefficients.R, homogeneousNormal);
	result.g = dot(coefficients.G, homogeneousNormal);
	result.b = dot(coefficients.B, homogeneousNormal);

	return result;
}

#endif // __SH__