#include "..\Public\Renderer.h"
#include "GameObject.h"
#include"UIObject.h"
#include"AlphaObject.h"
#include"GameInstance.h"
CRenderer::CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice { pDevice }
	, m_pContext { pContext }
	,m_pGameInstance{CGameInstance::GetInstance()}
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CRenderer::Initialize()
{
	/*여기서 렌더 타겟들을 위한 준비를 한다 - diffuse ormal shade*/
	_uint iNumViewports = { 1 };
	D3D11_VIEWPORT  ViewportDesc = {};
	m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);

	m_iOriginalViewportWidth = ViewportDesc.Width;
	m_iOriginalViewportHeight = ViewportDesc.Height;
	/* Target_Diffuse */
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Diffuse"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* Target_Normal */
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Normal"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;
		/* Target_Depth */
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Depth"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 1.f, 0.f, 1.f))))
			return E_FAIL;

	///* Target_Shade */
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Shade"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;
	/* Target_Specular */
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Specular"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	/*Target_lIGHTdEPTH _For shadow*/
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_LightDepth"), g_iMaxWidth, g_iMaxHeight, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

	/* Target_Final */
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Final"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* Target_Glow */

	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Glow_Original"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Glow_Intensity"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Glow_DownSampling"), ViewportDesc.Width/4, ViewportDesc.Height/4, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Glow_DownSampling_BlurX"), ViewportDesc.Width / 4, ViewportDesc.Height /4, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Glow_DownSampling_BlurY"), ViewportDesc.Width / 4, ViewportDesc.Height / 4, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Glow_UpSampling"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Glow_UpSampling_Deffered"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Blur_X"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Blur_Y"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Glow"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Glow_Deffered"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	
	/*TargetDistortion_Effect */
	if (FAILED(m_pGameInstance->Add_RenderTarget( TEXT("Target_Distortion"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Final_Distortion"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	// 렌더타겟생성하고 넣는다 개별렌더 타겟으로 넣는다 
	/* 장치에 동시에 바인딩될 타겟들을 모아놓는다. */
	/*생성하고 등록한 개별 렌더 타겟들을 -> MRT으로 묶는다. */
	/* MRT_GameObjects */
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Diffuse"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Normal"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Depth"))))
		return E_FAIL;
	
	/* MRT_LightAcc  -> 빛 마다 명암연산 텍스처를 저장하는 방법보다 BELEND 옵션을 켜서 더하는 방식으로 누적 명암 연산을 구하도록 한다 */
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Shade"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Specular"))))
		return E_FAIL;

	/* MRT_Shadow */
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Shadow"), TEXT("Target_LightDepth"))))
		return E_FAIL;

	/*MRT_Distortion Effect*/
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Distortion"), TEXT("Target_Distortion"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Final_Distortion"), TEXT("Target_Final_Distortion"))))
		return E_FAIL;

	/* MRT_Final */
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Final"), TEXT("Target_Final"))))
		return E_FAIL;


	/* MRT_Blur_X */
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Glow_Original"), TEXT("Target_Glow_Original"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Glow_Original"), TEXT("Target_Glow_Intensity"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Glow_DownSamping"), TEXT("Target_Glow_DownSampling"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Glow_DownSamping_BlurX"), TEXT("Target_Glow_DownSampling_BlurX"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Glow_DownSamping_BlurY"), TEXT("Target_Glow_DownSampling_BlurY"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Glow_UpSampling"), TEXT("Target_Glow_UpSampling"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Glow_UpSampling_Deffered"), TEXT("Target_Glow_UpSampling_Deffered"))))
		return E_FAIL;
	

	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Blur_X"), TEXT("Target_Blur_X"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Blur_Y"), TEXT("Target_Blur_Y"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Glow"), TEXT("Target_Glow"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Glow_Deffered"), TEXT("Target_Glow_Deffered"))))
		return E_FAIL;
	/*MRT_Effect_Glow*/

	
	

	



	/*직교 투영을 위한  셰이더와 VIBUFFER -> 렌더 타겟들을 디버그용으로 렌더*/
	m_pShader = CShader::Create(m_pDevice, m_pContext, TEXT("../../EngineSDK/Hlsl/Shader_Deferred.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements);
	if (nullptr == m_pShader)
		return E_FAIL;



	m_pVIBuffer = CVIBuffer_Rect::Create(m_pDevice, m_pContext);
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixScaling(ViewportDesc.Width, ViewportDesc.Height, 1.f));//명암연산용으로 VIBuffer을 하나 만들것-> 여기에 연산 기록 저장
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(ViewportDesc.Width, ViewportDesc.Height, 0.f, 1.f));
	//화면을꽉 채우는 VIBuffer-> 화면 해상도 크기만큼 스케일링후 거기에 명암 연산결과를 기록, 위치는 원래 로컬에서 0,0,이었으니까 translation은 필요 없다 

	/*우너하는 사이즈의 depth stencit view 준비*/
	if (FAILED(Ready_DepthStencilView(g_iMaxWidth, g_iMaxHeight,&m_pShadowDepthStencilView)))
		return E_FAIL;
	if (FAILED(Ready_DepthStencilView(ViewportDesc.Width / 4, ViewportDesc.Height / 4, &m_pSamplingDepthStencilView)))
		return E_FAIL;

