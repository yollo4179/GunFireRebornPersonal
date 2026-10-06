
#include "../../../EngineSDK/hlsl/Engine_Shader_Define.hlsli"


/* 상수테이블 ConstantTable */
float4x4		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;


vector g_vLightDir = float4(1.f, -1.f, 1.f, 0.f);
vector g_vLightDiffuse = float4(1.f, 1.f, 1.f, 1.f);
vector g_vLightAmbient = float4(1.f, 1.f, 1.f, 1.f);
vector g_vLightSpecular = float4(1.f, 1.f, 1.f, 1.f);
vector g_vCamPosition;


Texture2D g_DiffuseTexture;
Texture2D g_NormalMap;
TextureCube g_EnvironmentMap;
float Time;


/*  굴절을 위한 스크린 텍스처*/
Texture2D g_ScreenTexture;
Texture2D g_NoiseTexture;
Texture2D g_WaveTexture;
float g_NumWaves;
float g_Refractive_index=0.4f;
float g_NoiseStrength=0.08f;
float g_Time;
float2 g_vWinSize;
vector g_vCenter;

/*For_Glow*/
Texture2D g_EmissiveTexture;
float g_fIntensity;
float3 g_fThreshold;

/* VertexShader */

/* 정점쉐이더의 구동 시점. */
/* m_pContext->DrawIndexed() */
/* 그릴때 사용하고자했던 정점들중, 인덱스버퍼(0, 1, 2, 0, 2, 3)가 지정해주고 있는 정점을 골라서 */
/* VS_MAIN함수를 호출하며 인자로 전달해준다. */

struct VS_IN
{
	float3		vPosition : POSITION;
    float3      vNormal : NORMAL;
	float2		vTexcoord : TEXCOORD0;	
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
    float4      vNormal : NORMAL;
	
    float2		vTexcoord : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
    float4 vViewDirection : TEXCOORD2;
};

/* 1. 정점위치에 대한 기초적인 변환과정을 수행한다.(월드변환, 뷰변환, ) */

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    //vertexWave 
    In.vPosition.y += cos(abs(In.vTexcoord.x * 20.f - 1.f) * 10 + Time) * 0.05f;
    
    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
    Out.vNormal = normalize(mul(float4(In.vNormal, 0.f), g_WorldMatrix));
    Out.vTexcoord = In.vTexcoord;
    Out.vWorldPos = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
    Out.vViewDirection = normalize(g_vCamPosition - Out.vWorldPos);
    
    return Out;
	
}



/* 정점의 위치벡터를 위치벡터의 w로 나눈다. */
/* 뷰포트 영역으로 정점들을 변환한다. */
/* 래스터라이즈(정점 정보를 보간하여 픽셀의 정보가 생성된다.) */
/* 이렇게 픽셀을 PS_MAIN() 호출하면서 전달해준다. */

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
	
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vViewDirection : TEXCOORD2;
};

struct PS_OUT
{
	float4		vColor : SV_TARGET0;
};

/* PixelShader */
    
PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

    float3 normalMap1 = g_NormalMap.Sample(LinearSampler, In.vTexcoord*500 * 0.1 + Time * float2(0.1f, 0.2f)*1.6f).rgb;
    float3 normalMap2 = g_NormalMap.Sample(LinearSampler, In.vTexcoord*500 * 0.1 - Time * float2(0.1f, 0.2f) * 1.f).rgb;
    float3 normalMap = (normalMap1 + normalMap2) * 0.5f;
    
    float3 normal = normalize((float3)In.vNormal + normalMap);
    float3 reflectDir = reflect(-(float3) In.vViewDirection, normal);
    
    float4 reflectionSky = g_EnvironmentMap.Sample(LinearSampler, reflectDir);
    
    float4 waterColor = float4(0.f, 0.4f, 0.6f, 1.f);
    float4 finalColor = lerp(waterColor, reflectionSky, 0.8f);
    Out.vColor=finalColor;
    Out.vColor.a = 0.9;
    return Out;
 
	
	return Out;
}
/*굴절 웨이브 */
struct VS_WAVE_IN
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
};
struct VS_WAVE_OUT
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
    
};

