#include "Material.hlsli"
#include "Light.hlsli"
#include "BRDF.hlsli"
#include "SH.hlsli"

//////////////////////////////////////////////////////////////////////////////

struct Transform
{
	matrix	world;
	matrix	view;
	matrix	WVP;
};

Transform	g_Transform;
float3		g_ViewVector;				// 指向视线向量（从视点发射出的向量）的反方向

//////////////////////////////////////////////////////////////////////////////

half PhongApprox(half roughness, half reflectionDotLight)
{
	half a = roughness * roughness;		// 1个MUL（乘法）指令
	half a2 = a * a;					// 1个MUL指令
	//float rcp_a2 = rcp(a2);			// 1个RCP（求倒数）指令(DX11,即ShaderModel 5.0才支持这个指令)
	float rcp_a2 = 1.0 / a2;

	// 球面高斯方程的近似: pow( x, n ) ~= exp( (n + 0.775) * (x - 1) )
	// Phong光照模型: n = 0.5 / a2 - 0.5
	// 0.5 / ln(2), 0.275 / ln(2)
	half c = 0.72134752 * rcp_a2 + 0.39674113;			// 1个MAD指令
	return rcp_a2 * exp2(c * reflectionDotLight - c);	// 2个MAD指令，1个EXP2（求平方）指令，1个MUL指令
}

//////////////////////////////////////////////////////////////////////////////

struct VSOutput
{
	float4 position	: POSITION;
	float2 texCoord : TEXCOORD0;
	float3 tangent  : TANGENT;
	float3 binormal : BINORMAL;
	float3 normal	: NORMAL;
};

VSOutput BaseVS(float4 inPosition	: POSITION, 
				float2 inTexCoord	: TEXCOORD0,
				float3 inNormal		: NORMAL,
				float3 inBinormal	: BINORMAL,
				float3 inTangent	: TANGENT)
{
	VSOutput output = (VSOutput)0;

	output.position = inPosition;
	output.texCoord = inTexCoord;
	output.normal = inNormal;
	output.binormal = inBinormal;
	output.tangent = inTangent;

	return output;
}

//////////////////////////////////////////////////////////////////////////////

half4 BasePS(	float3 inPosition	: POSITION,
				float2 inTexCoord	: TEXCOORD0,
				float3 inNormal		: NORMAL,
				float3 inBinormal	: BINORMAL,
				float3 inTangent	: TANGENT) : COLOR
{
	half3	baseColor		= GetBaseColor(inTexCoord);
	half3	emissiveColor	= GetEmissiveColor(inTexCoord);
	half	metallic		= GetMetallic(inTexCoord);
	half	specular		= GetSpecular(inTexCoord);
	half	roughness		= GetRoughness(inTexCoord);
	half	opacity			= GetOpacity(inTexCoord);

	// 法线需要从切线空间转换到世界空间
	float3	normal = GetNormal(inTexCoord);
	normal = normalize(mul(normal, float3x3(inNormal, inBinormal, inTangent)));

	// ============================= 计算材质的漫反射和高光色 =============================
#ifdef MATERIAL_NONMETAL
	// 如果材质金属性为0，可以简化计算
	half3 diffuseColor = baseColor;
	half  specularColor = 0.04;
#else
	half  dielectricSpecular = 0.08 * specular;
	half3 diffuseColor = baseColor - baseColor * metallic;		// 1个MAD（乘加运算指令）
	half3 specularColor = (dielectricSpecular - dielectricSpecular * metallic) + baseColor * metallic;	// 2个MAD
#endif

#ifdef MATERIAL_FULLY_ROUGH
	// 如果材质粗糙度为1，可以简化计算
	diffuseColor += specular * 0.45;
	specularColor = 0;
#else
	half normalDotView = max(dot(normal, g_ViewVector), 0);

#	ifdef MATERIAL_NONMETAL
	specularColor = EnvBRDFApproxNonmetal(roughness, normalDotView);
#	else
	specularColor = EnvBRDFApprox(specularColor, roughness, normalDotView);
#	endif
#endif
	
	// ============================= 计算环境光（非方向光）的作用 =============================
	half3 diffuseGI = max(half3(0, 0, 0), DotSH(g_OneBandSHCoefficients, normal));

	half  indirectIrradiance = dot(diffuseGI, half3(0.3, 0.59, 0.11));
	half3 color = diffuseColor * diffuseGI;

	// ============================= 阴影计算（略） =============================

	// ============================= 计算方向光的作用 =============================
#ifndef MATERIAL_BLENDING_UNLIT
	half3 reflectionVector = -g_ViewVector + normal * dot(normal, g_ViewVector) * 2.0;
	half3 lightDirection = GetLightWorldDirection(inPosition);
	half  normalDotLight = max(0, dot(normal, lightDirection));
	half reflectionDotLight = max(0, dot(reflectionVector, lightDirection));

	color += GetLightDiffuseColor() * normalDotLight * (diffuseColor + specularColor * PhongApprox(roughness, reflectionDotLight));
#endif

	// ============================= 环境贴图计算（略） =============================

	// ============================= 计算自发光的作用 =============================
	color += emissiveColor;

	// ============================= 计算顶点雾化（略） =============================

	// ============================= 处理材质混合模式 =============================
#ifdef MATERIAL_BLENDING_ADDITIVE		
	half4 outColor = half4(color * opacity, 0.0f);
#else
	half4 outColor = half4(color, opacity);
#endif

	return outColor;
}

//////////////////////////////////////////////////////////////////////////////

technique BaseTechnique
{
	pass BasePass
	{
#ifdef TWO_SIDED
		CullMode = NONE;
#else
		CullMode = CCW;
#endif

#ifdef TWO_SIDED
		CullMode = CCW;
#else
		CullMode = NONE;
#endif
	
		VertexShader = compile vs_3_0 BaseVS();
		PixelShader = compile ps_3_0 BasePS();
	}
}