

#include "../../../EngineSDK/hlsl/Engine_Shader_Define.hlsli"
float4x4 g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D g_DiffuseTexture;

float2 g_BrushRange;
vector g_vBrushTextureLook;
vector g_vBrushPos;
texture2D g_BrushTexture;
texture2D g_MovingBrushTexture;
float g_fAccMovingLifeTime;
int g_iBrushNum;
bool g_bBrushSign = false;
int g_iSplattingIndex;



/*  굴절을 위한 스크린 텍스처 Monster 09 Shot*/
Texture2D g_ScreenTexture;
Texture2D g_NoiseTexture;
float g_Refractive_index = 0.6f;
float g_NoiseStrength = 0.09f;
float g_Time;
float2 g_vWinSize;
vector g_vCamPosition;

float3 g_fThreshold;
float g_fIntensity;

/*mONSTER09_Shot Glow Texture*/

struct VS_IN
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float3 vTangent : TANGENT;
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

    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
    Out.vNormal = normalize(mul(float4(In.vNormal, 0.f), g_WorldMatrix));
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

    
    vector vBrush_Moving = (vector) 0;
    vector vBrush_Range = (vector) 0;
    float fAlphaValue_Moving = 0.f;
    float fAlphaValue_Range = 0.f;
    if (g_bBrushSign)
    {
        bool bCondition = true;
        
        vector vBrushPos = g_vBrushPos;
        vBrushPos.y = 0.f;
       
       
        vector vBrushLook = g_vBrushTextureLook;
        vBrushLook.y = 0.f;
        vBrushLook = normalize(vBrushLook);
        vector vBrushLookNormal = { -1.f * vBrushLook.z, 0.f, vBrushLook.x, 0.f };
        vBrushLookNormal = normalize(vBrushLookNormal);
            
        vector PointA = vBrushPos + vBrushLook * g_BrushRange.y + vBrushLookNormal * g_BrushRange.x;
        vector PointB = vBrushPos + vBrushLook * g_BrushRange.y - vBrushLookNormal * g_BrushRange.x;
        vector PointC = vBrushPos - vBrushLook * g_BrushRange.y - vBrushLookNormal * g_BrushRange.x;
        vector PointD = vBrushPos - vBrushLook * g_BrushRange.y + vBrushLookNormal * g_BrushRange.x;
        
        vector vWorldPos =
        {
            In.vWorldPos.x, 0.f, In.vWorldPos.z, 1.f
        };
        vector vLine1 = (PointB - PointA);
        vector vNormal1 = { vLine1.z * -1.f, 0.f, vLine1.x, 0.f };
        vector vCheckLine1 = (vWorldPos - PointA);
        
        vector vLine2 = (PointC - PointB);
        vector vNormal2 = { -1 * vLine2.z, 0.f, vLine2.x, 0.f };
        vector vCheckLine2 = (vWorldPos - PointB);
        
        vector vLine3 = (PointD - PointC);
        vector vNormal3 = { -1 * vLine3.z, 0.f, vLine3.x, 0.f };
        vector vCheckLine3 = (vWorldPos - PointC);
        
        vector vLine4 = (PointA - PointD);
        vector vNormal4 = { -1 * vLine4.z, 0.f, vLine4.x, 0.f };
        vector vCheckLine4 = (vWorldPos - PointD);
        
   
        if (0.f < dot(vNormal1, vCheckLine1))
            bCondition = false;
        if (0.f < dot(vNormal2, vCheckLine2))
            bCondition = false;
        if (0.f < dot(vNormal3, vCheckLine3))
            bCondition = false;
        if (0.f < dot(vNormal4, vCheckLine4))
            bCondition = false;
        
   
    
        if (true == bCondition)
        {
            vector vMyMappingDir = vWorldPos - vBrushPos;
            float2 vTexcoord = 0.f;
           // vTexcoord.y = (g_BrushRange.y - dot(vBrushLook, vMyMappingDir)+g_fAccMovingLifeTime) / (g_BrushRange.y * 2.f);
           // vTexcoord.x = (g_BrushRange.x - dot(vBrushLookNormal, vMyMappingDir) + g_fAccMovingLifeTime) / (g_BrushRange.x * 2.f);
            
           // //for (int i = 0; i < g_iBrushNum; ++i)
           //// {
           // vBrush_Moving += g_MovingBrushTexture.Sample(LinearSampler, vTexcoord);
           // fAlphaValue_Moving = vBrush_Moving.a;
           // vBrush_Moving.gb = vBrush_Moving.gb * 0.3;
           // if (vBrush_Moving.a < 0.3f)
           //     vBrush_Moving = (vector) 0;
            
            vMyMappingDir = vWorldPos - vBrushPos;
            vTexcoord = 0.f;
            vTexcoord.y = (g_BrushRange.y - dot(vBrushLook, vMyMappingDir)) / (g_BrushRange.y * 2.f);
            vTexcoord.x = (g_BrushRange.x - dot(vBrushLookNormal, vMyMappingDir)) / (g_BrushRange.x * 2.f);
            
            
            vBrush_Range += g_BrushTexture.Sample(LinearSampler, vTexcoord);
            fAlphaValue_Range = vBrush_Range.a;
           //vBrush_Range.gb = vBrush_Range.gb * 0.3;
            if (vBrush_Range.a < 0.3f)
                vBrush_Range = (vector) 0;
            
            
            //}
              
            
        }
        
    }
    

       
   
    
    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    if (vMtrlDiffuse.a < 0.1f)
        discard;
    
    
    vMtrlDiffuse = lerp(vMtrlDiffuse, vBrush_Moving, fAlphaValue_Moving);
    Out.vDiffuse = lerp(vMtrlDiffuse, vBrush_Range, fAlphaValue_Range);

    /* -1.f ~ 1.f -> 0.f ~ 1.f */
    Out.vNormal = float4(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);
    Out.vWorld = In.vWorldPos;
    return Out;
}





