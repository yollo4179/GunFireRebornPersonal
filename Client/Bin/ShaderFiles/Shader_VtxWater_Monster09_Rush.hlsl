
#include "../../../EngineSDK/hlsl/Engine_Shader_Define.hlsli"


float4x4 g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

vector g_vCamPosition;

Texture2D g_DiffuseTexture;

Texture2D g_ScreenTexture;
Texture2D g_NoiseTexture;
Texture2D g_WaveTexture;
float g_NumWaves;
float g_Refractive_index = 0.4f;
float g_NoiseStrength = 0.3f;
float g_Time=0.f;
float2 g_vWinSize;
vector g_vCenter;

float g_WaveLength=6.f;
float g_WaveHeight=1.f;
float g_WaveSpeed = 2.f;
/*For_Glow*/
Texture2D g_EmissiveTexture;
float g_fIntensity;
float3 g_fThreshold;


/*굴절 웨이브 */
struct VS_RUSH_IN
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
};
struct VS_RUSH_OUT
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
    
};

VS_RUSH_OUT VS_MAIN_RUSH(VS_RUSH_IN In)
{
    VS_RUSH_OUT Out = (VS_RUSH_OUT) 0;
    
    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    float4 vLocalPos = float4(In.vPosition, 1.f);

    // 파도 효과 추가: Z 방향으로 전진하는 파도
    vLocalPos.y += sin((vLocalPos.x + g_Time * g_WaveSpeed) * (2 * 3.14159 / g_WaveLength)) * g_WaveHeight;
    vLocalPos.y += sin((vLocalPos.z + g_Time * g_WaveSpeed * 0.8) * (2 * 3.14159 / (g_WaveLength * 1.5))) * (g_WaveHeight * 0.5);
 
    
    In.vPosition.y = vLocalPos.y;
    
    
    //vertexWave    
    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP); //투영(ndc) 
    Out.vNormal = normalize(mul(float4(In.vNormal, 0.f), g_WorldMatrix)); //노말(월드)
    Out.vTexcoord = In.vTexcoord; //텍스쿠드
    Out.vWorldPos = mul(float4(In.vPosition, 1.f), g_WorldMatrix); //월드 pos
    Out.vProjPos = Out.vPosition; //투영관(클립) pos
    return Out;
}
struct PS_RUSH_IN
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
};
struct PS_RUSH_OUT
{
    float4 vDistortion : SV_TARGET0;
};
PS_RUSH_OUT PS_MAIN_RUSH(PS_RUSH_IN In)
{
    PS_RUSH_OUT Out = (PS_RUSH_OUT) 0;
    
    float3 vViewDir = normalize(In.vWorldPos - g_vCamPosition); //시야 입사각
    float3 RefractionDir = refract(vViewDir, In.vNormal.xyz, g_Refractive_index); //굴절벡터
    
   //나의 화면상좌표 Screen TO TEXCOORD 
    float2 vTexcoordScreen;
    vTexcoordScreen.x = In.vProjPos.x / In.vProjPos.w;
    vTexcoordScreen.y = In.vProjPos.y / In.vProjPos.w;
    
    
    vTexcoordScreen.x = (vTexcoordScreen.x) * 0.5f + 0.5f;
    vTexcoordScreen.y = (vTexcoordScreen.y) * -0.5f + 0.5f;
    
   
   //노이즈 샘플링 
    float2 vTexcoordNoise = In.vTexcoord;
    float3 vNoise = g_NoiseTexture.Sample(LinearSampler, vTexcoordNoise);
      
           
    //노이즈 move -
    float2 noiseOffset = (vNoise.gb - 0.5f) * g_NoiseStrength;
    noiseOffset.x = vNoise.x * g_NoiseStrength * cos(g_Time * 3.f);
    noiseOffset.y = vNoise.y * g_NoiseStrength * sin(g_Time * 3.f);
    
   
    
    //스크린 겹치는 스크린 샘플링 
    
 
    
    float2 Refraction_Texcoord = RefractionDir.xy * 0.1f + noiseOffset + vTexcoordScreen;
    float2 Diffuse_ReflectedTexcoord = RefractionDir.xy * 0.5f + noiseOffset + In.vTexcoord;
    vector vDiffuseColor = g_DiffuseTexture.Sample(LinearSampler, Diffuse_ReflectedTexcoord);
    vDiffuseColor.a = 0.8;
    //스크린 겹치는 스크린 샘플링 
    vector RefracedColor = g_ScreenTexture.Sample(LinearSampler, Refraction_Texcoord);
    vector vFinalColor = lerp(vDiffuseColor, RefracedColor, vDiffuseColor.a);
 
    
    
    
    Out.vDistortion = vFinalColor;
    return Out;
    
}


