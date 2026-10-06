#include"stdafx.h"
#include"Monster09_Wave.h"
#include"GameInstance.h"

CMonster09_Wave::CMonster09_Wave(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject{pDevice,pContext}
{
}

CMonster09_Wave::CMonster09_Wave(const CMonster09_Wave& Prototype)
	:CGameObject(Prototype)
{
}

HRESULT CMonster09_Wave::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonster09_Wave::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CMonster09_Wave::Priority_Update(_float fTimeDelta)
{
	if (m_bNoiseEvent_FinalTarget)
	{
		m_fAccEventNoiseTime += fTimeDelta;
	}
	if (m_fAccEventNoiseTime >= m_fEventNoiseTime)
	{
		m_bNoiseEvent_FinalTarget = false;
	}

	{
		if (true == m_bNoiseEvent_FinalTarget && true == m_bStartEventNoise_Sound)
		{
			m_pGameInstance->PlayEffect(L"Monster09_Bubble.mp3", 0.5f);
			m_iEventNoiseSoundID = m_pGameInstance->Get_NowEffectID();
			m_bStartEventNoise_Sound = false;
		}

		if (false == m_bNoiseEvent_FinalTarget)
		{
			if (0 <= m_iEventNoiseSoundID)
				m_pGameInstance->StopSound(m_iEventNoiseSoundID);
			m_iEventNoiseSoundID = -1;
			m_bStartEventNoise_Sound = true;
		}
	}

	if (!m_isActive)
		return;

	if (m_isActive && m_fAccNowLifeTime < m_fLifeTimeLimit)
	{
		m_fAccNowLifeTime += fTimeDelta;
	}
	else
	{
		m_isActive = false;
		if(m_iStopWaveSoundID>=0)
		m_pGameInstance->StopSound(m_iStopWaveSoundID);
		
		m_iStopWaveSoundID = -1;
	}

	
}

void CMonster09_Wave::Update(_float fTimeDelta)
{

	if (!m_isActive)
		return;

	_uint iCount = { 0 };
	if (WAVE_DIRECTION::WAVE_LEFT == m_eDirection)
	{
		m_pTransformCom->Go_Right(1.5f*fTimeDelta * m_fSpeed);
	}
	else if (WAVE_DIRECTION::WAVE_RIGHT == m_eDirection)
	{
		m_pTransformCom->Go_Left(1.5f*fTimeDelta * m_fSpeed);
	}

	_vector vPos = m_pTransformCom ->Get_State(CTransform::STATE_POSITION);
	_vector vLook = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK));
	_float fOffset = 3.f;
	for (auto& Collider : m_pColliderComs)
	{
		_vector vNowColliderPos = vPos + vLook * (iCount* fOffset);
		++iCount;
		_matrix matNowColliderTrans = m_pTransformCom->Get_WorldMatrix();
		matNowColliderTrans.r[3] = vNowColliderPos;
		Collider->Update(matNowColliderTrans);
		m_pGameInstance->Add_Collision_Group(CCollider_Manager::COLLISION_MONSTER_ATTACK, this,Collider);

	}
	
}

void CMonster09_Wave::Late_Update(_float fTimeDelta)
{

	if (true == m_bNoiseEvent_FinalTarget)
	{
		//·»´õ·¯¿¡ ftimedelta ¶û ³ëÀÌÁî ÅØ½ºÃ³ ´øÁö±â 
		m_pGameInstance->Set_Event_Properties(m_pTextureCom_Event_Noise, m_fAccEventNoiseTime);
	}

	if (!m_isActive)
		return;
	m_pGameInstance->Add_RenderObject(CRenderer::RG_DISTORTION, this);
	m_pGameInstance->Add_RenderObject(CRenderer::RG_GLOW, this);
	for (auto& pCollider : m_pColliderComs)
	{
		CGameObject* pTarget = { nullptr };
		if (m_pGameInstance->Check_Collision(pCollider, CCollider_Manager::COLLISION_PLAYER, &pTarget))
		{
			m_bNoiseEvent_FinalTarget = true;
			m_fAccEventNoiseTime = 0.f;
			
		}
	}
	

#ifdef _DEBUG

	for(auto& pCollider: m_pColliderComs)
		if(pCollider)
			m_pGameInstance->Add_DebugComponent(pCollider);
#endif
}

HRESULT CMonster09_Wave::Render()
{
	if (!m_isActive)
		return S_OK;
	if (true == m_bGlow)
	{

		if (FAILED(Bind_ShaderResources_Glow()))
			return E_FAIL;
		m_pShaderCom->Begin(2);

		m_pVIBufferCom->Bind_BufferDesc();


		m_pVIBufferCom->Render();

	}
	else {
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		m_pShaderCom->Begin(1);

		m_pVIBufferCom->Bind_BufferDesc();


		m_pVIBufferCom->Render();
	}
	
	return S_OK;

	
}

void CMonster09_Wave::Reset_Bullets(_fmatrix matInitialShootingPos)
{
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, XMVector3Normalize(matInitialShootingPos.r[0]));
	m_pTransformCom->Set_State(CTransform::STATE_UP, XMVector3Normalize(matInitialShootingPos.r[1]));
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, XMVector3Normalize(matInitialShootingPos.r[2]));
	m_pTransformCom->Scaling(4.6f, 4.6f, 4.6f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, matInitialShootingPos.r[3]);
	m_pGameInstance->PlayEffect(L"Monster09_Wave.mp3", 1.f);
	m_iStopWaveSoundID = m_pGameInstance->Get_NowEffectID();
	m_fAccNowLifeTime = 0.f;
}

