
#include "../../../EngineSDK/hlsl/Engine_Shader_Define.hlsli"


/* 상수테이블 ConstantTable */
float4x4		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D		g_Texture;
texture2D g_DepthTexture;


float3 g_vThreshold;
float  g_fIntensity;
/*World UI fOR Every World UI */
float4x4 g_UIWorldMatrix_In_World,g_UIViewMatrix_In_World, g_UIProjMatrix_In_World;
float2 g_WinSize;
float2 g_Size_For_WorldUI;
/*for WorldUI -HPBarForMonster*/
float g_ViewZDistance;
float g_AdjustingHeight = 0;;
float g_BarRatio = 1.f;

/*ForDemageFont*/
float g_OffsetToPlace=0.f;

/*For Skill Cool Time*/
float g_SkillCoolTimeRatio;

float g_ColorR = 1.f;
float g_ColorG = 1.f;
float g_ColorB =1.f;
float g_ColorA = 1.f;
bool g_bColorChange = false;

/*For LoadingBar Ratio */
float g_fLoadingBarRatio;

/*Boss9_Rush Sign */
texture2D g_MovingTexture;
texture2D g_RangeTexture;
float g_fAccTimeDelta;

vector g_vCamPosition;


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
VS_OUT VS_MAIN_FOR_WORLD_UI_HPBar(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;
	
    matrix matWV_In_World, matWVP_In_World;
	
	
	/*몬스터의 위치 행렬 받아와서 곱하기->파이프라인의 뷰 투영 가져와서 끌어옴*/
    matWV_In_World = mul(g_UIWorldMatrix_In_World, g_UIViewMatrix_In_World);
    matWVP_In_World = mul(matWV_In_World, g_UIProjMatrix_In_World);
	
	/*W나누기 수행*/
    float2 NDCPos =
    {
        matWVP_In_World._41/ matWVP_In_World._44,
        matWVP_In_World._42 / matWVP_In_World._44
};
    float2 ScreenPos =
    {
        (NDCPos.x + 1.f) * g_WinSize.x * 0.5f, -(NDCPos.y - 1.f) * g_WinSize.y * 0.5f
    };
    
    float fHeightRatio = 0.f;
    if (40.f>=g_ViewZDistance)
    {
        fHeightRatio = 0.8f;
    }
    else 
    {
        fHeightRatio = 0.8f * exp(-0.03 * (g_ViewZDistance - 40));

    }
   
   
    float fOffsetX = -g_Size_For_WorldUI.x * (1.f - g_BarRatio) * 0.5f;

    float4 WorldPosForOrtho = { fOffsetX + ScreenPos.x - g_WinSize.x * 0.5f, -ScreenPos.y + g_WinSize.y * 0.5F + g_ViewZDistance * fHeightRatio + g_AdjustingHeight, 0.F, 1.f};

   
    
    float4x4 WorldMatrixForOrtho =
    {
        { g_Size_For_WorldUI.x *g_BarRatio, 0.f, 0.f, 0.f },
        { 0.f, g_Size_For_WorldUI.y, 0.f, 0.f },
       { 0.f, 0.f, 1.f, 0.f },
       WorldPosForOrtho,
    };
    
   
	/*직교투영을위한 월드로 끌어내기 완료*/
        matrix matWV, matWVP;

        matWV = mul(WorldMatrixForOrtho, g_ViewMatrix);
        matWVP = mul(matWV, g_ProjMatrix);

        Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
        Out.vTexcoord = In.vTexcoord;

        return Out;

}