struct PS_MONSTER09_RUSH_GLOW_OUT
{
    float4 vColor : SV_TARGET0;
    float4 vIntensity : SV_TARGET1;
};

PS_MONSTER09_RUSH_GLOW_OUT PS_MAIN_RUSH_GLOW(PS_RUSH_IN In)
{
    PS_MONSTER09_RUSH_GLOW_OUT Out = (PS_MONSTER09_RUSH_GLOW_OUT) 0;
    
  
    float3 vViewDir = normalize(In.vWorldPos - g_vCamPosition); //시야 입사각
    float3 RefractionDir = refract(vViewDir, In.vNormal.xyz, g_Refractive_index); //굴절벡터
    
   //나의 화면상좌표 Screen TO TEXCOORD 
    float2 vTexcoordScreen;
    vTexcoordScreen.x = In.vProjPos.x / In.vProjPos.w;
    vTexcoordScreen.y = In.vProjPos.y / In.vProjPos.w;
    
    
    vTexcoordScreen.x = (vTexcoordScreen.x) * 0.5f + 0.5f;
    vTexcoordScreen.y = (vTexcoordScreen.y) * -0.5f + 0.5f;
    
   
   //노이즈 샘플링 
    float2 vTexcoordNoise = In.vTexcoord;
    float3 vNoise = g_NoiseTexture.Sample(LinearSampler, vTexcoordNoise);
      
           
    //노이즈 move -
    float2 noiseOffset = (vNoise.gb - 0.5f) * g_NoiseStrength;
    noiseOffset.x = vNoise.x * g_NoiseStrength * cos(g_Time * 0.8f);
    noiseOffset.y = vNoise.y * g_NoiseStrength * sin(g_Time * 0.8f);
    
   
    
    //스크린 겹치는 스크린 샘플링 
    
 
    
    float2 Refraction_Texcoord = RefractionDir.xy * 0.1f + noiseOffset + vTexcoordScreen;
    float2 Diffuse_ReflectedTexcoord = RefractionDir.xy * 0.5f + 3 * noiseOffset + In.vTexcoord;
    vector vDiffuseColor = g_DiffuseTexture.Sample(LinearSampler, Diffuse_ReflectedTexcoord);
    vDiffuseColor.a = 0.7;
    //스크린 겹치는 스크린 샘플링 
    vector RefracedColor = g_ScreenTexture.Sample(LinearSampler, Refraction_Texcoord);
    vector vFinalColor = lerp(vDiffuseColor, RefracedColor, vDiffuseColor.a);       
   
    vector vFinalColorOrigin = vFinalColor;
    vFinalColor = saturate(vFinalColor - float4(g_fThreshold, 0.f));
  
    if (vFinalColor.x >= 0.f && vFinalColor.y >= 0.f && vFinalColor.z >= 0.f )
    {
    
        Out.vColor = vFinalColorOrigin;
        
        Out.vIntensity.x = saturate(vFinalColorOrigin.b * g_fIntensity);
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

        VertexShader = compile vs_5_0 VS_MAIN_RUSH();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_RUSH();
    }
    pass  //1
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_RUSH();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_RUSH_GLOW();
    }
  


   

}
