#include"stdafx.h"
#include"Hands03.h"
#include"GameInstance.h"
#include"Player.h"
#include"UI_Arms.h"
#include"Particle_Player03_Hit.h"

CHands03::CHands03(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CHands(pDevice, pContext)
{
}

CHands03::CHands03(const CHands03& Prototype)
	:CHands(Prototype)
{
}

const _float4x4* CHands03::Get_BoneMatrix(const _char* pBoneName) const
{
	return m_pModelCom->Get_BoneMatrix(pBoneName);
}

HRESULT CHands03::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CHands03::Initialize(void* pArg)
{
	HANDS03_DESC* pDesc = static_cast<HANDS03_DESC*>(pArg);
	m_pState = pDesc->pState;

	m_HandsInfo= g_HandsInfo[2];
	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;


	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_isInfinity = true;
	m_pModelCom->SetUp_Animation(1, true);

	m_vecStates.resize((_uint)HANDS03_STATE::STATE_END);
	if (FAILED(Init_Mapping_Index_To_State()))
		return E_FAIL;

	if (FAILED(Init_AnimSpeed()))
		return E_FAIL;

	for (int i = 0; i < g_iNumParticle_Player03_Hit; ++i)
		m_fAccParticleTime[i] = 3.5f;

	m_pModelCom->Set_AnimSpeed(m_vecStates[(_uint)HANDS03_STATE::STATE_ATTACK_PRESSING], 2.5f);

	if (FAILED(Ready_Particle()))
		return E_FAIL;

		m_iNumBullets_ICanFireNow = m_iNumBullets_YouHave = m_HandsInfo.fMaxLoadBullets;
	return S_OK;

}

HRESULT CHands03::Init_Mapping_Index_To_State()
{
	m_vecStates[(_uint)HANDS03_STATE::STATE_IDLE] = 1;
	m_vecStates[(_uint)HANDS03_STATE::STATE_ATTACK_PRESSING] = 0;
	m_vecStates[(_uint)HANDS03_STATE::STATE_LOAD] = 2;
	m_vecStates[(_uint)HANDS03_STATE::STATE_POP_OUT] = 4;

	return S_OK;
}

HRESULT CHands03::Init_AnimSpeed()
{
	m_pModelCom->Set_AnimSpeed(m_vecStates[(_uint)HANDS03_STATE::STATE_ATTACK_PRESSING], 1.5f);
	return S_OK;
}

void CHands03::Priority_Update(_float fTimeDelta)
{
	
	for (int i = 0; i < g_iNumParticle_Player03_Hit; ++i)
	{
		if (m_fAccParticleTime[i] < 1.f) {
			m_fAccParticleTime[i] += fTimeDelta;
			m_pParticle_Hit[i]->Priority_Update(fTimeDelta);
		}
	}

	if (false == m_bActive)
		return;
	/* 컨테이너 오브젝트에서 키입력에따른 상태 변화를 이미 적용한 상태*/
}

void CHands03::Update(_float fTimeDelta)
{
	if (false == m_bActive)
		return;

	m_isFired = false;
	/*For UI 지금 총알 수 UI에 보여주기*/
	m_pArms->Set_BulletsICanFire(m_iNumBullets_ICanFireNow);
	m_pArms->Set_BulletsIHaveNow(m_iNumBullets_YouHave);
	m_pArms->Set_IsInfinityBullets(m_isInfinity);

	/*POP OUT끝난 후 IDLE 상태*/
	if (true == m_bStartSetUp && m_isAnimEnd)
	{
		*m_pState |= CPlayer::STATE_IDLE;
		*m_pState ^= CPlayer::STATE_POP_OUT;
		m_bStartSetUp = false;

		/*For UI*/
		m_pArms->Set_BulletsICanFire(m_iNumBullets_ICanFireNow);
		m_pArms->Set_BulletsIHaveNow(m_iNumBullets_YouHave);
	}
	/*장전 끝났을때 ->POPOUT과 장전 해제*/
	if (CONDITION_LOAD && m_isAnimEnd)
	{
		*m_pState |= CPlayer::STATE_IDLE;
		if (CONDITION_POP_OUT)
			*m_pState ^= CPlayer::STATE_POP_OUT;
		*m_pState ^= CPlayer::STATE_LOAD;
	}
	/*공격 _ MOUSE_UP 공격 그만둠*/
	if (CONDITION_ATTACK_UP)
	{
		if (true != m_isAnimEnd)
		{
		}
		/*애니메이션이끝났다.-> 공격 멈춘다*/
		else
		{

			if (CONDITION_ATTACK_UP)
				*m_pState ^= CPlayer::STATE_ATTACK_UP;
			if (CONDITION_ATTACK_DOWN)
				*m_pState ^= CPlayer::STATE_ATTACK_DOWN;
			if (CONDITION_ATTACK_PRESSING)
				*m_pState ^= CPlayer::STATE_ATTACK_PRESSING;

			if (!CONDITION_IDLE)
				*m_pState |= CPlayer::STATE_IDLE;
		}
	}
	/* To Pop_Up*/
	if (CONDITION_POP_OUT && false == m_bStartSetUp)
	{
		m_pGameInstance->PlayEffect(L"Player_Load.ogg", 0.2f);
		m_pModelCom->SetUp_Animation(m_vecStates[(_uint)HANDS03_STATE::STATE_POP_OUT], false);
		m_bStartSetUp = true;	

	}

	/*To Attack1 */
	else if (CONDITION_ATTACK)
	{
		m_fAccAttackDownTime += fTimeDelta;
		//루프 돌거나 장전중 캔슬 (다른 루프가 아닌 애니메이션이끝났을 때
		if (CONDITION_LOOP || CONDITION_LOAD || m_isAnimEnd)
		{

			if (m_isFired =__super::Fire_A_Bullet())
			{

				m_pArms->Set_BulletsICanFire(m_iNumBullets_ICanFireNow);
			}

			m_pModelCom->Move_NextAnim(m_vecStates[(_uint)HANDS03_STATE::STATE_ATTACK_PRESSING], 0.2, false);
			m_pGameInstance->PlayEffect(L"Player03_Attack.ogg", 0.2f);

			if (CONDITION_IDLE)
				*m_pState ^= CPlayer::STATE_IDLE;
			if (CONDITION_LOAD)
				*m_pState ^= CPlayer::STATE_LOAD;
		}
	}
	/* 장전일때, 장전 하고 나서 -> 총알 재충전 */
	else if (CONDITION_LOAD)
	{
		if (CONDITION_LOOP || m_isAnimEnd)
		{

			if (__super::Load_Bullets())
			{
				m_pArms->Set_BulletsICanFire(m_HandsInfo.fMaxLoadBullets);
				m_pArms->Set_BulletsIHaveNow(m_iNumBullets_YouHave = m_iNumBullets_ICanFireNow = m_HandsInfo.fMaxLoadBullets);
			}
			m_pGameInstance->PlayEffect(L"Player_Load.ogg", 0.2f);
			m_pModelCom->SetUp_Animation(m_vecStates[(_uint)HANDS03_STATE::STATE_LOAD], false);
			if (CONDITION_IDLE)
			{
				*m_pState ^= CPlayer::STATE_IDLE;
			}

		}

	}
	/* to Idle*/
	else if (CONDITION_IDLE && m_isAnimEnd)
	{
		m_pModelCom->Move_NextAnim(m_vecStates[(_uint)HANDS03_STATE::STATE_IDLE], 0.202F, true);
	}

	/*for debugging*/
	if (CONDITION_LOOP)
		int a = 0;
	if (CONDITION_LOAD)
		int a = 0;

	if (CONDITION_ATTACK)
	{

		if (CONDITION_ATTACK_PRESSING)
			int b = 0;
	}
	m_isAnimEnd = m_pModelCom->Play_Animation(fTimeDelta, m_isChangingMotion);
	DirectX::XMStoreFloat4x4(&m_WorldMatrix, m_pTransformCom->Get_WorldMatrix() * XMLoadFloat4x4(m_pParentMatrix));


	///*츙돌 로직 -> 레이발사*/
	if (m_isFired) {
		m_pGameInstance->Add_Collision_Group(CCollider_Manager::COLLISION_GROUP::COLLISION_PLAYER_ATTACK, this, m_pColliderCom);
		pair<_vector, _vector> pairRay = Get_PickingPosInWorld();
		_float3 RaySrc, RayDir;
		XMStoreFloat3(&RaySrc, pairRay.first);
		XMStoreFloat3(&RayDir, pairRay.second);
		m_pColliderCom->Update(make_pair(RaySrc, RayDir));
	}
	/*Particle Update*/
	for (int j = 0; j < g_iNumParticle_Player03_Hit; ++j)
	{
		if (m_fAccParticleTime[j] < 1.f)
			m_pParticle_Hit[j]->Update(fTimeDelta);
	}

}

void CHands03::Late_Update(_float fTimeDelta)
{
	if (false == m_bActive)
		return;

	if (m_isFired)
	{
		CGameObject* pTarget = { nullptr };
		if (m_pGameInstance->Check_Collision(m_pColliderCom, CCollider_Manager::COLLISION_GROUP::COLLISION_MONSTER, &pTarget))
		{
			_vector vParticlePos =m_pColliderCom->Compute_RayPos();
			m_pParticle_Hit[m_iNowParticleIndex]->Reset_Vertices(fTimeDelta);
			m_pParticle_Hit[m_iNowParticleIndex]->Set_Position(vParticlePos);
			m_fAccParticleTime[m_iNowParticleIndex] = 0.f;
			m_iNowParticleIndex = (m_iNowParticleIndex+1)% g_iNumParticle_Player03_Hit;
		
		}
	}
	/*Particle Late Update*/
	for (int j = 0 ; j < g_iNumParticle_Player03_Hit ; ++j)
	{
		if (m_fAccParticleTime[j] < 1.f)
			m_pParticle_Hit[j]->Late_Update(fTimeDelta);
	}

	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
	m_pGameInstance->Add_RenderObject(CRenderer::RG_GLOW, this);
}

HRESULT CHands03::Render()
{
	if (false == m_bActive)
		return S_OK;

	if (true == m_bGlow)
	{
		if (FAILED(Render_Glow()))
			return E_FAIL;
	}

	else {

		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

		for (size_t i = 0; i < iNumMeshes; i++)
		{
			if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE, 0)))
				return E_FAIL;
			if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i)))
				return E_FAIL;

			m_pShaderCom->Begin(0);

			m_pModelCom->Render(i);
		}
	}


	return S_OK;
}

