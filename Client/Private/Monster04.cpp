#include"stdafx.h"
#include "Monster04.h"
#include "GameInstance.h"
#include"Monster04_Head.h"
#include"Monster04_Body.h"
#include"Monster04_Sword.h"
CMonster04::CMonster04(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CMonster{ pDevice, pContext }
{
}

CMonster04::CMonster04(const CMonster04& Prototype)
	:CMonster(Prototype)
{
}

HRESULT CMonster04::Initialize_Prototype()
{


	return S_OK;
}


HRESULT CMonster04::Initialize(void* pArg)
{
	MONSTER_DESC* pDesc = static_cast<MONSTER_DESC*>(pArg);
	pDesc->iNumPartObject = PART_END;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	__super::m_HPInfo = { 3000,3000,3000,3000 };
	if (FAILED(Ready_Components()))
		return E_FAIL;
	/*For UI-HP_Bar*/
	XMStoreFloat4x4(&m_HPBarSocketMatrix, XMMatrixIdentity());//Get_BoneMatrix("MonsterHp");
	m_HPBarSocketMatrix._42 = 2.4f;

	if (FAILED(__super::Ready_UI_HPBar_LEVEL_GAMEPLAY()))
		return E_FAIL;

	m_pModelCom->SetUp_Animation(rand() % 6, true);
	

	if(FAILED(Mapping_Anim_States()))
		return E_FAIL;
	m_iState = M4_STATE::M4_STATE_FAR_IDLE;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	return S_OK;
}

void CMonster04::Priority_Update(_float fTimeDelta)
{

	if (false == m_bActive)
		return;
	
	Pri_Update_LoopCall_State();

	Pri_Update_Non_LoopCall_State();

	/*m_fAccTime*/
	{
		/*NonLoop_Attack*/
		
		/*쿨타임*/
		if (!CONDITION_RUSH_SLASH)
			m_fAcc_Rush_Slash_Time += fTimeDelta;
		
		if (!CONDITION_BATTLE_SLASH)
			m_fAcc_Slash_Time += fTimeDelta;


		/*실해중 누적이 필요한 것들*/
		if (CONDITION_ROLL)
			m_fAcc_StartRollTime += fTimeDelta;
		if (CONDITION_RUSH_SLASH)
			m_fAcc_StartRushSlashTime += fTimeDelta;
	}
	if ((!m_isAnimEnd) && (CONDITION_NON_LOOP))
	{
		if (CONDITION_RUSH_SLASH)
			m_fAccRushSlash_Timing += fTimeDelta;

	}
	else if (m_isAnimEnd && (CONDITION_NON_LOOP))
	{
		if (CONDITION_HIT)
		{
			m_fAcc_StartRollTime = 0.f;
			m_fAcc_StartRushSlashTime = 0.f;
			m_bHeadShot = false;
			m_iState ^= M4_STATE::M4_STATE_HIT;
			m_fAcc_StartRollTime = 0.f;
			m_iState = M4_STATE::M4_STATE_BATTLE_IDLE;
			m_fAcc_Battle_Idle_Time = 0.f;
		}

		if (CONDITION_ROLL_LEFT)
		{
			m_iState ^= M4_STATE::M4_STATE_ROLL_LEFT;
			m_fAcc_StartRollTime = 0.f;
			m_iState = M4_STATE::M4_STATE_BATTLE_IDLE;
			m_fAcc_Battle_Idle_Time = 0.f;
		}
		else if (CONDITION_ROLL_RIGHT)
		{
			m_iState ^= M4_STATE::M4_STATE_ROLL_RIGHT;
			m_fAcc_StartRollTime = 0.f;
			m_iState = M4_STATE::M4_STATE_BATTLE_IDLE;
			m_fAcc_Battle_Idle_Time = 0.f;
		}
		else if (CONDITION_BATTLE_SLASH)
		{
			m_iState ^= M4_STATE::M4_STATE_BATTLE_SLASH;
			m_fAcc_Slash_Time = 0.f;

			m_iState = M4_STATE::M4_STATE_BATTLE_IDLE;
			m_fAcc_Battle_Idle_Time = 0.f;
		}
		else if (CONDITION_RUSH_SLASH)
		{
			m_iState ^= M4_STATE::M4_STATE_RUSH_SLASH;
			m_fAcc_Rush_Slash_Time = 0.f;
			m_fAcc_StartRushSlashTime = 0.f;

			m_iState = M4_STATE::M4_STATE_BATTLE_IDLE;
			m_fAcc_Battle_Idle_Time = 0.f;

			m_fAccRushSlash_Timing = 0.f;
			m_bSound_RushSlash = true;
		}
	}
	else
	{
			if (0.5f > m_fAcc_Battle_Idle_Time && CONDITION_BATTLE_IDLE)
			{
				m_fAcc_Battle_Idle_Time += fTimeDelta;
			}
			else if (m_fAcc_Random_WalkTime >= m_fAcc_Far_Walk_Left_Time && CONDITION_FAR_WALK_LEFT)
			{
				m_fAcc_Far_Walk_Left_Time += fTimeDelta;
			}
			else if (m_fAcc_Random_WalkTime >= m_fAcc_Far_Walk_Right_Time && CONDITION_FAR_WALK_RIGHT)
			{
				m_fAcc_Far_Walk_Right_Time += fTimeDelta;
			}
			else if (m_fAcc_Random_WalkTime >= m_fAcc_Far_Walk_Time && CONDITION_FAR_WALK)
			{
				m_fAcc_Far_Walk_Time += fTimeDelta;
			}
			else if (m_fAcc_Random_WalkTime >= m_fAcc_Battle_Walk_Back_Time && CONDITION_BATTLE_WALK_BACK)
			{
				m_fAcc_Battle_Walk_Back_Time += fTimeDelta;
			}
			else if (CONDITION_MIDDLE_RUN && (!__super::Util_isPlayer_InRange(m_fBattleRange)))
			{
			
			}
			else
			{

				/*상태 해제*/
				if (0.5f <= m_fAcc_Battle_Idle_Time && CONDITION_BATTLE_IDLE)
				{
					m_fAcc_Battle_Idle_Time = 0.f;
					m_iState ^= M4_STATE::M4_STATE_BATTLE_IDLE;
					m_iState = M4_STATE::M4_STATE_LOOP;
				}
				else if (m_fAcc_Random_WalkTime < m_fAcc_Far_Walk_Left_Time && CONDITION_FAR_WALK_LEFT)
				{
					m_fAcc_Far_Walk_Left_Time = 0.f;
					m_iState ^= M4_STATE::M4_STATE_FAR_WALK_LEFT;
					m_iState = M4_STATE::M4_STATE_LOOP;
				}
				else if (m_fAcc_Random_WalkTime < m_fAcc_Far_Walk_Right_Time && CONDITION_FAR_WALK_RIGHT)
				{
					m_fAcc_Far_Walk_Right_Time = 0.F;
					m_iState ^= M4_STATE::M4_STATE_FAR_WALK_RIGHT;
					m_iState = M4_STATE::M4_STATE_LOOP;
				}
				else if (m_fAcc_Random_WalkTime < m_fAcc_Far_Walk_Time && CONDITION_FAR_WALK)
				{
					m_fAcc_Far_Walk_Time = 0.F;
					m_iState ^= M4_STATE::M4_STATE_FAR_WALK;
					m_iState = M4_STATE::M4_STATE_LOOP;
				}
				else if (CONDITION_MIDDLE_RUN &&(__super::Util_isPlayer_InRange(m_fBattleRange)) )
				{
	
					m_iState ^= M4_STATE::M4_STATE_MIDDLE_RUN;
					m_iState = M4_STATE::M4_STATE_LOOP;
				}
				else if (m_fAcc_Random_WalkTime < m_fAcc_Battle_Walk_Back_Time && CONDITION_BATTLE_WALK_BACK)
				{
					m_fAcc_Battle_Walk_Back_Time = 0.F;
					m_iState ^= M4_STATE::M4_STATE_BATTLE_WALK_BACK;
					m_iState = M4_STATE::M4_STATE_LOOP;
				}



				if (CONDITION_BATTLE_IDLE)
				{

				}
				else if (CONDITION_FAR_WALK_LEFT)
				{

				}
				else if (CONDITION_FAR_WALK_RIGHT)
				{

				}
				else if (CONDITION_FAR_WALK)
				{

				}
				else if (CONDITION_BATTLE_WALK_BACK)
				{

				}
				else if (CONDITION_MIDDLE_RUN)
				{

				}
				else
				{

					if (__super::Util_isPlayer_InRange(m_fBattleRange))
					{
						_float fChance = m_pGameInstance->Compute_Random(0.f, 10.f);

						if (fChance < 8.5f && (m_isAnimEnd || CONDITION_LOOP))
						{
							m_iState = M4_STATE::M4_STATE_BATTLE_WALK_BACK;
						}
						else if (fChance <= 9.25f && (m_isAnimEnd || CONDITION_LOOP))
						{
							m_iState = M4_STATE::M4_STATE_ROLL_LEFT;
						}
						else if (fChance <= 10.f && (m_isAnimEnd || CONDITION_LOOP))
						{
							m_iState = M4_STATE::M4_STATE_ROLL_RIGHT;
						}
						fChance = m_pGameInstance->Compute_Random(0.5f, 1.5f);
						m_fAcc_Random_WalkTime = fChance;

					}

					else if (__super::Util_isPlayer_InRange(m_fMiddleRange))
					{
						_float fChance = m_pGameInstance->Compute_Random(0.f, 10.f);

						if (fChance <= 7.f && (m_isAnimEnd || CONDITION_LOOP))
						{
							m_iState = M4_STATE::M4_STATE_MIDDLE_RUN;
						}
						else if (fChance <= 8.5f && (m_isAnimEnd || CONDITION_LOOP))
						{
							m_iState = M4_STATE::M4_STATE_ROLL_LEFT;
						}
						else if (fChance <= 10.f && (m_isAnimEnd || CONDITION_LOOP))
						{
							m_iState = M4_STATE::M4_STATE_ROLL_RIGHT;
						}
						fChance = m_pGameInstance->Compute_Random(0.5f, 1.5f);
						m_fAcc_Random_WalkTime = fChance;
					}
					else if (__super::Util_isPlayer_InRange(m_fFarRange))
					{
						_float fChance = m_pGameInstance->Compute_Random(0.f, 10.f);

						if (fChance <= 8.5f && (m_isAnimEnd || CONDITION_LOOP))
						{
							m_iState = M4_STATE::M4_STATE_FAR_WALK;
						}
						else if (fChance <= 9.25f && (m_isAnimEnd || CONDITION_LOOP))
						{
							m_iState = M4_STATE::M4_STATE_FAR_WALK_RIGHT;
						}
						else if (fChance <= 10.f && (m_isAnimEnd || CONDITION_LOOP))
						{
							m_iState = M4_STATE::M4_STATE_FAR_WALK_LEFT;
						}
						fChance = m_pGameInstance->Compute_Random(0.5f, 1.5f);
						m_fAcc_Random_WalkTime = fChance;
					}
					else
					{
						if (m_isAnimEnd || CONDITION_LOOP)
							m_iState = M4_STATE::M4_STATE_FAR_IDLE;
					}


					/*Slash*/
					if ( 1.f<=m_fAcc_Slash_Time  && (m_isAnimEnd || CONDITION_LOOP) && __super::Util_isPlayer_InRange(m_fBattleRange))
					{
						m_iState |= M4_STATE::M4_STATE_BATTLE_SLASH;
					}
					/*Rush_Slash*/
					else if (
						(__super::Util_isPlayer_InRange(m_fRush_SlashRange)) &&
						(!__super::Util_isPlayer_InRange(m_fBattleRange + 1.f)) && 
						1.f <= m_fAcc_Rush_Slash_Time &&
						(m_isAnimEnd || CONDITION_LOOP) 
						)
					{
						
						/*m_fAcc_StartRushSlashTime = 0.f;*/
						m_iState |= M4_STATE::M4_STATE_RUSH_SLASH;

					}

				}


			}

		
	}

	if (true == m_bHeadShot)
	{
		m_iState = M4_STATE_HIT;
	}
		/*Anumation State*/
	if (CONDITION_RUSH_SLASH)
	{
		if (1.f <= m_fAcc_Rush_Slash_Time&&(m_isAnimEnd || CONDITION_LOOP))
		{
			m_pModelCom->Move_NextAnim(m_vecAnimStates[M4_ANIM::M4_ANIM_RUSH_SLASH],0.2f,false);
			Pri_Update_Cancle_LoopState();
			m_fAcc_Rush_Slash_Time = 0.f;
		}
	}
	else if (CONDITION_BATTLE_SLASH)
	{
		if (1.f <= m_fAcc_Slash_Time && (m_isAnimEnd || CONDITION_LOOP))
		{
			m_pGameInstance->PlayEffect(L"Sword.ogg", 0.08f);
			m_pModelCom->Move_NextAnim(m_vecAnimStates[M4_ANIM::M4_ANIM_BATTLE_SLASH], 0.2f, false);
			Pri_Update_Cancle_LoopState();
			m_fAcc_Slash_Time = 0.f;
		}
	}
	else if (CONDITION_FAR_WALK)
		{
			if (m_bCall_Far_Walk)
			{
				m_pModelCom->Move_NextAnim(m_vecAnimStates[M4_ANIM::M4_ANIM_WALK], 0.2f, true);
				m_bCall_Far_Walk = false;
			}
		}
		else if (CONDITION_FAR_WALK_LEFT)
		{
			if (m_bCall_Far_Walk_Left)
			{
				m_pModelCom->Move_NextAnim(m_vecAnimStates[M4_ANIM::M4_ANIM_FAR_WALK_LEFT], 0.2f, true);
				m_bCall_Far_Walk_Left = false;
			}
		}

		else if (CONDITION_FAR_WALK_RIGHT)
		{
			if (m_bCall_Far_Walk_Right)
			{
				m_pModelCom->Move_NextAnim(m_vecAnimStates[M4_ANIM::M4_ANIM_FAR_WALK_RIGHT], 0.2f, true);
				m_bCall_Far_Walk_Right = false;
			}
		}
		else if (CONDITION_FAR_IDLE)
		{
			if (m_bCall_Far_Idle)
			{
				m_pModelCom->Move_NextAnim(m_vecAnimStates[M4_ANIM::M4_ANIM_FAR_IDLE], 0.2f, true);
				m_bCall_Far_Idle = false;
			}
		}

		else if (CONDITION_MIDDLE_RUN)
		{
			if (m_bCall_Middle_Run)
			{
				m_pModelCom->Move_NextAnim(m_vecAnimStates[M4_ANIM::M4_ANIM_MIDDLE_RUN], 0.2F, true);
				m_bCall_Middle_Run = false;
			}
		}
		else if (CONDITION_BATTLE_WALK_BACK)
		{
			if (m_bCall_Battle_Walk_Back)
			{
				m_pModelCom->Move_NextAnim(m_vecAnimStates[M4_ANIM::M4_ANIM_BATTLE_WALK_BACK], 0.2f, true);
				m_bCall_Battle_Walk_Back = false;
			}
		}
		else if (CONDITION_ROLL_LEFT)
		{
			if (m_bCall_Roll_Left)
			{
				m_pModelCom->Move_NextAnim(m_vecAnimStates[M4_ANIM::M4_ANIM_ROLL_LEFT], 0.2f, false);
				m_bCall_Roll_Left = false;
			}
		}
		else if (CONDITION_ROLL_RIGHT)
		{
			if (m_bCall_Roll_Right)
			{
				m_pModelCom->Move_NextAnim(m_vecAnimStates[M4_ANIM::M4_ANIM_ROLL_RIGHT], 0.2f, false);
				m_bCall_Roll_Right = false;
			}
		}
		else if (CONDITION_BATTLE_IDLE)
		{
			if (m_bCall_Battle_Idle)
			{
				m_pModelCom->Move_NextAnim(m_vecAnimStates[M4_ANIM::M4_ANIM_BATTLE_IDLE], 0.2F, true);
				m_bCall_Battle_Idle = false;
			}
		}
		else if (CONDITION_HIT)
		{
			if (m_bCall_Hit)
			{
				m_pGameInstance->PlayEffect(L"MonsterHit.wav", 0.05f);
				m_pModelCom->Move_NextAnim(m_vecAnimStates[M4_ANIM::M4_ANIM_HIT], 0.2F, false);
				m_bCall_Hit = false;
			}
		}
		/*Anim End*/

	if (Util_isPlayer_InRange(m_fFarRange))
		Util_TurnTo_Player(fTimeDelta * 2.f);


	if (m_fAcc_StartRollTime > 0.15f && m_fAcc_StartRollTime < 0.30f && CONDITION_ROLL_RIGHT)
	{

		Util_Go_Right(fTimeDelta, 1.f);
	}

	else if (m_fAcc_StartRollTime > 0.15f && m_fAcc_StartRollTime < 0.30f && CONDITION_ROLL_LEFT)
		Util_Go_Left(fTimeDelta, 1.f);


	else if (CONDITION_RUSH_SLASH && m_fAcc_StartRushSlashTime < 0.4f)
	{
		Util_Go_Straight(fTimeDelta, 0.8f);
	}
	else if (CONDITION_FAR_WALK) {
		Util_Go_Straight(fTimeDelta, 0.2f);
	}
	else if (CONDITION_FAR_WALK_LEFT)
	{
		Util_Go_Left(fTimeDelta, 0.2f);
	}
	else if (CONDITION_FAR_WALK_RIGHT)
	{
		Util_Go_Right(fTimeDelta, 0.2f);
	}
	else if (CONDITION_MIDDLE_RUN)
	{
		Util_Go_Straight(fTimeDelta, 0.6f);
	}


	else if (CONDITION_BATTLE_WALK_BACK)
		Util_Go_Back(fTimeDelta, 0.3f);






	__super::Pri_Update_NaviMesh();
	__super::Priority_Update(fTimeDelta);

	/*Sound*/
	if (m_fAccRushSlash_Timing > 0.15f && true == m_bSound_RushSlash)
	{
		m_pGameInstance->PlayEffect(L"Rush_Slash.mp3", 0.08f);
		m_bSound_RushSlash = false;
	}

}
	






void CMonster04::Update(_float fTimeDelta)
{

	if (false == m_bActive)
		return;
	if (true == m_isRemoving)
		return;
	
	m_isAnimEnd =m_pModelCom->Play_Animation(fTimeDelta, m_isChangingMotion);
	
	
	__super::Update(fTimeDelta);

}

void CMonster04::Late_Update(_float fTimeDelta)
{

	if (false == m_bActive)
		return;


	if (true == m_pGameInstance->isIn_Frustum_InWorldSpace(m_pTransformCom->Get_WorldMatrix().r[3], 2.f))
	{
		m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
		if (true == m_isRemoving)
		{
			m_pGameInstance->Add_RenderObject(CRenderer::RG_GLOW, this);
		}
	}


	__super::Late_Update(fTimeDelta);
}

HRESULT CMonster04::Render()
{

	if (false == m_bActive)
		return S_OK;
	


	__super::Render();
	


	
	return S_OK;
}

HRESULT CMonster04::Render_Shadow()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;


	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Shadow_Transform(CShadow::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Shadow_Transform(CShadow::D3DTS_PROJ))))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; i++)
	{
		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE, 0)))
			return E_FAIL;
		if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i)))
			return E_FAIL;

		m_pShaderCom->Begin(1);

		m_pModelCom->Render(i);
	}


	return S_OK;
}