#ifdef _DEBUG
	/*m_pGameInstance->Ready_RT_Debug(TEXT("Target_Diffuse"), 50.f, 250.f, 100.f, 100.f);
	m_pGameInstance->Ready_RT_Debug(TEXT("Target_Normal"), 50.f, 350.f, 100.f, 100.f);
	m_pGameInstance->Ready_RT_Debug(TEXT("Target_Shade"), 150.f, 250.f, 100.f, 100.f);
	m_pGameInstance->Ready_RT_Debug(TEXT("Target_Specular"), 150.f, 350.f, 100.f, 100.f);*/
	//m_pGameInstance->Ready_RT_Debug(TEXT("Target_Final"), ViewportDesc.Width - 100.f, 100.f, 100.f, 100.f);
	
	//m_pGameInstance->Ready_RT_Debug(TEXT("Target_Glow_Original"), 450.f, 150.f, 300.f, 300.f);
	//m_pGameInstance->Ready_RT_Debug(TEXT("Target_Glow_Intensity"), ViewportDesc.Width - 100.f, 300.f, 100.f, 100.f);

	
	//m_pGameInstance->Ready_RT_Debug(TEXT("Target_Blur_Y"), ViewportDesc.Width - 300.f, 100.f, 300.f, 300.f);
	//m_pGameInstance->Ready_RT_Debug(TEXT("Target_Glow_UpSampling_Deffered"),  300.f, 450.f, 300.f, 300.f);
	
	
	//m_pGameInstance->Ready_RT_Debug(TEXT("Target_Glow_UpSampling"),150.f, 150.f, 300.f, 300.f);
	
	
	//m_pGameInstance->Ready_RT_Debug(TEXT("Target_LightDepth"), ViewportDesc.Width - 150.f, 450.f, 300.f, 300.f);

#endif

	return S_OK;
}

HRESULT CRenderer::Add_RenderObject(RENDERGROUP eRenderGroup, CGameObject * pRenderObject)
{
	if (eRenderGroup >= RG_END || 
		nullptr == pRenderObject)
		return E_FAIL;

	m_RenderObjects[eRenderGroup].push_back(pRenderObject);

	Safe_AddRef(pRenderObject);

	return S_OK;
}

HRESULT CRenderer::Draw_RenderObject()
{
	if (FAILED(Render_Priority()))
		return E_FAIL;
	if (FAILED(Render_Shadow()))
		return E_FAIL;
	if (FAILED(Render_NonBlend()))
		return E_FAIL;
	
	if (FAILED(Render_Lights()))
		return E_FAIL;
	
	if (FAILED(Render_Final()))
		return E_FAIL;
	/*distortion*/
	if (FAILED(Render_Distortion()))
		return E_FAIL;
	if (FAILED(Render_Assemble_Distortion()))
		return E_FAIL;
	
	/*Distortion*/
	/*Glow*/
	if (FAILED(Render_Glow_Original()))
		return E_FAIL;
	
	if (FAILED(Render_Glow_DownSamping()))
		return E_FAIL;
	if (FAILED(Render_Glow_DownSampling_BlurX()))
		return E_FAIL;

	if (FAILED(Render_Glow_DownSampling_BlurY()))
		return E_FAIL;

	if (FAILED(Render_Glow_UpSampling()))
		return E_FAIL;
	
	if (FAILED(Render_Glow_Deffered_UpSampling()))
		return E_FAIL;

	/*if (FAILED(Render_Blur_X()))
		return E_FAIL;
	if (FAILED(Render_Blur_Y()))
		return E_FAIL;
	if (FAILED(Render_Glow()))
		return E_FAIL;*/
	
	if (FAILED(Render_Assemble()))
		return E_FAIL;

	/* 파티클이나 콜라이더는 빛연산을 먹으면 안되니까 빛연산 끝나고 그린다. */
	if (FAILED(Render_NonLight()))
		return E_FAIL;
	if (FAILED(Render_Blend()))
		return E_FAIL;
	if (FAILED(Render_UI()))
		return E_FAIL;

#ifdef _DEBUG
	/*if (FAILED(Render_Debug()))
		return E_FAIL;*/
#endif

	return S_OK;
}