VS_WAVE_OUT VS_MAIN_WAVE(VS_WAVE_IN In)
{
    VS_WAVE_OUT Out = (VS_WAVE_OUT) 0;
    
    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    /*복합 파도 계산*/
    float PI = 3.14159265359f;
    
    float fHeight = 0.f;
    for (int i = 0; i < g_NumWaves; ++i)
    {
        
        float4 vWaveData = g_WaveTexture.SampleLevel(LinearSampler, float2((float) i / g_NumWaves, 0.f), 0);
        float fAmplitude = vWaveData.r;
        float fWaveLength = vWaveData.g;
        float fSpeed = vWaveData.b;
        float fPhase = vWaveData.a;
        
        float k = 2.0f * PI / fWaveLength;
        float fOmega = 2.0f * PI * fSpeed / fWaveLength;
        float fRet = fAmplitude * sin(k * (In.vPosition.x + In.vPosition.z) - fOmega * g_Time + fPhase);
      
        fHeight += fRet; 
    }
    

 
    
    float BasicHeight
    =
    sqrt(saturate(1 - (In.vPosition.x - 5.f) * (In.vPosition.x - 5.f) / (3.f * 3.f)
    -(In.vPosition.z - 5.f) * (In.vPosition.z - 5.f) / (10.f * 10.f)));
    
    In.vPosition.y = BasicHeight*2.3f;
    
    In.vPosition.x += 0.15f * sin(2.f * g_Time * In.vPosition.y);
    In.vPosition.y += 0.1f * (2 * sin(2.f * g_Time * In.vPosition.y) - 1.5 * sin(-1 * 3.f * g_Time * In.vPosition.z));
    In.vPosition.z += 0.15f * (1.5 * cos(2.f * g_Time) - sin(g_Time * In.vPosition.x));
  
    
    
    //vertexWave    
    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP); //투영(ndc) 
    Out.vNormal = normalize(mul(float4(In.vNormal, 0.f), g_WorldMatrix));  //노말(월드)
    Out.vTexcoord = In.vTexcoord; //텍스쿠드
    Out.vWorldPos = mul(float4(In.vPosition, 1.f), g_WorldMatrix); //월드 pos
    Out.vProjPos = Out.vPosition;  //투영관(클립) pos
    return Out;
}
struct PS_WAVE_IN
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
};
struct PS_WAVE_OUT
{
    float4 vDistortion : SV_TARGET0;
};

PS_WAVE_OUT PS_MAIN_WAVE(PS_WAVE_IN In)
{
    PS_WAVE_OUT Out = (PS_WAVE_OUT) 0;
    
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
    noiseOffset.x = vNoise.x * g_NoiseStrength * cos(g_Time*0.8f );
    noiseOffset.y = vNoise.y * g_NoiseStrength * sin(g_Time *0.8f);
    
   
    
    //스크린 겹치는 스크린 샘플링 
    
 
    
    float2 Refraction_Texcoord = RefractionDir.xy*0.1f  + noiseOffset + vTexcoordScreen;
    float2 Diffuse_ReflectedTexcoord = RefractionDir.xy * 0.5f + noiseOffset + In.vTexcoord;
    vector vDiffuseColor = g_DiffuseTexture.Sample(LinearSampler, Diffuse_ReflectedTexcoord);
    vDiffuseColor.a = 0.5;
    //스크린 겹치는 스크린 샘플링 
    vector RefracedColor = g_ScreenTexture.Sample(LinearSampler, Refraction_Texcoord);
    vector vFinalColor = lerp(vDiffuseColor, RefracedColor, vDiffuseColor.a);
    vFinalColor.a = 0.5f;
    
    
    
    Out.vDistortion = vFinalColor;
    return Out;
    
}
struct PS_MONSTER09_WAVE_OUT
{
    float4 vColor : SV_TARGET0;
    float4 vIntensity : SV_TARGET1;
};

PS_MONSTER09_WAVE_OUT PS_MAIN_WAVE_GLOW(PS_WAVE_IN In)
{
    PS_MONSTER09_WAVE_OUT Out = (PS_MONSTER09_WAVE_OUT) 0;
    
  
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
    float2 Diffuse_ReflectedTexcoord = RefractionDir.xy * 0.5f + 3*noiseOffset + In.vTexcoord;
    vector vDiffuseColor = g_DiffuseTexture.Sample(LinearSampler, Diffuse_ReflectedTexcoord);
    vDiffuseColor.a = 0.7;
    //스크린 겹치는 스크린 샘플링 
    vector RefracedColor = g_ScreenTexture.Sample(LinearSampler, Refraction_Texcoord);
    vector vFinalColor = lerp(vDiffuseColor, RefracedColor, vDiffuseColor.a);
    vector vFinalColorOrigin = vFinalColor;
    
    vFinalColor =saturate(vFinalColor - float4(g_fThreshold, 0.f));
    vFinalColor.w = 0;
    if (vFinalColor.x > 0.f && vFinalColor.y > 0.f && vFinalColor.z>0.f)
    {
    
        Out.vColor = vFinalColorOrigin;
        Out.vColor.b *= 1.5f;
        Out.vIntensity.x = g_fIntensity;
    }
    return Out;
    
}


technique11			DefaultTechnique
{
	/* 우리가 수행해야할 정점, 픽셀 셰이더의 진입점 함수를 지정한다. */
	pass DefaultPass
	{
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);


       
		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

    pass Boss09_WavePass//1
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);


       
        VertexShader = compile vs_5_0 VS_MAIN_WAVE();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_WAVE();
    }

    pass Boss09_WaveGlowPass //2
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);


       
        VertexShader = compile vs_5_0 VS_MAIN_WAVE();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_WAVE_GLOW();
    }

   


}
