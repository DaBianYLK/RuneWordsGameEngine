#ifndef __SH__
#define __SH__

// ������ͼȡ������һ����г������˵Ľ�����������Ѿ�Ԥ���㽫������ڳ���ϵ���У�,
// ֱ�������ط����������Ϳ��Եõ����ص������ܵĻ�������
// ���У�ÿ����ɫͨ��R/G/B��Ҫ�ֱ����
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