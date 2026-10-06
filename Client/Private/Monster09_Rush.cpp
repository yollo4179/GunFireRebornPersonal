#include"stdafx.h"
#include"Monster09_Rush.h"
#include"GameInstance.h"

CMonster09_Rush::CMonster09_Rush(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject{pDevice,pContext}
{
}

CMonster09_Rush::CMonster09_Rush(const CMonster09_Rush& Prototype)
	:CGameObject(Prototype)
{
}

HRESULT CMonster09_Rush::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonster09_Rush::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CMonster09_Rush::Priority_Update(_float fTimeDelta)
{
	if (m_fAccNowLifeTime < m_fLifeTimeLimit)
	{
		m_fAccNowLifeTime += fTimeDelta;
	}
	else
	{
		m_fAccNowLifeTime = 0.f;
		
	}

	if (!m_isActive)
		return;

	
}

void CMonster09_Rush::Update(_float fTimeDelta)
{

	if (!m_isActive)
		return;

	
}

void CMonster09_Rush::Late_Update(_float fTimeDelta)
{
	if (!m_isActive)
		return;
	m_pGameInstance->Add_RenderObject(CRenderer::RG_DISTORTION, this);
	m_pGameInstance->Add_RenderObject(CRenderer::RG_GLOW, this);
}

HRESULT CMonster09_Rush::Render()
{
	if (!m_isActive)
		return S_OK;
	if (true == m_bGlow)
	{

		if (FAILED(Bind_ShaderResources_Glow()))
			return E_FAIL;
		m_pShaderCom->Begin(1);

		m_pVIBufferCom->Bind_BufferDesc();


		m_pVIBufferCom->Render();

	}
	else {
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		m_pShaderCom->Begin(0);

		m_pVIBufferCom->Bind_BufferDesc();


		m_pVIBufferCom->Render();
	}

	return S_OK;
}

void CMonster09_Rush::Set_Active(_bool isActive)
{
	 m_isActive = isActive;
	 if (false ==isActive && 0 <= m_iSoundID)
	 {
		 m_pGameInstance->StopSound(m_iSoundID);
		 m_iSoundID = -1;
	 }

}

void CMonster09_Rush::Reset_Bullets(_fmatrix matInitialShootingPos)
{
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT,XMVector3Normalize(matInitialShootingPos.r[0]));
	m_pTransformCom->Set_State(CTransform::STATE_UP, XMVector3Normalize(matInitialShootingPos.r[1]));
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, XMVector3Normalize(matInitialShootingPos.r[2]));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, matInitialShootingPos.r[3]);
	m_pTransformCom->Scaling(1.5f, 1.f, 2.f);

	m_pGameInstance->PlayEffect(L"Monster09_Wave.mp3", 0.8f);
	m_iSoundID = m_pGameInstance->Get_NowEffectID();

	m_fAccNowLifeTime = 0.f;
}

HRESULT CMonster09_Rush::Ready_Components()
{
	CVIBuffer_Flat::VIBUFFER_FLAT_DESC Flat_Desc = {};

	m_vNumTerrainFlat_Vertices.x = Flat_Desc.iNumX_Vertices = 10;
	m_vNumTerrainFlat_Vertices.y = Flat_Desc.iNumZ_Vertices = 40;
	Flat_Desc.fSizeX = 1.f;
	Flat_Desc.fSizeZ = 1.f;
	/* Com_VIBuffer */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Flat"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), &Flat_Desc)))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_Texture_Water"),
		TEXT("Com_Texture1"), reinterpret_cast<CComponent**>(&m_pTextureCom_Water))))
		return E_FAIL;
	if (FAILED(Add_Component(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_Texture_Monster09_Wave_Noise"),
		TEXT("Com_Texture2"), reinterpret_cast<CComponent**>(&m_pTextureCom_Noise))))
		return E_FAIL;
	if (FAILED(Add_Component(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_Texture_Monster09_Wave_Diffuse"),
		TEXT("Com_Texture3"), reinterpret_cast<CComponent**>(&m_pTextureCom_Diffuse))))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_Shader_VtxWater_Monster09_Rush"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;
	return S_OK;
}

HRESULT CMonster09_Rush::Bind_ShaderResources()
{
	/*±¼Àý¿ë ¼ÎÀÌ´õ */
	

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pShaderCom, "g_ScreenTexture", TEXT("Target_Final"))))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;




	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Time", &m_fAccNowLifeTime, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom_Noise->Bind_ShaderResource(m_pShaderCom, "g_NoiseTexture", 0)))
		return E_FAIL;

	if (FAILED(m_pTextureCom_Noise->Bind_ShaderResource(m_pShaderCom, "g_WaveTexture", 0)))
		return E_FAIL;

	_float fRefractiveIndex = 0.6f;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Refractive_index", &fRefractiveIndex, sizeof(_float))))
		return E_FAIL;

	
	_float2 vWinSize = { (_float)g_iWinSizeX,(_float)g_iWinSizeY };
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vWinSize", &vWinSize, sizeof(_float2))))
		return E_FAIL;

	if (FAILED(m_pTextureCom_Diffuse->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;


	return S_OK;
}

HRESULT CMonster09_Rush::Bind_ShaderResources_Glow()
{
	/*±¼Àý¿ë ¼ÎÀÌ´õ */
	

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pShaderCom, "g_ScreenTexture", TEXT("Target_Final"))))
		return E_FAIL;


	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom,"g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;




	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Time", &m_fAccNowLifeTime, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom_Noise->Bind_ShaderResource(m_pShaderCom, "g_NoiseTexture", 0)))
		return E_FAIL;

	if (FAILED(m_pTextureCom_Noise->Bind_ShaderResource(m_pShaderCom, "g_WaveTexture", 0)))
		return E_FAIL;

	_float fRefractiveIndex = 0.6f;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Refractive_index", &fRefractiveIndex, sizeof(_float))))
		return E_FAIL;

	
	_float2 vWinSize = { (_float)g_iWinSizeX,(_float)g_iWinSizeY };
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vWinSize", &vWinSize, sizeof(_float2))))
		return E_FAIL;
	_vector vCenter = m_pTransformCom->Get_WorldMatrix().r[3];
	_float4 CenterFloat4;
	XMStoreFloat4(&CenterFloat4, vCenter);
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCenter", &CenterFloat4, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pTextureCom_Diffuse->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;

	/*Ps_Shader*/

	_float3 vThreshold = { 0.1f,0.1f,0.1f };
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fThreshold", &vThreshold, sizeof(_float3))))
		return E_FAIL;
	_float vIntensity = 0.5f;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fIntensity", &vIntensity, sizeof(_float))))
		return E_FAIL;
	return S_OK;
	
}

CMonster09_Rush* CMonster09_Rush::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMonster09_Rush* pInstance = new CMonster09_Rush(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to create: CMonster09_Rush");
		return nullptr;
	}

	return pInstance;
}

CGameObject* CMonster09_Rush::Clone(void* pArg)
{
	CMonster09_Rush* pInstance = new CMonster09_Rush(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone: CMonster09_Rush");
		return nullptr;
	}

	return pInstance;
}

void CMonster09_Rush::Free()
{
	__super::Free();
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom_Noise);
	Safe_Release(m_pTextureCom_Water);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom_Diffuse);
	Safe_Release(m_pTextureCom_Glow_Noise);
}
