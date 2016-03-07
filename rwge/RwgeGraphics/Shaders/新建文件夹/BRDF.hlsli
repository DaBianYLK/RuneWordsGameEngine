#ifndef __BRDF__
#define __BRDF__

half EnvBRDFApproxNonmetal(half roughness, half normalDotView)
{
	// 本函数等价于EnvBRDFApprox( 0.04, Roughness, NoV )
	const half2 c0 = { -1, -0.0275 };
	const half2 c1 = {  1,  0.0425 };
	half2 r = roughness * c0 + c1;

	return min(r.x * r.x, exp2(-9.28 * normalDotView)) * r.x + r.y;
}

half3 EnvBRDFApprox(half3 specularColor, half roughness, half normalDotView)
{
	// BRDF方程中G项的一种替代
	const half4 c0 = { -1, -0.0275, -0.572, 0.022 };
	const half4 c1 = {  1,  0.0425,   1.04, -0.04 };
	half4 r = roughness * c0 + c1;
	half a004 = min(r.x * r.x, exp2(-9.28 * normalDotView)) * r.x + r.y;
	half2 AB = half2(-1.04, 1.04) * a004 + r.zw;

	// 任何小于2%的光都是不可见的，因此，它们被认为是阴影。
	AB.y *= saturate(50.0 * specularColor.g);

	return specularColor * AB.x + AB.y;
}

#endif // __BRDF__