HRESULT CHands03::Render_Glow()
{
	if (FAILED(Bind_ShaderResources_Glow()))
		return E_FAIL;

	if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom_Glow, "g_DiffuseTexture", 0, aiTextureType_DIFFUSE, 0)))
		return E_FAIL;
	if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom_Glow, "g_BoneMatrices", 0)))
		return E_FAIL;

	m_pShaderCom_Glow->Begin(2);

	m_pModelCom->Render(0);
	return S_OK;
}

HRESULT CHands03::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Model  for Anim*/
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Player03"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;


	
	/*Com_Collider_Ray*/
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Ray"),
		TEXT("Com_Collider5"), reinterpret_cast<CComponent**>(&m_pColliderCom),nullptr)))
		return E_FAIL;
	/*ForGlow*/
	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimMesh_Player01_Glow"),
		TEXT("Com_Shader_Glow"), reinterpret_cast<CComponent**>(&m_pShaderCom_Glow))))
		return E_FAIL;
	/*ComTexture*/
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player01_Glow"),
		TEXT("Com_Texture_Glow"), reinterpret_cast<CComponent**>(&m_pTextureCom_Glow))))
		return E_FAIL;

	return S_OK;
}

HRESULT CHands03::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	const LIGHT_DESC* pLightDesc = m_pGameInstance->Get_LightDesc(0);

	return S_OK;
}