HRESULT CRenderer::Render_Priority()
{
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Final"))))
		return E_FAIL;

	for (auto& pRenderObject : m_RenderObjects[RG_PRIORITY])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render();

		Safe_Release(pRenderObject);
	}
	m_RenderObjects[RG_PRIORITY].clear();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;


	return S_OK;
}

HRESULT CRenderer::Render_Shadow()
{
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Shadow"), m_pShadowDepthStencilView)))
		return E_FAIL;
	/*뷰포트 크기 재설정하여 원하는 크기만큼 렌더타겟에 그린다 - 후에 원상복구 해줘애함*/
	if (FAILED(SetUp_Viewport(g_iMaxWidth, g_iMaxHeight)))
		return E_FAIL;

	for (auto& pRenderObject : m_RenderObjects[RG_SHADOW])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render_Shadow();

		Safe_Release(pRenderObject);
	}
	m_RenderObjects[RG_SHADOW].clear();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	if (FAILED(SetUp_Viewport(m_iOriginalViewportWidth, m_iOriginalViewportHeight)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_NonBlend()
{
	/* 이 그룹에서 그려지는 객체들은 빛 연산이 필요하다. */
	/* 객체드르그릴때 마다 수행한다(x), 후처릴로 한번(해상도픽셀갯수만큼)에 수행한다. */
	/* 후처리로 ㅍ빛연산을 수행하기위해 어떤 데이터가 필요한가?! */
	/* 명암만 계산한다라과 가정한다면 Normal정보가 필요하다.(후처리로 명암연산이 가능) */
	/* 색상을 입혀서 그려주기위해 Diffuse정보도 필요했다. */
	/* Diffuse + vNormal */

	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_GameObjects"))))
		return E_FAIL;
	
	//-> 장치에 바인딩하고 지형과 오브젝트의  노말 , 디퓨즈 기록한다
	for (auto& pRenderObject : m_RenderObjects[RG_NONBLEND])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render();

		Safe_Release(pRenderObject);
	}	
	m_RenderObjects[RG_NONBLEND].clear();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_Lights()
{
	/* Shade */
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_LightAcc"))))
		return E_FAIL;

	m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix);
	m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix);
	m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix);
	
	m_pShader->Bind_Matrix("g_ViewMatrixInv", &m_pGameInstance->Get_TransformInversedFloat4x4(CPipeLine::D3DTS_VIEW));
	m_pShader->Bind_Matrix("g_ProjMatrixInv", &m_pGameInstance->Get_TransformInversedFloat4x4(CPipeLine::D3DTS_PROJ));

	m_pShader->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4));


	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pShader, "g_NormalTexture", TEXT("Target_Normal"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pShader, "g_DepthTexture", TEXT("Target_Depth"))))
		return E_FAIL;

	m_pVIBuffer->Bind_BufferDesc();

	if (FAILED(m_pGameInstance->Render_Lights(m_pShader, m_pVIBuffer)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_Final()
{

	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Final"), nullptr, false)))
		return E_FAIL;

	m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix);
	m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix);
	m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix);


	m_pShader->Bind_Matrix("g_ViewMatrixInv", &m_pGameInstance->Get_TransformInversedFloat4x4(CPipeLine::D3DTS_VIEW));
	m_pShader->Bind_Matrix("g_ProjMatrixInv", &m_pGameInstance->Get_TransformInversedFloat4x4(CPipeLine::D3DTS_PROJ));

	m_pShader->Bind_Matrix("g_LightViewMatrix", m_pGameInstance->Get_Shadow_Transform(CShadow::D3DTS_VIEW));
	m_pShader->Bind_Matrix("g_LightProjMatrix", m_pGameInstance->Get_Shadow_Transform(CShadow::D3DTS_PROJ));

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pShader, "g_DiffuseTexture", TEXT("Target_Diffuse"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pShader, "g_ShadeTexture", TEXT("Target_Shade"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pShader, "g_SpecularTexture", TEXT("Target_Specular"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pShader, "g_LightDepthTexture", TEXT("Target_LightDepth"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pShader, "g_DepthTexture", TEXT("Target_Depth"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pShader, "g_DistortionTexture", TEXT("Target_Distortion"))))
		return E_FAIL;

	m_pShader->Begin(3);

	m_pVIBuffer->Bind_BufferDesc();

	m_pVIBuffer->Render();


	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}
HRESULT CRenderer::Render_Distortion()
{
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Distortion"))))
		return E_FAIL;

	//-> 장치에 바인딩하고 지형과 오브젝트의  노말 , 디퓨즈 기록한다
	for (auto& pRenderObject : m_RenderObjects[RG_DISTORTION])
	{
		if (nullptr != pRenderObject)
		{
			pRenderObject->Set_Distortion(true);
			pRenderObject->Render();
			pRenderObject->Set_Distortion(false);
		}
		Safe_Release(pRenderObject);
	}
	m_RenderObjects[RG_DISTORTION].clear();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
	
}

HRESULT CRenderer::Render_Assemble_Distortion()
{
	//distortion옮기기 파이널과 합치기 
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Final_Distortion"))))
		return E_FAIL;

	m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix);
	m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix);
	m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix);
	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pShader, "g_DistortionTexture", TEXT("Target_Distortion"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pShader, "g_FinalTexture", TEXT("Target_Final"))))
		return E_FAIL;

	m_pShader->Begin(4);

	m_pVIBuffer->Bind_BufferDesc();

	m_pVIBuffer->Render();
	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}


