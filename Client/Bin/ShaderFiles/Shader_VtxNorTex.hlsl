
#include "../../../EngineSDK/hlsl/Engine_Shader_Define.hlsli"
/*For Terrain*/
float4x4		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

vector g_vLightDir = float4(1.f, -1.f, 1.f, 0.f);
vector g_vLightDiffuse = float4(1.f, 1.f, 1.f, 1.f);
vector g_vLightAmbient = float4(1.f, 1.f, 1.f, 1.f);
vector g_vLightSpecular = float4(1.f, 1.f, 1.f, 1.f);
/*For Spliting*/
texture2D g_DiffuseTexture[2];
texture2D g_MaskTexture;
texture2D g_BrushTexture;


vector g_vMtrlAmbient = float4(0.3f, 0.3f, 0.3f, 1.f);
vector g_vMtrlSpecular = float4(1.f, 1.f, 1.f, 1.f);

vector g_vCamPosition;

/*마방진 그리기(마스킹)*/
vector g_vBrushPos = float4(20.f, 0.f, 20.f, 1.f);
float g_fBrushRange = 10.f;

struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float2		vTexcoord : TEXCOORD0;	
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
    float4		vNormal	  : NORMAL;	
    float2		vTexcoord : TEXCOORD0;
    float4		vWorldPos : TEXCOORD1;
};

/* 1. 정점위치에 대한 기초적인 변환과정을 수행한다.(월드변환, 뷰변환, ) */

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
    Out.vNormal = normalize(mul(float4(In.vNormal, 0.f), g_WorldMatrix));
	Out.vTexcoord = In.vTexcoord;
    Out.vWorldPos = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
	
	return Out;
}



struct PS_IN
{
	float4		vPosition : SV_POSITION;
    float4		vNormal   : NORMAL;
	float2		vTexcoord : TEXCOORD0;
    float4		vWorldPos :	TEXCOORD1;
};

struct PS_OUT
{
	float4		vColor : SV_TARGET0;
};

/* PixelShader */
PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    vector vSourDiffuse = g_DiffuseTexture[0].Sample(LinearSampler, In.vTexcoord * 50.f);
    vector vDestDiffuse = g_DiffuseTexture[1].Sample(LinearSampler, In.vTexcoord * 50.f);
    vector vMask = g_MaskTexture.Sample(LinearSampler, In.vTexcoord);
    vector vBrush = /**/(vector) 0.f;

    if (g_vBrushPos.x - g_fBrushRange <= In.vWorldPos.x && In.vWorldPos.x < g_vBrushPos.x + g_fBrushRange &&
		g_vBrushPos.z - g_fBrushRange <= In.vWorldPos.z && In.vWorldPos.z < g_vBrushPos.z + g_fBrushRange)
    {
        float2 vTexcoord = 0.f;
		
        vTexcoord.x = (In.vWorldPos.x - (g_vBrushPos.x - g_fBrushRange)) / (2.f * g_fBrushRange);
        vTexcoord.y = ((g_vBrushPos.z + g_fBrushRange) - In.vWorldPos.z) / (2.f * g_fBrushRange);

        vBrush = g_BrushTexture.Sample(LinearSampler, vTexcoord);
    }

    vector vMtrlDiffuse = vDestDiffuse * vMask + vSourDiffuse * (1.f - vMask) + vBrush;

	
	/* 명암 */
    float4 vShade = saturate(max(dot(normalize(g_vLightDir) * -1.f, In.vNormal), 0.f) + (g_vLightAmbient * g_vMtrlAmbient));
	
	/* 0.f ~ 1.f */
	/* 빛이 들어와서 맞고 튕긴 반사벡터와 이 픽셀을 바라보는 시선벡터가 이루는 각이 180도 일때 최대 */
    float4 vReflect = reflect(normalize(g_vLightDir), normalize(In.vNormal));
    float4 vLook = In.vWorldPos - g_vCamPosition;

    float fSpecular = pow(max(dot(normalize(vReflect) * -1.f, normalize(vLook)), 0.f), 50.f);

	/* (g_vLightDiffuse * vMtrlDiffuse) : 빛을 받은 픽셀이 반사하는 색상.  */
	/* (g_vLightSpecular * vMtrlSpecular) : 빛을 받은 픽셀이 반사하는 색상.  */
    Out.vColor = (g_vLightDiffuse * vMtrlDiffuse) * vShade + (g_vLightSpecular * g_vMtrlSpecular) * fSpecular;

    return Out;
}


technique11			DefaultTechnique
{
	
	pass DefaultPass
	{
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		
		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}


}