HRESULT CMonster04::Mapping_Anim_States()
{

	m_vecAnimStates.resize(M4_ANIM::M4_ANIM_END);
	m_vecAnimStates[M4_ANIM::M4_ANIM_BATTLE_IDLE] = 0;
	m_vecAnimStates[M4_ANIM::M4_ANIM_BATTLE_WALK_BACK] = 3;
	m_vecAnimStates[M4_ANIM::M4_ANIM_BATTLE_SLASH] = 15;

	m_vecAnimStates[M4_ANIM::M4_ANIM_RUSH_SLASH] = 14;

	m_vecAnimStates[M4_ANIM::M4_ANIM_MIDDLE_RUN] = 8;

	m_vecAnimStates[M4_ANIM::M4_ANIM_FAR_WALK_LEFT] = 4;
	m_vecAnimStates[M4_ANIM::M4_ANIM_FAR_WALK_RIGHT] = 2;
	m_vecAnimStates[M4_ANIM::M4_ANIM_WALK] = 5;
	m_vecAnimStates[M4_ANIM::M4_ANIM_FAR_IDLE] = 6;

	m_vecAnimStates[M4_ANIM::M4_ANIM_ROLL_LEFT] = 12;
	m_vecAnimStates[M4_ANIM::M4_ANIM_ROLL_RIGHT] = 11;
	
	m_vecAnimStates[M4_ANIM::M4_ANIM_HIT] = 7;


	

	return S_OK;
}