HRESULT CRenderer::Render_Glow_Original()
{
	/*빛을 낼 텍스처를 일단 찍자.*/
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Glow_Original"))))
		return E_FAIL;

	for (auto& pRenderObject : m_RenderObjects[RG_GLOW])
	{
		if (nullptr != pRenderObject)
		{
			pRenderObject->Set_Glow(true);
			pRenderObject->Render();
			pRenderObject->Set_Glow(false);
		
		}
		Safe_Release(pRenderObject);
	}
	m_RenderObjects[RG_GLOW].clear();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_Glow_DownSamping()
{
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Glow_DownSamping"),m_pSamplingDepthStencilView)))
		return E_FAIL;
	
	if (FAILED(SetUp_Viewport(m_iOriginalViewportWidth/4, m_iOriginalViewportHeight/4)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pShader, "g_SamplingTexture", TEXT("Target_Glow_Original"))))
		return E_FAIL;
	
	_float2 TexelSize = { (_float)m_iOriginalViewportWidth ,(_float) m_iOriginalViewportHeight  };
 	if(FAILED(m_pShader->Bind_RawValue("g_TexelSizeToSampling", reinterpret_cast<void*>(&TexelSize), sizeof(_float2))))
		return E_FAIL;
	_uint Multiple = 4;
	if (FAILED(m_pShader->Bind_RawValue("g_MultiSize", reinterpret_cast<void*>(&Multiple), sizeof(_uint))))
		return E_FAIL;
	m_pShader->Begin(9);

	m_pVIBuffer->Bind_BufferDesc();

	m_pVIBuffer->Render();

	if (FAILED(SetUp_Viewport(m_iOriginalViewportWidth, m_iOriginalViewportHeight)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;
	return S_OK;

}

HRESULT CRenderer::Render_Glow_DownSampling_BlurX()
{
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Glow_DownSamping_BlurX"), m_pSamplingDepthStencilView)))
		return E_FAIL;
	
	if (FAILED(SetUp_Viewport(m_iOriginalViewportWidth / 4, m_iOriginalViewportHeight / 4)))
		return E_FAIL;

	_float2 TexelSize = { (_float)m_iOriginalViewportWidth/4 ,(_float)m_iOriginalViewportHeight/4 };
	if (FAILED(m_pShader->Bind_RawValue("g_TexelSizeToSampling", reinterpret_cast<void*>(&TexelSize), sizeof(_float2))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pShader, "g_SamplingBlurXTexture", TEXT("Target_Glow_DownSampling"))))
		return E_FAIL;
	
	m_pShader->Begin(10);

	m_pVIBuffer->Bind_BufferDesc();

	m_pVIBuffer->Render();

	if (FAILED(SetUp_Viewport(m_iOriginalViewportWidth, m_iOriginalViewportHeight)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;
	return S_OK;
}

HRESULT CRenderer::Render_Glow_DownSampling_BlurY()
{
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Glow_DownSamping_BlurY"), m_pSamplingDepthStencilView)))
		return E_FAIL;

	if (FAILED(SetUp_Viewport(m_iOriginalViewportWidth / 4, m_iOriginalViewportHeight / 4)))
		return E_FAIL;
	
	_float2 TexelSize = { (_float)m_iOriginalViewportWidth/4 ,(_float)m_iOriginalViewportHeight/4 };
	if (FAILED(m_pShader->Bind_RawValue("g_TexelSizeToSampling", reinterpret_cast<void*>(&TexelSize), sizeof(_float2))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pShader, "g_SamplingBlurYTexture", TEXT("Target_Glow_DownSampling_BlurX"))))
		return E_FAIL;


	m_pShader->Begin(11);

	m_pVIBuffer->Bind_BufferDesc();

	m_pVIBuffer->Render();

	if (FAILED(SetUp_Viewport(m_iOriginalViewportWidth, m_iOriginalViewportHeight)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;
	return S_OK;
}

HRESULT CRenderer::Render_Glow_UpSampling()
{
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Glow_UpSampling"))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pShader, "g_Glow_UpSampling", TEXT("Target_Glow_DownSampling_BlurY"))))
		return E_FAIL;
	_float2 TexelSize = { (_float)m_iOriginalViewportWidth  ,(_float)m_iOriginalViewportHeight };
	if (FAILED(m_pShader->Bind_RawValue("g_TexelSizeToSampling", reinterpret_cast<void*>(&TexelSize), sizeof(_float2))))
		return E_FAIL;
	m_pShader->Begin(12);

	m_pVIBuffer->Bind_BufferDesc();

	m_pVIBuffer->Render();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;
	return S_OK;
}

HRESULT CRenderer::Render_Glow_Deffered_UpSampling()
{
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Glow_UpSampling_Deffered"))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pShader, "g_Glow_UpSampling", TEXT("Target_Glow_UpSampling"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pShader, "g_Glow_Original_Texture", TEXT("Target_Glow_Original"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pShader, "g_Glow_Intensity_Texture", TEXT("Target_Glow_Intensity"))))
		return E_FAIL;

	m_pShader->Begin(13);

	m_pVIBuffer->Bind_BufferDesc();

	m_pVIBuffer->Render();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;
	return S_OK;

}


HRESULT CRenderer::Render_Blur_X()
{
	
	/*먼저 그리고 그려져있으면 렌더러의 4번 셰이더로 넘긴다 ( 채워진 부분만 블러먹인다.*/
	/*블러 처리*/
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Blur_Y"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pShader, "g_FinalTexture", TEXT("Target_Final_Distortion"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pShader, "g_Glow_Original_Texture", TEXT("Target_Glow_Original"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pShader, "g_Glow_Intensity_Texture", TEXT("Target_Glow_Intensity"))))
		return E_FAIL;


	m_pShader->Begin(5);

	m_pVIBuffer->Bind_BufferDesc();

	m_pVIBuffer->Render();
	
	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;
	

	return S_OK;
}

HRESULT CRenderer::Render_Blur_Y()
{
	/*tpgh 섞어서 glow 만들기 */
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Glow"))))
		return E_FAIL;
	m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix);
	m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix);
	m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix);

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pShader, "g_BlurYTexture", TEXT("Target_Blur_Y"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pShader, "g_FinalTexture", TEXT("Target_Final_Distortion"))))
		return E_FAIL;
	
	m_pShader->Begin(6);

	m_pVIBuffer->Bind_BufferDesc();

	m_pVIBuffer->Render();
	
	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;


	return S_OK;
}

