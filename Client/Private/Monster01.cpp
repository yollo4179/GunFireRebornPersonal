#include"stdafx.h"
#include "Monster01.h"
#include "GameInstance.h"
#include"Monster01_Body.h"
#include"Monster01_Head.h"

CMonster01::CMonster01(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CMonster{ pDevice, pContext }
{
}

CMonster01::CMonster01(const CMonster01& Prototype)
	:CMonster(Prototype)
{
}

HRESULT CMonster01::Initialize_Prototype()
{


	return S_OK;
}


HRESULT CMonster01::Initialize(void* pArg)
{
	MONSTER_DESC* pDesc = static_cast<MONSTER_DESC*>(pArg);
	pDesc->iNumPartObject = PART_END;
	
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	__super::m_HPInfo = { 1000,1000,1000,1000 };
	if (FAILED(Ready_Components()))
		return E_FAIL;
	/*For UI-HP_Bar*/
	XMStoreFloat4x4(&m_HPBarSocketMatrix, XMMatrixIdentity());//Get_BoneMatrix("MonsterHp");
	m_HPBarSocketMatrix._42 = 2.4f;

	if (FAILED(__super::Ready_UI_HPBar_LEVEL_GAMEPLAY()))
		return E_FAIL;


	m_pModelCom->SetUp_Animation(0, true);
	m_iState = M1_STATE::M1_STATE_BATTLE_IDLE;

	if (FAILED(Mapping_Anim_States()))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;
	return S_OK;

}

void CMonster01::Priority_Update(_float fTimeDelta)
{
	if (false == m_bActive)
		return;

	Pri_Update_LoopCall_State();

	Pri_Update_Non_LoopCall_State();
	/*m_fAccTime*/
	{
		if (!CONDITION_BATTLE_ATTACK1)
			m_fAcc_Attack1_Time+=fTimeDelta;
		if (!CONDITION_BATTLE_ATTACK2)
			m_fAcc_Attack2_Time += fTimeDelta;

	}

	if ((!m_isAnimEnd) && (CONDITION_NON_LOOP))
	{

	}
	else if (m_isAnimEnd && (CONDITION_NON_LOOP))
	{
		if (CONDITION_BATTLE_ATTACK1)
		{
			m_iState ^= M1_STATE::M1_STATE_BATTLE_ATTACK1;
			m_fAcc_Attack1_Time = 0.f;
			
			m_iState = M1_STATE_BATTLE_IDLE;
			m_fAcc_Battle_Idle_Time = 0.f;
		}
		else if (CONDITION_BATTLE_ATTACK2)
		{
			m_iState ^= M1_STATE::M1_STATE_BATTLE_ATTACK2;
			m_fAcc_Attack2_Time = 0.f;

			m_iState = M1_STATE::M1_STATE_BATTLE_IDLE;
			m_fAcc_Battle_Idle_Time = 0.f;
		}
	}
	else
	{
		if (0.5 > m_fAcc_Battle_Idle_Time && CONDITION_BATTLE_IDLE)
		{
			m_fAcc_Battle_Idle_Time += fTimeDelta;
		}
		else if (m_fAcc_Random_WalkTime > m_fAcc_Far_Walk_Time && CONDITION_FAR_WALK)
		{
			m_fAcc_Far_Walk_Time += fTimeDelta;
		}
		else if (m_fAcc_Random_WalkTime > m_fAcc_Middle_Run_Time && CONDITION_MIDDLE_RUN)
		{
			m_fAcc_Middle_Run_Time += fTimeDelta;
		}
		else
		{
			/* 상태 해제 */

			if (0.5 <= m_fAcc_Battle_Idle_Time && CONDITION_BATTLE_IDLE)
			{
				m_fAcc_Battle_Idle_Time = 0.f;
				m_iState ^= M1_STATE::M1_STATE_BATTLE_IDLE;
				m_iState = M1_STATE::M1_STATE_LOOP;
			}
			else if (m_fAcc_Random_WalkTime <= m_fAcc_Far_Walk_Time && CONDITION_FAR_WALK)
			{
				m_fAcc_Far_Walk_Time = 0.F;
				m_iState ^= M1_STATE::M1_STATE_FAR_WALK;
				m_iState = M1_STATE::M1_STATE_LOOP;
			}
			else if (m_fAcc_Random_WalkTime <= m_fAcc_Middle_Run_Time && CONDITION_MIDDLE_RUN)
			{
				m_fAcc_Middle_Run_Time = 0.F;
				m_iState ^= M1_STATE::M1_STATE_MIDDLE_RUN;
				m_iState = M1_STATE::M1_STATE_LOOP;
			}

			if (CONDITION_BATTLE_IDLE)
			{

			}
			else if (CONDITION_FAR_WALK)
			{

			}
			else if (CONDITION_MIDDLE_RUN)
			{

			}
			else
			{
				if ((__super::Util_isPlayer_InRange(m_fBattleRange)))
				{
					if ((m_isAnimEnd) || CONDITION_LOOP)
					{
						m_fAcc_Battle_Idle_Time = 0.f;
						m_iState = M1_STATE::M1_STATE_BATTLE_IDLE;
					}
				}

				else if (__super::Util_isPlayer_InRange(m_fMiddleRange)&&!(__super::Util_isPlayer_InRange(m_fBattleRange))) 
				{
					if ((m_isAnimEnd) || CONDITION_LOOP)
					{
						m_iState = M1_STATE::M1_STATE_MIDDLE_RUN;
					}

					_float fChance = m_pGameInstance->Compute_Random(0.5f, 1.f);
					m_fAcc_Random_WalkTime = fChance;
				}
				else if (__super::Util_isPlayer_InRange(m_fFarRange))
				{

					if (m_isAnimEnd || CONDITION_LOOP)
					{
						m_iState = M1_STATE::M1_STATE_FAR_WALK;
					}
					_float fChance = m_pGameInstance->Compute_Random(0.5f, 1.f);
					m_fAcc_Random_WalkTime = fChance;
				}
				else
				{
					if (m_isAnimEnd || CONDITION_LOOP)
						m_iState = M1_STATE::M1_STATE_BATTLE_IDLE;
				}
				/*Slash*/
				
				if (1.f <= m_fAcc_Attack2_Time && (m_isAnimEnd || CONDITION_LOOP) && (__super::Util_isPlayer_InRange(m_fBattleRange)))
				{
					m_iState |= M1_STATE::M1_STATE_BATTLE_ATTACK2;

				}
				if (5.f <= m_fAcc_Attack1_Time && (m_isAnimEnd || CONDITION_LOOP) && __super::Util_isPlayer_InRange(m_fMiddleRange))
				{
					m_iState |= M1_STATE::M1_STATE_BATTLE_ATTACK1;
				}


			}

		}

	}
	/*Animation*/

	if (CONDITION_BATTLE_ATTACK1)
	{
		if (5.f <= m_fAcc_Attack1_Time && (m_isAnimEnd || CONDITION_LOOP))
		{
			m_pModelCom->Move_NextAnim(m_vecAnimStates[M1_ANIM::M1_ANIM_BATTLE_ATTACK1], 0.2f, false);
			Pri_Update_Cancle_LoopState();
			m_fAcc_Attack1_Time = 0.f;
		}
	}

	else if (CONDITION_BATTLE_ATTACK2)
	{
		if (1.f <= m_fAcc_Attack2_Time && (m_isAnimEnd || CONDITION_LOOP))
		{
			m_pModelCom->Move_NextAnim(m_vecAnimStates[M1_ANIM::M1_ANIM_BATTLE_ATTACK2], 0.2f, false);
			Pri_Update_Cancle_LoopState();
			m_fAcc_Attack2_Time = 0.f;
		}
	}
	
	else if (CONDITION_FAR_WALK)
	{
		if (m_bCall_Far_Walk)
		{
			m_pModelCom->Move_NextAnim(m_vecAnimStates[M1_ANIM::M1_ANIM_FAR_WALK], 0.2f, true);
			m_bCall_Far_Walk = false;
		}
	}
	else if (CONDITION_BATTLE_IDLE)
	{
		if (m_bCall_Battle_Idle)
		{
			m_pModelCom->Move_NextAnim(m_vecAnimStates[M1_ANIM_BATTLE_IDLE], 0.2f, true);
			m_bCall_Battle_Idle = false;
		}
	}
	else if (CONDITION_MIDDLE_RUN)
	{
		if (m_bCall_Middle_Run)
		{
			m_pModelCom->Move_NextAnim(m_vecAnimStates[M1_ANIM::M1_ANIM_MIDDLE_RUN], 0.2f, true);
			m_bCall_Middle_Run = false;
		}
	}
	


	__super::Pri_Update_NaviMesh();
	__super::Priority_Update(fTimeDelta);


}

void CMonster01::Update(_float fTimeDelta)
{
	if (false == m_bActive)
		return;
	m_isAnimEnd = m_pModelCom->Play_Animation(fTimeDelta, m_isChangingMotion);

	__super::Update(fTimeDelta);
}

void CMonster01::Late_Update(_float fTimeDelta)
{
	if (false == m_bActive)
		return;
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);

	__super::Late_Update(fTimeDelta);
}

