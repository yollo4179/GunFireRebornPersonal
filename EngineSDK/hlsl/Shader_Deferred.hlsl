

#include "Engine_Shader_Define.hlsli"


float4x4 g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
float4x4 g_ViewMatrixInv, g_ProjMatrixInv;
float4x4 g_LightViewMatrix, g_LightProjMatrix;
texture2D g_Texture;
texture2D g_NormalTexture;
texture2D g_DiffuseTexture;
texture2D g_ShadeTexture;
texture2D g_DepthTexture;
texture2D g_SpecularTexture;
texture2D g_LightDepthTexture;

texture2D g_DistortionTexture;

texture2D g_FinalTexture;
texture2D g_Distortion_FinalTexture;
texture2D g_BlurXTexture;
texture2D g_BlurYTexture;
texture2D g_Glow_Original_Texture;
texture2D g_Glow_Range_Texture;
texture2D g_Glow_Intensity_Texture;
texture2D g_GlowTexture;

texture2D g_EventTexture;
float g_fAccEventTimeDelta;
bool g_EventOn;

float2 g_TexelSizeToSampling;
texture2D g_SamplingTexture;
uint g_MultiSize;
texture2D g_SamplingBlurXTexture;
texture2D g_SamplingBlurYTexture;
texture2D g_Glow_UpSampling;

vector g_vLightDir;
vector g_vLightPos;
float g_fLightRange;

vector g_vLightDiffuse;
vector g_vLightAmbient;
vector g_vLightSpecular;

vector g_vMtrlAmbient = 1.f;
vector g_vMtrlSpecular = 1.f;

vector g_vCamPosition;

//내부 외각선 검출 ( 법선)
float DetectNormalEdges(float2 uv, float2 PixelSize)
{ //중심 픽셀의 깊이값 
    vector vCenterNormal = g_NormalTexture.Sample(LinearSampler, uv) * 2.f - 1.f;
    
    vector NormalOffsets[4];
    NormalOffsets[0] = g_NormalTexture.Sample(LinearSampler_Clamp, uv + float2(-PixelSize.x/5, 0.f)) * 2.f - 1.f;
    NormalOffsets[1] = g_NormalTexture.Sample(LinearSampler_Clamp, uv + float2(PixelSize.x/5, 0.f)) * 2.f - 1.f;
    NormalOffsets[2] = g_NormalTexture.Sample(LinearSampler_Clamp, uv + float2(0.f, -PixelSize.y/5)) * 2.f - 1.f;
    NormalOffsets[3] = g_NormalTexture.Sample(LinearSampler_Clamp, uv + float2(0.f, PixelSize.y/5)) * 2.f - 1.f;
  
    
    //상하좌우 깊이 변화로 엣지 강도 계산 
    float fEdgeStrength = 0.0f;
    for (int i = 0; i < 4; ++i)
    {
        fEdgeStrength += length(vCenterNormal - NormalOffsets[i]);
    }
    return saturate(fEdgeStrength * 2.f);

}

//내부 Depth 검출 깊이 ( )
float DetectDepthEdges(float2 uv, float2 PixelSize)
{ //중심 픽셀의 깊이값 
    vector vCenterDepth = g_DepthTexture.Sample(LinearSampler, uv);
    
    float fViewZ = vCenterDepth.y * 1000.f; 
    float DepthOffsets[4];
    DepthOffsets[0] = g_DepthTexture.Sample(LinearSampler_Clamp, uv + float2(-PixelSize.x, 0.f)).y*1000.f;
    DepthOffsets[1] = g_DepthTexture.Sample(LinearSampler_Clamp, uv + float2(PixelSize.x, 0.f)).y * 1000.f;
    DepthOffsets[2] = g_DepthTexture.Sample(LinearSampler_Clamp, uv + float2(0.f, -PixelSize.y)).y * 1000.f;
    DepthOffsets[3] = g_DepthTexture.Sample(LinearSampler_Clamp, uv + float2(0.f, PixelSize.y)).y * 1000.f;
    //상하좌우 깊이 변화로 엣지 강도 계산 
    float fEdgeStrength = 0.0f;
    for (int i = 0; i < 4; ++i)
    {
        fEdgeStrength += length(fViewZ - DepthOffsets[i]);
    }
    return saturate(fEdgeStrength * 20.f);

}