void CMonster04::Pri_Update_Cancle_LoopState()
{
	if (CONDITION_BATTLE_IDLE)
		m_iState ^= M4_STATE::M4_STATE_BATTLE_IDLE;

	if (CONDITION_BATTLE_WALK_BACK)
		m_iState ^= M4_STATE::M4_STATE_BATTLE_WALK_BACK;

	if (CONDITION_MIDDLE_RUN)
		m_iState ^= M4_STATE::M4_STATE_MIDDLE_RUN;

	if (CONDITION_FAR_WALK)
		m_iState ^= M4_STATE::M4_STATE_FAR_WALK;
	if (CONDITION_FAR_WALK_LEFT)
		m_iState ^= M4_STATE::M4_STATE_FAR_WALK_LEFT;
	if (CONDITION_FAR_WALK_RIGHT)
		m_iState ^= M4_STATE::M4_STATE_FAR_WALK_RIGHT;
	if (CONDITION_FAR_IDLE)
		m_iState ^= M4_STATE::M4_STATE_FAR_IDLE;

	if (CONDITION_STATE_LOOP)
		m_iState ^= M4_STATE::M4_STATE_LOOP;
}

void CMonster04::Pri_Update_LoopCall_State()
{
	/*Battle IDle*/
	if (!CONDITION_BATTLE_IDLE && false == m_bCall_Battle_Idle)
		m_bCall_Battle_Idle = true;
	/*Close Walk Back*/
	if (!CONDITION_BATTLE_WALK_BACK && false == m_bCall_Battle_Walk_Back)
		m_bCall_Battle_Walk_Back = true;
	/*Middle Run*/
	if (!CONDITION_MIDDLE_RUN && false == m_bCall_Middle_Run)
		m_bCall_Middle_Run = true;
	/*FAR */
	if (!CONDITION_FAR_WALK && false == m_bCall_Far_Walk)
		m_bCall_Far_Walk = true;
	if (!CONDITION_FAR_WALK_RIGHT && false == m_bCall_Far_Walk)
		m_bCall_Far_Walk_Right = true;
	if (!CONDITION_FAR_WALK_LEFT && false == m_bCall_Far_Walk_Left)
		m_bCall_Far_Walk_Left = true;
	if (!CONDITION_FAR_IDLE && false == m_bCall_Far_Idle)
		m_bCall_Far_Idle = true;
}