HRESULT CMonster09_Wave::Ready_Components()
{
	m_pColliderComs.resize(7);
	TCHAR szBaseColliderTag[MAX_PATH] = L"Com_Collider%d";
	
	CBounding_Sphere::BOUND_SPHERE_DESC  ColliderDesc = {};

	ColliderDesc.vCenter = _float3(0.f, 0.f, 0.f);
	ColliderDesc.fRadius = 0.5f;
	_uint iCount = 0;

	for (int i = 0; i<m_pColliderComs.size();++i)
	{
		TCHAR szColliderTag[MAX_PATH] =L"";
		wsprintf(szColliderTag, szBaseColliderTag, iCount++);


		if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
			szColliderTag, reinterpret_cast<CComponent**>(&m_pColliderComs[i]), &ColliderDesc)))
			return E_FAIL;
	}

	CVIBuffer_Flat::VIBUFFER_FLAT_DESC Flat_Desc = {};

	m_vNumTerrainFlat_Vertices.x = Flat_Desc.iNumX_Vertices = 10;
	m_vNumTerrainFlat_Vertices.y = Flat_Desc.iNumZ_Vertices = 10;
	Flat_Desc.fSizeX = 1.f;
	Flat_Desc.fSizeZ = 1.f;
	
	/* Com_VIBuffer */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Flat"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), &Flat_Desc)))
		return E_FAIL;
	/* Com_Texture */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_Texture_Water"),
		TEXT("Com_Texture1"), reinterpret_cast<CComponent**>(&m_pTextureCom_Water))))
		return E_FAIL;
	if (FAILED(Add_Component(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_Texture_Monster09_Wave_Noise"),
		TEXT("Com_Texture2"), reinterpret_cast<CComponent**>(&m_pTextureCom_Noise))))
		return E_FAIL;
	if (FAILED(Add_Component(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_Texture_Monster09_Wave_Diffuse"),
		TEXT("Com_Texture3"), reinterpret_cast<CComponent**>(&m_pTextureCom_Diffuse))))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_Texture_Monster09_Wave_Glow_Noise"),
		TEXT("Com_Texture4"), reinterpret_cast<CComponent**>(&m_pTextureCom_Glow_Noise))))
		return E_FAIL;
	
	if (FAILED(Add_Component(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_Texture_Monster09_WaveEvent_Noise"),
		TEXT("Com_Texture5"), reinterpret_cast<CComponent**>(&m_pTextureCom_Event_Noise))))
		return E_FAIL;
	

	
	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_Shader_VtxWater"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;


	

	return S_OK;
}

HRESULT CMonster09_Wave::Bind_ShaderResources()
{
	/*±¼Àý¿ë ¼ÎÀÌ´õ */
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_matrix matWaves = XMMatrixIdentity();
	matWaves.r[0] *= 0.6f;
	matWaves.r[2] *= 4.f;
	matWaves.r[3] = vPos +XMVectorSet(-3.2f,0.09f,-25.f,0.f);
	_float4x4 waveFloat4x4;
	XMStoreFloat4x4(&waveFloat4x4, matWaves);

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pShaderCom, "g_ScreenTexture", TEXT("Target_Final"))))
		return E_FAIL;


	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &waveFloat4x4)))
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

	_float fRefractiveIndex = 2.f;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Refractive_index", &fRefractiveIndex, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_NumWaves", &m_fNumWaves, sizeof(_float))))
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

	
	return S_OK;
}

HRESULT CMonster09_Wave::Bind_ShaderResources_Glow()
{
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_matrix matWaves = XMMatrixIdentity();
	matWaves.r[0] *= 0.6f;
	matWaves.r[2] *= 4.f;
	matWaves.r[3] = vPos + XMVectorSet(-3.2f, 0.09f, -25.f, 0.f);
	_float4x4 waveFloat4x4;
	XMStoreFloat4x4(&waveFloat4x4, matWaves);

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pShaderCom, "g_ScreenTexture", TEXT("Target_Final"))))
		return E_FAIL;


	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &waveFloat4x4)))
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

	_float fRefractiveIndex = 2.f;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Refractive_index", &fRefractiveIndex, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_NumWaves", &m_fNumWaves, sizeof(_float))))
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
	
	_float3 vThreshold = { 0.f,0.f,0.f };
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fThreshold", &vThreshold, sizeof(_float3))))
		return E_FAIL;
	_float vIntensity = 1.f;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fIntensity", &vIntensity, sizeof(_float))))
		return E_FAIL;
	return S_OK;
}

CMonster09_Wave* CMonster09_Wave::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMonster09_Wave* pInstance = new CMonster09_Wave(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to create: CMonster09_Wave");
		return nullptr;
	}

	return pInstance;
}

CGameObject* CMonster09_Wave::Clone(void* pArg)
{
	CMonster09_Wave* pInstance = new CMonster09_Wave(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone: CMonster09_Wave");
		return nullptr;
	}

	return pInstance;
}

void CMonster09_Wave::Free()
{
	for (auto& pCollider : m_pColliderComs)
		Safe_Release(pCollider);
	m_pColliderComs.clear();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom_Noise);
	Safe_Release(m_pTextureCom_Water);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom_Diffuse);
	Safe_Release(m_pTextureCom_Glow_Noise);
	Safe_Release(m_pTextureCom_Event_Noise);
	__super::Free();
}