HRESULT CHands03::Bind_ShaderResources_Glow()
{
	if (FAILED(m_pShaderCom_Glow->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom_Glow->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom_Glow->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	_float3 vThreshold = { 0.f,0.5f,0.5f };
	if (FAILED(m_pShaderCom_Glow->Bind_RawValue("g_vThreshold", &vThreshold, sizeof(_float3))))
		return E_FAIL;
	_float vIntensity = 0.2f;
	if (FAILED(m_pShaderCom_Glow->Bind_RawValue("g_fIntensity", &vIntensity, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pTextureCom_Glow->Bind_ShaderResource(m_pShaderCom_Glow, "g_EmissiveTexture", 0)))
		return E_FAIL;


	return S_OK;
}

HRESULT CHands03::Ready_Particle()
{
	for(int i=0;i<g_iNumParticle_Player03_Hit;++i)
		m_pParticle_Hit[i] = static_cast<CParticle_Player03_Hit*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_Particle_Player03_Hit"), nullptr));

	return S_OK;
}

CHands03* CHands03::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CHands03* pInstance = new CHands03(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed Create CHands03");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CHands03::Clone(void* pArg)
{
	CGameObject* pInstance = new CHands03(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed CHands03 :Clone");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CHands03::Free()
{
	for (int i = 0; i < g_iNumParticle_Player03_Hit; ++i)
	{
		Safe_Release(m_pParticle_Hit[i]);
		m_pParticle_Hit[i] = nullptr;
	}
	Safe_Release(m_pColliderCom);
	g_HandsInfo[2] =m_HandsInfo ;
	/*ForGlow*/
	Safe_Release(m_pShaderCom_Glow);
	Safe_Release(m_pTextureCom_Glow);

	__super::Free();

}
