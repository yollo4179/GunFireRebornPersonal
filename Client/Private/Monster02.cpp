#include"stdafx.h"
#include "Monster02.h"
#include "GameInstance.h"
#include"Monster02_Sword.h"
#include"Monster02_Head.h"
#include"Monster02_Body.h"
CMonster02::CMonster02(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CMonster{ pDevice, pContext }
{
}

CMonster02::CMonster02(const CMonster02& Prototype)
	:CMonster(Prototype)
{
}

HRESULT CMonster02::Initialize_Prototype()
{


	return S_OK;
}


HRESULT CMonster02::Initialize(void* pArg)
{
	
	MONSTER_DESC* pDesc = static_cast<MONSTER_DESC*>(pArg);
	pDesc->iNumPartObject = PART_END;
	
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	
	__super::m_HPInfo = { 3000,3000,3000,3000 };
	if (FAILED(Ready_Components()))
		return E_FAIL;
	/*For UI-HP_Bar*/
	XMStoreFloat4x4(&m_HPBarSocketMatrix,XMMatrixIdentity());//Get_BoneMatrix("MonsterHp");
	m_HPBarSocketMatrix._42 = 2.4f;

	if (FAILED(__super::Ready_UI_HPBar_LEVEL_GAMEPLAY()))
		return E_FAIL;


	m_pModelCom->SetUp_Animation(6, true);

	if (FAILED(Mapping_Anim_States()))
		return E_FAIL;

	m_iState = M2_STATE::M2_STATE_FAR_IDLE;
	
	if (FAILED(Ready_PartObjects()))
		return E_FAIL;



	return S_OK;
}

void CMonster02::Priority_Update(_float fTimeDelta)
{
	
	if (false == m_bActive)
		return;
	

	if (0 == m_iState)
		int a = 0;

	Pri_Update_LoopCall_State();
	Pri_Update_Non_LoopCall_State();

	/*m_fAccTime*/
	{
		/*쿨타임*/
		if(!CONDITION_DOUBLE_SLASH)
			m_fAcc_DoubleSlash_Time += fTimeDelta;

		if (!CONDITION_RUSH_SLASH)
			m_fAcc_Rush_Slash_Time += fTimeDelta;
		
		if (!CONDITION_SLASH)
			m_fAcc_Slash_Time += fTimeDelta;

		/*실행중일때 올라야 하는것들 */
		if(CONDITION_ROLL)
			m_fAcc_StartRollTime += fTimeDelta;
		if (CONDITION_RUSH_SLASH)
			m_fAcc_StartRushSlashTime += fTimeDelta;

	}
		
	
	if ((!m_isAnimEnd) && (CONDITION_NON_LOOP)  )
	{

		if (CONDITION_RUSH_SLASH)
		{
			m_fAccRushSlashTiming += fTimeDelta;
		}
		if (CONDITION_DOUBLE_SLASH)
		{
			m_fAccDoubleSlashTiming += fTimeDelta;
		}
		
		//지금 애님이 않끝났고  애님이 돌고 있으면 공격중인경우  거리,시간 에의한 상태변화를 막는다
	}
	
	
	else if (m_isAnimEnd && CONDITION_NON_LOOP ) {
		/* 아직 애니메이션이끝나지않았을때 조건을 더해줌*/
		
		
		if (CONDITION_ROLL_LEFT)
		{
			m_iState ^= M2_STATE::M2_STATE_ROLL_LEFT;
			m_fAcc_StartRollTime = 0.f;
			m_iState = M2_STATE::M2_STATE_BATTLE_IDLE;
			m_fAcc_Battle_Idle_Time = 0.f;
		}
		else if (CONDITION_ROLL_RIGHT)
		{
			m_iState ^= M2_STATE::M2_STATE_ROLL_RIGHT;
			m_fAcc_StartRollTime = 0.f;
			m_iState = M2_STATE::M2_STATE_BATTLE_IDLE;
			m_fAcc_Battle_Idle_Time = 0.f;
		}
		
		
		else if (CONDITION_SLASH) {
			m_iState ^= M2_STATE::M2_STATE_BATTLE_SLASH;
			m_fAcc_Slash_Time = 0.f;

			m_iState = M2_STATE::M2_STATE_BATTLE_IDLE;
			m_fAcc_Battle_Idle_Time = 0.f;
			
		}
		else if (CONDITION_DOUBLE_SLASH) {
			m_iState ^= M2_STATE::M2_STATE_BATTLE_DOUBLE_SLASH;
			m_fAcc_DoubleSlash_Time = 0.f;

			m_iState = M2_STATE::M2_STATE_BATTLE_IDLE;
			m_fAcc_Battle_Idle_Time =0.f;

			m_fAccDoubleSlashTiming = 0.f;
			m_bDoubleSlashSound = true;
		}

		else if (CONDITION_RUSH_SLASH) {
			m_iState ^= M2_STATE::M2_STATE_RUSH_SLASH;
			m_fAcc_Rush_Slash_Time = 0.f;
			m_fAcc_StartRushSlashTime = 0.f;
			m_iState = M2_STATE::M2_STATE_BATTLE_IDLE;
			m_fAcc_Battle_Idle_Time = 0.f;

			m_fAccRushSlashTiming = 0.f;
			m_bRushSlashSound = true;

		}
		/*else if (CONDITION_HIT)
		{
			m_iState ^= M2_STATE::M2_STATE_HIT;
			m_iState = M2_STATE::M2_STATE_BATTLE_IDLE;
			m_fAcc_Battle_Idle_Time = 0.f;
		}*/
		

	}
	/*그다음  공간에 의한 상태변화*/
	else {


			/* loop이지만 사실상 시간이 정해져있는 것들 */

		if (0.5f >= m_fAcc_Battle_Idle_Time && CONDITION_BATTLE_IDLE) //고정으로 1초는 가져감
		{
			m_fAcc_Battle_Idle_Time += fTimeDelta;
		}
		else if (m_fAcc_Random_WalkTime > m_fAcc_Far_Walk_Left_Time && CONDITION_WALK_LEFT)
		{
			m_fAcc_Far_Walk_Left_Time += fTimeDelta;
		}
		else if (m_fAcc_Random_WalkTime > m_fAcc_Far_Walk_Right_Time && CONDITION_WALK_RIGHT)
		{
			m_fAcc_Far_Walk_Right_Time += fTimeDelta;
		}
		else if (m_fAcc_Random_WalkTime > m_fAcc_Far_Walk_Time && CONDITION_WALK)
		{
			m_fAcc_Far_Walk_Time += fTimeDelta;
		}
		else if (m_fAcc_Random_WalkTime > m_fAcc_Battle_Walk_Back_Time && CONDITION_WALK_BACK)
		{
			m_fAcc_Battle_Walk_Back_Time += fTimeDelta;
		}
		else if (CONDITION_RUN && (!__super::Util_isPlayer_InRange(m_fBattleRange)))
		{
			/*m_fAcc_Middle_Run_Time += fTimeDelta;*/
		}
		

		else {
			
			/*상태 해제*/
			if (0.5f <= m_fAcc_Battle_Idle_Time && CONDITION_BATTLE_IDLE)
			{
				m_fAcc_Battle_Idle_Time = 0.f;
				m_iState ^= M2_STATE::M2_STATE_BATTLE_IDLE;
				m_iState = M2_STATE::M2_STATE_LOOP;
			}
			else if (m_fAcc_Random_WalkTime < m_fAcc_Far_Walk_Left_Time && CONDITION_WALK_LEFT)
			{
				m_fAcc_Far_Walk_Left_Time = 0.f;
				m_iState ^= M2_STATE::M2_STATE_FAR_WALK_LEFT;
				m_iState = M2_STATE::M2_STATE_LOOP;
			}
			else if (m_fAcc_Random_WalkTime < m_fAcc_Far_Walk_Right_Time && CONDITION_WALK_RIGHT)
			{
				m_fAcc_Far_Walk_Right_Time = 0.F;
				m_iState ^= M2_STATE::M2_STATE_FAR_WALK_RIGHT;
				m_iState = M2_STATE::M2_STATE_LOOP;
			}
			else if (m_fAcc_Random_WalkTime < m_fAcc_Far_Walk_Time && CONDITION_WALK)
			{
				m_fAcc_Far_Walk_Time = 0.F;
				m_iState ^= M2_STATE::M2_STATE_FAR_WALK;
				m_iState = M2_STATE::M2_STATE_LOOP;
			}
			else if ( CONDITION_RUN && (__super::Util_isPlayer_InRange(m_fBattleRange)))
			{
				m_iState ^= M2_STATE::M2_STATE_MIDDLE_RUSH;
				m_iState = M2_STATE::M2_STATE_LOOP;
			}
			else if (m_fAcc_Random_WalkTime < m_fAcc_Battle_Walk_Back_Time && CONDITION_WALK_BACK)
			{
				m_fAcc_Battle_Walk_Back_Time = 0.F;
				m_iState ^= M2_STATE::M2_STATE_BATTLE_WALK_BACK;
				m_iState = M2_STATE::M2_STATE_LOOP;
			}
			
			if (CONDITION_BATTLE_IDLE)
			{

			}
			else if (CONDITION_WALK_LEFT)
			{

			}
			else if (CONDITION_WALK_RIGHT)
			{

			}
			else if (CONDITION_WALK)
			{

			}
			else if (CONDITION_WALK_BACK)
			{

			}
			else if (CONDITION_RUN)
			{

			}


			else
			{

				if (__super::Util_isPlayer_InRange(m_fBattleRange))
				{
					_float fChance = m_pGameInstance->Compute_Random(0.f, 10.f);

					if (fChance < 9.f && (m_isAnimEnd || CONDITION_LOOP))
					{
						m_iState = M2_STATE::M2_STATE_BATTLE_WALK_BACK;
					}
					else if (fChance <= 9.5f && (m_isAnimEnd || CONDITION_LOOP))
					{
						m_iState = M2_STATE::M2_STATE_ROLL_LEFT;
					}
					else if (fChance <= 10.f && (m_isAnimEnd || CONDITION_LOOP))
					{
						m_iState = M2_STATE::M2_STATE_ROLL_RIGHT;
					}
					fChance = m_pGameInstance->Compute_Random(0.5f, 0.5f);
					m_fAcc_Random_WalkTime = fChance;

				}

				else if (__super::Util_isPlayer_InRange(m_fMiddleRange))
				{
					_float fChance = m_pGameInstance->Compute_Random(0.f, 10.f);

					if (fChance <= 9.f && (m_isAnimEnd || CONDITION_LOOP))
					{
						m_iState = M2_STATE::M2_STATE_MIDDLE_RUSH;
					}
					else if (fChance <= 9.5f && (m_isAnimEnd || CONDITION_LOOP))
					{
						m_iState = M2_STATE::M2_STATE_ROLL_LEFT;
					}
					else if (fChance <= 10.f && (m_isAnimEnd || CONDITION_LOOP))
					{
						m_iState = M2_STATE::M2_STATE_ROLL_RIGHT;
					}
					fChance = m_pGameInstance->Compute_Random(0.5f, 0.5f);
					m_fAcc_Random_WalkTime = fChance;
				}
				
				else if (__super::Util_isPlayer_InRange(m_fFarRange))
				{
					_float fChance = m_pGameInstance->Compute_Random(0.f, 10.f);

					if (fChance <= 8.5f && (m_isAnimEnd || CONDITION_LOOP))
					{
						m_iState = M2_STATE::M2_STATE_FAR_WALK;
					}
					else if (fChance <= 9.25f && (m_isAnimEnd || CONDITION_LOOP))
					{
						m_iState = M2_STATE::M2_STATE_FAR_WALK_RIGHT;
					}
					else if (fChance <= 10.f && (m_isAnimEnd || CONDITION_LOOP))
					{
						m_iState = M2_STATE::M2_STATE_FAR_WALK_LEFT;
					}
					fChance = m_pGameInstance->Compute_Random(0.5f, 1.5f);
					m_fAcc_Random_WalkTime = fChance;
				}
				else
				{
					if (m_isAnimEnd || CONDITION_LOOP)
						m_iState = M2_STATE::M2_STATE_FAR_IDLE;
				}
				
				
					

					{
						
						
						/*Slash*/
						if ((__super::Util_isPlayer_InRange(m_fRush_SlashRange))&& (!__super::Util_isPlayer_InRange(m_fBattleRange+1.f)) && (m_isAnimEnd || CONDITION_LOOP))
						{
							m_iState |= M2_STATE::M2_STATE_RUSH_SLASH;

						}
						else if (2.f <= m_fAcc_DoubleSlash_Time && (m_isAnimEnd || CONDITION_LOOP) && (__super::Util_isPlayer_InRange(m_fBattleRange)))
						{
							m_iState |= M2_STATE::M2_STATE_BATTLE_DOUBLE_SLASH;

						}
						else if (2.f <= m_fAcc_Slash_Time && (m_isAnimEnd || CONDITION_LOOP) && __super::Util_isPlayer_InRange(m_fBattleRange))
						{
							m_iState |= M2_STATE::M2_STATE_BATTLE_SLASH;
						}
						
					}
					
			}



		}
			
	}

	
	/*<------------------------ATTACK--------------------------------->*/

	


	

	/* ANIM STATE*/
	
	
 
 	
	if (2.f <= m_fAcc_DoubleSlash_Time&&CONDITION_DOUBLE_SLASH)
	{
		if (CONDITION_LOOP|| m_isAnimEnd)
		{
			m_pGameInstance->PlayEffect(L"Monster02_Double_Slash.wav", 0.08f);
			m_pModelCom->Move_NextAnim(m_vecAnimStates[(_uint)M2_ANIM::M2_ANIM_BATTLE_DOUBLE_SLASH], 0.2f,false);
			Pri_Update_Cancle_LoopState();
			m_fAcc_DoubleSlash_Time = 0.f;
			
		}
	
	}
	else if (2.f <= m_fAcc_Slash_Time && CONDITION_SLASH)
	{
		if (CONDITION_LOOP || m_isAnimEnd)
		{
			m_pGameInstance->PlayEffect(L"Sword.ogg", 0.08f);
			m_pModelCom->Move_NextAnim(m_vecAnimStates[(_uint)M2_ANIM::M2_ANIM_BATTLE_SLASH], 0.2f, false);
			Pri_Update_Cancle_LoopState();
			m_fAcc_Slash_Time = 0.f;

			
		}

	}
	else if (CONDITION_RUSH_SLASH)
	{
		if (CONDITION_LOOP||m_isAnimEnd )
		{
			m_pModelCom->Move_NextAnim(m_vecAnimStates[(_uint)M2_ANIM::M2_ANIM_RUSH_SLASH],0.1F,false);
			m_fAcc_Rush_Slash_Time = 0.f;
			Pri_Update_Cancle_LoopState();
		
		}
		
	}
	
	
	
	else if (CONDITION_WALK)
	{
		if (m_bCall_Walk_Slow) {
			m_pModelCom->Move_NextAnim(m_vecAnimStates[(_uint)M2_ANIM::M2_ANIM_FAR_WALK], 0.2f, true); //루프
			m_bCall_Walk_Slow = false;
		}
	}
	else if (CONDITION_WALK_LEFT)
	{
		if (m_bCall_Walk_Left) {
			m_pModelCom->Move_NextAnim(m_vecAnimStates[(_uint)M2_ANIM::M2_ANIM_FAR_WALK_LEFT], 0.2f, true); //루프
			m_bCall_Walk_Left = false;
		}
	}

	else if (CONDITION_WALK_RIGHT)
	{
		if (m_bCall_Walk_Right) {
			m_pModelCom->Move_NextAnim(m_vecAnimStates[(_uint)M2_ANIM::M2_ANIM_FAR_WALK_RIGHT], 0.2f, true); //루프
			m_bCall_Walk_Right = false;
		}
	}



	else if (CONDITION_RUN)
	{
		if ( m_bCall_Rush)
		{
			m_pModelCom->Move_NextAnim(m_vecAnimStates[(_uint)M2_ANIM::M2_ANIM_MIDDLE_RUSH], 0.2f, true);
			m_bCall_Rush = false;
		}
	}
	else if (CONDITION_FAR_IDLE)
	{
		if ( m_bCall_Far_Idle) 
		{
			m_pModelCom->Move_NextAnim(m_vecAnimStates[(_uint)M2_ANIM::M2_ANIM_FAR_IDLE], 0.2f, true);
			m_bCall_Far_Idle = false;
		}
	}

	else if (CONDITION_BATTLE_IDLE)
	{
		if (m_bCall_Battle_Idle)
		{
			m_pModelCom->Move_NextAnim(m_vecAnimStates[(_uint)M2_ANIM::M2_ANIM_BATTLE_IDLE], 0.5f, true);
			m_bCall_Battle_Idle = false;
		}
	}

	else if (CONDITION_WALK_BACK)
	{
		if (m_bCall_Walk_Back)
		{
			m_pModelCom->Move_NextAnim(m_vecAnimStates[(_uint)M2_ANIM::M2_ANIM_BATTLE_WALK_BACK], 0.2f, true);
			m_bCall_Walk_Back = false;
		}
	}

	else if (CONDITION_ROLL_LEFT)
	{
		if (m_bCall_Roll_Left)
		{
			m_pModelCom->Move_NextAnim(m_vecAnimStates[(_uint)M2_ANIM::M2_ANIM_ROLL_LEFT], 0.2f, false);
			m_bCall_Roll_Left = false;
		}

	}
	else if (CONDITION_ROLL_RIGHT)
	{
		if (m_bCall_Roll_Right)
		{
			m_pModelCom->Move_NextAnim(m_vecAnimStates[(_uint)M2_ANIM::M2_ANIM_ROLL_RIGHT], 0.2f, false);
			m_bCall_Roll_Right = false;
		}

	}
	else if (CONDITION_HIT)
	{
		if (m_bCall_Hit)
		{
			m_pModelCom->Move_NextAnim(m_vecAnimStates[(_uint)M2_ANIM::M2_ANIM_HIT], 0.2f, false);
			m_bCall_Hit = false;
		}
		}

	/*<-----------------------------------Animation-----------------------------------> */

	__super::Pri_Update_NaviMesh();
	/*Logic depending on anim state*/
	__super::Priority_Update(fTimeDelta);
	if (true == m_isRemoving)
		return;


	bool debug = false;
	if (Util_isPlayer_InRange(m_fFarRange))
		Util_TurnTo_Player(fTimeDelta*2.f);
	

	if (m_fAcc_StartRollTime > 0.15f && m_fAcc_StartRollTime < 0.30f && CONDITION_ROLL_RIGHT) 
	{
		
		Util_Go_Right(fTimeDelta, 1.f);
	}

	else if (m_fAcc_StartRollTime > 0.15f &&  m_fAcc_StartRollTime < 0.30&&CONDITION_ROLL_LEFT)
		Util_Go_Left(fTimeDelta, 1.f);

	
	else if (CONDITION_RUSH_SLASH&& m_fAcc_StartRushSlashTime<0.3f)
	{
		Util_Go_Straight(fTimeDelta, 1.f);
	}
	else if (CONDITION_WALK) {
		Util_Go_Straight(fTimeDelta, 0.2f);
		debug = true;
	}
	else if (CONDITION_WALK_LEFT)
	{
		Util_Go_Left(fTimeDelta, 0.2f);
	}
	else if (CONDITION_WALK_RIGHT)
	{
		Util_Go_Right(fTimeDelta, 0.2f);
	}
	else if (CONDITION_RUN)
	{
		Util_Go_Straight(fTimeDelta, 0.6f);
	}
	

	else if (CONDITION_WALK_BACK)
		Util_Go_Back(fTimeDelta, 0.3f);


	if (debug && CONDITION_BATTLE_IDLE)
		int a = 0;


	/*Sound*/
	if (true == m_bRushSlashSound && 0.1f <= m_fAccRushSlashTiming)
	{
		m_bRushSlashSound = false;
		m_pGameInstance->PlayEffect(L"Rush_Slash.mp3",0.08f);
	}
	if (true == m_bDoubleSlashSound && 0.1f <= m_fAccDoubleSlashTiming)
	{
		m_bDoubleSlashSound = false;
		m_pGameInstance->PlayEffect(L"Monster02_Double_Slash.wav",0.08f);
	}
}

void CMonster02::Update(_float fTimeDelta)
{

	if (false == m_bActive)
		return;
	if (true == m_isRemoving)
		return;

	m_isAnimEnd = m_pModelCom->Play_Animation(fTimeDelta, m_isChangingMotion);

	/*ChangeState When Condition is All right*/
	__super::Update(fTimeDelta);


}

void CMonster02::Late_Update(_float fTimeDelta)
{
	if (false == m_bActive)
		return;
	if (true == m_isRemoving)
	{
		m_pGameInstance->Add_RenderObject(CRenderer::RG_GLOW, this);
	}
	
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
	__super::Late_Update(fTimeDelta);
	
}

HRESULT CMonster02::Render()
{
	if (false == m_bActive)
		return S_OK;
	
	__super::Render();
	
	return S_OK;
}

HRESULT CMonster02::Mapping_Anim_States()
{

	m_vecAnimStates.resize((_uint)M2_ANIM::M2_ANIM_END);
	m_vecAnimStates[(_uint)M2_ANIM::M2_ANIM_BATTLE_IDLE] = 0;
	m_vecAnimStates[(_uint)M2_ANIM::M2_ANIM_FAR_WALK_RIGHT] = 2;
	m_vecAnimStates[(_uint)M2_ANIM::M2_ANIM_BATTLE_WALK_BACK] = 3;
	m_vecAnimStates[(_uint)M2_ANIM::M2_ANIM_FAR_WALK_LEFT] = 4;
	m_vecAnimStates[(_uint)M2_ANIM::M2_ANIM_FAR_WALK] = 5;
	m_vecAnimStates[(_uint)M2_ANIM::M2_ANIM_FAR_IDLE] = 6;
	m_vecAnimStates[(_uint)M2_ANIM::M2_ANIM_HIT] = 7;
	m_vecAnimStates[(_uint)M2_ANIM::M2_ANIM_MIDDLE_RUSH] = 8;
	m_vecAnimStates[(_uint)M2_ANIM::M2_ANIM_ROLL_RIGHT] =	11;
	m_vecAnimStates[(_uint)M2_ANIM::M2_ANIM_ROLL_LEFT] = 12;
	m_vecAnimStates[(_uint)M2_ANIM::M2_ANIM_BATTLE_DOUBLE_SLASH] = 18;
	m_vecAnimStates[(_uint)M2_ANIM::M2_ANIM_BATTLE_SLASH] = 15;
	m_vecAnimStates[(_uint)M2_ANIM::M2_ANIM_RUSH_SLASH] = 14;
	




	return S_OK;
}


void CMonster02:: Pri_Update_Cancle_LoopState()
{

	if (CONDITION_BATTLE_IDLE)
		m_iState ^= M2_STATE::M2_STATE_BATTLE_IDLE;
	

	if (CONDITION_FAR_IDLE)
		m_iState ^= M2_STATE::M2_STATE_FAR_IDLE;
		
	if (CONDITION_WALK_LEFT)
		m_iState ^= M2_STATE::M2_STATE_FAR_WALK_LEFT;
	if (CONDITION_WALK_RIGHT)
		m_iState ^= M2_STATE::M2_STATE_FAR_WALK_RIGHT;
	if (CONDITION_WALK_BACK)
		m_iState ^= M2_STATE::M2_STATE_BATTLE_WALK_BACK;
	
	if (CONDITION_WALK)
		m_iState ^= M2_STATE::M2_STATE_FAR_WALK;
	
	if (CONDITION_RUN)
		m_iState ^= M2_STATE::M2_STATE_MIDDLE_RUSH;

	return ;
}

void CMonster02::Pri_Update_LoopCall_State()
{

	

	if (!CONDITION_WALK && false == m_bCall_Walk_Slow)
		m_bCall_Walk_Slow = true;


	if (!CONDITION_WALK_LEFT && false == m_bCall_Walk_Left)
		m_bCall_Walk_Left = true;

	if (!CONDITION_WALK_RIGHT && false == m_bCall_Walk_Right)
		m_bCall_Walk_Right = true;

	if (!CONDITION_RUN && false == m_bCall_Rush)
		m_bCall_Rush = true;

	if (!CONDITION_WALK_BACK && false == m_bCall_Walk_Back)
		m_bCall_Walk_Back = true;

	if (!CONDITION_BATTLE_IDLE && false == m_bCall_Battle_Idle)
		m_bCall_Battle_Idle = true;

	if (!CONDITION_FAR_IDLE && false == m_bCall_Far_Idle)
		m_bCall_Far_Idle = true;

	if (!CONDITION_HIT && false == m_bCall_Hit)
		m_bCall_Hit = true;



}

void CMonster02::Pri_Update_Non_LoopCall_State()
{
	if (!CONDITION_ROLL_RIGHT && false == m_bCall_Roll_Right)
		m_bCall_Roll_Right = true;

	if (!CONDITION_ROLL_LEFT && false == m_bCall_Roll_Left)
		m_bCall_Roll_Left = true;

}



HRESULT CMonster02::Ready_Components()
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

	/* Com_Model */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Monster02"),
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

HRESULT CMonster02::Bind_ShaderResources()
{
	__super::Bind_ShaderResources();

	return S_OK;
}

HRESULT CMonster02::Ready_PartObjects()
{

	CMonster02_Sword::MONSTER02_SWORD_DESC Sword_Desc = {};
	Sword_Desc.fSpeedPerSec = 10.f;
	Sword_Desc.fRotationPerSec = XMConvertToRadians(90.f);
	Sword_Desc.pState = &m_iState;
	Sword_Desc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4_Ptr();
	Sword_Desc.pSocketMatrix[0] = Get_BoneMatrix("Bone002");
	m_PartObjects[PART_SWORD]= static_cast<CPartObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster02_Sword"), &Sword_Desc));
	if (nullptr == m_PartObjects[PART_SWORD])
		return E_FAIL;
	

	

	CMonster02_Body::MONSTER02_BODY_DESC Body_Desc = {};
	Body_Desc.fRotationPerSec = XMConvertToRadians(90.f);
	Body_Desc.fSpeedPerSec = 10.f;
	Body_Desc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4_Ptr();
	Body_Desc.pSocketMatrix = Get_BoneMatrix("Bip001 Spine");
	Body_Desc.pisMonsterPicked = &m_isMonsterPicked;
	Body_Desc.pHitTime = &m_fHitTime;
	Body_Desc.pAccHitTime= &m_fAccHitTime;
	Body_Desc.pHPInfo=& m_HPInfo;
	Body_Desc.pHit_NormalMonster = &m_bHit_NormalMonster;
	m_PartObjects[PART_BODY]=static_cast<CPartObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster02_Body"), &Body_Desc));
	if (nullptr == m_PartObjects[PART_BODY])
		return E_FAIL;

	CMonster02_Head::MONSTER02_HEAD_DESC Head_Desc = {};
	Head_Desc.fRotationPerSec = XMConvertToRadians(90.f);
	Head_Desc.fSpeedPerSec = 10.f;
	Head_Desc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4_Ptr();
	Head_Desc.pSocketMatrix = Get_BoneMatrix("Bip001 Head");
	Head_Desc.pisMonsterPicked= &m_isMonsterPicked;
	Head_Desc.pHitTime = &m_fHitTime;
	Head_Desc.pAccHitTime = &m_fAccHitTime;
	Head_Desc.pHPInfo = &m_HPInfo;
	Head_Desc.pHit_NormalMonster = &m_bHit_NormalMonster;
	/*Head_Desc.pHeadShot = &m_bHeadShot;*/
	m_PartObjects[PART_HEAD] = static_cast<CPartObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster02_Head"), &Head_Desc));
	if (nullptr == m_PartObjects[PART_HEAD])
		return E_FAIL;


	return S_OK;
}

CMonster02* CMonster02::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMonster02* pInstance = new CMonster02(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Created Monster02_ Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CMonster02::Clone(void* pArg)
{
	CMonster02* pInstance = new CMonster02(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Clone Monster02 Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CMonster02::Free()
{
	__super::Free();
	
}