VS_OUT VS_MAIN_FOR_WORLD_UI_DEMAGEFONT(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;
	
    matrix matWV_In_World, matWVP_In_World;
	
	
	/*몬스터의 위치 행렬 받아와서 곱하기->파이프라인의 뷰 투영 가져와서 끌어옴*/
    matWV_In_World = mul(g_UIWorldMatrix_In_World, g_UIViewMatrix_In_World);
    matWVP_In_World = mul(matWV_In_World, g_UIProjMatrix_In_World);
	
	/*W나누기 수행*/
    float2 NDCPos =
    {
        matWVP_In_World._41 / matWVP_In_World._44,
        matWVP_In_World._42 / matWVP_In_World._44
    };
    float2 ScreenPos =
    {
        (NDCPos.x + 1.f) * g_WinSize.x * 0.5f, -(NDCPos.y - 1.f) * g_WinSize.y * 0.5f
    };
    
   
   
    float fOffsetX = g_Size_For_WorldUI.x * g_OffsetToPlace;

    float4 WorldPosForOrtho = { fOffsetX + ScreenPos.x - g_WinSize.x * 0.5f, -ScreenPos.y + g_WinSize.y * 0.5F, 0.F, 1.f };

   
    
    float4x4 WorldMatrixForOrtho =
    {
        { g_Size_For_WorldUI.x , 0.f, 0.f, 0.f },
        { 0.f, g_Size_For_WorldUI.y, 0.f, 0.f },
        { 0.f, 0.f, 1.f, 0.f },
       WorldPosForOrtho,
    };
    
   
	/*직교투영을위한 월드로 끌어내기 완료*/
    matrix matWV, matWVP;

    matWV = mul(WorldMatrixForOrtho, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
    Out.vTexcoord = In.vTexcoord;

    return Out;

}

struct VS_OUT_SOFT_EFFECT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float4 vProjPos : TEXCOORD1;
};

VS_OUT_SOFT_EFFECT VS_MAIN_SOFT_EFFECT(VS_IN In)
{
    //VS_OUT_SOFT_EFFECT Out = (VS_OUT_SOFT_EFFECT) 0;
    
    
   

    //matrix matWV, matWVP;

    //matWV = mul(g_WorldMatrix, g_ViewMatrix);
    //matWVP = mul(matWV, g_ProjMatrix);

    //Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);//픽셀까지
    //Out.vTexcoord = In.vTexcoord; 
    //Out.vProjPos = Out.vPosition;  //투영관 좌표
    
    
    VS_OUT_SOFT_EFFECT Out = (VS_OUT_SOFT_EFFECT) 0;
    
    
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

    
    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP); //픽셀까지
    Out.vTexcoord = In.vTexcoord;
    Out.vProjPos = Out.vPosition; //투영관 좌표
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

struct PS_OUT
{
	float4		vColor : SV_TARGET0;
};

/* PixelShader */
PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	/*Out.vColor = g_Texture.Sample(어떻게얻어올래?, In.vTexcoord);*/
	
    vector vFinalColor = (vector) 0;
    vector vDiffuse = g_Texture.Sample(LinearSampler, In.vTexcoord);
    float Alpha = vDiffuse.a;
    vFinalColor = lerp(vFinalColor, vDiffuse,Alpha);
    Out.vColor = vDiffuse;
    //vFinalColor;
    
    if (Out.vColor.a < 0.05f)
        discard;
	
	/*For Number*/
    if (g_bColorChange)
    {
    
        Out.vColor.r *= g_ColorR;
        Out.vColor.b *= g_ColorB;
        Out.vColor.g *= g_ColorG;
    }
	return Out;
}

///* PixelShader */
PS_OUT PS_MAIN_BOSS9_RUSH_SIGN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
	
    vector RangeTexture = g_RangeTexture.Sample(LinearSampler, In.vTexcoord);
    
    In.vTexcoord.y += g_fAccTimeDelta;
   vector MovingTexture = g_MovingTexture.Sample(LinearSampler, In.vTexcoord);
  
    vector FinalColor =  (vector)0;
    FinalColor = lerp(FinalColor, RangeTexture, RangeTexture.a);
    FinalColor = lerp(FinalColor, MovingTexture, MovingTexture.a);
    FinalColor.gb = FinalColor.gb * 0.3;
    Out.vColor = FinalColor;
   
    if (Out.vColor.a < 0.3f)
        discard;
    
    return Out;
}


struct PS_IN_SOFT_EFFECT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float4 vProjPos : TEXCOORD1;
};

