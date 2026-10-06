
#include "../../../EngineSDK/hlsl/Engine_Shader_Define.hlsli"


/* 상수테이블 ConstantTable */
float4x4		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D g_EmissiveTexture;
texture2D g_DiffuseTexture;

vector g_vCamPosition;

float g_fIntensity;
float3 g_vThreshord;

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
VS_OUT VS_MAIN_BILLBOARD (VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;
    
    matrix LocalAdjMatrix = g_WorldMatrix;
    vector vMyPos = LocalAdjMatrix._41_42_43_44;
    LocalAdjMatrix._41_42_43_44 = (0.f, 0.f, 0.f, 1.f);
    
    vector vCamPos = g_vCamPosition;
    float3 vMyLook = normalize(vCamPos.xyz - vMyPos.xyz);
    
    float3 vMyRight = normalize(cross(float3(0.f, 1.f, 0.f), vMyLook.xyz));
    
    float3 vMyUp = normalize(cross(vMyLook.xyz, vMyRight.xyz));
   
    matrix BillboardMatrix = { { vMyRight, 0 }, { vMyUp, 0 }, { vMyUp, 0 }, { 0.f, 0.f, 0.f, 1.f } };
    
    matrix vMyWorldMatrix = mul(LocalAdjMatrix, BillboardMatrix);
    vMyWorldMatrix._41_42_43_44 = vMyPos;
    
    matrix matWV, matWVP;
    matWV = mul(vMyWorldMatrix, g_ViewMatrix);
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
	float4		vPosition : SV_POSITION;
	float2		vTexcoord : TEXCOORD0;
};

struct PS_OUT_GLOW
{
	float4		vColor : SV_TARGET0;
    float4      vIntensity : SV_TARGET1;
};

struct PS_OUT
{
    float4 vColor : SV_TARGET0;
 
};



PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    vector vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
 
    
    Out.vColor = vDiffuse;
    if (Out.vColor.a < 0.3f)
        discard;
    
    return Out;
}

/* PixelShader */
PS_OUT_GLOW PS_MAIN_GLOW(PS_IN In)
{
    PS_OUT_GLOW Out = (PS_OUT_GLOW) 0;
    vector vEmissive = g_EmissiveTexture.Sample(LinearSampler, In.vTexcoord);
    vector vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    
    vector vFinalColor = vEmissive;
    vFinalColor = saturate(vFinalColor - float4(g_vThreshord, 0.f));
    
    vFinalColor.w = 0.f;
   
    if (dot(vFinalColor.xyz, vFinalColor.xyz) > 0.f)
    {
        Out.vColor = vDiffuse;
        Out.vIntensity.x = g_fIntensity;
    }
    if (vDiffuse.a < 0.3f)
        discard;
    return Out;
}


PS_OUT PS_MAIN_RED(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    vector vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
 
    
    Out.vColor = vDiffuse;
    Out.vColor.gb *= 0.1f;
    
    if (Out.vColor.a < 0.05f)
        discard;
    
    return Out;
}
/* PixelShader */
PS_OUT_GLOW PS_MAIN_GLOW_RED(PS_IN In)
{
    PS_OUT_GLOW Out = (PS_OUT_GLOW) 0;
    vector vEmissive = g_EmissiveTexture.Sample(LinearSampler, In.vTexcoord);
    vector vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    
    vector vFinalColor = vEmissive;
    vFinalColor = saturate(vFinalColor - float4(g_vThreshord, 0.f));
    
    vFinalColor.w = 0.f;
   
    if (dot(vFinalColor.xyz, vFinalColor.xyz) > 0.f)
    {
        vDiffuse.gb *= 0.1f;
        Out.vColor = vDiffuse;
        Out.vIntensity.x = g_fIntensity;
    }
    if (vDiffuse.a < 0.3f)
        discard;
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

        VertexShader = compile vs_5_0 VS_MAIN_BILLBOARD();
        GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}
    


    pass GlowPass //1
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_BILLBOARD();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_GLOW();
    }

    /*NonBillboard*/

    pass NonBillboardPass //2
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }
    


    pass NonBillboardGlowPass //3
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_GLOW();
    }

    pass NonBillboardPass_Red //4
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_RED();
    }

    pass NonBillboardGlowPass_Red //5
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_GLOW_RED();
    }
}
