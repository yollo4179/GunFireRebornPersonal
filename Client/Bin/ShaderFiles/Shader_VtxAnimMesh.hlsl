#include "../../../EngineSDK/hlsl/Engine_Shader_Define.hlsli"

float4x4 g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
float4x4 g_BoneMatrices[512];

vector g_vLightDir = float4(1.f, -1.f, 1.f, 0.f);
vector g_vLightDiffuse = float4(1.f, 1.f, 1.f, 1.f);
vector g_vLightAmbient = float4(1.f, 1.f, 1.f, 1.f);
vector g_vLightSpecular = float4(1.f, 1.f, 1.f, 1.f);

texture2D g_DiffuseTexture;
vector g_vMtrlAmbient = float4(0.3f, 0.3f, 0.3f, 1.f);
vector g_vMtrlSpecular = float4(1.f, 1.f, 1.f, 1.f);

vector g_vCamPosition;

/*Glow*/
float g_fIntensity;
float3 g_vThreshold;
texture2D g_EmissiveTexture;

/*Desolve*/ 
float g_NoiseStrength ;
float g_GlowIntensity ;
float g_NoiseCurTime;
float g_NoiseMaxTime; 
texture2D g_NoiseTexture;




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

struct PS_OUT
{
    float4 vDiffuse : SV_TARGET0;
    float4 vNormal : SV_TARGET1;
    float4 vDepth : SV_TARGET2;
    float4 vWorld : SV_TARGET3;
};

/* PixelShader */
PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    if (vMtrlDiffuse.a < 0.1f)
        discard;

	/* 명암 */
    float4 vShade = saturate(max(dot(normalize(g_vLightDir) * -1.f, In.vNormal), 0.f) + (g_vLightAmbient * g_vMtrlAmbient));
	
	/* 0.f ~ 1.f */
	/* 빛이 들어와서 맞고 튕긴 반사벡터와 이 픽셀을 바라보는 시선벡터가 이루는 각이 180도 일때 최대 */
    float4 vReflect = reflect(normalize(g_vLightDir), normalize(In.vNormal));
    float4 vLook = In.vWorldPos - g_vCamPosition;

    float fSpecular = pow(max(dot(normalize(vReflect) * -1.f, normalize(vLook)), 0.f), 50.f);

	/* (g_vLightDiffuse * vMtrlDiffuse) : 빛을 받은 픽셀이 반사하는 색상.  */
	/* (g_vLightSpecular * vMtrlSpecular) : 빛을 받은 픽셀이 반사하는 색상.  */
   
   // Out.vColor = (g_vLightDiffuse * vMtrlDiffuse) * vShade + (g_vLightSpecular * g_vMtrlSpecular) * fSpecular;

    
    Out.vDiffuse = vMtrlDiffuse;
	/* -1.f ~ 1.f -> 0.f ~ 1.f */
    Out.vNormal = float4(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);
    Out.vWorld = In.vWorldPos;
    return Out;
}



/*Pixel_Glow*/ 
/* PixelShader */
struct PS_GLOW_OUT
{
    float4 vColor : SV_TARGET0;
    float4 vIntensity : SV_TARGET1;
};
PS_GLOW_OUT PS_MAIN_GLOW(PS_IN In)
{
  
    PS_GLOW_OUT Out = (PS_GLOW_OUT) 0;
    
     /*Mask 로 glow할 부분을 찍는다 -> ....*/
    vector vEmmisive = g_EmissiveTexture.Sample(LinearSampler, In.vTexcoord);
    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    vector vEmmisive_Origin = vEmmisive;
    vEmmisive = saturate(vEmmisive - float4(g_vThreshold, 0.f));
 

    if ((vEmmisive.b) > 0.f)
    {
        Out.vColor = vMtrlDiffuse * 0.9f;
        Out.vIntensity.x += vEmmisive_Origin.b*g_fIntensity;
    }
   
    
    return Out;

}


struct PS_OUT_LIGHTDEPTH
{
    float4 vLightDepth : SV_TARGET0;
};


