#include"stdafx.h"
#include"Hands04.h"
#include"Particle_Player04_Rasor.h"
#include"Hands04_Rasor.h"
#include"GameInstance.h"
#include"Player.h"
#include"UI_Arms.h"

CHands04::CHands04(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CHands{pDevice, pContext}
{
}

CHands04::CHands04(const CHands04& Prototype)
	:CHands(Prototype)
{
}

const _float4x4* CHands04::Get_BoneMatrix(const _char* pBoneName) const
{
	return m_pModelCom->Get_BoneMatrix(pBoneName);
}

HRESULT CHands04::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CHands04::Initialize(void* pArg)
{

	HANDS04_DESC* pDesc = static_cast<HANDS04_DESC*> (pArg);

	m_pState = pDesc->pState;

	m_HandsInfo =g_HandsInfo[3];
	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;
	m_isInfinity = true;
	m_pModelCom->SetUp_Animation(1, true);
	m_pMuzzle_Socket_Matrix = Get_BoneMatrix("FireLight");
	m_vecStates.resize((_uint)HANDS04_STATE::STATE_END);
	
	if (FAILED(Init_Mapping_Index_To_State()))
		return E_FAIL;
	if (FAILED(Init_AnimSpeed()))
		return E_FAIL;
	
	if (FAILED(Ready_Particle()))
		return E_FAIL;

	if (FAILED(Ready_Rasor()))
		return E_FAIL;
	m_iNumBullets_ICanFireNow = m_iNumBullets_YouHave = m_HandsInfo.fMaxLoadBullets;
	return S_OK;
}

HRESULT CHands04::Init_Mapping_Index_To_State()
{

	m_vecStates[(_uint)HANDS04_STATE::STATE_IDLE] = 0;
	m_vecStates[(_uint)HANDS04_STATE::STATE_ATTACK_PRESSING] = 4;
	m_vecStates[(_uint)HANDS04_STATE::STATE_LOAD] = 2;
	m_vecStates[(_uint)HANDS04_STATE::STATE_POP_OUT] = 1;
	return S_OK;
}

HRESULT CHands04::Init_AnimSpeed()
{
	m_pModelCom->Set_AnimSpeed(m_vecStates[(_uint)HANDS04_STATE::STATE_ATTACK_PRESSING], 1.5f);
	return S_OK;
}

void CHands04::Priority_Update(_float fTimeDelta)
{
	if (false == m_bActive)
		return;

	
	
}

void CHands04::Update(_float fTimeDelta)
{
	

	


	if (false == m_bActive)
		return;

	if (m_fAccAttackSoundTime >= 3.f)
		m_fAccAttackSoundTime = 0.f;
	if (CONDITION_ATTACK && 0.f == m_fAccAttackSoundTime) {
		m_pGameInstance->PlayEffect(L"Player04_Rasor.ogg", 0.5f);
		m_iAttackID = m_pGameInstance->Get_NowEffectID();
		if (m_iAttackID > 10)
			int a = 0;
	}
	if (CONDITION_ATTACK)
		m_fAccAttackSoundTime += fTimeDelta;

	m_isFired = false;
	/*For UI 지금 총알 수 UI에 보여주기*/
	m_pArms->Set_BulletsICanFire(m_iNumBullets_ICanFireNow);
	m_pArms->Set_BulletsIHaveNow(m_iNumBullets_YouHave);
	m_pArms->Set_IsInfinityBullets(m_isInfinity);

	{
		if (!CONDITION_IDLE && false == m_bCall_Idle)
		{
			m_bCall_Idle = true;
		}
	}


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
	if (CONDITION_ATTACK_UP )
	{
		m_fAccAttackSoundTime = 0.f;
		m_pGameInstance->StopSound(m_iAttackID);
		
		/*애니메이션이끝났다.-> 공격 멈춘다*/
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
		m_pModelCom->SetUp_Animation(m_vecStates[(_uint)HANDS04_STATE::STATE_POP_OUT], false);
		m_bStartSetUp = true;
		m_pGameInstance->PlayEffect(L"Player04_Load.ogg", 0.3f);
	}

	/*To Attack1 */
	else if (CONDITION_ATTACK)
	{
		m_fAccAttackDownTime += fTimeDelta;
		//루프 돌거나 장전중 캔슬 (다른 루프가 아닌 애니메이션이끝났을 때
		
		m_isFired = true;
			/*if (m_isFired = __super::Fire_A_Bullet())
			{

				m_pArms->Set_BulletsICanFire(m_iNumBullets_ICanFireNow);
			}*/
		if(false ==m_bShootSign)
			m_pModelCom->Move_NextAnim(m_vecStates[(_uint)HANDS04_STATE::STATE_ATTACK_PRESSING], 0.2, true);


			if (CONDITION_IDLE)
				*m_pState ^= CPlayer::STATE_IDLE;
			if (CONDITION_LOAD)
				*m_pState ^= CPlayer::STATE_LOAD;
		
	}
	/* 장전일때, 장전 하고 나서 -> 총알 재충전 */
	else if (CONDITION_LOAD)
	{
		if (CONDITION_LOOP || m_isAnimEnd)
		{
			
			if ( __super::Load_Bullets())
			{
				m_pArms->Set_BulletsICanFire(m_HandsInfo.fMaxLoadBullets);
				m_pArms->Set_BulletsIHaveNow(m_iNumBullets_YouHave = m_iNumBullets_ICanFireNow = m_HandsInfo.fMaxLoadBullets);
			}
			m_pGameInstance->PlayEffect(L"Player04_Load.ogg", 0.3f);
			m_pModelCom->SetUp_Animation(m_vecStates[(_uint)HANDS04_STATE::STATE_LOAD], false);
			if (CONDITION_IDLE)
			{
				*m_pState ^= CPlayer::STATE_IDLE;
			}

		}

	}
	/* to Idle*/
	else if (CONDITION_IDLE&& true==m_bCall_Idle)
	{
		m_bCall_Idle = false;
		m_pModelCom->Move_NextAnim(m_vecStates[(_uint)HANDS04_STATE::STATE_IDLE], 0.202F, true);
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
}
void CHands04::Late_Update(_float fTimeDelta)
{
	if (false == m_bActive)
		return;
	_bool isCollided = false;
	if (m_isFired)
	{
		
		CGameObject* pTarget = { nullptr };
		if (isCollided=m_pGameInstance->Check_Collision(m_pColliderCom, CCollider_Manager::COLLISION_GROUP::COLLISION_MONSTER, &pTarget))
		{

			m_bShootSign = true;
			_vector vPickingPos = m_pColliderCom->Compute_RayPos();
			//m_pParticle_Ray->Reset_Vertices(fTimeDelta);
			//m_pParticle_Ray->Set_Position(vPickingPos);
			
			 
			_matrix InitBulletMatrix;
			if (m_pMuzzle_Socket_Matrix)
			{
				_matrix SocketMatrix = XMLoadFloat4x4(m_pMuzzle_Socket_Matrix);
				for (size_t i = 0; i < 3; ++i)
				{
					SocketMatrix.r[i] = XMVector3Normalize(SocketMatrix.r[i]);
				}
				InitBulletMatrix = SocketMatrix * XMLoadFloat4x4(&m_WorldMatrix);
			}
			
			_vector vMuzzlePos = InitBulletMatrix.r[3];
			m_fRayDist = XMVectorGetX(XMVector3Length(vPickingPos - vMuzzlePos));
			_vector vLook = XMVector3Normalize(vPickingPos - vMuzzlePos);
			_vector vRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook));
			_vector vUp = XMVector3Cross(vLook, vRight);
			//InitBulletMatrix.r[0] = vRight*0.1f;
			//InitBulletMatrix.r[1] = vUp*m_fRayDist;
			//InitBulletMatrix.r[2] = vLook;
			//
			//InitBulletMatrix *= XMMatrixRotationAxis(vRight, XMConvertToRadians(90.f));
			//InitBulletMatrix.r[3] = vMuzzlePos; //+ vLook * (m_fRayDist * 0.5f);

			//m_pParticle_Ray->Set_ParticleMatrix(InitBulletMatrix);
			if (m_bShootSign)
			{
				m_pRasor->SetPos(vMuzzlePos, vMuzzlePos + vLook* m_fRayDist);
				//m_pParticle_Ray->Update(fTimeDelta);
				//m_pParticle_Ray->Set_RayLength(m_fRayDist);
			}
			else
			{

			}
				//m_pParticle_Ray->Set_RayLength(0.f);

			
			
		}
		
		

	}
	if (false == isCollided)
	{
		m_bShootSign = false;
		m_pRasor->Set_Active(false);
		//m_pParticle_Ray->Set_Matrix(XMMatrixIdentity());
	}
	if (m_bShootSign)
	{
		m_pRasor->Set_Active(true);
		m_pRasor->Late_Update(fTimeDelta);
		//m_pParticle_Ray->Late_Update(fTimeDelta);
	}
	


	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
	m_pGameInstance->Add_RenderObject(CRenderer::RG_GLOW, this);

}