/*굴절 샷*/

struct VS_SHOT_IN
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float3 vTangent : TANGENT;
};

struct VS_SHOT_OUT
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
};



VS_SHOT_OUT VS_MAIN_SHOT_BOSS09(VS_SHOT_IN In)
{
    VS_SHOT_OUT Out = (VS_SHOT_OUT) 0;

    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    In.vPosition.x += 0.15f * sin(2.f * g_Time + In.vPosition.x);
    In.vPosition.y += 0.15f * (2 * sin(2.f * g_Time + In.vPosition.y) - 1.5 * sin(-1 * 3.f * g_Time + In.vPosition.y));
    if (In.vPosition.z > -0.5f)
        In.vPosition.z += 0.15f * (1.5 * cos(2.f * g_Time) - sin(g_Time + In.vPosition.z));
    else
        In.vPosition.z -= 2.f * (0.3f * cos(2.f * g_Time) - 0.2f * sin(g_Time + In.vPosition.z) + 0.15f * sin(2.f * g_Time * In.vPosition.x + 2.f * g_Time * In.vPosition.z));
    
    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
   
    Out.vNormal = normalize(mul(float4(In.vNormal, 0.f), g_WorldMatrix));
    Out.vTexcoord = In.vTexcoord;
    Out.vWorldPos = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
    Out.vProjPos = Out.vPosition;
    return Out;
      
}

struct PS_SHOT_IN
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
    
};

struct PS_SHOT_OUT
{
    float4 vDistortion : SV_TARGET0;
};

PS_SHOT_OUT PS_SHOT_BOSS09(PS_SHOT_IN In)
{
    
    PS_SHOT_OUT Out = (PS_SHOT_OUT) 0;
    
    vector vViewDir = normalize(In.vWorldPos - g_vCamPosition);
    vector vRefracDir = normalize(refract(vViewDir, normalize(In.vNormal), g_Refractive_index));
    
    
      
    
    float2 vTexcoordScreen;
    vTexcoordScreen.x = In.vProjPos.x / In.vProjPos.w;
    vTexcoordScreen.y = In.vProjPos.y / In.vProjPos.w;
    
    
    vTexcoordScreen.x = (vTexcoordScreen.x) * 0.5f + 0.5f;
    vTexcoordScreen.y = (vTexcoordScreen.y) * -0.5f + 0.5f;
    
   
   //노이즈 샘플링 
    float2 vTexcoordNoise = In.vTexcoord;
    float3 vNoise = g_NoiseTexture.Sample(LinearSampler, vTexcoordNoise);
      
           
    //노이즈 move -
    float2 noiseOffset = (vNoise.gb * 0.1f - 0.1f) * g_NoiseStrength;
    noiseOffset.x = vNoise.x * g_NoiseStrength * sin(g_Time * 3.f);
    noiseOffset.y = vNoise.y * g_NoiseStrength * sin(g_Time * 3.f);
    
   
    
    //스크린 겹치는 스크린 샘플링 
    
  
    
    float2 Refraction_Texcoord = vRefracDir.x * 0.1f + noiseOffset + vTexcoordScreen;
    float2 Diffuse_ReflectedTexcoord = noiseOffset + In.vTexcoord;
    vector vDiffuseColor = g_DiffuseTexture.Sample(LinearSampler, Diffuse_ReflectedTexcoord);
    vDiffuseColor.a = 0.7;
    //스크린 겹치는 스크린 샘플링 
    vector RefracedColor = g_ScreenTexture.Sample(LinearSampler, Refraction_Texcoord);
    vector vFinalColor = lerp(vDiffuseColor, RefracedColor, vDiffuseColor.a);

    Out.vDistortion = vFinalColor;
    return Out;
    
   
}