//shade 연산을 위해 노말 target과 빛 타겟이필요  노말+디퓨즈-> 셰이드 -(그리기 전에 직교vubuffer에 기록)scaling 사이즈는 해상도만큼

struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)   /*0*/
{
    VS_OUT Out = (VS_OUT) 0;

    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
    Out.vTexcoord = In.vTexcoord;

    return Out;
}
struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;

};

struct PS_OUT
{
    float4 vColor : SV_TARGET0;
};

/* PixelShader */
PS_OUT PS_MAIN_DEBUG(PS_IN In)//0
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);

    return Out;
}

struct PS_OUT_LIGHT
{
    float4 vShade : SV_TARGET0;
    float4 vSpecular : SV_TARGET1;// 엔진 빛 연산에서 , 스페큘러를 위한 렌더타겟 결과 반환
};

PS_OUT_LIGHT PS_MAIN_LIGHT_DIRECTIONAL(PS_IN In)//1
{
    PS_OUT_LIGHT Out = (PS_OUT_LIGHT) 0;

    vector vNormalDesc = g_NormalTexture.Sample(PointSampler, In.vTexcoord);
    vector vDepthDesc = g_DepthTexture.Sample(PointSampler, In.vTexcoord);
    
    float fViewZ = vDepthDesc.y * 1000.f; 
    //깊이의 x에는 ndc z값, y에는  viewZ를 far로 나누값이 있다 (저장포맷이 0.f~1.f 사이값밖에 저장을 할수 없으므로-> 0~1로 치화변환해서 저장했다)
    vector vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);
    
    vector vShade = saturate(dot(normalize(g_vLightDir) * -1.f, vNormal)) + (g_vLightAmbient * g_vMtrlAmbient);;

    Out.vShade = g_vLightDiffuse * saturate(vShade);

    
    vector vReflect = reflect(normalize(g_vLightDir), vNormal);// 빛의 반사벡터
    
    
    /* 현재 픽셀의 월드위치가 필요하다. */
	/* 현재 픽셀 : 화면에 그려지고 있는 디퍼드 셰이딩을 요하는 객체들(지형, 프렐이어 몬스터)을 구성하는 픽셀들 <-> 직교투영으로 그리느느 사각형 버퍼의 픽셀(x)*/
	/* 뷰포트, 투영스페이스는 화면에 그려지는 영역이 고정되어있다. */
	/* 뷰포트 상 왼쪽 위에그려지는 픽셀의 텍스쿠드 (0, 0) == 투영스페이스 상의 왼쪽위 픽셀의 위치(-1.f, 1.f) */
	/* 이 연산을 통해 투영스페이스 상의 위치를 구하낟.*/

    vector vPosition = 0.f;

	/* 로컬정점위치 * 월드행렬 * 뷰행렬 * 투영행렬 * 1.f / w */
    //texcoord(0, 0)~texcoord(1, 1)-> ndc(-1,1)~ndc(1,-1)
    vPosition.x = In.vTexcoord.x * 2.f - 1.f;
    vPosition.y = In.vTexcoord.y * -2.f + 1.f;
    vPosition.z = vDepthDesc.x;  //ndc 깊이 값(0~1)
    vPosition.w = 1.f;// viewZ 로 나눈 결과니까 그대로 1이 나와야 한다.

	/* 로컬정점위치 * 월드행렬 * 뷰행렬 * 투영행렬 */
    vPosition *= fViewZ;  //z 나누기를 곱해서 복원한 후에 투영의 역행렬을 곱한다 -> 뷰행렬

	/* 로컬정점위치 * 월드행렬 * 뷰행렬 */
    vPosition = mul(vPosition, g_ProjMatrixInv);

	/* 로컬정점위치 * 월드행렬 */
    vPosition = mul(vPosition, g_ViewMatrixInv); //뷰의 역행렬을 곱한다 ->월드위치 

    vector vLook = vPosition - g_vCamPosition; // 내위치에 카메라의 위치를 빼서 카메라가 위치를 바라보는 방향 벡터를 구한다

    float fSpecular = pow(saturate(dot(normalize(vLook) * -1.f, normalize(vReflect))), 30.f); //빛의 반사벡터와 카메라사이의 각이 작을수록 카메라에 빛이 많이 들어옴

    Out.vSpecular = (g_vLightSpecular * g_vMtrlSpecular) * fSpecular;

    return Out;
   
}

