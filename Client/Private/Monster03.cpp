#include"stdafx.h"
#include "Monster03.h"
#include "GameInstance.h"
#include"Monster03_Head.h"
#include"Monster03_Body.h"
#include"Monster03_Gun.h"

CMonster03::CMonster03(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CMonster{ pDevice, pContext }
{
}

CMonster03::CMonster03(const CMonster03& Prototype)
	:CMonster(Prototype)
{
}

HRESULT CMonster03::Initialize_Prototype()
{


	return S_OK;
}


HRESULT CMonster03::Initialize(void* pArg)
{
	MONSTER_DESC* pDesc = static_cast<MONSTER_DESC*>(pArg);
	pDesc->iNumPartObject = PART_END;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;



	__super::m_HPInfo = { 3000,3000,4000,4000 };
	if (FAILED(Ready_Components()))
		return E_FAIL;
	/*For UI-HP_Bar*/
	XMStoreFloat4x4(&m_HPBarSocketMatrix, XMMatrixIdentity());//Get_BoneMatrix("MonsterHp");
	m_HPBarSocketMatrix._42 = 2.6;
	if (FAILED(__super::Ready_UI_HPBar_LEVEL_GAMEPLAY()))
		return E_FAIL;

	m_pModelCom->SetUp_Animation(4, true);

	if (FAILED(Mapping_Anim_States()))
		return E_FAIL;
	m_iState = M3_STATE::M3_STATE_LOOP;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	return S_OK;
}

void CMonster03::Priority_Update(_float fTimeDelta)
{
	if (false == m_bActive)
		return;

	
	Pri_Update_LoopCall_State();
	PriUpdate_Non_LoopCall_State();

	/*m_fAccTime*/
	{
	/*Non Loop -Attack*/

		


		if(!CONDITION_FAR_SNIPE)
			m_fAcc_Snipe_Time += fTimeDelta;
		
		/*연속공격은 쌍으로 관리*/
		if(!CONDITION_CLOSE_MULTISHOT)
			m_fAcc_MultiShot_Time += fTimeDelta;//쿨타임
		if (CONDITION_CLOSE_MULTISHOT)
		{
			m_fAcc_Multishot_TrackPosition += fTimeDelta;//지속시간
		}
		/*실행중일때 올라야 하는것들 */
		if (CONDITION_ROLL)
			m_fAcc_StartRollTime += fTimeDelta;
	}


	/*루프가 아닌모션 우선 처리*/
	if ((!m_isAnimEnd) && (CONDITION_NON_LOOP))
	{
		if (CONDITION_CLOSE_MULTISHOT) {
			m_fAcc_MultiShot_Timing += fTimeDelta;
			if (1 <= m_iMultiShot_Count)
			{
				m_bMultiShotSign = false;
			}

			else if (0==m_iMultiShot_Count && m_fAcc_MultiShot_Timing >= 0.45f)
			{
				m_iMultiShot_Count = 1;
				m_bMultiShotSign = true;
				m_fAcc_MultiShot_Timing = 0.f;
			}
		}
	}
	else if (m_isAnimEnd && CONDITION_NON_LOOP)
	{
		if (CONDITION_CLOSE_ROLL_LEFT)
		{
			m_iState ^= M3_STATE::M3_STATE_CLOSE_ROLL_LEFT;
			m_fAcc_StartRollTime = 0.f;
			m_iState = M3_STATE::M3_STATE_CLOSE_IDLE;
			m_fAcc_Close_Idle_Time = 0.f;
		}
		else if (CONDITION_CLOSE_ROLL_RIGHT)
		{
			m_iState ^= M3_STATE::M3_STATE_CLOSE_ROLL_RIGHT;
			m_fAcc_StartRollTime = 0.f;
			m_iState =M3_STATE::M3_STATE_CLOSE_IDLE;
			m_fAcc_Close_Idle_Time = 0.f;
		}
		else if (CONDITION_LOAD)
		{
			m_iState ^= M3_STATE::M3_STATE_LOAD;
			m_iNumBullets = 3;

			m_iState = M3_STATE::M3_STATE_CLOSE_IDLE;
			m_fAcc_Close_Idle_Time = 0.f;
			
		}
		else if (CONDITION_FAR_SNIPE)
		{
			m_iState ^= M3_STATE::M3_STATE_FAR_SNIPE;
			m_fAcc_Snipe_Time = 0.F;
			--m_iNumBullets;

			m_iState = M3_STATE::M3_STATE_CLOSE_IDLE;
			m_fAcc_Close_Idle_Time = 0.f;
			
		}
		else if (CONDITION_CLOSE_MULTISHOT)
		{
			m_iMultiShot_Count = 0;
			m_fAcc_MultiShot_Timing = 0;
			/*조건이 만족하지 않으면 애니메이션을 끝내는게 아니라 반복해서 수행한다.*/
			/* 애니메이션은 m_bCall이 true이거나 이전 애니메이션이 끝났을 때만 재생된다*/
			if (m_fAcc_Multishot_TrackPosition >= m_fAcc_Multishot_Duration)
			{
				m_iState ^= M3_STATE::M3_STATE_CLOSE_MULTISHOT;
				m_fAcc_MultiShot_Time = 0.f;//쿨타임
				m_fAcc_Multishot_TrackPosition = 0.f;
				m_bCall_Close_MultiShot = true;
				m_iNumBullets = 0;

				m_iState = M3_STATE::M3_STATE_CLOSE_IDLE;
				m_fAcc_Close_Idle_Time = 0.f;
			}
		}

		else if (CONDITION_HIT)
		{
			m_iState ^= M3_STATE::M3_STATE_HIT;
			m_bHeadShot = false;
			/*끊으면 영향을 주는것들 */
			/*멀티샷*/
			m_fAcc_MultiShot_Time = 0.f;//쿨타임
			m_fAcc_Multishot_TrackPosition = 0.f;
			m_bCall_Close_MultiShot = true;
			/*Roll*/
			m_fAcc_StartRollTime = 0.f;


		}
	}
	/*공간에 대한 상태변화(루프인 모션 처리)*/
	else
	{


		if (0.5f > m_fAcc_Close_Idle_Time && CONDITION_CLOSE_IDLE)
		{
			m_fAcc_Close_Idle_Time += fTimeDelta;
		}
		//loop이지만 시간이 정해져 있는것들
		if (m_fAcc_Random_WalkTime >= m_fAcc_Far_Walk_Left_Time && CONDITION_FAR_WALK_LEFT)
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

		else if (m_fAcc_Random_WalkTime >= m_fAcc_Close_Run_Time && CONDITION_CLOSE_RUN)
		{
			m_fAcc_Close_Run_Time += fTimeDelta;
		}
		else if ( CONDITION_CLOSE_WALK_BACK&& m_fAcc_Random_WalkTime >= m_fAcc_Close_Walk_Back_Time )
		{
			m_fAcc_Close_Walk_Back_Time += fTimeDelta;
		}

		/* [상태 해제 or상태 유지] or  상태 업데이트 */
		else
		{
			/* 시간이 지났다면 상태해제*/
			/* far 제한시간 루프*/
			if (m_fAcc_Random_WalkTime < m_fAcc_Far_Walk_Left_Time && CONDITION_FAR_WALK_LEFT)
			{
				m_fAcc_Far_Walk_Left_Time = 0.f;
				m_iState ^= M3_STATE::M3_STATE_FAR_WALK_LEFT;
				m_iState = M3_STATE::M3_STATE_LOOP;
			}
					
			else if (m_fAcc_Random_WalkTime < m_fAcc_Far_Walk_Right_Time && CONDITION_FAR_WALK_RIGHT)
			{
				m_fAcc_Far_Walk_Right_Time = 0.f;
				m_iState ^= M3_STATE::M3_STATE_FAR_WALK_RIGHT;
				m_iState = M3_STATE::M3_STATE_LOOP;
			}
			else if (m_fAcc_Random_WalkTime < m_fAcc_Far_Walk_Time && CONDITION_FAR_WALK)
			{
				m_fAcc_Far_Walk_Time = 0.f;
				m_iState ^= M3_STATE::M3_STATE_FAR_WALK;
				m_iState = M3_STATE::M3_STATE_LOOP;
			}
			/*close 제한시간 루프*/
			else if (m_fAcc_Random_WalkTime < m_fAcc_Close_Run_Time && CONDITION_CLOSE_RUN)
			{
				m_fAcc_Close_Run_Time = 0.f;
				m_iState ^= M3_STATE::M3_STATE_CLOSE_RUN;
				m_iState = M3_STATE::M3_STATE_LOOP;
			}
			else if ( CONDITION_CLOSE_WALK_BACK &&m_fAcc_Random_WalkTime < m_fAcc_Close_Walk_Back_Time)
			{
				m_fAcc_Close_Walk_Back_Time = 0.f;
				m_iState ^= M3_STATE::M3_STATE_CLOSE_WALK_BACK;
				m_iState = M3_STATE::M3_STATE_LOOP;
			}
			else if (m_fAcc_Close_Idle_Time >= 0.5f && CONDITION_CLOSE_IDLE)
			{
				m_fAcc_Close_Idle_Time = 0.f;
				m_iState ^= M3_STATE::M3_STATE_CLOSE_IDLE;
				m_iState = M3_STATE::M3_STATE_LOOP;
			}



		}

		if (CONDITION_FAR_WALK_LEFT)
		{

		}
		else if (CONDITION_FAR_WALK_RIGHT)
		{

		}
		else if (CONDITION_FAR_WALK)
		{

		}
		else if (CONDITION_CLOSE_WALK_BACK)
		{

		}
		else if(CONDITION_CLOSE_RUN)
		{
		
		}
		else if (CONDITION_CLOSE_IDLE)
		{

		}
		else {



			if (__super::Util_isPlayer_InRange(m_fMultiShotRange))
			{
				_float fChance = m_pGameInstance->Compute_Random(0.f, 10.f);


				if (fChance <= 8.f && (m_isAnimEnd || CONDITION_LOOP || CONDITION_ROLL))
				{
					m_iState = M3_STATE::M3_STATE_CLOSE_WALK_BACK;
				}
				else if (fChance <= 9.375f && (m_isAnimEnd || CONDITION_LOOP || CONDITION_ROLL))
				{
					m_iState = M3_STATE::M3_STATE_CLOSE_ROLL_LEFT;
				}
				else if (fChance <= 10.f && (m_isAnimEnd || CONDITION_LOOP || CONDITION_ROLL))
				{
					m_iState = M3_STATE::M3_STATE_CLOSE_ROLL_RIGHT;
				}
				fChance = m_pGameInstance->Compute_Random(2.f, 2.f);
				m_fAcc_Random_WalkTime = fChance;
			}

			else if (__super::Util_isPlayer_InRange(m_fCloseRange))
			{
				_float fChance = m_pGameInstance->Compute_Random(0.f, 10.f);
				if (fChance <= 8.75f && (m_isAnimEnd || CONDITION_LOOP))
				{
					m_iState = M3_STATE::M3_STATE_CLOSE_RUN;
				}
				/*끝이 있는 애니메이션*/
				else if (fChance <= 9.375f && (m_isAnimEnd || CONDITION_LOOP))
				{
					m_iState = M3_STATE::M3_STATE_CLOSE_ROLL_LEFT;
				}
				else if (fChance <= 10.f && (m_isAnimEnd || CONDITION_LOOP))
				{
					m_iState = M3_STATE::M3_STATE_CLOSE_ROLL_RIGHT;
				}
				fChance = m_pGameInstance->Compute_Random(0.1f, 0.1f);
				m_fAcc_Random_WalkTime = fChance;
			}

			else if (__super::Util_isPlayer_InRange(m_fFarRange))
			{
				_float fChance = m_pGameInstance->Compute_Random(0.f, 10.f);



				if (fChance <= 7.f && (m_isAnimEnd || CONDITION_LOOP))
					m_iState = M3_STATE::M3_STATE_FAR_WALK;

				else if (fChance <= 8.5f &&( m_isAnimEnd || CONDITION_LOOP))
				{
					m_iState = M3_STATE::M3_STATE_FAR_WALK_LEFT;
				}
				else if (fChance <= 10.f &&( m_isAnimEnd || CONDITION_LOOP))
				{
					m_iState = M3_STATE::M3_STATE_FAR_WALK_RIGHT;
				}
				fChance = m_pGameInstance->Compute_Random(1.5f, 2.f);

				if (CONDITION_FAR_WALK_LEFT || CONDITION_FAR_WALK_RIGHT)
					fChance = m_pGameInstance->Compute_Random(0.5f, 1.f);

				m_fAcc_Random_WalkTime = fChance;

			}
			else 
			{
				if (m_isAnimEnd || CONDITION_LOOP)
					m_iState = M3_STATE::M3_STATE_FAR_IDLE;
			}

			
			


			if ((m_isAnimEnd || CONDITION_LOOP)&&0 >= m_iNumBullets)
			{
				m_iState |= M3_STATE::M3_STATE_LOAD;
			}

			else if (m_iNumBullets && 5.f <= m_fAcc_Snipe_Time&&(m_isAnimEnd||CONDITION_LOOP) && __super::Util_isPlayer_InRange(m_fFarRange))
			{
				
				m_iState |= M3_STATE::M3_STATE_FAR_SNIPE;

			}
			else if (m_iNumBullets && 10.f <= m_fAcc_MultiShot_Time&& (m_isAnimEnd || CONDITION_LOOP) && __super::Util_isPlayer_InRange(m_fMultiShotRange))
			{
				if (m_bCall_Close_MultiShot)
				{
					
					m_fAcc_Multishot_Duration = m_pGameInstance->Compute_Random(4.f, 5.f);//지속 시간
					m_fAcc_Multishot_TrackPosition = 0.f;//현재 진행 위치
					m_iState |= M3_STATE::M3_STATE_CLOSE_MULTISHOT;
				}				
			}



		}

	}
	if (true == m_bHeadShot)
	{
		m_iState = M3_STATE::M3_STATE_HIT;
	}

	/*Anim State*/


 	if (CONDITION_CLOSE_MULTISHOT)
	{
		if (10.f <= m_fAcc_MultiShot_Time && m_fAcc_Multishot_Duration > m_fAcc_Multishot_TrackPosition && ( m_bCall_Close_MultiShot || m_isAnimEnd))
		{
			
			m_pModelCom->Move_NextAnim(m_vecAnimStates[M3_ANIM::M3_ANIM_CLOSE_MULTISHOT], 0.2F,false);//애니메이션 끝날때마다 총알 발사
			Pri_Update_Cancle_LoopState();
			m_bCall_Close_MultiShot = false;
		}
	}

	else if (CONDITION_FAR_SNIPE)
	{
		if (5.f <= m_fAcc_Snipe_Time &&(CONDITION_LOOP||m_isAnimEnd))
		{
			m_pModelCom->Move_NextAnim(m_vecAnimStates[M3_ANIM::M3_ANIM_FAR_SNIPE],0.2F,false);
			Pri_Update_Cancle_LoopState();
			m_fAcc_Snipe_Time = 0.f;

		}
	}
	else if (CONDITION_LOAD)
	{
		if (m_bCall_Load)
		{
			m_pGameInstance->PlayEffect(L"Monster03_Reload.mp3", 0.1f);
			m_pModelCom->Move_NextAnim(m_vecAnimStates[M3_ANIM::M3_ANIM_LOAD], 0.2F, false);
			Pri_Update_Cancle_LoopState();
			m_bCall_Load = false;
		}

	}
	else if (CONDITION_FAR_IDLE)
	{
		if (m_bCall_Far_Idle)
		{
			m_pModelCom->Move_NextAnim(m_vecAnimStates[M3_ANIM::M3_ANIM_FAR_IDLE], 0.2F,true);
			m_bCall_Far_Idle = false;
		}
	}
	else if (CONDITION_FAR_WALK)
	{
		if (m_bCall_Far_Walk)
		{
			m_pModelCom->Move_NextAnim(m_vecAnimStates[M3_ANIM::M3_ANIM_FAR_WALK], 0.2F, true);
			m_bCall_Far_Walk = false;
		}
	}

	else if (CONDITION_FAR_WALK_LEFT)
	{
		if (m_bCall_Far_Walk_Left)
		{
			m_pModelCom->Move_NextAnim(m_vecAnimStates[M3_ANIM::M3_ANIM_FAR_WALK_LEFT], 0.2F, true);
			m_bCall_Far_Walk_Left = false;
		}
	}
	else if (CONDITION_FAR_WALK_RIGHT)
	{
		if (m_bCall_Far_Walk_Right)
		{
			m_pModelCom->Move_NextAnim(m_vecAnimStates[M3_ANIM::M3_ANIM_FAR_WALK_RIGHT], 0.2F, true);
			m_bCall_Far_Walk_Right = false;
		}
	}

	else if (CONDITION_CLOSE_IDLE)
	{
		if (m_bCall_Close_Idle)
		{
			m_pModelCom->Move_NextAnim(m_vecAnimStates[M3_ANIM::M3_ANIM_CLOSE_IDLE], 0.2F, true);
			m_bCall_Close_Idle=false;
		}
	}
	else if (CONDITION_CLOSE_RUN)
	{
		if (m_bCall_Close_Run)
		{
			m_pModelCom->Move_NextAnim(m_vecAnimStates[M3_ANIM::M3_ANIM_CLOSE_RUN], 0.2F, true);
			m_bCall_Close_Run = false;
		}
	}
	else if (CONDITION_CLOSE_WALK_BACK)
	{
		if (m_bCall_Close_Walk_Back)
		{
			m_pModelCom->Move_NextAnim(m_vecAnimStates[M3_ANIM::M3_ANIM_CLOSE_WALK_BACK], 0.2F, true);
			m_bCall_Close_Walk_Back = false;
		}

	}
	else if (CONDITION_CLOSE_ROLL_LEFT)
	{
		if (m_bCall_Close_Roll_Left)
		{
			m_pModelCom->Move_NextAnim(m_vecAnimStates[M3_ANIM::M3_ANIM_CLOSE_ROLL_LEFT], 0.2F, false);
			m_bCall_Close_Roll_Left = false;
		}
	}
	else if (CONDITION_CLOSE_ROLL_RIGHT)
	{
		if (m_bCall_Close_Roll_Right)
		{
			m_pModelCom->Move_NextAnim(m_vecAnimStates[M3_ANIM::M3_ANIM_CLOSE_ROLL_RIGHT], 0.2F, false);
			m_bCall_Close_Roll_Right = false;
		}
	}
	
	else if (CONDITION_HIT)
	{
		
		if (m_bCall_Hit)
		{
			m_pGameInstance->PlayEffect(L"MonsterHit.wav", 0.1f);
			m_pModelCom->Move_NextAnim(m_vecAnimStates[M3_ANIM::M3_ANIM_HIT], 0.2F, false);
			m_bCall_Hit = false;
		}
	}
	/*Action_By Anim*/
	if (Util_isPlayer_InRange(m_fFarRange))
		Util_TurnTo_Player(fTimeDelta * 2.f);


	if (m_fAcc_StartRollTime > 0.15f && m_fAcc_StartRollTime < 0.30f && CONDITION_CLOSE_ROLL_RIGHT)
	{

		Util_Go_Right(fTimeDelta, 1.f);
	}

	else if (m_fAcc_StartRollTime > 0.15f && m_fAcc_StartRollTime < 0.30 && CONDITION_CLOSE_ROLL_LEFT)
		Util_Go_Left(fTimeDelta, 1.f);


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
	else if (CONDITION_CLOSE_RUN)
	{
		Util_Go_Straight(fTimeDelta, 0.6f);
	}


	else if (CONDITION_CLOSE_WALK_BACK)
		Util_Go_Back(fTimeDelta, 0.3f);


	__super::Pri_Update_NaviMesh();

	__super::Priority_Update(fTimeDelta);
	
}

void CMonster03::Update(_float fTimeDelta)
{

	if (false == m_bActive)
		return;
	if (true == m_isRemoving)
		return;
	m_isAnimEnd = m_pModelCom->Play_Animation(fTimeDelta, m_isChangingMotion);

	__super::Update(fTimeDelta);
}

void CMonster03::Late_Update(_float fTimeDelta)
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

HRESULT CMonster03::Render()
{

	if (false == m_bActive)
		return S_OK;
	
	
	__super::Render();
	


	return S_OK;

}

HRESULT CMonster03::Mapping_Anim_States()
{

	m_vecAnimStates.resize(M3_ANIM::M3_ANIM_END);
	/*Far*/
	m_vecAnimStates[M3_ANIM::M3_ANIM_FAR_IDLE] = 4;
	m_vecAnimStates[M3_ANIM::M3_ANIM_FAR_SNIPE] = 1;
	m_vecAnimStates[M3_ANIM::M3_ANIM_FAR_WALK_RIGHT] = 8;
	m_vecAnimStates[M3_ANIM::M3_ANIM_FAR_WALK_LEFT] = 10;
	m_vecAnimStates[M3_ANIM::M3_ANIM_FAR_WALK] = 2;

	/*Close*/
	m_vecAnimStates[M3_ANIM::M3_ANIM_CLOSE_IDLE] = 0;
	m_vecAnimStates[M3_ANIM::M3_ANIM_CLOSE_MULTISHOT] = 14;
	m_vecAnimStates[M3_ANIM::M3_ANIM_CLOSE_ROLL_LEFT] = 12;
	m_vecAnimStates[M3_ANIM::M3_ANIM_CLOSE_ROLL_RIGHT] = 13;
	m_vecAnimStates[M3_ANIM::M3_ANIM_CLOSE_WALK_BACK] = 9;
	m_vecAnimStates[M3_ANIM::M3_ANIM_IN_THE_MIDDLE_WALK] = 11;
	m_vecAnimStates[M3_ANIM::M3_ANIM_CLOSE_RUN] = 3;
	/*Any*/
	m_vecAnimStates[M3_ANIM::M3_ANIM_LOAD] = 15;
	m_vecAnimStates[M3_ANIM::M3_ANIM_HIT] = 17;
	





	return S_OK;
}

void CMonster03::Pri_Update_Cancle_LoopState()
{

	if (CONDITION_FAR_IDLE)
		m_iState ^= M3_STATE::M3_STATE_FAR_IDLE;
	if (CONDITION_FAR_WALK_LEFT)
		m_iState ^= M3_STATE::M3_STATE_FAR_WALK_LEFT;
	if (CONDITION_FAR_WALK_RIGHT)
		m_iState ^= M3_STATE::M3_STATE_FAR_WALK_RIGHT;
	if (CONDITION_FAR_WALK)
		m_iState ^= M3_STATE::M3_STATE_FAR_WALK;


	

	if (CONDITION_CLOSE_IDLE)
		m_iState ^= M3_STATE::M3_STATE_CLOSE_IDLE;
	if (CONDITION_CLOSE_RUN)
		m_iState ^= M3_STATE::M3_STATE_CLOSE_RUN;
	if (CONDITION_CLOSE_WALK_BACK)
		m_iState ^= M3_STATE::M3_STATE_CLOSE_WALK_BACK;
	if (CONDITION_IN_THE_MIDDLE_WALK)
		m_iState ^= M3_STATE::M3_STATE_IN_THE_MIDDLE_WALK;
	if (CONDITION_STATE_LOOP)
		m_iState ^= M3_STATE::M3_STATE_LOOP;

	return;
}

void CMonster03::Pri_Update_LoopCall_State()
{
	if (!CONDITION_FAR_IDLE && false == m_bCall_Far_Idle)
		m_bCall_Far_Idle = true;

	if (!CONDITION_FAR_WALK_RIGHT && false == m_bCall_Far_Walk_Right)
		m_bCall_Far_Walk_Right = true;
	if (!CONDITION_FAR_WALK_LEFT && false == m_bCall_Far_Walk_Left)
		m_bCall_Far_Walk_Left = true;

	if (!CONDITION_FAR_WALK && false == m_bCall_Far_Walk)
		m_bCall_Far_Walk = true;



	if (!CONDITION_CLOSE_IDLE && false == m_bCall_Close_Idle)
		m_bCall_Close_Idle = true;
	if (!CONDITION_CLOSE_RUN && false == m_bCall_Close_Run)
		m_bCall_Close_Run = true;
	if (!CONDITION_CLOSE_WALK_BACK && false == m_bCall_Close_Walk_Back)
		m_bCall_Close_Walk_Back = true;
	if (!CONDITION_IN_THE_MIDDLE_WALK && false == m_bCall_In_The_Middle_Walk)
		m_bCall_In_The_Middle_Walk = true;



}

void CMonster03::PriUpdate_Non_LoopCall_State()
{
	if (!CONDITION_CLOSE_ROLL_LEFT && false == m_bCall_Close_Roll_Left)
		m_bCall_Close_Roll_Left = true;

	if (!CONDITION_CLOSE_ROLL_RIGHT && false == m_bCall_Close_Roll_Right)
		m_bCall_Close_Roll_Right = true;

	if (!CONDITION_LOAD && false == m_bCall_Load)
		m_bCall_Load = true;

	if (!CONDITION_HIT && false == m_bCall_Hit)
		m_bCall_Hit = true;
}

HRESULT CMonster03::Ready_Components()
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
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Monster03"),
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

HRESULT CMonster03::Bind_ShaderResources()
{
	__super::Bind_ShaderResources();

	return S_OK;
}

HRESULT CMonster03::Ready_PartObjects()
{
	CMonster03_Head::MONSTER03_HEAD_DESC Head_Desc = {};
	Head_Desc.fRotationPerSec = XMConvertToRadians(90.f);
	Head_Desc.fSpeedPerSec = 10.f;
	Head_Desc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4_Ptr();
	Head_Desc.pSocketMatrix = Get_BoneMatrix("Bip001 Head");
	Head_Desc.pHeadSchot = &m_bHeadShot;
	Head_Desc.pisMonsterPicked = &m_isMonsterPicked;
	Head_Desc.pHitTime = &m_fHitTime;
	Head_Desc.pAccHitTime = &m_fAccHitTime;
	Head_Desc.pHPInfo = &m_HPInfo;
	Head_Desc.pHit_NormalMonster = &m_bHit_NormalMonster;
	m_PartObjects[PART_HEAD] = static_cast<CPartObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster03_Head"), &Head_Desc));
	if (nullptr == m_PartObjects[PART_HEAD])
		return E_FAIL;



	CMonster03_Body::MONSTER03_BODY_DESC Body_Desc = {};
	Body_Desc.fRotationPerSec = XMConvertToRadians(90.f);
	Body_Desc.fSpeedPerSec = 10.f;
	Body_Desc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4_Ptr();
	Body_Desc.pSocketMatrix = Get_BoneMatrix("Bip001 Spine1");
	Body_Desc.pisMonsterPicked = &m_isMonsterPicked;
	Body_Desc.pHitTime = &m_fHitTime;
	Body_Desc.pAccHitTime = &m_fAccHitTime;
	Body_Desc.pHPInfo = &m_HPInfo;
	Body_Desc.pHit_NormalMonster = &m_bHit_NormalMonster;
	m_PartObjects[PART_BODY] = static_cast<CPartObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster03_Body"), &Body_Desc));
	if (nullptr == m_PartObjects[PART_BODY])
		return E_FAIL;


	CMonster03_Gun::MONSTER03_GUN_DESC Gun_Desc = {};
	Gun_Desc.fRotationPerSec = XMConvertToRadians(90.f);
	Gun_Desc.fSpeedPerSec = 10.f;
	Gun_Desc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4_Ptr();
	Gun_Desc.pSocketMatrix = Get_BoneMatrix("Bip001 Prop1");
	Gun_Desc.pState = &m_iState;
	Gun_Desc.pIsAnimEnd = &m_isAnimEnd;
	Gun_Desc.pMultiShotSign = &m_bMultiShotSign;
	m_PartObjects[PART_GUN] = static_cast<CPartObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster03_Gun"), &Gun_Desc));
	if (nullptr == m_PartObjects[PART_GUN])
		return E_FAIL;

	return S_OK;

}

CMonster03* CMonster03::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMonster03* pInstance = new CMonster03(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Created Monster03_ Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CMonster03::Clone(void* pArg)
{
	CMonster03* pInstance = new CMonster03(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Clone Monster03 Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CMonster03::Free()
{

	__super::Free();
	
}