void CMonster04::Pri_Update_Non_LoopCall_State()
{
	if (!CONDITION_ROLL_LEFT && false == m_bCall_Roll_Left)
		m_bCall_Roll_Left = true;
	if (!CONDITION_ROLL_RIGHT && false == m_bCall_Roll_Right)
		m_bCall_Roll_Right = true;
	if (!CONDITION_HIT && false == m_bCall_Hit)
		m_bCall_Hit = true;
}

HRESULT CMonster04::Ready_Components()
{


	/* Com_Navigation */
	CNavigation::NAVIGATION_DESC		NaviDesc{};
	NaviDesc.iStartIndex =m_iInitial_NaviIndex;
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Navigation"),
		TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), &NaviDesc)))
		return E_FAIL;
	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;
	/* Com_Model */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Monster04"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;


	/*Com_ColliderFor Range*/
	CBounding_Sphere::BOUND_SPHERE_DESC ColliderDesc{};
	ColliderDesc.vCenter = _float3(0.f, 0.f, 0.f);
	ColliderDesc.fRadius = 0.6f;
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom_Range), &ColliderDesc)))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Monster_Desolve"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom_Desolve))))
		return E_FAIL;

	return S_OK;


}

HRESULT CMonster04::Bind_ShaderResources()
{
	__super::Bind_ShaderResources();

	return S_OK;
}