PS_OUT_LIGHT PS_MAIN_LIGHT_POINT(PS_IN In)//2
{
    PS_OUT_LIGHT Out = (PS_OUT_LIGHT) 0;
    vector vNormalDesc = g_NormalTexture.Sample(PointSampler, In.vTexcoord);
    vector vDepthDesc = g_DepthTexture.Sample(PointSampler, In.vTexcoord);
    float fViewZ = vDepthDesc.y * 1000.f;

    vector vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);

	/* 현재 픽셀의 월드위치가 필요하다. */
	/* 현재 픽셀 : 화면에 그려지고 있는 디퍼드 셰이딩을 요하는 객체들(지형, 프렐이어 몬스터)을 구성하는 픽셀들 <-> 직교투영으로 그리느느 사각형 버퍼의 픽셀(x)*/
	/* 뷰포트, 투영스페이스는 화면에 그려지는 영역이 고정되어있다. */
	/* 뷰포트 상 왼쪽 위에그려지는 픽셀의 텍스쿠드 (0, 0) == 투영스페이스 상의 왼쪽위 픽셀의 위치(-1.f, 1.f) */
	/* 이 연산을 통해 투영스페이스 상의 위치를 구하낟.*/

    vector vPosition = 0.f;

	/* 로컬정점위치 * 월드행렬 * 뷰행렬 * 투영행렬 * 1.f / w */
    vPosition.x = In.vTexcoord.x * 2.f - 1.f;
    vPosition.y = In.vTexcoord.y * -2.f + 1.f;
    vPosition.z = vDepthDesc.x;
    vPosition.w = 1.f;

	/* 로컬정점위치 * 월드행렬 * 뷰행렬 * 투영행렬 */
    vPosition *= fViewZ;

	/* 로컬정점위치 * 월드행렬 * 뷰행렬 */
    vPosition = mul(vPosition, g_ProjMatrixInv);

	/* 로컬정점위치 * 월드행렬 */
    vPosition = mul(vPosition, g_ViewMatrixInv);

    vector vLightDir = vPosition - g_vLightPos; //월드 위치까지 구했고 , 점광원 의치에서 뺀다.

    float fDistance = length(vLightDir);//나와 점광원사이의 거리

	/* 빛의 감쇄값*/
	/* 픽셀과 빛이 가까우면 1에 가까운값으로 멀면 0에 가까운 값으로 범위를 벗어나면 다 0으로. */
    float fAtt = saturate((g_fLightRange - fDistance) / g_fLightRange);//내위치가 점광원 range보다 크면(음수) 0으로 자른다 ( 광원효과가 안미친다는 의미)

    //reflect 도 필요없음 모든 위치로 향하는 광원을 발사 
    vector vShade = fAtt * (saturate(dot(normalize(vLightDir) * -1.f, vNormal)) + (g_vLightAmbient * g_vMtrlAmbient));

    Out.vShade = g_vLightDiffuse * saturate(vShade);

    vector vReflect = reflect(normalize(vLightDir), vNormal);

    vector vLook = vPosition - g_vCamPosition;


    float fSpecular = fAtt * pow(saturate(dot(normalize(vLook) * -1.f, normalize(vReflect))), 30.f);

    Out.vSpecular = (g_vLightSpecular * g_vMtrlSpecular) * fSpecular;

    return Out;
}

