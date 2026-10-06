

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
float4x4		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
TextureCube		g_Texture;

/* D3D11_SAMPLER_DESC */
sampler			LinearSampler = sampler_state 
{	
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = wrap;
	AddressV = wrap;
    AddressW = wrap;
};

/* VertexShader */

/* 정점쉐이더의 구동 시점. */
/* m_pContext->DrawIndexed() */
/* 그릴때 사용하고자했던 정점들중, 인덱스버퍼(0, 1, 2, 0, 2, 3)가 지정해주고 있는 정점을 골라서 */
/* VS_MAIN함수를 호출하며 인자로 전달해준다. */

struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vTexcoord : TEXCOORD0;	
 
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float3		vTexcoord : TEXCOORD0;
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



/* 정점의 위치벡터를 위치벡터의 w로 나눈다. */
/* 뷰포트 영역으로 정점들을 변환한다. */
/* 래스터라이즈(정점 정보를 보간하여 픽셀의 정보가 생성된다.) */
/* 이렇게 픽셀을 PS_MAIN() 호출하면서 전달해준다. */

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float3		vTexcoord : TEXCOORD0;
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
	Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);

	//Out.vColor.gb = 1.f;	

	return Out;
}


technique11			DefaultTechnique
{
	/* 우리가 수행해야할 정점, 픽셀 셰이더의 진입점 함수를 지정한다. */
	pass DefaultPass
	{
		VertexShader = compile vs_5_0 VS_MAIN();
		PixelShader = compile ps_5_0 PS_MAIN();
	}


}
