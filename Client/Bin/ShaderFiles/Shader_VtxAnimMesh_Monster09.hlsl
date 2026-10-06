#include "../../../EngineSDK/hlsl/Engine_Shader_Define.hlsli"

float4x4 g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
float4x4 g_BoneMatrices[512];


texture2D g_DiffuseTexture;
vector g_vCamPosition;
texture2D g_Emissive_Eye_Texture;
texture2D g_Emissive_Body_Texture;
float3 g_vThreshold;
float3 g_vBodyThreshold;
float g_fIntensity;
float g_fBodyIntensity;

struct VS_IN
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float3 vTangent : TANGENT;
    uint4 vBlendIndices : BLENDINDEX;
    float4 vBlendWeights : BLENDWEIGHT;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
};

/* 1. 정점위치에 대한 기초적인 변환과정을 수행한다.(월드변환, 뷰변환, ) */

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    //뼈의 ㅊ상태행렬을 전달한다->정점에 영향을 미치는 뼈의 인덱스로부터 *(메시(정점 버퍼)가 가지는 뼈의 인덱스)
    //->매핑된 진짜 뼈의 인덱스로 접근 -> 매트릭스 얻어온다 
   
    float fWeightW = 1.f - (In.vBlendWeights.x + In.vBlendWeights.y + In.vBlendWeights.z);
    
    
    matrix BoneMatrix = g_BoneMatrices[In.vBlendIndices.x] * In.vBlendWeights.x +
		g_BoneMatrices[In.vBlendIndices.y] * In.vBlendWeights.y +
		g_BoneMatrices[In.vBlendIndices.z] * In.vBlendWeights.z +
		g_BoneMatrices[In.vBlendIndices.w] * fWeightW;

    vector vPosition = mul(float4(In.vPosition, 1.f), BoneMatrix);
    vector vNormal = mul(float4(In.vNormal, 0.f), BoneMatrix);

    Out.vPosition = mul(vPosition, matWVP);
    Out.vNormal = normalize(mul(vNormal, g_WorldMatrix));
    Out.vTexcoord = In.vTexcoord;
    Out.vWorldPos = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
    Out.vProjPos = Out.vPosition;
    return Out;
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
};

struct PS_OUT_GLOW
{
    float4 vDiffuse : SV_TARGET0;
    float4 vIntensity : SV_TARGET1;
};

/* PixelShader */
PS_OUT_GLOW PS_MAIN(PS_IN In)
{
    PS_OUT_GLOW Out = (PS_OUT_GLOW) 0;

    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    if (vMtrlDiffuse.a < 0.1f)  
        discard;
   
    
    /*Mask 로 glow할 부분을 찍는다 -> ....*/
    vector vEmmisive_Body = g_Emissive_Body_Texture.Sample(LinearSampler, In.vTexcoord);
    vector vEmissive_Eye = g_Emissive_Eye_Texture.Sample(LinearSampler, In.vTexcoord);
    
   
    vEmissive_Eye = saturate(vEmissive_Eye);
    
    vector vEmmisive_Body_Origin = vEmmisive_Body;
    vEmmisive_Body = saturate(vEmmisive_Body - float4(g_vBodyThreshold, 0.f));
 
    if (dot(vEmissive_Eye.xyz, vEmissive_Eye.xyz) != 0.f)
    {
        Out.vDiffuse = vMtrlDiffuse;
        Out.vIntensity.x = g_fIntensity;
    }
    else
    {
    
        if ((vEmmisive_Body.b) > 0.f)
        {
            Out.vDiffuse = vMtrlDiffuse;
            Out.vIntensity.x = vEmmisive_Body_Origin.b*0.01;
            return Out;
        }
        else if ((vEmmisive_Body.g) <= 0.0f)
        {
            Out.vDiffuse = vMtrlDiffuse;
            Out.vIntensity.x = vEmmisive_Body_Origin.g * 0.000000000001;
        }
        
    }
    
    return Out;
}



technique11 DefaultTechnique
{
	
    pass GlowPass //Glow : 0
    {

        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        /*블렌드는 디폴트, 내가 직접 섞을 색은 존재하지 않는다 , 0xffffffff-> 알파 블랜딩에 영향을 받을 색상영역을 정의 rgb 모두 설정 */
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }

   

}
