

#include "../../../EngineSDK/hlsl/Engine_Shader_Define.hlsli"

/* 상수테이블 ConstantTable */
float4x4 g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D g_Texture;

vector g_vCamPosition;

/* VertexShader */

/* 정점쉐이더의 구동 시점. */
/* m_pContext->DrawIndexed() */
/* 그릴때 사용하고자했던 정점들중, 인덱스버퍼(0, 1, 2, 0, 2, 3)가 지정해주고 있는 정점을 골라서 */
/* VS_MAIN함수를 호출하며 인자로 전달해준다. */

struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vPSize : PSIZE;

    row_major float4x4 TransformMatrix : WORLD;  
    //이거는 로컬배치 행렬 -> 사이즈랑 위치를 분리해서 일단 월드에 배치후 지오에서 크기 다시적용 -> 점하나에는 행렬에 크기가 의미가 없으므로 
    //이동량에 문제가 생길수도있다 점에 스케일 있으면 월드 곱할 때
    float2 vLifeTime : TEXCOORD0;
};

struct VS_OUT
{
    /*여기서 SV는 붙이면 안된다 SV는 내가 정점 연산을 셰이더에서 다끝냈으니 WVP 연산을끝냈으니W 나누기 등 장치 연산을 하라는 얘기인데*/
    /*w 나누기를 하면 GEOMETRY shader에서 정점 만드는 작업을 제대로할수 없다(안끝났고, 기하에서 더 처리해야됨) -> 레스터라이즈 , 선형보간 등의 작업은 기하셰이더 에서 정점을 추가하고 그 정점들을 추가한 것을 기반으로 정점 선형보간등을 거쳐 픽셀 연산을 수행하게한다. */
    float4 vPosition : POSITION;
    float2 vPSize    : PSIZE;
    float2 vLifeTime : TEXCOORD0;
};

/* 1. 정점위치에 대한 기초적인 변환과정을 수행한다.(월드변환, 뷰변환, ) */

/*로컬에서 파티클을 먼저 배치하고 월드에 파티클을 배치한다*/
VS_OUT VS_MAIN(VS_IN In)
{
    /*하는일 점에서 스케일 분리 -> 월드로 배치,(카메라의 월드를 가져와 카메라를 바라보도록 하려면(빌보드)월드로 점을 배치해야 나머지 4점의 위치를 결정하기 쉽다)*/
    /*월드 배치하고 4점 카메라바라보게 월드 좌표 구하고 ->기하에서 스케일 을 이영해 4점의 거리 결정, -> 인덱스 (점 6개)매핑*/
    
    VS_OUT Out = (VS_OUT) 0;

    matrix matWV, matWVP;
    
    float fScaleX = length(In.TransformMatrix._11_12_13);
    float fScaleY = length(In.TransformMatrix._21_22_23);
    float fScaleZ = length(In.TransformMatrix._31_32_33);

    matrix TransformMatrix = float4x4(float4(1.f, 0.f, 0.f, 0.f), float4(0.f, 1.f, 0.f, 0.f), float4(0.f, 0.f, 1.f, 0.f), In.TransformMatrix._41_42_43_44);
    /* 크기 분리한 후 원점->로컬 배치 행렬 구하기*/
    
    vector vPosition = mul(float4(In.vPosition, 1.f), TransformMatrix);
    /*(원점상에 배치한 내 점point위치 * 크기추출한 로컬배치 행렬)-> 원점을 로컬 배치 */
    //월드만 콥할거다 -> 기하에서 월드 계산이 더 필요하니까( 카메라를 이용한 빌보드 후 뷰투영 곱할거)

    Out.vPosition = mul(vPosition, g_WorldMatrix);
    Out.vPSize = float2(In.vPSize.x * fScaleX, In.vPSize.y * fScaleY);
    /*vpSize는 처음설정할때1이고 트랜스폼에 크기 적용시켰다 -> 곱해야함*/
    Out.vLifeTime = In.vLifeTime;

    return Out;
}

struct VS_OUT_NON_BILLBOARD
{
    float4 vPosition : POSITION;
    row_major float4x4 TransformMatrix : WORLD;
    float2 vLifeTime : TEXCOORD0;
};
VS_OUT_NON_BILLBOARD VS_MAIN_NON_BILLBOARD(VS_IN In)
{
 
    VS_OUT_NON_BILLBOARD Out = (VS_OUT_NON_BILLBOARD) 0;
    matrix matWV, matWVP;
    
    matrix TransformMatrix = (matrix)0;
    TransformMatrix._11_22_33 = 1;
    TransformMatrix._41_42_43_44 = In.TransformMatrix._41_42_43_44;
    vector vPosition = mul(float4(In.vPosition, 1.f), TransformMatrix);

    Out.vPosition = mul(vPosition, g_WorldMatrix);
   
    Out.vLifeTime = In.vLifeTime;
    Out.TransformMatrix = In.TransformMatrix;
    return Out;
}


