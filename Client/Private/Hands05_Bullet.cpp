#include"stdafx.h""
#include"Hands05_Bullet.h"
#include"Particle_Player05_Bullet_Chasing.h"
#include"Particle_Player05_Bomb.h"
#include"GameInstance.h"
#include"UI_MiniMapMain.h"
#include"Monster.h"
#include"Hands.h"
#include"Player.h"
CHands05_Bullet::CHands05_Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject{pDevice, pContext}
{
}

CHands05_Bullet::CHands05_Bullet(const CHands05_Bullet& Prototype)
	:CGameObject(Prototype)
	
{
}

HRESULT CHands05_Bullet::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CHands05_Bullet::Initialize(void* pArg)
{
	HANDS05_BULLET_DESC* pDesc = static_cast<HANDS05_BULLET_DESC*>(pArg);
	
	if (FAILED(__super::Initialize(pDesc)))
	{
		return E_FAIL;
	}
	_matrix matWorld = XMMatrixIdentity();
	

	if (FAILED(Ready_Components()))
		return E_FAIL;

	// start Animation Setting
	m_pModelCom->SetUp_Animation(2 , true);


	if (FAILED(Ready_Particle()))
		return E_FAIL;
	return S_OK;
}

void CHands05_Bullet::Priority_Update(_float fTimeDelta)
{
	
	m_pParticle_Bomb->Priority_Update(fTimeDelta);

	if (!m_isActive)
		return;

	m_pParticle_Chasing->Priority_Update(fTimeDelta);
	
	m_fAccTimeDelta += fTimeDelta;
	if ( m_fAccNowLifeTime < m_fLifeTimeLimit)
	{
		m_fAccNowLifeTime += fTimeDelta;
	}
	else
	{
		m_isActive = false;
	}



	if (m_bColMoving)
	{

		_matrix matRot = XMMatrixRotationAxis(XMLoadFloat4(&m_vInitUp), 0.2f * sin(m_fAccTimeDelta * 5.f));

		_vector vecLook = XMVector3TransformNormal(XMLoadFloat4(&m_vInitLook), matRot);

		_vector vecRight = XMVector3TransformNormal(XMLoadFloat4(&m_vInitRight), matRot);

		vecLook = XMVector3Normalize(vecLook) * 1.5f;
		vecRight = XMVector3Normalize(vecRight) * 1.5f;
		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, 1.5f * vecRight);
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, 1.5f * vecLook);
		m_pTransformCom->Go_Straight(fTimeDelta * 2.f);
	}

	else
	{
		_matrix matRot = XMMatrixRotationAxis(XMLoadFloat4(&m_vInitRight), 0.2f * sin(m_fAccTimeDelta * 5.f));

		_vector vecUp = XMVector3TransformNormal(XMLoadFloat4(&m_vInitUp), matRot);

		_vector vecLook = XMVector3TransformNormal(XMLoadFloat4(&m_vInitLook), matRot);

		vecLook = XMVector3Normalize(vecLook) * 1.5f;
		vecUp = XMVector3Normalize(vecUp) * 1.5f;
		m_pTransformCom->Set_State(CTransform::STATE_UP, 1.5f * vecUp);
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, 1.5f * vecLook);
		m_pTransformCom->Go_Straight(fTimeDelta * 2.f);
	}
		m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());
		m_pParticle_Chasing->Set_Matrix(m_pTransformCom->Get_WorldMatrix());


}

void CHands05_Bullet::Update(_float fTimeDelta)
{
	m_pParticle_Bomb->Update(fTimeDelta);



	if (!m_isActive)
		return;

	{

		CHands* pHands05 = static_cast<CHands*>(m_pGameInstance->Get_GameObject(m_pGameInstance->Get_Current_Level(), TEXT("Layer_APlayer"), CPlayer::PART_HANDS05, 0));
		m_pGameInstance->Add_Collision_Group(CCollider_Manager::COLLISION_GROUP::COLLISION_PLAYER_ATTACK, pHands05, m_pColliderCom);
		m_isAnimEnd = m_pModelCom->Play_Animation(fTimeDelta, m_isChangingMotion);
	}
	m_pParticle_Chasing->Update(fTimeDelta);
	
	
}

