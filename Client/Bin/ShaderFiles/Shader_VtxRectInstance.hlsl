

#include "../../../EngineSDK/hlsl/Engine_Shader_Define.hlsli"

// float, float2, float3, float4 == vector
//float3		vTmp = (float3)0;
//
//float2		vTmp1 = vTmp.xy;
//
//vTmp.xz = 1.f;
//
//vTmp.rgb = 1.f;

// float1x4, float3x3, float4x4 == matrix


/* 상수테이블 ConstantTable */
float4x4 g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D g_Texture;


/* VertexShader */

/* 정점쉐이더의 구동 시점. */
/* m_pContext->DrawIndexed() */
/* 그릴때 사용하고자했던 정점들중, 인덱스버퍼(0, 1, 2, 0, 2, 3)가 지정해주고 있는 정점을 골라서 */
/* VS_MAIN함수를 호출하며 인자로 전달해준다. */

struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;

    float4 vRight : TEXCOORD1;
    float4 vUp : TEXCOORD2;
    float4 vLook : TEXCOORD3;
    float4 vTranslation : TEXCOORD4;
    float2 vLifeTime : TEXCOORD5;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float2 vLifeTime : TEXCOORD1;
};

/* 1. 정점위치에 대한 기초적인 변환과정을 수행한다.(월드변환, 뷰변환, ) */

/*로컬에서 파티클을 먼저 배치하고 월드에 파티클을 배치한다*/
VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

    matrix matWV, matWVP;

    matrix TransformMatrix = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);

    vector vPosition = mul(float4(In.vPosition, 1.f), TransformMatrix);

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(vPosition, matWVP);
    Out.vTexcoord = In.vTexcoord;
    Out.vLifeTime = In.vLifeTime;

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
    float2 vLifeTime : TEXCOORD1;
};

struct PS_OUT
{
    float4 vColor : SV_TARGET0;
};

/* PixelShader */
PS_OUT PS_MAIN_LOOP(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

	/*Out.vColor = g_Texture.Sample(어떻게얻어올래?, In.vTexcoord);*/
    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);

    if(Out.vColor.a<0.3f)
        discard;
    
    Out.vColor.rgb = 1.f - saturate(In.vLifeTime.x - In.vLifeTime.y);//처음에 검정-> 하얀색으로 

    return Out;
}

PS_OUT PS_MAIN_ONCE(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

	/*Out.vColor = g_Texture.Sample(어떻게얻어올래?, In.vTexcoord);*/
    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);

    Out.vColor.gb = 0.f;

    Out.vColor.a = saturate(In.vLifeTime.x - In.vLifeTime.y) * 2.f;

    Out.vColor.r = 1.f - Out.vColor.a;

    if (In.vLifeTime.x <= In.vLifeTime.y)
        discard;
    return Out;
}


technique11 DefaultTechnique
{
	/* 우리가 수행해야할 정점, 픽셀 셰이더의 진입점 함수를 지정한다. */
    pass Loop
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_LOOP();
    }

    pass Once
    {
        SetRasterizerState(RS_Cull_Front);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_ONCE();
    }


}