PS_OUT_LIGHTDEPTH PS_MAIN_LIGHTDEPTH(PS_IN In)
{
    PS_OUT_LIGHTDEPTH Out = (PS_OUT_LIGHTDEPTH) 0;

    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    if (vMtrlDiffuse.a < 0.1f)
        discard;
	
    Out.vLightDepth = float4(In.vProjPos.w / 1000.f, 0.f, 0.f, 0.f);

    return Out;
}



PS_OUT PS_MAIN_DESOLVE(PS_IN In)//3
{
    PS_OUT Out = (PS_OUT) 0;

    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    if(vMtrlDiffuse.a<0.1f)
        discard;
    
    Out.vDiffuse = vMtrlDiffuse;
    //
    float fRatio = (g_NoiseCurTime / g_NoiseMaxTime);
    
    vector vNoise = g_NoiseTexture.Sample(LinearSampler, In.vTexcoord);
    //UV왜곡 게산 
    
    float2 UVDistortion = (vNoise.rg - 0.5f) * g_NoiseStrength * fRatio;
    float2 DistortedUV = clamp(In.vTexcoord + UVDistortion, 0.f, 1.f);
    
    //왜곡된 uv로 다시 노이즈 샘플링
    vector vDistortedNoise = g_NoiseTexture.Sample(LinearSampler, DistortedUV);
    
    float Alpha_ToDiscard = vDistortedNoise.r;
    
    /**/
    if(Alpha_ToDiscard <fRatio) // 노이즈에서ratio보다 노이즈 텍스처의 r보다 작으면 잘라버린다. 
    {
        discard;
    }
    //qjsdkfvkrk ㄱㅁ샤ㅐ-0.02한거보다 작다는것은 디졸브할 비율의 번알파 ( 디졸브해서 없어진ㄹ 비율을 말한다 -> 현재 비율  -0/02~+0.02사이의 보간될값을 가져오는 이유는 디졸브해서 없어질 부분의 편차를 -> =- 0.02부분에 빨간색을 넣고 싶다 -> 번알파는 ratio랑 같을 때 같다고 치고 하면 빨간간약해서 -> 비율에 해당하는 
   //   // BurnAlpha가 디졸브할 비율을 말한다.// 빨갛게 타오르는 듯한 부분의 영역을 넓히기 위해서 +- 0.02
    else if( Alpha_ToDiscard>= fRatio -0.2f && Alpha_ToDiscard<= fRatio+0.2f)
    {
        float GlowFactor = smoothstep(fRatio - 0.02f, fRatio + 0.02f, Alpha_ToDiscard);
        Out.vDiffuse.r = 1.0f + GlowFactor * 2.0f; // 빨강 색상 강조
        Out.vDiffuse.g = Out.vDiffuse.r * 0.2f; // 약간의 주황색 톤 추가
        Out.vDiffuse.b = Out.vDiffuse.r * 0.1f;
        Out.vDiffuse.rgb *= GlowFactor * g_GlowIntensity;

    }
    else
    {
    
        float GlowFactor = smoothstep(fRatio - 0.05f, fRatio + 0.05f, Alpha_ToDiscard);
        Out.vDiffuse.rgb += GlowFactor * g_GlowIntensity;    
        Out.vDiffuse.rgb *= lerp(1.f, 0.f, fRatio);
    }
    
    Out.vNormal = float4(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);
    Out.vWorld = In.vWorldPos;
    return Out;
        
}
struct PS_OUT_DESOLVE_GLOW
{
    float4 vColor : SV_TARGET0;
    float4 vIntensity : SV_TARGET1;
};