struct PS_OUT_FINAL
{
    float4 vFinal : SV_TARGET0;
};
PS_OUT_FINAL PS_MAIN_FINAL(PS_IN In)//3
{
    PS_OUT_FINAL Out = (PS_OUT_FINAL) 0;

    vector vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    if (0.f == vDiffuse.a)
        discard;

    vector vShade = g_ShadeTexture.Sample(LinearSampler, In.vTexcoord);

    vector vSpecular = g_SpecularTexture.Sample(LinearSampler, In.vTexcoord);

   
    Out.vFinal = vDiffuse * vShade; //+ vSpecular;
 
    vector vDepthDesc = g_DepthTexture.Sample(PointSampler, In.vTexcoord);
    float fViewZ = vDepthDesc.y * 1000.f;


    vector vPosition = 0.f;

	/* 로컬정점위치 * 월드행렬 * 뷰행렬 * 투영행렬 * 1.f / w */
    vPosition.x = In.vTexcoord.x * 2.f - 1.f;
    vPosition.y = In.vTexcoord.y * -2.f + 1.f;
    vPosition.z = vDepthDesc.x;
    vPosition.w = 1.f;

	/* 로컬정점위치 * 월드행렬 * 뷰행렬 * 투영행렬 */
    vPosition *= fViewZ;

	/* 로컬정점위치 * 월드행렬 * 뷰행렬 */
    vPosition = mul(vPosition, g_ProjMatrixInv);

	/* 로컬정점위치 * 월드행렬 */
    vPosition = mul(vPosition, g_ViewMatrixInv);

    vPosition = mul(vPosition, g_LightViewMatrix);
    vPosition = mul(vPosition, g_LightProjMatrix);
            
    float2 vTexcoord = 0.f;

    vTexcoord.x = (vPosition.x / vPosition.w) * 0.5f + 0.5f;
    vTexcoord.y = (vPosition.y / vPosition.w) * -0.5f + 0.5f;


    float4 vLightDepth = g_LightDepthTexture.Sample(LinearSampler, vTexcoord);

    if ((vLightDepth.x * 1000.f) < vPosition.w - 1.0f)
    {
        Out.vFinal = vector(Out.vFinal.rgb * 0.5f, 1.f);
    }
    {/*toon shading*/
        vector vDistortion = g_DistortionTexture.Sample(LinearSampler, In.vTexcoord);
        
       
        if (0.8f <= DetectDepthEdges(In.vTexcoord, float2((1.f / 1280.f), (1.f / 720.f))) && 0.2f <= DetectNormalEdges(In.vTexcoord, float2((1.f / 1280.f), (1.f / 720.f))))
            Out.vFinal = float4(0.f, 0.f, 0.f, 1.f);
    }
  
    return Out;
}
PS_OUT PS_MAIN_ASSEMBLE_DISTORTION(PS_IN In)//4
{
    PS_OUT Out = (PS_OUT) 0;
    
    Out.vColor = g_FinalTexture.Sample(LinearSampler, In.vTexcoord);
    {/*Distortion*/
    
        float4 vDistortion = g_DistortionTexture.Sample(LinearSampler, In.vTexcoord);
    
        if (dot(vDistortion, vDistortion) > 1e-12)
            Out.vColor = vDistortion;
    }
    return Out;
}


/*Blur*/
float g_fWeights[13] =
{
    0.0561, 0.1353, 0.278, 0.4868, 0.7261, 0.9231, 1.f, 0.9231, 0.7261, 0.4868, 0.278, 0.1353, 0.0561
};



struct PS_OUT_BLURX
{
    vector vBlurX : SV_TARGET0;
};

