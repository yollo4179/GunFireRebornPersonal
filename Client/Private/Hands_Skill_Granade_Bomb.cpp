#include"stdafx.h"
#include"Hands_Skill_Granade_Bomb.h"
#include"GameInstance.h"
#include"Particle_Player_Skill_Poison.h"
#include"Hands.h"
#include"Player.h"
CHands_Skill_Granade_Bomb::CHands_Skill_Granade_Bomb(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject{pDevice, pContext}
{
}

CHands_Skill_Granade_Bomb::CHands_Skill_Granade_Bomb(const CHands_Skill_Granade_Bomb& Prototype)
	:CGameObject(Prototype)
{
}

void CHands_Skill_Granade_Bomb::Set_Active(_bool isActive)
{
	m_isActive = isActive;
}

void CHands_Skill_Granade_Bomb::Reset_Bullets(_fmatrix matInitialShootingPos)
{

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, m_fBombRange * XMVector3Normalize(matInitialShootingPos.r[0]));
	m_pTransformCom->Set_State(CTransform::STATE_UP, m_fBombRange * XMVector3Normalize(matInitialShootingPos.r[1]));
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, m_fBombRange * XMVector3Normalize(matInitialShootingPos.r[2]));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, matInitialShootingPos.r[3]);
	m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());


	m_pParticle->Set_Matrix(m_pTransformCom->Get_WorldMatrix());
	_float fZero = 0;
	m_pParticle->Reset_Vertices(fZero);
	m_fAccNowLifeTime = 0.f;
}

HRESULT CHands_Skill_Granade_Bomb::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CHands_Skill_Granade_Bomb::Initialize(void* pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if(FAILED(Ready_Components()))
		return E_FAIL;
	if (FAILED(Ready_Particle()))
		return E_FAIL;



	return S_OK;
}

void CHands_Skill_Granade_Bomb::Priority_Update(_float fTimeDelta)
{
	if (false == m_isActive)
		return;

	if (m_fAccNowLifeTime >= m_fLifeTime)
	{
		m_isActive = false;
		CShader* pTerrainShaderCom = static_cast<CShader*>(m_pGameInstance->Get_Component(m_pGameInstance->Get_Current_Level(), TEXT("Layer_Map"), TEXT("Com_Shader"), 0));
		if (nullptr == pTerrainShaderCom)
		{
			MSG_BOX("MISSING Component _terrain shader");
			return ;
		}
		if (FAILED(pTerrainShaderCom->Bind_RawValue("g_bBrushSign", &m_isActive, sizeof(_bool))))
			return ;
	}
	else
	{
		m_fAccNowLifeTime += fTimeDelta;
	}

	m_pParticle->Priority_Update(fTimeDelta);
}

void CHands_Skill_Granade_Bomb::Update(_float fTimeDelta)
{
	if (false == m_isActive)
		return;

	m_pParticle->Update(fTimeDelta);




	CHands* pHandsSkill = static_cast<CHands*>(m_pGameInstance->Get_GameObject(m_pGameInstance->Get_Current_Level(), TEXT("Layer_APlayer"), CPlayer::PART_HANDS_SKILL, 0));
	m_pGameInstance->Add_Collision_Group(CCollider_Manager::COLLISION_GROUP::COLLISION_PLAYER_ATTACK, pHandsSkill, m_pColliderCom);
	

}

void CHands_Skill_Granade_Bomb::Late_Update(_float fTimeDelta)
{

	if (false == m_isActive)
		return;
	
	{
		CGameObject* pTarget = nullptr;
		if (m_pGameInstance->Check_Collision(m_pColliderCom, CCollider_Manager::COLLISION_GROUP::COLLISION_MONSTER, &pTarget))
		{
			
		}
		
	

	}
	m_pParticle->Late_Update(fTimeDelta);
	
	/*지형에 내 텍스처 렌더링하기전에 폭탄 텍스처 지형에뿌리자*/
	Bind_ShaderResources_ToTerrain();



#ifdef _DEBUG
	if (m_isActive)
		m_pGameInstance->Add_DebugComponent(m_pColliderCom);
#endif


}

