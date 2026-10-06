#include"stdafx.h"
#include "Monster05.h"
#include "GameInstance.h"
#include"Monster05_Gun.h"
#include"Monster05_Head.h"
#include"Monster05_Body.h"

CMonster05::CMonster05(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CMonster{ pDevice, pContext }
{
}

CMonster05::CMonster05(const CMonster05& Prototype)
	:CMonster(Prototype)
{
}

HRESULT CMonster05::Initialize_Prototype()
{


	return S_OK;
}


HRESULT CMonster05::Initialize(void* pArg)
{
	MONSTER_DESC* pDesc = static_cast<MONSTER_DESC*>(pArg);
	pDesc->iNumPartObject = PARTOBJECT::PART_END;
	
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;



	__super::m_HPInfo = { 3000,3000,3500,3500 };
	if (FAILED(Ready_Components()))
		return E_FAIL;
	/*For UI-HP_Bar*/
	XMStoreFloat4x4(&m_HPBarSocketMatrix, XMMatrixIdentity());//Get_BoneMatrix("MonsterHp");
	m_HPBarSocketMatrix._42 = 2.4f;

	if (FAILED(__super::Ready_UI_HPBar_LEVEL_GAMEPLAY()))
		return E_FAIL;

	m_pModelCom->SetUp_Animation(4, true);
	m_iState = M5_STATE::M5_STATE_FAR_IDLE;

	if (FAILED(Mapping_Anim_States()))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	return S_OK;
}

void CMonster05::Priority_Update(_float fTimeDelta)
{

	if (false == m_bActive)
		return;

	
	Pri_Update_LoopCall_State();

	Pri_Update_Non_LoopCall_State();

	/*m_fAccTime*/
	{

		if (!CONDITION_CLOSE_FIRE)
			m_fAcc_CloseFire_Time += fTimeDelta;

		/*NonLoop_Attack*/
		if (!CONDITION_FAR_SNIPE)
			m_fAcc_Snipe_Time += fTimeDelta;
		else if (CONDITION_FAR_SNIPE)
			m_fAcc_Snipe_TrackPosition += fTimeDelta;


		if (CONDITION_ROLL)
			m_fAcc_StartRollTime += fTimeDelta;

	}
	
	

	if ((!m_isAnimEnd) && (CONDITION_NON_LOOP))
	{

	}

	else if (m_isAnimEnd && (CONDITION_NON_LOOP))
	{

		if (CONDITION_HIT)
		{

			m_fAcc_Snipe_Time = 0;//쿨타임 초기화
			m_fAcc_Snipe_TrackPosition = 0.f;
			m_bCall_Far_Snipe = true;
			m_iState ^= M5_STATE::M5_STATE_HIT;
			m_bHeadShot = false;
			m_iState = M5_STATE::M5_STATE_BATTLE_IDLE;
			m_fAcc_Battle_Idle_Time = 0.f;


		}
		else if (CONDITION_ROLL_LEFT)
		{
			m_iState ^= M5_STATE::M5_STATE_ROLL_LEFT;
			m_fAcc_StartRollTime = 0.f;
			m_iState = M5_STATE::M5_STATE_BATTLE_IDLE;
			m_fAcc_Battle_Idle_Time = 0.f;
		}
		else if (CONDITION_ROLL_RIGHT)
		{
			m_iState ^= M5_STATE::M5_STATE_ROLL_RIGHT;
			m_fAcc_StartRollTime = 0.f;
			m_iState = M5_STATE::M5_STATE_BATTLE_IDLE;
			m_fAcc_Battle_Idle_Time = 0.f;
		}
		else if (CONDITION_LOAD)
		{

			m_iState ^= M5_STATE::M5_STATE_LOAD;
			m_iNumBullets = 5;
			m_iState = M5_STATE::M5_STATE_BATTLE_IDLE;
			m_fAcc_Battle_Idle_Time = 0.f;
		}
		else if (CONDITION_FAR_SNIPE)
		{
			/*살짝 텀을 더줘서 종료조건에 딱 발사하게 (이게먼저 수행되면 총알 발사가 안됨)*/
			if (m_bSniperShootSign)
			{

				m_bSniperShootSign = false;//실제로 파츠(건)에 넘겨줄 스나이퍼 신호 불 변수

				m_iState ^= M5_STATE::M5_STATE_FAR_SNIPE;//상태 해제
				m_fAcc_Snipe_Time = 0;//쿨타임 초기화
				m_fAcc_Snipe_TrackPosition = 0.f;
				m_bCall_Far_Snipe = true;
				if (0 < m_iNumBullets)
					--m_iNumBullets;
				m_iState = M5_STATE::M5_STATE_BATTLE_IDLE;
				m_fAcc_Battle_Idle_Time = 0.f;
			}


		}
		else if (CONDITION_CLOSE_FIRE)
		{
			m_iState ^= M5_STATE::M5_STATE_CLOSE_FIRE;
			m_fAcc_CloseFire_Time = 0.f;
			if (0 < m_iNumBullets)
				--m_iNumBullets;
			m_iState = M5_STATE::M5_STATE_BATTLE_IDLE;
			m_fAcc_Battle_Idle_Time = 0.f;
		}

		
	}
	else
	{
		//루프이지만 시간이 정해져있는것들 
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
		else if (m_fAcc_Random_WalkTime >= m_fAcc_Close_Walk_Back_Time && CONDITION_CLOSE_WALK_BACK)
		{
			m_fAcc_Close_Walk_Back_Time += fTimeDelta;
		}
		else if (m_fAcc_Random_WalkTime >= m_fAcc_Middle_Run_Time && CONDITION_MIDDLE_RUN)
		{
			m_fAcc_Middle_Run_Time += fTimeDelta;
		}

		else {
			/*상태 해제*/
			if (0.5f <= m_fAcc_Battle_Idle_Time && CONDITION_BATTLE_IDLE)
			{
				m_fAcc_Battle_Idle_Time = 0.f;
				m_iState ^= M5_STATE::M5_STATE_BATTLE_IDLE;
				m_iState = M5_STATE::M5_STATE_LOOP;
			}
			else if (m_fAcc_Random_WalkTime < m_fAcc_Far_Walk_Left_Time && CONDITION_FAR_WALK_LEFT)
			{
				m_fAcc_Far_Walk_Left_Time = 0.f;
				m_iState ^= M5_STATE::M5_STATE_FAR_WALK_LEFT;
				m_iState = M5_STATE::M5_STATE_LOOP;
			}
			else if (m_fAcc_Random_WalkTime < m_fAcc_Far_Walk_Right_Time && CONDITION_FAR_WALK_RIGHT)
			{
				m_fAcc_Far_Walk_Right_Time = 0.F;
				m_iState ^= M5_STATE::M5_STATE_FAR_WALK_RIGHT;
				m_iState = M5_STATE::M5_STATE_LOOP;
			}
			else if (m_fAcc_Random_WalkTime < m_fAcc_Far_Walk_Time && CONDITION_FAR_WALK)
			{
				m_fAcc_Far_Walk_Time = 0.F;
				m_iState ^= M5_STATE::M5_STATE_FAR_WALK;
				m_iState = M5_STATE::M5_STATE_LOOP;
			}
			else if (m_fAcc_Random_WalkTime < m_fAcc_Middle_Run_Time && CONDITION_MIDDLE_RUN)
			{
				m_fAcc_Middle_Run_Time = 0.F;
				m_iState ^= M5_STATE::M5_STATE_MIDDLE_RUN;
				m_iState = M5_STATE::M5_STATE_LOOP;
			}
			else if (m_fAcc_Random_WalkTime < m_fAcc_Close_Walk_Back_Time && CONDITION_CLOSE_WALK_BACK)
			{
				m_fAcc_Close_Walk_Back_Time = 0.F;
				m_iState ^= M5_STATE::M5_STATE_CLOSE_WALK_BACK;
				m_iState = M5_STATE::M5_STATE_LOOP;
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
			else if (CONDITION_CLOSE_WALK_BACK)
			{

			}
			else if (CONDITION_MIDDLE_RUN)
			{

			}
			else
			{


				if (__super::Util_isPlayer_InRange(m_fCloseRange))
				{
					_float fChance = m_pGameInstance->Compute_Random(0.f, 10.f);

					if (fChance < 8.5f && (m_isAnimEnd || CONDITION_LOOP))
					{
						m_iState = M5_STATE::M5_STATE_CLOSE_WALK_BACK;
					}
					else if (fChance <= 9.25f && (m_isAnimEnd || CONDITION_LOOP))
					{
						m_iState = M5_STATE::M5_STATE_ROLL_LEFT;
					}
					else if (fChance <= 10.f && (m_isAnimEnd || CONDITION_LOOP))
					{
						m_iState = M5_STATE::M5_STATE_ROLL_RIGHT;
					}
					fChance = m_pGameInstance->Compute_Random(0.5f, 1.f);
					m_fAcc_Random_WalkTime = fChance;

				}

				else if (__super::Util_isPlayer_InRange(m_fMiddleRange))
				{
					_float fChance = m_pGameInstance->Compute_Random(0.f, 10.f);

					if (fChance <= 5.f && (m_isAnimEnd || CONDITION_LOOP))
					{
						m_iState = M5_STATE::M5_STATE_MIDDLE_RUN;
					}
					else if (fChance <= 9.5f && (m_isAnimEnd || CONDITION_LOOP))
					{
						m_iState = M5_STATE::M5_STATE_ROLL_LEFT;
					}
					else if (fChance <= 10.f && (m_isAnimEnd || CONDITION_LOOP))
					{
						m_iState = M5_STATE::M5_STATE_ROLL_RIGHT;
					}
					fChance = m_pGameInstance->Compute_Random(0.5f, 0.5f);

					m_fAcc_Random_WalkTime = fChance;
				}

				else if (__super::Util_isPlayer_InRange(m_fFarRange))
				{
					_float fChance = m_pGameInstance->Compute_Random(0.f, 10.f);

					if (fChance <= 8.5f && (m_isAnimEnd || CONDITION_LOOP))
					{
						m_iState = M5_STATE::M5_STATE_FAR_WALK;
					}
					else if (fChance <= 9.25f && (m_isAnimEnd || CONDITION_LOOP))
					{
						m_iState = M5_STATE::M5_STATE_FAR_WALK_RIGHT;
					}
					else if (fChance <= 10.f && (m_isAnimEnd || CONDITION_LOOP))
					{
						m_iState = M5_STATE::M5_STATE_FAR_WALK_LEFT;
					}
					fChance = m_pGameInstance->Compute_Random(0.5f, 1.5f);
					m_fAcc_Random_WalkTime = fChance;
				}
				else
				{
					if (m_isAnimEnd || CONDITION_LOOP)
						m_iState = M5_STATE::M5_STATE_FAR_IDLE;
				}

				

				/*공격이 가능하다면 공격 (NON LOOP)*/
				if (0 >= m_iNumBullets && (m_isAnimEnd || CONDITION_LOOP))
				{
					m_iState |= M5_STATE::M5_STATE_LOAD;
				}
				/* snipe */
				else if (
					m_iNumBullets &&
					(__super::Util_isPlayer_InRange(m_fFarRange)) &&
					(!__super::Util_isPlayer_InRange(m_fCloseRange)) &&
					3.f <= m_fAcc_Snipe_Time &&
					(m_isAnimEnd || CONDITION_LOOP))
				{
					m_fAcc_Snipe_Duration = m_pGameInstance->Compute_Random(3.f, 4.f);
					m_fAcc_Snipe_TrackPosition = 0.f;
					m_iState |= M5_STATE::M5_STATE_FAR_SNIPE;
				}
				/*Close shot*/
				else if (m_iNumBullets && 1.f <= m_fAcc_CloseFire_Time && (m_isAnimEnd || CONDITION_LOOP) && __super::Util_isPlayer_InRange(m_fCloseRange))
				{
					m_iState |= M5_STATE::M5_STATE_CLOSE_FIRE;
				}

			}

		}

	}
	
	if (true == m_bHeadShot)
	{
		Pri_Update_Cancle_LoopState();
		m_iState = M5_STATE::M5_STATE_HIT;
	}

	/*AnimState*/

	if (CONDITION_FAR_SNIPE)
	{
		if (3.F <= m_fAcc_Snipe_Time && m_bCall_Far_Snipe)
		{
			m_pModelCom->Move_NextAnim(m_vecAnimStates[M5_ANIM::M5_ANIM_FAR_SNIPE], 0.2f, false);
			Pri_Update_Cancle_LoopState();
			m_fAcc_Snipe_Time = 0.f;
			m_bCall_Far_Snipe = false;
		}
	}
	else if (CONDITION_CLOSE_FIRE)
	{
		if (1.f <= m_fAcc_CloseFire_Time && (CONDITION_LOOP || m_isAnimEnd))
		{
			m_pModelCom->Move_NextAnim(M5_ANIM::M5_ANIM_CLOSE_FIRE, 0.2f, false);
			Pri_Update_Cancle_LoopState();
			m_fAcc_CloseFire_Time = 0.f;
		}
	}
	else if (CONDITION_LOAD)
	{
		if (m_bCall_Load)
		{
			m_pGameInstance->PlayEffect(L"Monster05_Reload.mp3", 0.1f);
			m_pModelCom->Move_NextAnim(m_vecAnimStates[M5_ANIM::M5_ANIM_LOAD], 0.2F, false);
			Pri_Update_Cancle_LoopState();
			m_bCall_Load = false;
		}
	}
	else if (CONDITION_FAR_WALK)
	{
		if (m_bCall_Far_Walk)
		{
			m_pModelCom->Move_NextAnim(m_vecAnimStates[M5_ANIM::M5_ANIM_FAR_WALK], 0.2f, true);
			m_bCall_Far_Walk = false;
		}
	}
	else if (CONDITION_FAR_WALK_LEFT)
	{
		if (m_bCall_Far_Walk_Left)
		{
			m_pModelCom->Move_NextAnim(m_vecAnimStates[M5_ANIM::M5_ANIM_FAR_WALK_LEFT], 0.2f, true);
			m_bCall_Far_Walk_Left = false;
		}
	}

	else if (CONDITION_FAR_WALK_RIGHT)
	{
		if (m_bCall_Far_Walk_Right)
		{
			m_pModelCom->Move_NextAnim(m_vecAnimStates[M5_ANIM::M5_ANIM_FAR_WALK_RIGHT], 0.2f, true);
			m_bCall_Far_Walk_Right = false;
		}
	}
	else if (CONDITION_FAR_IDLE)
	{
		if (m_bCall_Far_Idle)
		{
			m_pModelCom->Move_NextAnim(m_vecAnimStates[M5_ANIM::MS_ANIM_FAR_IDLE], 0.2f, true);
			m_bCall_Far_Idle = false;
		}
	}

	else if (CONDITION_MIDDLE_RUN)
	{
		if (m_bCall_Middle_Run)
		{
			m_pModelCom->Move_NextAnim(m_vecAnimStates[M5_ANIM::M5_ANIM_MIDDLLE_RUN], 0.2F, true);
			m_bCall_Middle_Run = false;
		}
	}
	else if (CONDITION_CLOSE_WALK_BACK)
	{
		if (m_bCall_Close_Walk_Back)
		{
			m_pModelCom->Move_NextAnim(m_vecAnimStates[M5_ANIM::M5_ANIM_CLOSE_WALK_BACK], 0.2f, true);
			m_bCall_Close_Walk_Back = false;
		}
	}
	else if (CONDITION_ROLL_LEFT)
	{
		if (m_bCall_Roll_Left)
		{
			m_pModelCom->Move_NextAnim(m_vecAnimStates[M5_ANIM::M5_ANIM_ROLL_LEFT], 0.2f, false);
			m_bCall_Roll_Left = false;
		}
	}
	else if (CONDITION_ROLL_RIGHT)
	{
		if (m_bCall_Roll_Right)
		{
			m_pModelCom->Move_NextAnim(m_vecAnimStates[M5_ANIM::M5_ANIM_ROLL_RIGHT], 0.2f, false);
			m_bCall_Roll_Right = false;
		}
	}
	else if (CONDITION_BATTLE_IDLE)
	{
		if (m_bCall_Battle_Idle)
		{
			m_pModelCom->Move_NextAnim(m_vecAnimStates[M5_ANIM::M5_ANIM_BATTLE_IDLE], 0.2F, true);
			m_bCall_Battle_Idle = false;
		}
	}
	else if (CONDITION_HIT)
	{
		if (m_bCall_Hit)
		{
			m_pModelCom->Move_NextAnim(m_vecAnimStates[M5_ANIM::M5_ANIM_HIT], 0.2f, false);
			m_bCall_Hit = false;
		}
	}
	

	/*attack*/
	if (CONDITION_FAR_SNIPE&&(m_fAcc_Snipe_TrackPosition >= m_fAcc_Snipe_Duration))
	{
		m_bSniperShootSign = true;
	}

	/*파츠 업뎃*/
	

	if (Util_isPlayer_InRange(m_fFarRange))
		Util_TurnTo_Player(fTimeDelta * 2.f);


	if (m_fAcc_StartRollTime > 0.15f && m_fAcc_StartRollTime < 0.30f && CONDITION_ROLL_RIGHT)
	{

		Util_Go_Right(fTimeDelta, 1.f);
	}

	else if (m_fAcc_StartRollTime > 0.15f && m_fAcc_StartRollTime < 0.30 && CONDITION_ROLL_LEFT)
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
	else if (CONDITION_MIDDLE_RUN)
	{
		Util_Go_Straight(fTimeDelta, 0.6f);
	}


	else if (CONDITION_CLOSE_WALK_BACK)
		Util_Go_Back(fTimeDelta, 0.3f);








	__super::Pri_Update_NaviMesh();
	__super::Priority_Update(fTimeDelta);

}

void CMonster05::Update(_float fTimeDelta)
{
	if (false == m_bActive)
		return;

	if (true == m_isRemoving)
		return;

	m_isAnimEnd= m_pModelCom->Play_Animation(fTimeDelta, m_isChangingMotion);

	/*파츠 업뎃*/
	__super::Update(fTimeDelta);
}

void CMonster05::Late_Update(_float fTimeDelta)
{
	if (false == m_bActive)
		return;

	if (true == m_isRemoving)
	{
		m_pGameInstance->Add_RenderObject(CRenderer::RG_GLOW, this);
	}
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);

	/*파츠 업뎃*/
	__super::Late_Update(fTimeDelta);
}

HRESULT CMonster05::Render()
{
	if (false == m_bActive)
		return S_OK;

	
	__super::Render();
	
	


	return S_OK;

}

HRESULT CMonster05::Mapping_Anim_States()
{
	m_vecAnimStates.resize(M5_ANIM::M5_ANIM_END);
	/*Close_Range && Far_Range*/
	m_vecAnimStates[M5_ANIM::M5_ANIM_BATTLE_IDLE]		 = 0;
	/*Close_Range*/
	m_vecAnimStates[M5_ANIM::M5_ANIM_CLOSE_FIRE]		 = 15;
	m_vecAnimStates[M5_ANIM::M5_ANIM_CLOSE_WALK_BACK]	 = 11;
	/*Middle_Range*/
	m_vecAnimStates[M5_ANIM::M5_ANIM_MIDDLLE_RUN]		 = 3;
	/*Close_Range && Middle_Range*/
	m_vecAnimStates[M5_ANIM::M5_ANIM_ROLL_RIGHT]		 = 13;
	m_vecAnimStates[M5_ANIM::M5_ANIM_ROLL_LEFT]			 = 14;
	/*Far_Range*/
	m_vecAnimStates[M5_ANIM::M5_ANIM_FAR_WALK]			 = 9;
	m_vecAnimStates[M5_ANIM::M5_ANIM_FAR_WALK_LEFT]		 = 12;
	m_vecAnimStates[M5_ANIM::M5_ANIM_FAR_WALK_RIGHT]	 = 10;
	m_vecAnimStates[M5_ANIM::M5_ANIM_FAR_SNIPE]			 = 5;
	m_vecAnimStates[M5_ANIM::MS_ANIM_FAR_IDLE]			 = 4;
	/*All Range*/
	m_vecAnimStates[M5_ANIM::M5_ANIM_HIT]				 = 6;
	m_vecAnimStates[M5_ANIM::M5_ANIM_LOAD]				 = 16;
	
	return S_OK;
}

void CMonster05::Pri_Update_Cancle_LoopState()
{

	if (CONDITION_BATTLE_IDLE)
		m_iState ^= M5_STATE::M5_STATE_BATTLE_IDLE;
	
	if(CONDITION_CLOSE_WALK_BACK)
		m_iState ^= M5_STATE::M5_STATE_CLOSE_WALK_BACK;
	
	if (CONDITION_MIDDLE_RUN)
		m_iState ^= M5_STATE::M5_STATE_MIDDLE_RUN;

	if (CONDITION_FAR_WALK)
		m_iState ^= M5_STATE::M5_STATE_FAR_WALK;
	if (CONDITION_FAR_WALK_LEFT)
		m_iState ^= M5_STATE::M5_STATE_FAR_WALK_LEFT;
	if (CONDITION_FAR_WALK_RIGHT)
		m_iState ^= M5_STATE::M5_STATE_FAR_WALK_RIGHT;
	if (CONDITION_FAR_IDLE)
		m_iState ^= M5_STATE::M5_STATE_FAR_IDLE;

	if (CONDITION_STATE_LOOP)
		m_iState ^= M5_STATE::M5_STATE_LOOP;

}

void CMonster05::Pri_Update_LoopCall_State()
{
	/*Battle IDle*/
	if (!CONDITION_BATTLE_IDLE && false == m_bCall_Battle_Idle)
		m_bCall_Battle_Idle = true;
	/*Close Walk Back*/
	if (!CONDITION_CLOSE_WALK_BACK && false == m_bCall_Close_Walk_Back)
		m_bCall_Close_Walk_Back = true;
	/*Middle Run*/
	if (!CONDITION_MIDDLE_RUN && false == m_bCall_Middle_Run)
		m_bCall_Middle_Run = true;
	/*Close */
	if (!CONDITION_FAR_WALK && false == m_bCall_Far_Walk)
		m_bCall_Far_Walk = true;
	if (!CONDITION_FAR_WALK_RIGHT && false == m_bCall_Far_Walk)
		m_bCall_Far_Walk_Right = true;
	if (!CONDITION_FAR_WALK_LEFT && false == m_bCall_Far_Walk_Left)
		m_bCall_Far_Walk_Left = true;
	if (!CONDITION_FAR_IDLE && false == m_bCall_Far_Idle)
		m_bCall_Far_Idle = true;
}

void CMonster05::Pri_Update_Non_LoopCall_State()
{
	if (!CONDITION_ROLL_LEFT && false == m_bCall_Roll_Left)
		m_bCall_Roll_Left = true;
	if (!CONDITION_ROLL_RIGHT && false == m_bCall_Roll_Right)
		m_bCall_Roll_Right = true;
	
	if (!CONDITION_LOAD && false == m_bCall_Load)
		m_bCall_Load = true;

	if (!CONDITION_HIT && false == m_bCall_Hit)
		m_bCall_Hit = true;

}

HRESULT CMonster05::Ready_Components()
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
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Monster05"),
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

HRESULT CMonster05::Ready_PartObjects()
{
	CMonster05_Gun::MONSTER05_GUN_DESC Gun_Desc = {};
	Gun_Desc.fSpeedPerSec = 10.f;
	Gun_Desc.fRotationPerSec = XMConvertToRadians(90.f);
	Gun_Desc.pState = &m_iState;
	Gun_Desc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4_Ptr();
	Gun_Desc.pSocketMatrix = Get_BoneMatrix("Bip001 Prop1");
	Gun_Desc.pIsAnimEnd = &m_isAnimEnd;
	Gun_Desc.pSniperShootSign = &m_bSniperShootSign;

	m_PartObjects[PART_GUN] = static_cast<CPartObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster05_Gun"), &Gun_Desc));
	if (nullptr == m_PartObjects[PART_GUN])
		return E_FAIL;






	CMonster05_Head::MONSTER05_HEAD_DESC Head_Desc = {};
	Head_Desc.fRotationPerSec= XMConvertToRadians(90.f);
	Head_Desc.fSpeedPerSec = 10.f;
	Head_Desc.pParentMatrix= m_pTransformCom->Get_WorldFloat4x4_Ptr();
	Head_Desc.pSocketMatrix= Get_BoneMatrix("aimatWeaknessPoint");
	Head_Desc.pHeadShot = &m_bHeadShot;
	Head_Desc.pisMonsterPicked = &m_isMonsterPicked;
	Head_Desc.pHitTime = &m_fHitTime;
	Head_Desc.pAccHitTime = &m_fAccHitTime;
	Head_Desc.pHPInfo = &m_HPInfo;
	Head_Desc.pHit_NormalMonster = &m_bHit_NormalMonster;
	m_PartObjects[PART_HEAD] = static_cast<CPartObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster05_Head"), &Head_Desc));
	if (nullptr == m_PartObjects[PART_HEAD])
		return E_FAIL;



	CMonster05_Body::MONSTER05_BODY_DESC Body_Desc = {};
	Body_Desc.fRotationPerSec = XMConvertToRadians(90.f);
	Body_Desc.fSpeedPerSec = 10.f;
	Body_Desc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4_Ptr();
	Body_Desc.pSocketMatrix = Get_BoneMatrix("Bip001 Spine");
	Body_Desc.pisMonsterPicked = &m_isMonsterPicked;
	Body_Desc.pHitTime = &m_fHitTime;
	Body_Desc.pAccHitTime = &m_fAccHitTime;
	Body_Desc.pHPInfo = &m_HPInfo;
	Body_Desc.pHit_NormalMonster = &m_bHit_NormalMonster;
	m_PartObjects[PART_BODY] = static_cast<CPartObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster05_Body"), &Body_Desc));
	if (nullptr == m_PartObjects[PART_BODY])
		return E_FAIL;
	return S_OK;
}

HRESULT CMonster05::Bind_ShaderResources()
{
	__super::Bind_ShaderResources();

	return S_OK;
}

CMonster05* CMonster05::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMonster05* pInstance = new CMonster05(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Created Monster05_ Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CMonster05::Clone(void* pArg)
{
	CMonster05* pInstance = new CMonster05(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Clone Monster05 Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CMonster05::Free()
{
	__super::Free();
	

	
}