HRESULT CMonster01::Render()
{
	if (false == m_bActive)
		return S_OK;

	__super::Render();
	
	
	return S_OK;
}

HRESULT CMonster01::Mapping_Anim_States()
{

	
	m_vecAnimStates.resize(M1_ANIM::M1_ANIM_END);
	m_vecAnimStates[M1_ANIM::M1_ANIM_BATTLE_IDLE] = 0;
	m_vecAnimStates[M1_ANIM::M1_ANIM_BATTLE_ATTACK1] = 7;
	m_vecAnimStates[M1_ANIM::M1_ANIM_BATTLE_ATTACK2] = 8;

	m_vecAnimStates[M1_ANIM::M1_ANIM_FAR_WALK] = 1;

	m_vecAnimStates[M1_ANIM::M1_ANIM_MIDDLE_RUN] = 2;

	m_vecAnimStates[M1_ANIM::M1_ANIM_HIT] = 4;
	m_vecAnimStates[M1_ANIM::M1_ANIM_BREATHING] = 9;
	
	return S_OK;
}

void CMonster01::Pri_Update_Cancle_LoopState()
{
	if (CONDITION_BATTLE_IDLE)
		m_iState ^= M1_STATE::M1_STATE_BATTLE_IDLE;
	if (CONDITION_MIDDLE_RUN)
		m_iState ^= M1_STATE::M1_STATE_MIDDLE_RUN;
	if (CONDITION_FAR_WALK)
		m_iState ^= M1_STATE::M1_STATE_FAR_WALK;
	if (CONDITION_STATE_LOOP)
		m_iState ^= M1_STATE::M1_STATE_LOOP;
}