HRESULT CMonster04::Ready_PartObjects()
{
	CMonster04_Head::MONSTER04_HEAD_DESC Head_Desc = {};
	Head_Desc.fRotationPerSec = XMConvertToRadians(90.f);
	Head_Desc.fSpeedPerSec = 10.f;
	Head_Desc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4_Ptr();
	Head_Desc.pSocketMatrix = Get_BoneMatrix("aimatWeaknessPoint");
	Head_Desc.pHeadShot = &m_bHeadShot;
	Head_Desc.pisMonsterPicked = &m_isMonsterPicked;
	Head_Desc.pHitTime = &m_fHitTime;
	Head_Desc.pAccHitTime = &m_fAccHitTime;
	Head_Desc.pHPInfo = &m_HPInfo;
	Head_Desc.pHit_NormalMonster = &m_bHit_NormalMonster;
	m_PartObjects[PART_HEAD] = static_cast<CPartObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster04_Head"), &Head_Desc));
	if (nullptr == m_PartObjects[PART_HEAD])
		return E_FAIL;



	CMonster04_Body::MONSTER04_BODY_DESC Body_Desc = {};
	Body_Desc.fRotationPerSec = XMConvertToRadians(90.f);
	Body_Desc.fSpeedPerSec = 10.f;
	Body_Desc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4_Ptr();
	Body_Desc.pSocketMatrix = Get_BoneMatrix("Bip001 Spine");
	Body_Desc.pisMonsterPicked = &m_isMonsterPicked;
	Body_Desc.pHitTime = &m_fHitTime;
	Body_Desc.pAccHitTime = &m_fAccHitTime;
	Body_Desc.pHPInfo = &m_HPInfo;
	Body_Desc.pHit_NormalMonster = &m_bHit_NormalMonster;
	m_PartObjects[PART_BODY] = static_cast<CPartObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster04_Body"), &Body_Desc));
	if (nullptr == m_PartObjects[PART_BODY])
		return E_FAIL;

	CMonster04_Sword::MONSTER04_SWORD_DESC Sword_Desc = {};
	Sword_Desc.fRotationPerSec = XMConvertToRadians(90.f);
	Sword_Desc.fSpeedPerSec = 10.f;
	Sword_Desc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4_Ptr();
	Sword_Desc.pSocketMatrix = Get_BoneMatrix("Bip001 Prop1");
	Sword_Desc.pState = &m_iState;
	m_PartObjects[PART_SWORD] = static_cast<CPartObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster04_Sword"), &Sword_Desc));
	if (nullptr == m_PartObjects[PART_SWORD])
		return E_FAIL;

	return S_OK;
}

CMonster04* CMonster04::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMonster04* pInstance = new CMonster04(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Created Monster04_ Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CMonster04::Clone(void* pArg)
{
	CMonster04* pInstance = new CMonster04(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Clone Monster04 Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CMonster04::Free()
{
	__super::Free();
	
}