/* VS : 정점하나 당 호출되는 함수다. */
/* GS : 그리고자하는 도형(POINT, LINE, TRIANGLE) 하나당 호출되는 함수이다. */
/* PS : 픽셀하나 당 호출되는 함수다. */
struct GS_IN
{
    float4 vPosition : POSITION; //최종연산안끈남 ->SV 는 X
    float2 vPSize : PSIZE;
    float2 vLifeTime : TEXCOORD0;
    
};
struct GS_OUT
{
    float4 vPosition : SV_POSITION; //gs에서 빌보드하고 vp까지 곱해서 연산끝낼거
    float2 vTexcoord : TEXCOORD0;
    float2 vLifeTime : TEXCOORD1;
};


/* 인자는 점 한개 , 삼각형을 담는 벡터라고 생각 삼각형 2개만들거니까->점 6개를 push (append) 디폴드로 triangle_stream 으로 처리하니까*/
/*triangle_list 방식으로 정점 매핑할수있게 함수 따로 호출해야 편함-> 시계방향도 신경써야하니까*/
/*한 세이더가 기하셰이더쓰면 다른 셰이더에도 적용되버리니까 안스는것은 패스에서 null로 막아둔다 */
/* 이거 안하면 터진다 벡터의 resize같은 느낌 -> 삼각형 2개 *인스턴스 개수만큼 호출*/
[maxvertexcount(6)]
void GS_MAIN(point GS_IN In[1], inout TriangleStream<GS_OUT> OutStream)
{
    
    GS_OUT Out[4];
    vector vLook = g_vCamPosition - In[0].vPosition;  //점에서 카메라를 행하는 방향(룩)벡터
    float3 vRight = normalize(cross(float3(0.f, 1.f, 0.f), vLook.xyz)) * In[0].vPSize.x * 0.5f; //정규화 후 저장해놓은 사이즈만큼 보정
    float3 vUp = normalize(cross(vLook.xyz, vRight)) * In[0].vPSize.y * 0.5f; //정규화 후 저장해놓은 사이즈만큼 보정
    
    
    matrix matVP = mul(g_ViewMatrix, g_ProjMatrix);
   
    
    Out[0].vPosition = float4(In[0].vPosition.xyz + vRight + vUp, 1.f);
    Out[0].vPosition = mul(Out[0].vPosition, matVP);
    Out[0].vTexcoord = float2(0.f, 0.f);
    Out[0].vLifeTime = In[0].vLifeTime;
    
    
    Out[1].vPosition = float4(In[0].vPosition.xyz - vRight + vUp, 1.f);
    Out[1].vPosition = mul(Out[1].vPosition, matVP);
    Out[1].vTexcoord = float2(1.f, 0.f);
    Out[1].vLifeTime = In[0].vLifeTime;

    Out[2].vPosition = float4(In[0].vPosition.xyz - vRight - vUp, 1.f);
    Out[2].vPosition = mul(Out[2].vPosition, matVP);
    Out[2].vTexcoord = float2(1.f, 1.f);
    Out[2].vLifeTime = In[0].vLifeTime;

    Out[3].vPosition = float4(In[0].vPosition.xyz + vRight - vUp, 1.f);
    Out[3].vPosition = mul(Out[3].vPosition, matVP);
    Out[3].vTexcoord = float2(0.f, 1.f);
    Out[3].vLifeTime = In[0].vLifeTime;
    
    /* 카메라를 바로보는 포인트를 기준으로 4방향의 정점을 구한다-> 이 4개의 정점을 인덱스 매핑해서 6개의 정점으로 삼각형을 만든다.*/
    OutStream.Append(Out[0]);
    OutStream.Append(Out[1]);
    OutStream.Append(Out[2]);
    OutStream.RestartStrip();//triangle stream 방식을 하지 않도록 삼각형 만들고 끊음으로서 triangle_list 와같은 형태로 삼각형 정보를 채운다 )*/

    OutStream.Append(Out[0]);
    OutStream.Append(Out[2]);
    OutStream.Append(Out[3]);
    OutStream.RestartStrip();
}