PS_OUT_BLURX PS_MAIN_BLUR_X(PS_IN In)//5
{
    PS_OUT_BLURX Out = (PS_OUT_BLURX) 0;

    //vector vColorToCheck = g_Glow_Original_Texture.Sample(LinearSampler, In.vTexcoord);
    //vColorToCheck.w = 0;
    //if (dot(vColorToCheck, vColorToCheck) == 0.f)
    //{
       
    //    Out.vBlurX = 0;
    //    return Out;
        
    //}    
    float2 vTexcoord = 0.f;

    /*내 양옆으로 가우시안 블러 가중치 비율 값 만큼 합친다 -> sv_Position-> 스크린 좌표 , -> 그값의 평균이 픽셀의 색상*/  /*스크린위치 /해상도 -> 텍스쿠드*/
    
    //vector vDistortion = g_DistortionTexture.Sample(LinearSampler, In.vTexcoord);
    //vDistortion.w = 0.f;
    //vector vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    //vDiffuse.w = 0.f;
    
    for (int i = -6; i < 7; i++)
    {
        vTexcoord = float2(In.vTexcoord.x + (1.f / 1280.f) * i, In.vTexcoord.y);

        
       
        vector vIntensity = g_Glow_Intensity_Texture.Sample(LinearSampler_Clamp, vTexcoord);
        vector vBlur = g_Glow_Original_Texture.Sample(LinearSampler_Clamp, vTexcoord);
        vBlur.w = 0;
        //if (dot(vBlur, vBlur) != 0.f)
        //{
          
            Out.vBlurX += g_fWeights[i + 6] * vBlur;
      //  }
        //else
        //{
        //    Out.vBlurX += g_fWeights[i + 6] * g_FinalTexture.Sample(LinearSampler_Clamp, vTexcoord);
        //}
    }
    /*파이널 텍스처에 적용-> */
    
    Out.vBlurX /= 6.f;

    return Out;
}

PS_OUT PS_MAIN_BLUR_Y(PS_IN In)//6
{
    
    /*X 블러 먹인 후 -> Y 블러 먹임 ,,->진짜 파이널 */
    PS_OUT Out = (PS_OUT) 0;

    float2 vTexcoord = 0.f;

    //vector vColorToCheck = g_BlurYTexture.Sample(LinearSampler_Clamp, In.vTexcoord);
    //vColorToCheck.w = 0.f;
    //if (dot(vColorToCheck, vColorToCheck) == 0.f)
    //{
    //    Out.vColor = 0;
    //    return Out;
    
    // }
    
    vector vBlur = 0.f;
    for (int i = -6; i < 7; i++)
    {
        vTexcoord = float2(In.vTexcoord.x, In.vTexcoord.y + (1.f / 720.f) * i);

        vector vBlurY = g_BlurYTexture.Sample(LinearSampler_Clamp, vTexcoord);
        vBlurY.w = 0;
        
       // if (dot(vBlurY, vBlurY) != 0.f)
       // {
        
            vBlur += g_fWeights[i + 6] * vBlurY;
       // }
       // else
       // {
         //   vBlur += g_fWeights[i + 6] * g_FinalTexture.Sample(LinearSampler_Clamp, vTexcoord);
       // }
    }

    vBlur /= 6.f;

    /*X 축 블러 적용한 파이널 텍스처에 적용*/
    Out.vColor = vBlur;

    return Out;
}
PS_OUT PS_MAIN_GLOW(PS_IN In)//7
{
    PS_OUT Out = (PS_OUT) 0;
    vector vGlowOriginColor = g_Glow_Original_Texture.Sample(LinearSampler, In.vTexcoord);
    vector vGlowColor = g_GlowTexture.Sample(LinearSampler, In.vTexcoord);
    vector vGlowIntensity = g_Glow_Intensity_Texture.Sample(LinearSampler, In.vTexcoord);
   
    vGlowIntensity.w = 0.f;
    if (0.f == vGlowIntensity.x&&
        dot(vGlowColor.xyz, vGlowColor.xyz)!= 0
        )
        vGlowIntensity.x = 3.f;
  else 
        vGlowIntensity.x *= 6.f;
    Out.vColor = vGlowOriginColor + vGlowColor * vGlowIntensity.x;
    return Out;
}