PS_OUT PS_MAIN_SOFT_EFFECT(PS_IN_SOFT_EFFECT In)
{
    PS_OUT Out = (PS_OUT) 0;
	
    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);  //텍스처 샘플링(00~11)

    float2 vTexcoord = 0.f;
    //텍스 쿠드 그대로 가져가면 안되고 , 내픽셀의 위치를 찾아서 투영관 좌표까지 바꾸고  그 위치로부터 TEXCOORD를 구해야함
    
    //(-1, 1)~(1, -1) ndc -> texcoord
    vTexcoord.x = (In.vProjPos.x / In.vProjPos.w) * 0.5f + 0.5f; //이펙트가 있는위치의, NDC좌표를얻어오고->TEXCOORD로대응 -> 대응되는 뎊스영역을 따온다.
    vTexcoord.y = (In.vProjPos.y / In.vProjPos.w) * -0.5f + 0.5f;

    //->텍스쿠드를 뎊스 스텐실 영역까지 끌고 온다
    
    vector vDepth = g_DepthTexture.Sample(LinearSampler, vTexcoord);  //해당위치의 텍스처의 texcoord는 받았다. 근데 이펙트 텍스처의 위치와 텍스쿠드는 아는데,,,

    float fOldZ = vDepth.y * 1000.f;   //(지형) 뎊스텍스처의 y값(viewz 를 far로 나눠 0~1로 맞춘 값)에 far값을 곱해 (지형의)이전의 깊이갚을 구함

    //원래 알파가 0이라면 알파가 그대로 0이 되도록알파를 곱하고  oldz 에 지형의 뷰z 를 빼서  차이가 많이나면 알파가 1에 가깝도록 해서 지형에 닿은부분은 알파가 0이되어 투명하게 섞이게 만든다.
    Out.vColor.a = Out.vColor.a * saturate(fOldZ - In.vProjPos.w);//(지형 깊이값에서 나의 viewzZ를 뺸다.투영관 행렬의 W 가 나의 VIEWZ (이펙트의))

    return Out;

}



PS_OUT PS_MAIN_UI_SKILL_COOLTIME(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

	/*Out.vColor = g_Texture.Sample(어떻게얻어올래?, In.vTexcoord);*/
	
    vector vFinalColor = (vector) 0;
    vector vDiffuse = g_Texture.Sample(LinearSampler, In.vTexcoord);
    float Alpha = vDiffuse.a;
    vFinalColor = lerp(vFinalColor, vDiffuse, Alpha);
    Out.vColor = vFinalColor;
    
    if (Out.vColor.a < 0.05f)
        discard;
	
	/*For Number*/
    
    if (In.vTexcoord.x < g_SkillCoolTimeRatio)
    {
        discard;
    }
       
    return Out;
}



PS_OUT PS_MAIN_UI_LOADING(PS_IN In)//8
{
    PS_OUT Out = (PS_OUT) 0;

    vector vFinalColor = (vector) 0;
    vector vDiffuse = g_Texture.Sample(LinearSampler, In.vTexcoord);
    Out.vColor = vDiffuse;
    
    if (Out.vColor.a < 0.05f)
        discard;
	
	/*For Number*/
    
    if (In.vTexcoord.x > g_fLoadingBarRatio)
    {
        discard;
    }
       
    return Out;
}



technique11			DefaultTechnique
{
	/* 우리가 수행해야할 정점, 픽셀 셰이더의 진입점 함수를 지정한다. */
	pass DefaultPass//0
	{
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}
    pass UI_MonsterHPBar1//1
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_FOR_WORLD_UI_HPBar();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }
    pass UI_DemageFont2//2
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_FOR_WORLD_UI_DEMAGEFONT();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }

    pass Boss9_RushSign //3
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BOSS9_RUSH_SIGN();
    }

    pass SoftEffect//4
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_SOFT_EFFECT();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_SOFT_EFFECT();
    }

    pass TrailPass //5
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }


    pass BilloardPass //6
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_BILLBOARD();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }

    pass UI_Skill_CoolTime_Pass //7
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_UI_SKILL_COOLTIME();
    }

    pass UI_LoadingBarRatioPass //8
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_UI_LOADING();
    }

}