struct GS_IN_NON_BILLBOARD
{
    float4 vPosition : POSITION; //최종연산안끈남 ->SV 는 X
    row_major float4x4 TransformMatrix : WORLD;
    float2 vLifeTime : TEXCOORD0;    
};


[maxvertexcount(6)]
void GS_MAIN_NON_BILLBOARD(point GS_IN_NON_BILLBOARD In[1], inout TriangleStream<GS_OUT> OutStream)
{
    GS_OUT Out[4];
  
    float fRightSize = length(In[0].TransformMatrix._11_12_13);
    float fUpSize = length(In[0].TransformMatrix._21_22_23);
    vector vLook = In[0].TransformMatrix._31_32_33_34;
    float3 vRight = normalize(cross(float3(0.f, 1.f, 0.f), vLook.xyz)) * fRightSize ;
    float3 vUp =   normalize(cross(vLook.xyz, vRight)) * fUpSize ;
    matrix matVP = mul(g_ViewMatrix, g_ProjMatrix);
   
    Out[0].vPosition = float4(In[0].vPosition.xyz + vRight + vUp, 1.f);
    Out[0].vPosition = mul(Out[0].vPosition, matVP);
    Out[0].vTexcoord = float2(0.f, 0.f);
    Out[0].vLifeTime = In[0].vLifeTime;
    
    
    Out[1].vPosition = float4(In[0].vPosition.xyz - vRight + vUp, 1.f);
    Out[1].vPosition = mul(Out[1].vPosition, matVP);
    Out[1].vTexcoord = float2(1.f, 0.f);
    Out[1].vLifeTime = In[0].vLifeTime;

    Out[2].vPosition = float4(In[0].vPosition.xyz - vRight - vUp, 1.f);
    Out[2].vPosition = mul(Out[2].vPosition, matVP);
    Out[2].vTexcoord = float2(1.f, 1.f);
    Out[2].vLifeTime = In[0].vLifeTime;

    Out[3].vPosition = float4(In[0].vPosition.xyz + vRight - vUp, 1.f);
    Out[3].vPosition = mul(Out[3].vPosition, matVP);
    Out[3].vTexcoord = float2(0.f, 1.f);
    Out[3].vLifeTime = In[0].vLifeTime;
    
    /* 카메라를 바로보는 포인트를 기준으로 4방향의 정점을 구한다-> 이 4개의 정점을 인덱스 매핑해서 6개의 정점으로 삼각형을 만든다.*/
    OutStream.Append(Out[0]);
    OutStream.Append(Out[1]);
    OutStream.Append(Out[2]);
    OutStream.RestartStrip(); //triangle stream 방식을 하지 않도록 삼각형 만들고 끊음으로서 triangle_list 와같은 형태로 삼각형 정보를 채운다 )*/

    OutStream.Append(Out[0]);
    OutStream.Append(Out[2]);
    OutStream.Append(Out[3]);
    OutStream.RestartStrip();
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

    if (Out.vColor.a < 0.3f)
        discard;

    Out.vColor.gb = 0.f;

    Out.vColor.a = saturate(In.vLifeTime.x - In.vLifeTime.y) * 2.f;

    Out.vColor.r = 1.f - Out.vColor.a;

    if (In.vLifeTime.x <= In.vLifeTime.y)
        discard;

    return Out;
}


PS_OUT PS_MAIN_ONCE_NORMAL(PS_IN In)//PASS2
{
    PS_OUT Out = (PS_OUT) 0;

	/*Out.vColor = g_Texture.Sample(어떻게얻어올래?, In.vTexcoord);*/
    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);

    if (Out.vColor.a < 0.5f)
        discard;

   
    //점점 투명해진다
    Out.vColor.a = saturate(In.vLifeTime.x - In.vLifeTime.y) * 2.f;

    //라이프 타임 끝나면 죽인다
    if (In.vLifeTime.x <= In.vLifeTime.y)
        discard;

    return Out;
}



technique11 DefaultTechnique
{
	/* 우리가 수행해야할 정점, 픽셀 셰이더의 진입점 함수를 지정한다. */
    pass Loop//0
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN_LOOP();
    }

    pass Once_Boss09//1
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN_ONCE();
    }

    pass Once//2
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN_ONCE_NORMAL();
    }


    pass Once_NonBillboard//3
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_NON_BILLBOARD();
        GeometryShader = compile gs_5_0 GS_MAIN_NON_BILLBOARD();
        PixelShader = compile ps_5_0 PS_MAIN_ONCE_NORMAL();


    }




}