PS_OUT    PS_MAIN_ASSEMBLE(PS_IN In) //8
{
    PS_OUT Out = (PS_OUT)0;

    vector vFinalColor = g_FinalTexture.Sample(LinearSampler, In.vTexcoord);
  
    Out.vColor = vFinalColor;
    
        /*EventNoise*/
    {
        if (true == g_EventOn)
        {
        
           
        
            float2 vTexcoordNoise = In.vTexcoord;
            float3 vNoise = g_EventTexture.Sample(LinearSampler, vTexcoordNoise);
           
    //노이즈 move -
            float2 noiseOffset = (vNoise.gb - 0.5f) * 0.5;
            noiseOffset.x = vNoise.x * 0.05f * cos(g_fAccEventTimeDelta * 0.8f);
            noiseOffset.y = vNoise.y * 0.05f * sin(g_fAccEventTimeDelta * 0.8f);
    
   
    
    //스크린 겹치는 스크린 샘플링 
    
            float2 Refraction_Texcoord = noiseOffset + vTexcoordNoise;
        
            Out.vColor = g_FinalTexture.Sample(LinearSampler, Refraction_Texcoord);;
            Out.vColor.b *= 2.f;
            return Out;
        }
    }
      
   
    
    {/*Glow*/
        vector vGlowOriginColor = g_Glow_Original_Texture.Sample(LinearSampler, In.vTexcoord);
        float4 vGlow = g_GlowTexture.Sample(LinearSampler, In.vTexcoord);
        
        if (dot(vGlow, vGlow) > 0.f)
        {
           
            if (dot(vGlowOriginColor.xyz, vGlowOriginColor.xyz)>0)
                Out.vColor -= vFinalColor;
            Out.vColor += vGlow;

        }
    }
    
    
      {/*toon shading*/
        vector vDistortion = g_DistortionTexture.Sample(LinearSampler, In.vTexcoord);
        
        if (dot(vDistortion.xyz, vDistortion.xyz) == 0) //왜곡 아님
            if (0.8f <= DetectDepthEdges(In.vTexcoord, float2((1.f / 1280.f), (1.f / 720.f))) && 0.2f <= DetectNormalEdges(In.vTexcoord, float2((1.f / 1280.f), (1.f / 720.f))))
                Out.vColor = float4(0.f, 0.f, 0.f, 1.f);
    }
    
   
       
    return Out;
    

}
float CubicWeight(float x)
{
    x = abs(x);
    if (x <= 1.0)
    {   
        return (1.5 * x - 2.5) * x * x + 1.0;
    }
    else if (x < 2.0)
    {
        return ((-0.5 * x + 2.5) * x - 4.0) * x + 2.0;
    }
    return 0.0;
}

PS_OUT PS_MAIN_DOWNSAMPLING (PS_IN In)//9
{
    PS_OUT Out = (PS_OUT) 0;
    float4 vColor = g_SamplingTexture.Sample(LinearSampler, In.vTexcoord );
    Out.vColor = vColor;
    return Out;
}
PS_OUT PS_MAIN_DOWNSAMPLING_BLURX(PS_IN In)//10
{
    PS_OUT Out = (PS_OUT) 0;
    float2 vTexcoord = 0.f;
    for (int i = -6; i < 7; i++)
    {
        vTexcoord = float2(In.vTexcoord.x + (1.f / g_TexelSizeToSampling.x) * i, In.vTexcoord.y);
        vector vBlur = g_SamplingBlurXTexture.Sample(LinearSampler_Clamp, vTexcoord);
        vBlur.w = 0;
         Out.vColor += g_fWeights[i + 6] * vBlur;
   
    } 
    Out.vColor /= 6.f;
    return Out; 
}
PS_OUT PS_MAIN_DOWNSAMPLING_BLURY(PS_IN In)//11
{
    PS_OUT Out = (PS_OUT) 0;
    float2 vTexcoord = 0.f;
    vector vBlur = 0.f;
    for (int i = -6; i < 7; i++)
    {
        vTexcoord = float2(In.vTexcoord.x, In.vTexcoord.y + (1.f / g_TexelSizeToSampling.y) * i);
        vector vBlurY = g_SamplingBlurYTexture.Sample(LinearSampler_Clamp, vTexcoord);
        vBlurY.w = 0; 
        vBlur += g_fWeights[i + 6] * vBlurY;
    }
    vBlur /= 6.f;
    /*X 축 블러 적용한 파이널 텍스처에 적용*/
    Out.vColor = vBlur;
    return Out;
  
}