void CHands05_Bullet::Late_Update(_float fTimeDelta)
{
	m_pParticle_Bomb->Late_Update(fTimeDelta);

	if (!m_isActive)
		return;

	m_pParticle_Chasing->Late_Update(fTimeDelta);

	{
		CGameObject* pTarget = nullptr;
		if (m_pGameInstance->Check_Collision(m_pColliderCom, CCollider_Manager::COLLISION_GROUP::COLLISION_MONSTER, &pTarget))
		{
			m_pParticle_Bomb->Reset_Vertices(fTimeDelta);
			m_pParticle_Bomb->Set_Matrix(m_pTransformCom->Get_WorldMatrix());
			m_isActive = false;
		}
		m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
		m_pGameInstance->Add_RenderObject(CRenderer::RG_GLOW, this);
	}	

#ifdef _DEBUG
	if (m_isActive)
		m_pGameInstance->Add_DebugComponent(m_pColliderCom);
#endif
}

HRESULT CHands05_Bullet::Render()
{
	if (false == m_isActive)
		return S_OK;
	
	if (true == m_bGlow)
	{
		if(FAILED(Bind_ShaderResources_Glow()))
			return E_FAIL;
		_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

		for (size_t i = 0; i < iNumMeshes; i++)
		{
			if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE, 0)))
				return E_FAIL;
			if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i)))
				return E_FAIL;

			m_pShaderCom->Begin(5);

			m_pModelCom->Render(i);
		}

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

void CHands05_Bullet::Set_Active(_bool isActive)
{
	m_isActive = isActive;  

	
	

}

void CHands05_Bullet::Reset_Bullets(_fmatrix matInitialShootingPos)
{
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, 1.5f * XMVector3Normalize(matInitialShootingPos.r[0]));
	m_pTransformCom->Set_State(CTransform::STATE_UP, 1.5f * XMVector3Normalize(matInitialShootingPos.r[1]));
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, 1.5f * XMVector3Normalize(matInitialShootingPos.r[2]));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, matInitialShootingPos.r[3]);
	m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());

	XMStoreFloat4(&m_vInitUp, m_pTransformCom->Get_State(CTransform::STATE_UP));
	XMStoreFloat4(&m_vInitRight, m_pTransformCom->Get_State(CTransform::STATE_RIGHT));
	XMStoreFloat4(&m_vInitLook, m_pTransformCom->Get_State(CTransform::STATE_LOOK));


	_float fChance = m_pGameInstance->Compute_Random(0.f, 10.f);
	if (5.f <= fChance)
	{
		m_bColMoving = true;
		m_bRowMoving = false;
	}
	else
	{
		m_bRowMoving = true;
		m_bColMoving = false;
	}

	m_fAccNowLifeTime = 0.f;
}

HRESULT CHands05_Bullet::Ready_Components()
{
	CBounding_Sphere::BOUND_SPHERE_DESC ColliderDesc{};
	ColliderDesc.vCenter = _float3(0.f, 0.f, 0.f);
	ColliderDesc.fRadius = 0.5f;

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Model *///(0번 애니메이션만 사용하자)
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Player05_Bullet"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))


	return S_OK;
}

HRESULT CHands05_Bullet::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;



	return S_OK;
}

HRESULT CHands05_Bullet::Bind_ShaderResources_Glow()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	_float fIntensity = 0.1f;
	if(FAILED(m_pShaderCom->Bind_RawValue("g_fIntensity",reinterpret_cast<void*>(&fIntensity),sizeof(fIntensity))))
		return E_FAIL;
	return S_OK;
}

HRESULT CHands05_Bullet::Ready_Particle()
{
	m_pParticle_Chasing = static_cast<CParticle_Player05_Bullet_Chasing*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_Particle_Player05_Bullet_Chasing"), nullptr));
	/*CParticle_Player05_Bomb*/
	m_pParticle_Bomb =static_cast<CParticle_Player05_Bomb*> (m_pGameInstance ->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_Particle_Player05_Bomb"), nullptr));
	return S_OK;
}

CHands05_Bullet* CHands05_Bullet::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CHands05_Bullet* pInstance = new CHands05_Bullet(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to create: CHands05_Bullet");
		return nullptr;
	}

	return pInstance;
}

CGameObject* CHands05_Bullet::Clone(void* pArg)
{
	CHands05_Bullet* pInstance = new CHands05_Bullet(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone: CHands05_Bullet");
		return nullptr;
	}

	return pInstance;
}

void CHands05_Bullet::Free()
{
	
	Safe_Release(m_pParticle_Chasing);
	Safe_Release(m_pParticle_Bomb);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pColliderCom);

	__super::Free();
	
}