PS_OUT_DESOLVE_GLOW  PS_MAIN_DESOLVE_GLOW(PS_IN In)//4
{
    PS_OUT_DESOLVE_GLOW Out = (PS_OUT_DESOLVE_GLOW) 0;
    
    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    
    vector vEmissive = vMtrlDiffuse;
    vector vEmmisive_Origin = vMtrlDiffuse;
    vEmissive = saturate(vMtrlDiffuse - float4(g_vThreshold, 0.f));
   
    //
    float fRatio = (g_NoiseCurTime / g_NoiseMaxTime);
    
    vector vNoise = g_NoiseTexture.Sample(LinearSampler, In.vTexcoord);
    //UV왜곡 게산 
    
    float2 UVDistortion = (vNoise.rg - 0.5f) * g_NoiseStrength * fRatio;
    float2 DistortedUV = clamp(In.vTexcoord + UVDistortion, 0.f, 1.f);
    
    //왜곡된 uv로 다시 노이즈 샘플링
    vector vDistortedNoise = g_NoiseTexture.Sample(LinearSampler, DistortedUV);
    
    float Alpha_ToDiscard = vDistortedNoise.r;
    
    /**/
    if (Alpha_ToDiscard < fRatio) // 노이즈에서ratio보다 노이즈 텍스처의 r보다 작으면 잘라버린다. 
    {
        discard;
    }
    //qjsdkfvkrk ㄱㅁ샤ㅐ-0.02한거보다 작다는것은 디졸브할 비율의 번알파 ( 디졸브해서 없어진ㄹ 비율을 말한다 -> 현재 비율  -0/02~+0.02사이의 보간될값을 가져오는 이유는 디졸브해서 없어질 부분의 편차를 -> =- 0.02부분에 빨간색을 넣고 싶다 -> 번알파는 ratio랑 같을 때 같다고 치고 하면 빨간간약해서 -> 비율에 해당하는 
   //   // BurnAlpha가 디졸브할 비율을 말한다.// 빨갛게 타오르는 듯한 부분의 영역을 넓히기 위해서 +- 0.02
    else if (Alpha_ToDiscard >= fRatio - 0.2f && Alpha_ToDiscard <= fRatio + 0.2f)
    {
        float GlowFactor = smoothstep(fRatio - 0.02f, fRatio + 0.02f, Alpha_ToDiscard);
        Out.vColor.r = 1.0f + GlowFactor * 2.0f; // 빨강 색상 강조
        Out.vColor.g = Out.vColor.r * 0.2f; // 약간의 주황색 톤 추가
        Out.vColor.b = Out.vColor.r * 0.1f;
        Out.vColor.rgb *= GlowFactor * g_GlowIntensity;

    }
    else
    {
    
        float GlowFactor = smoothstep(fRatio - 0.05f, fRatio + 0.05f, Alpha_ToDiscard);
        Out.vColor.rgb += GlowFactor * g_GlowIntensity;
        Out.vColor.rgb *= lerp(1.f, 0.f, fRatio);
    }

    
    
    if (Out.vColor.r > 0.f )
    {
        Out.vIntensity.x = g_fIntensity;
    }
   
    
    return Out;
}



PS_GLOW_OUT PS_MAIN_GLOW_ALL(PS_IN In)//5 NoMask
{
  
    PS_GLOW_OUT Out = (PS_GLOW_OUT) 0;
    
    
    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
     Out.vColor = vMtrlDiffuse ;
      Out.vIntensity.x =  g_fIntensity;
    
   
    
    return Out;

}
technique11 DefaultTechnique
{
	
    pass DefaultPass
    {

        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        /*블렌드는 디폴트, 내가 직접 섞을 색은 존재하지 않는다 , 0xffffffff-> 알파 블랜딩에 영향을 받을 색상영역을 정의 rgb 모두 설정 */
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }

    pass LightDepth //Shadow  1
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_LIGHTDEPTH();
    }
    pass GlowPass
    {

        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        /*블렌드는 디폴트, 내가 직접 섞을 색은 존재하지 않는다 , 0xffffffff-> 알파 블랜딩에 영향을 받을 색상영역을 정의 rgb 모두 설정 */
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_GLOW();
    }

    pass DesolvePass//3
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        /*블렌드는 디폴트, 내가 직접 섞을 색은 존재하지 않는다 , 0xffffffff-> 알파 블랜딩에 영향을 받을 색상영역을 정의 rgb 모두 설정 */
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DESOLVE();
    }

    pass Desolve_GlowPass //4
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DESOLVE_GLOW();
    }

    pass Glow_All_Pass //5
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_GLOW_ALL();
    }



}