HRESULT CHands04::Render()
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

HRESULT CHands04::Render_Glow()
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

HRESULT CHands04::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Model  for Anim*/
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Player04"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/*Com_Collider_Ray*/
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Ray"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), nullptr)))
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

HRESULT CHands04::Bind_ShaderResources()
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

HRESULT CHands04::Bind_ShaderResources_Glow()
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
	_float vIntensity = 1.f;
	if (FAILED(m_pShaderCom_Glow->Bind_RawValue("g_fIntensity", &vIntensity, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pTextureCom_Glow->Bind_ShaderResource(m_pShaderCom_Glow, "g_EmissiveTexture", 0)))
		return E_FAIL;


	return S_OK;
}

HRESULT CHands04::Ready_Particle()
{
	//m_pParticle_Ray =static_cast<CParticle_Player04_Rasor*> (m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_Hands04_Particle_Player04_Rasor"), nullptr));
	return S_OK;
}

HRESULT CHands04::Ready_Rasor()
{
	CHands04_Rasor::HANDS04_RASOR_DESC desc = {};
	desc.fRotationPerSec = XMConvertToRadians(90.f);
	desc.fSpeedPerSec=10.f;

	m_pRasor = static_cast<CHands04_Rasor*> (m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_Hands04_Rasor"), &desc));
	return S_OK;
}


CHands04* CHands04::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CHands04* pInstance = new CHands04(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed Create CHands04");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CHands04::Clone(void* pArg)
{
	CGameObject* pInstance = new CHands04(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed CHands04 :Clone");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CHands04::Free()
{
	__super::Free();
	Safe_Release(m_pParticle_Ray);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pShaderCom);

	Safe_Release(m_pRasor);
	/*ForGlow*/
	Safe_Release(m_pShaderCom_Glow);
	Safe_Release(m_pTextureCom_Glow);
	g_HandsInfo[3] = m_HandsInfo;
}