void CMonster01::Pri_Update_LoopCall_State()
{
	/*Battle IDle*/
	if (!CONDITION_BATTLE_IDLE && false == m_bCall_Battle_Idle)
		m_bCall_Battle_Idle = true;
	/*Middle Run*/
	if (!CONDITION_MIDDLE_RUN && false == m_bCall_Middle_Run)
		m_bCall_Middle_Run = true;
	/*FAR */
	if (!CONDITION_FAR_WALK && false == m_bCall_Far_Walk)
		m_bCall_Far_Walk = true;

	
}

void CMonster01::Pri_Update_Non_LoopCall_State()
{
}

HRESULT CMonster01::Ready_Components()
{
	
	/* Com_Navigation */

	CNavigation::NAVIGATION_DESC		NaviDesc{};
	NaviDesc.iStartIndex = m_iInitial_NaviIndex;
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Navigation"),
		TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), &NaviDesc)))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Model  for Anim*/
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Monster01"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;
	/*Com_ColliderFor Range*/
	CBounding_Sphere::BOUND_SPHERE_DESC ColliderDesc{};
	ColliderDesc.vCenter = _float3(0.f, 0.f, 0.f);
	ColliderDesc.fRadius = 0.5f;
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom_Range), &ColliderDesc)))
		return E_FAIL;


	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Monster_Desolve"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom_Desolve))))
		return E_FAIL;

	return S_OK;


}

HRESULT CMonster01::Bind_ShaderResources()
{
	__super::Bind_ShaderResources();

	return S_OK;
}

HRESULT CMonster01::Ready_PartObjects()
{
	CMonster01_Head::MONSTER01_HEAD_DESC Head_Desc = {};
	Head_Desc.fRotationPerSec = XMConvertToRadians(90.f);
	Head_Desc.fSpeedPerSec = 10.f;
	Head_Desc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4_Ptr();
	Head_Desc.pSocketMatrix = Get_BoneMatrix("aimatWeaknessPoint");
	Head_Desc.pisMonsterPicked = &m_isMonsterPicked;
	Head_Desc.pHitTime = &m_fHitTime;
	Head_Desc.pAccHitTime = &m_fAccHitTime;
	Head_Desc.pHPInfo = &m_HPInfo;
	Head_Desc.pHit_NormalMonster = &m_bHit_NormalMonster;
	/*Head_Desc.pHeadShot = &m_bHeadShot;*/
	m_PartObjects[PART_HEAD] = static_cast<CPartObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster01_Head"), &Head_Desc));
	if (nullptr == m_PartObjects[PART_HEAD])
		return E_FAIL;



	CMonster01_Body::MONSTER01_BODY_DESC Body_Desc = {};
	Body_Desc.fRotationPerSec = XMConvertToRadians(90.f);
	Body_Desc.fSpeedPerSec = 10.f;
	Body_Desc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4_Ptr();
	Body_Desc.pSocketMatrix = Get_BoneMatrix("Bip001 Spine1");
	Body_Desc.pisMonsterPicked = &m_isMonsterPicked;
	Body_Desc.pHitTime = &m_fHitTime;
	Body_Desc.pAccHitTime = &m_fAccHitTime;
	Body_Desc.pHPInfo = &m_HPInfo;
	Body_Desc.pHit_NormalMonster = &m_bHit_NormalMonster;
	m_PartObjects[PART_BODY] = static_cast<CPartObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster01_Body"), &Body_Desc));
	if (nullptr == m_PartObjects[PART_BODY])
		return E_FAIL;

	return S_OK;
}

CMonster01* CMonster01::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMonster01* pInstance = new CMonster01(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Created Monster01_ Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CMonster01::Clone(void* pArg)
{
	CMonster01* pInstance = new CMonster01(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Clone Monster01 Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CMonster01::Free()
{
	__super::Free();
	
}