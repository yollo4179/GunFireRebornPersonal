
#include "../../../EngineSDK/hlsl/Engine_Shader_Define.hlsli"


/* 상수테이블 ConstantTable */
float4x4		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D		g_Texture;
float3 g_vThreshord;
float3 g_fIntensity;
texture2D g_EmissiveTexture;

texture2D g_DiffuseTexture;

/* VertexShader */

/* 정점쉐이더의 구동 시점. */
/* m_pContext->DrawIndexed() */
/* 그릴때 사용하고자했던 정점들중, 인덱스버퍼(0, 1, 2, 0, 2, 3)가 지정해주고 있는 정점을 골라서 */
/* VS_MAIN함수를 호출하며 인자로 전달해준다. */

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

/* 정점의 위치벡터를 위치벡터의 w로 나눈다. */
/* 뷰포트 영역으로 정점들을 변환한다. */
/* 래스터라이즈(정점 정보를 보간하여 픽셀의 정보가 생성된다.) */
/* 이렇게 픽셀을 PS_MAIN() 호출하면서 전달해준다. */

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct PS_OUT
{
    float4 vDiffuse : SV_TARGET0;
   
};
PS_OUT PS_MAIN_TRAIL(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    vector vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
 
    
    Out.vDiffuse = vDiffuse;
    if (Out.vDiffuse.a < 0.3f)
        discard;
    
    return Out;
}


struct PS_OUT_GLOW
{
	float4		vColor : SV_TARGET0;
    float4 vIntensity : SV_TARGET1;
};

/* PixelShader */
PS_OUT_GLOW PS_MAIN_TRAIL_GLOW(PS_IN In)
{
    PS_OUT_GLOW Out = (PS_OUT_GLOW) 0;
    vector vEmissive = g_EmissiveTexture.Sample(LinearSampler, In.vTexcoord);
    vector vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    
    vector vFinalColor = vEmissive;
    vFinalColor = saturate(vFinalColor - float4(g_vThreshord, 0.f));
    vFinalColor.w = 0.f;
    
    if (vFinalColor.x > 0 && vFinalColor.y > 0 && vFinalColor.z>0)
    {
        //vDiffuse.xyz =vDiffuse.xyz - vFinalColor.xyz;
        vDiffuse.x = 0.5f;
        vDiffuse.yz = 0.f;
        Out.vColor =vDiffuse;
        Out.vIntensity.x = g_fIntensity;
    }
	
	return Out;
}



technique11			DefaultTechnique
{
	/* 우리가 수행해야할 정점, 픽셀 셰이더의 진입점 함수를 지정한다. */
	pass GlowPass//0
	{
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_TRAIL_GLOW();
    }

    pass DiffusePass //1
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_TRAIL();
    }
    

}