float4 Gaussian_Row_UpSampling(float2 _vtexcoord)
{
    float4 GaussianColor = 0;
    float2 vTexcoord = 0;
    for (int i = -6; i < 7; i++)
    {
        vTexcoord = float2(_vtexcoord.x + (1.f / 1280.f) * i, _vtexcoord.y);
        vector vBlur = g_Glow_UpSampling.Sample(LinearSampler_Clamp, vTexcoord);
        vBlur.w = 0;
       
          
        GaussianColor+= g_fWeights[i + 6] * vBlur;
     
    }
    /*파이널 텍스처에 적용-> */
    GaussianColor /= 6.f;
    return GaussianColor;
}
float4 Gaussian_Col_UpSampling(float2 _vtexcoord)
{
    float4 GaussianColor = 0;
    float2 vTexcoord = 0;
    for (int i = -6; i < 7; i++)
    {
        vTexcoord = float2(_vtexcoord.x, _vtexcoord.y + (1.f / 720.f) * i);
        vector vBlur = g_Glow_UpSampling.Sample(LinearSampler_Clamp, vTexcoord);
        vBlur.w = 0;
       
          
        GaussianColor += g_fWeights[i + 6] * vBlur;
     
    }
    /*파이널 텍스처에 적용-> */
    GaussianColor /= 6.f;
    return GaussianColor;
}
PS_OUT PS_MAIN_UPSAMPLING(PS_IN In)//12
{
    PS_OUT Out = (PS_OUT) 0;
    vector vColor = g_Glow_UpSampling.Sample(LinearSampler, In.vTexcoord);
    Out.vColor = vColor;
    return Out;   

}

PS_OUT PS_MAIN_GLOW_DEFFERED_UPSMAPLING(PS_IN In)//13
{
    PS_OUT Out = (PS_OUT) 0;
    vector vGlowOriginColor = g_Glow_Original_Texture.Sample(LinearSampler, In.vTexcoord);
    vector vGlowColor = g_Glow_UpSampling.Sample(LinearSampler, In.vTexcoord);
    vector vGlowIntensity = g_Glow_Intensity_Texture.Sample(LinearSampler, In.vTexcoord);
    vGlowColor = Gaussian_Col_UpSampling(In.vTexcoord);
   
   
    vGlowIntensity.w = 0.f;
    if (0.f == vGlowIntensity.x &&
        dot(vGlowColor.xyz, vGlowColor.xyz) != 0
        )
        vGlowIntensity.x = 3.f;
    else
        vGlowIntensity.x *= 6.f;
    Out.vColor = vGlowOriginColor + vGlowColor * vGlowIntensity.x;
    return Out;
}

technique11 DefaultTechnique
{
	/* 우리가 수행해야할 정점, 픽셀 셰이더의 진입점 함수를 지정한다. */
    pass Debug//0
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DEBUG();
    }

    pass Light_Directional//1
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_OneBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_LIGHT_DIRECTIONAL();
    }

    pass Light_Point//2
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_OneBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_LIGHT_POINT();
    }

    pass Final//3
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_FINAL();
    }
    pass Assemble_Distortion//4
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_ASSEMBLE_DISTORTION();
    }
  
    
    pass BlurX//5
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLUR_X();
    }

    pass BlurY //6
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLUR_Y();
    }
    pass Glow //7
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_GLOW();
    }


    pass AssmebleAll//8
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_ASSEMBLE();
    }
    pass DownSampling //9
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DOWNSAMPLING();
    }
    pass DownSampling_BlurX //10
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DOWNSAMPLING_BLURX();
    }
    pass DownSampling_BlurY //11
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DOWNSAMPLING_BLURY();
    }
    pass UpSampling //12
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_UPSAMPLING();
    }

    pass UpSampling_Glow_Defferd //13
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_GLOW_DEFFERED_UPSMAPLING();
    }


}