HRESULT CRenderer::Render_Glow()
{
	/*original이랑 섞기*/

	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Glow_Deffered"))))
		return E_FAIL;
	m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix);
	m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix);
	m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix);

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pShader, "g_GlowTexture", TEXT("Target_Glow"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pShader, "g_Glow_Original_Texture", TEXT("Target_Glow_Original"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pShader, "g_Glow_Intensity_Texture", TEXT("Target_Glow_Intensity"))))
		return E_FAIL;




	m_pShader->Begin(7);

	m_pVIBuffer->Bind_BufferDesc();

	m_pVIBuffer->Render();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;
	return S_OK;
}

HRESULT CRenderer::Render_Assemble()
{
	m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix);
	m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix);
	m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix);

	/*glow*/
	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pShader, "g_GlowTexture", TEXT("Target_Glow_UpSampling_Deffered"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pShader, "g_FinalTexture", TEXT("Target_Final_Distortion"))))
		return E_FAIL;
	
	/*toon shading*/
	m_pShader->Bind_Matrix("g_ViewMatrixInv", &m_pGameInstance->Get_TransformInversedFloat4x4(CPipeLine::D3DTS_VIEW));
	m_pShader->Bind_Matrix("g_ProjMatrixInv", &m_pGameInstance->Get_TransformInversedFloat4x4(CPipeLine::D3DTS_PROJ));

	m_pShader->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4));

	/*Event UV*/
	if (nullptr != m_pEventTextureCom)
	{
		_bool m_bEventOn = { true };
		m_pShader->Bind_RawValue("g_EventOn", reinterpret_cast<void*>(&m_bEventOn), sizeof(_bool));
		m_pShader->Bind_RawValue("g_fAccEventTimeDelta",reinterpret_cast<void*>(&m_fAccEventTimeDelta), sizeof(_float));
		m_pEventTextureCom->Bind_ShaderResource(m_pShader, "g_EventTexture", 0);
		m_pEventTextureCom = nullptr;
		m_fAccEventTimeDelta = 0.f;
	}
	else
	{
		_bool m_bEventOn = { false };
		m_pShader->Bind_RawValue("g_EventOn", reinterpret_cast<void*>(&m_bEventOn), sizeof(_bool));
	}

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pShader, "g_NormalTexture", TEXT("Target_Normal"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pShader, "g_DepthTexture", TEXT("Target_Depth"))))
		return E_FAIL;

	m_pShader->Begin(8);
	 
	m_pVIBuffer->Bind_BufferDesc();

	m_pVIBuffer->Render();
	return S_OK;
}