HRESULT CHands_Skill_Granade_Bomb::Render()
{

	if (false == m_isActive)
		return S_OK;

	return S_OK;
}

HRESULT CHands_Skill_Granade_Bomb::Ready_Components()
{

	CBounding_Sphere::BOUND_SPHERE_DESC ColliderDesc{};
	ColliderDesc.vCenter = _float3(0.f, 0.f, 0.f);
	ColliderDesc.fRadius = 0.5f;
	/*Com_Collider*/
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;

/* 지형에뿌릴 텍스처만있으면 되고  지형의 셰이더컴포넌트는 나중에 얻어온다. 바인드할때마다 얻어온다 */
	/* Com_Texture */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Skill_Poison_Plane"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	

	return S_OK;
}

HRESULT CHands_Skill_Granade_Bomb::Bind_ShaderResources_ToTerrain()
{
	

	CShader* pTerrainShaderCom = static_cast<CShader*>(m_pGameInstance->Get_Component(m_pGameInstance->Get_Current_Level(), TEXT("Layer_Map"), TEXT("Com_Shader"), 0));
	
	if (nullptr == pTerrainShaderCom)
	{
		MSG_BOX("MISSING Component _terrain shader");
		return E_FAIL;
	}
	m_vRange.x =XMVectorGetX( XMVector3Length(m_pTransformCom->Get_WorldMatrix().r[0]))*0.5f;
	m_vRange.y = XMVectorGetY(XMVector3Length(m_pTransformCom->Get_WorldMatrix().r[2]))*0.5f;

	if (FAILED(pTerrainShaderCom->Bind_RawValue("g_BrushRange", &m_vRange,sizeof(_float2))))
		return E_FAIL;
		
	_float4 g_vBrushTextureLook;
	XMStoreFloat4(&g_vBrushTextureLook,XMVector3Normalize(m_pTransformCom->Get_WorldMatrix().r[2]));
	if (FAILED(pTerrainShaderCom->Bind_RawValue("g_vBrushTextureLook", &g_vBrushTextureLook, sizeof(_float4))))
		return E_FAIL;

	_float4 g_vBrushPos = {};
	XMStoreFloat4( &g_vBrushPos, m_pTransformCom->Get_WorldMatrix().r[3]);
	if (FAILED(pTerrainShaderCom->Bind_RawValue("g_vBrushPos", &g_vBrushPos, sizeof(_float4))))
			return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(pTerrainShaderCom, "g_BrushTexture")))
			return E_FAIL;
		
		
	if (FAILED(pTerrainShaderCom->Bind_RawValue("g_bBrushSign", &m_isActive, sizeof(_bool))))
		return E_FAIL;

	return S_OK;
}

HRESULT CHands_Skill_Granade_Bomb::Ready_Particle()
{

	//Prototype_GameObject_Particle_Player_Skill_Poison
	CParticle_Player_Skill_Poison::PARTICLE_PLAYER_SKILL_BOMB_POISON_DESC desc = {};
	desc.fRotationPerSec = XMConvertToRadians(90.f);
	desc.fSpeedPerSec = 10.f;
	desc.iData=0.f;
	m_pParticle = static_cast<CParticle_Player_Skill_Poison*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_STATIC, TEXT("Prototype_GameObject_Particle_Player_Skill_Poison"), &desc));

	return S_OK;
}

CHands_Skill_Granade_Bomb* CHands_Skill_Granade_Bomb::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CHands_Skill_Granade_Bomb* pInstance = new CHands_Skill_Granade_Bomb(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to create: CHands_Skill_Granade_Bomb");
		return nullptr;
	}

	return pInstance;
}

CGameObject* CHands_Skill_Granade_Bomb::Clone(void* pArg)
{
	CHands_Skill_Granade_Bomb* pInstance = new CHands_Skill_Granade_Bomb(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone: CHands_Skill_Granade_Bomb");
		return nullptr;
	}

	return pInstance;
}


void CHands_Skill_Granade_Bomb::Free()
{
	__super::Free();
	Safe_Release(m_pParticle);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pColliderCom);
}
