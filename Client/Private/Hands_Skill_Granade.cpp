#include"stdafx.h"
#include"Hands_Skill_Granade.h"
#include"Hands_Skill_Granade_Bomb.h"
#include"GameInstance.h"
CHands_Skill_Granade::CHands_Skill_Granade(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject{ pDevice, pContext }
{
}

CHands_Skill_Granade::CHands_Skill_Granade(const CHands_Skill_Granade& Prototype)
	:CGameObject(Prototype)
{
}

HRESULT CHands_Skill_Granade::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CHands_Skill_Granade::Initialize(void* pArg)
{


	HANDS_SKILL_GRANADE_DESC* pDesc = static_cast<HANDS_SKILL_GRANADE_DESC*> (pArg);
	m_iNowMapStage = pDesc->iNowMapStage;

	if (FAILED(__super::Initialize(pArg)))
	{
		return E_FAIL;
	}

	if (FAILED(Ready_Components()))
		return E_FAIL;

	// start Animation Setting
	m_pModelCom->SetUp_Animation(1, false);


	if (FAILED(Ready_Particle()))
		return E_FAIL;
	if (FAILED(Ready_Bullet()))
		return E_FAIL;
	return S_OK;
}

void CHands_Skill_Granade::Priority_Update(_float fTimeDelta)
{
	m_pBomb->Priority_Update(fTimeDelta);

	if (!m_isActive)
		return;

 	m_fCurHeight += m_v0 * fTimeDelta;
	m_v0 -= m_fGravity * fTimeDelta;


	_vector vPos = m_pNavigationCom->SetUp_HeightOnNavigation(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	_float fBaseHeight = XMVectorGetY(vPos);
	
 
	if (fBaseHeight >= m_fCurHeight)
	{
 
		m_isActive = false;
		m_fCurHeight = 0.F;
		m_pBomb->Set_Active(true);
		m_pBomb->Reset_Bullets(m_pTransformCom->Get_WorldMatrix());

	}
	

}

void CHands_Skill_Granade::Update(_float fTimeDelta)
{
	m_pBomb->Update(fTimeDelta);

	if (!m_isActive)
		return;

	m_pTransformCom->Go_Straight(fTimeDelta,false,m_pNavigationCom,true,m_fCurHeight);
	m_pGameInstance->Add_Collision_Group(CCollider_Manager::COLLISION_GROUP::COLLISION_PLAYER_ATTACK, this, m_pColliderCom);
	m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());

	m_isAnimEnd = m_pModelCom->Play_Animation(fTimeDelta, m_isChangingMotion);
} 

void CHands_Skill_Granade::Late_Update(_float fTimeDelta)
{
	m_pBomb->Late_Update(fTimeDelta);
	
	if (!m_isActive)
		return;

	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);



#ifdef _DEBUG
      if(m_isActive)
		m_pGameInstance->Add_DebugComponent(m_pColliderCom);
#endif
}

HRESULT CHands_Skill_Granade::Render()
{
	

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
	return S_OK;
}

void CHands_Skill_Granade::Set_Active(_bool isActive)
{
	m_isActive = isActive;
}

void CHands_Skill_Granade::Reset_Bullets(_fmatrix matInitialShootingPos)
{
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, 1.f * XMVector3Normalize(matInitialShootingPos.r[0]));
	m_pTransformCom->Set_State(CTransform::STATE_UP, 1.f * XMVector3Normalize(matInitialShootingPos.r[1]));
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, 1.f * XMVector3Normalize(matInitialShootingPos.r[2]));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, matInitialShootingPos.r[3]);
	

	CNavigation* pPlayerNavi = static_cast<CNavigation*>(m_pGameInstance->Get_Component(m_pGameInstance->Get_Current_Level(), TEXT("Layer_APlayer"), TEXT("Com_Navigation"),0));
	m_pModelCom->SetUp_Animation(1, true);
	m_pNavigationCom->Set_Index(pPlayerNavi->Get_Index());
	
	m_fGravity = 9.8f;
	m_fCurHeight = XMVectorGetY(matInitialShootingPos.r[3]);
	m_v0 = 1.f;


}

void CHands_Skill_Granade::Set_BombRange(_float fBombRange)
{
	m_pBomb->Set_BombRange(fBombRange);
}

HRESULT CHands_Skill_Granade::Ready_Components()
{
	/*Com_Navigagtion*/
	CNavigation::NAVIGATION_DESC		NaviDesc{};

	NaviDesc.iStartIndex = 0;
	if (1 == m_iNowMapStage) {
		if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Navigation"),
			TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), &NaviDesc)))
			return E_FAIL;
	}
	else if (2 == m_iNowMapStage)
	{
		if (FAILED(Add_Component(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_Navigation"),
			TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), &NaviDesc)))
			return E_FAIL;
	}

	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Model *///(0번 애니메이션만 사용하자)
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Player_Skill_Granade"),
		TEXT("Com_Model_"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	CBounding_Sphere::BOUND_SPHERE_DESC ColliderDesc{};
	ColliderDesc.vCenter = _float3(0.f, 0.f, 0.f);
	ColliderDesc.fRadius = 0.5f;
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CHands_Skill_Granade::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	const LIGHT_DESC* pLightDesc = m_pGameInstance->Get_LightDesc(0);
	return S_OK;
}

HRESULT CHands_Skill_Granade::Ready_Particle()
{
	return S_OK;
}

HRESULT CHands_Skill_Granade::Ready_Bullet()
{
	CHands_Skill_Granade_Bomb::HANDS_SKILL_GRANADE_BOMB_DESC desc = {};
	desc.fRotationPerSec = XMConvertToRadians(90.f);
	desc.fSpeedPerSec =10.f;

	m_pBomb = static_cast<CHands_Skill_Granade_Bomb*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_Hands_Skill_Granade_Bomb"), &desc));



	return S_OK;
}

CHands_Skill_Granade* CHands_Skill_Granade::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CHands_Skill_Granade* pInstance = new CHands_Skill_Granade(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to create: CHands_Skill_Granade");
		return nullptr;
	}

	return pInstance;
}

CGameObject* CHands_Skill_Granade::Clone(void* pArg)
{
	CHands_Skill_Granade* pInstance = new CHands_Skill_Granade(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone: CHands_Skill_Granade");
		return nullptr;
	}

	return pInstance;
}

void CHands_Skill_Granade::Free()
{
	__super::Free();
	Safe_Release(m_pBomb);
	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pColliderCom);
}
