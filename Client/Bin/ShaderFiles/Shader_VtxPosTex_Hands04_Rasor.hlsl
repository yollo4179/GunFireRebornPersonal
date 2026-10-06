
#include "../../../EngineSDK/hlsl/Engine_Shader_Define.hlsli"


/* 상수테이블 ConstantTable */
float4x4		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D		g_Texture;
texture2D g_EmissiveTexture;
texture2D g_EmissiveTexture_2;
float3 g_vThreshold;
float g_fIntensity;

float g_fTime;
float g_fUVLength;


struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexcoord : TEXCOORD0;	
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexcoord : TEXCOORD0;
  
};

/* 1. 정점위치에 대한 기초적인 변환과정을 수행한다.(월드변환, 뷰변환, ) */

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
	Out.vTexcoord = In.vTexcoord;
  
	return Out;
}


struct PS_IN
{
	float4		vPosition : SV_POSITION; 
	float2		vTexcoord : TEXCOORD0;
  
};

struct PS_OUT
{
	float4		vColor : SV_TARGET0;
   
};

/* PixelShader */
PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	/*Out.vColor = g_Texture.Sample(어떻게얻어올래?, In.vTexcoord);*/
	
    vector vMtrlDiffuse = g_Texture.Sample(LinearSampler, In.vTexcoord);
   
    float2 vEmissiveTexcoord = In.vTexcoord;
    
    if (g_fUVLength>=1.f)
        vEmissiveTexcoord.y = ((int) g_fUVLength * vEmissiveTexcoord.y) + g_fTime * 3.f;
    else
    {
        vEmissiveTexcoord.y = In.vTexcoord + g_fTime * 3.f;

    }
   
 
    vector vEmissive = g_EmissiveTexture.Sample(LinearSampler, vEmissiveTexcoord);
    vector vEmissive2 = g_EmissiveTexture_2.Sample(LinearSampler, vEmissiveTexcoord);
    
    
    vector vFinalColorOrigin = vEmissive;
    vEmissive = saturate(vEmissive - float4(g_vThreshold, 0.f));
  
    if (vEmissive.x > 0.f && vEmissive.y > 0.f && vEmissive.z > 0.f)
    {
        Out.vColor = vMtrlDiffuse;
    }
	
   
    
    vector vFinalColorOrigin2 = vEmissive2;
    vEmissive2 = saturate(vEmissive2 - float4(g_vThreshold, 0.f));
  
    if (vEmissive2.x > 0.f && vEmissive2.y > 0.f && vEmissive2.z > 0.f)
    {
        Out.vColor = vMtrlDiffuse;
    }
	
    if (Out.vColor.a < 0.05f)
        discard;
    
    
  
	return Out;
}

struct PS_RASOR_GLOW_OUT
{
    float4 vColor : SV_TARGET0;
    float4 vIntensity : SV_TARGET1;
};

PS_RASOR_GLOW_OUT PS_MAIN_GLOW(PS_IN In)
{
    PS_RASOR_GLOW_OUT Out = (PS_RASOR_GLOW_OUT) 0;
    
  
    vector vMtrlDiffuse = g_Texture.Sample(LinearSampler, In.vTexcoord);
    float2 vEmissiveTexcoord = In.vTexcoord;
    
    if (g_fUVLength >= 1.f)
        vEmissiveTexcoord.y = ((int) g_fUVLength * vEmissiveTexcoord.y) + g_fTime * 3.f;
    else
    {
        vEmissiveTexcoord.y = In.vTexcoord + g_fTime * 3.f;

    }
 
    vector vEmissive = g_EmissiveTexture.Sample(LinearSampler, vEmissiveTexcoord);
    vector vEmissive2 = g_EmissiveTexture_2.Sample(LinearSampler, vEmissiveTexcoord);
    
    vector vFinalColorOrigin = vEmissive;
    vEmissive = saturate(vEmissive - float4(g_vThreshold, 0.f));
  
    if (vEmissive.x > 0.f && vEmissive.y > 0.f && vEmissive.z > 0.f)
    {
    
        Out.vColor = vMtrlDiffuse;
        Out.vIntensity.x =  g_fIntensity;
    }
    
    
    vector vFinalColorOrigin2 = vEmissive2;
    vEmissive2 = saturate(vEmissive2 - float4(g_vThreshold, 0.f));
  
    if (vEmissive2.x > 0.f && vEmissive2.y > 0.f && vEmissive2.z > 0.f)
    {
        Out.vColor = vMtrlDiffuse;
        Out.vIntensity.x = g_fIntensity;
    }
	
   
    
    
    return Out;
    
}


technique11			DefaultTechnique
{
	/* 우리가 수행해야할 정점, 픽셀 셰이더의 진입점 함수를 지정한다. */
	pass DefaultPass//0
	{
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

    pass GlowPass //0
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_GLOW();
    }
   
}