HRESULT CRenderer::Render_NonLight()
{

	

	for (auto& pRenderObject : m_RenderObjects[RG_NONLIGHT])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render();

		Safe_Release(pRenderObject);
	}
	m_RenderObjects[RG_NONLIGHT].clear();

	return S_OK;
}

HRESULT CRenderer::Render_Blend()
{

	m_RenderObjects[RG_BLEND].sort([](CGameObject* pSour, CGameObject* pDest)->_bool
		{
			return static_cast<CAlphaObject*>(pSour)->Get_ViewZ() > static_cast<CAlphaObject*>(pDest)->Get_ViewZ();
		});

	for (auto& pRenderObject : m_RenderObjects[RG_BLEND])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render();
		
		Safe_Release(pRenderObject);
	}
	m_RenderObjects[RG_BLEND].clear();

	return S_OK; 
}

HRESULT CRenderer::Render_UI()
{

 	m_RenderObjects[RG_UI].sort([](CGameObject* pUIObject, CGameObject* pUIObject2) {
		return static_cast<CUIObject*>(pUIObject)->Get_SortDepth() < static_cast<CUIObject*>(pUIObject2)->Get_SortDepth();
		});

	for (auto& pRenderObject : m_RenderObjects[RG_UI])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render();

		Safe_Release(pRenderObject);
	}
	m_RenderObjects[RG_UI].clear();

	return S_OK;
}
HRESULT CRenderer::Ready_DepthStencilView(_uint iWidth, _uint iHeight, ID3D11DepthStencilView** DepthStencilView )
{
	if (nullptr == m_pDevice)
		return E_FAIL;

	ID3D11Texture2D* pDepthStencilTexture = nullptr;

	D3D11_TEXTURE2D_DESC	TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	/* 깊이 버퍼의 픽셀으 ㄴ백버퍼의 픽셀과 갯수가 동일해야만 깊이 텍스트가 가능해진다. */
	/* 픽셀의 수가 다르면 아에 렌덜잉을 못함. */
	TextureDesc.Width = iWidth;
	TextureDesc.Height = iHeight;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;

	/* 동적? 정적?  */
	TextureDesc.Usage = D3D11_USAGE_DEFAULT /* 정적 */;
	/* 추후에 어떤 용도로 바인딩 될 수 있는 View타입의 텍스쳐를 만들기위한 Texture2D입니까? */
	TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL
		/*| D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE*/;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &pDepthStencilTexture)))
		return E_FAIL;

	if (FAILED(m_pDevice->CreateDepthStencilView(pDepthStencilTexture, nullptr, DepthStencilView)))
		return E_FAIL;

	Safe_Release(pDepthStencilTexture);

	return S_OK;
}
HRESULT CRenderer::SetUp_Viewport(_uint iWidth, _uint iHeight)
{
	D3D11_VIEWPORT			ViewPortDesc;
	ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));
	ViewPortDesc.TopLeftX = 0;
	ViewPortDesc.TopLeftY = 0;
	ViewPortDesc.Width = (_float)iWidth;
	ViewPortDesc.Height = (_float)iHeight;
	ViewPortDesc.MinDepth = 0.f;
	ViewPortDesc.MaxDepth = 1.f;

	m_pContext->RSSetViewports(1, &ViewPortDesc);

	return S_OK;
}
#ifdef _DEBUG
HRESULT CRenderer::Render_Debug()
{


	for (auto& pDebugCom : m_DebugComponents)
	{
		if (nullptr != pDebugCom)
			pDebugCom->Render();
		Safe_Release(pDebugCom);
	}
	m_DebugComponents.clear();
	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->Bind_BufferDesc()))
		return E_FAIL;

	/*if (FAILED(m_pGameInstance->Render_RT_Debug(TEXT("MRT_GameObjects"), m_pShader, m_pVIBuffer)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Render_RT_Debug(TEXT("MRT_LightAcc"), m_pShader, m_pVIBuffer)))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Render_RT_Debug(TEXT("MRT_Shadow"), m_pShader, m_pVIBuffer)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Render_RT_Debug(TEXT("MRT_Final"), m_pShader, m_pVIBuffer)))
		return E_FAIL;*/
	/*if (FAILED(m_pGameInstance->Render_RT_Debug(TEXT("MRT_Blur_X"), m_pShader, m_pVIBuffer)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Render_RT_Debug(TEXT("MRT_Blur_Y"), m_pShader, m_pVIBuffer)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Render_RT_Debug(TEXT("MRT_Glow"), m_pShader, m_pVIBuffer)))
		return E_FAIL;*/
	/*if (FAILED(m_pGameInstance->Render_RT_Debug(TEXT("MRT_Glow_DownSamping"), m_pShader, m_pVIBuffer)))
		return E_FAIL;*/
	if (FAILED(m_pGameInstance->Render_RT_Debug(TEXT("MRT_Glow_UpSampling"), m_pShader, m_pVIBuffer)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Render_RT_Debug(TEXT("MRT_Glow_UpSampling_Deffered"), m_pShader, m_pVIBuffer)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Render_RT_Debug(TEXT("MRT_Glow_Original"), m_pShader, m_pVIBuffer)))
		return E_FAIL;
	/*if (FAILED(m_pGameInstance->Render_RT_Debug(TEXT("MRT_Glow_Deffered"), m_pShader, m_pVIBuffer)))
		return E_FAIL;*/
	
	
	

	return S_OK;
}
#endif

CRenderer * CRenderer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CRenderer*		pInstance = new CRenderer(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CRenderer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRenderer::Free()
{
	__super::Free();

	Safe_Release(m_pShader);
	Safe_Release(m_pVIBuffer);

	for (size_t i = 0; i < RG_END; i++)
	{
		for (auto& pRenderObject : m_RenderObjects[i])
			Safe_Release(pRenderObject);	

		m_RenderObjects[i].clear();
	}
	for (auto& pDebugCom : m_DebugComponents)
	{
		if (nullptr != pDebugCom)
			pDebugCom->Render();
		Safe_Release(pDebugCom);
	}
	m_DebugComponents.clear();

	Safe_Release(m_pShadowDepthStencilView);
	Safe_Release(m_pSamplingDepthStencilView);
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}