struct PS_SHOT_GLOW_OUT
{
    float4 vColor : SV_TARGET0;
    float4 vIntensity : SV_TARGET1;
};

/* PixelShader */
PS_SHOT_GLOW_OUT PS_MONSTER09_SHOT_MAIN_GLOW(PS_SHOT_IN In)
{
    PS_SHOT_GLOW_OUT Out = (PS_SHOT_GLOW_OUT) 0;

    vector vViewDir = normalize(In.vWorldPos - g_vCamPosition);
    vector vRefracDir = normalize(refract(vViewDir, normalize(In.vNormal), g_Refractive_index));
    
    
    
    float2 vTexcoordScreen;
    vTexcoordScreen.x = In.vProjPos.x / In.vProjPos.w;
    vTexcoordScreen.y = In.vProjPos.y / In.vProjPos.w;
    
    
    vTexcoordScreen.x = (vTexcoordScreen.x) * 0.5f + 0.5f;
    vTexcoordScreen.y = (vTexcoordScreen.y) * -0.5f + 0.5f;
    
   
   //노이즈 샘플링 
    float2 vTexcoordNoise = In.vTexcoord;
    float3 vNoise = g_NoiseTexture.Sample(LinearSampler, vTexcoordNoise);
      
           
    //노이즈 move -
    float2 noiseOffset = (vNoise.gb * 0.1f - 0.1f) * g_NoiseStrength;
    noiseOffset.x = vNoise.x * g_NoiseStrength * sin(g_Time * 3.f);
    noiseOffset.y = vNoise.y * g_NoiseStrength * sin(g_Time * 3.f);
    
    //스크린 겹치는 스크린 샘플링 
    
    float2 Refraction_Texcoord = vRefracDir.x * 0.1f + noiseOffset + vTexcoordScreen;
    float2 Diffuse_ReflectedTexcoord = noiseOffset + In.vTexcoord;
    vector vDiffuseColor = g_DiffuseTexture.Sample(LinearSampler, Diffuse_ReflectedTexcoord);
    vDiffuseColor.a = 0.7;
    //스크린 겹치는 스크린 샘플링 
    vector RefracedColor = g_ScreenTexture.Sample(LinearSampler, Refraction_Texcoord);
    vector vFinalColor = lerp(vDiffuseColor, RefracedColor, vDiffuseColor.a);

    vector vFinalColor_Origin = vFinalColor;
    vFinalColor = saturate(vFinalColor - float4(g_fThreshold, 0.f));
    
    if (vFinalColor.x > 0.f && vFinalColor.y > 0.f && vFinalColor.z > 0.f)
    {
        Out.vColor = vFinalColor_Origin;
        Out.vColor.b *= 1.5f;
        Out.vIntensity.x = saturate(vFinalColor_Origin.b * g_fIntensity);
    }
    return Out;
    

}
technique11 DefaultTechnique
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


    pass Boss9_ShotPass //1
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);


       
        VertexShader = compile vs_5_0 VS_MAIN_SHOT_BOSS09();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_SHOT_BOSS09();
    }

    pass Boss9_ShotGlow_Pass //2
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);


       
        VertexShader = compile vs_5_0 VS_MAIN_SHOT_BOSS09();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MONSTER09_SHOT_MAIN_GLOW();
    }
}
