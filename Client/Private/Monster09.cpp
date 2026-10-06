#include"stdafx.h"
#include"Monster09.h"
#include"GameInstance.h"

#include"Monster09_Body.h"
#include"Monster09_Head.h"
#include"Monster09_Wave.h"

#include"Particle_Monster09_Attack.h"
#include"Particle_Monster09_Moving.h"
#include"Effect_Monster09_Moving.h"
#include"Monster09_Rush.h"

CMonster09::CMonster09(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CMonster{ pDevice, pContext }
{
}

CMonster09::CMonster09(const CMonster09& Prototype)
	:CMonster(Prototype)
{
}

HRESULT CMonster09::Initialize_Prototype()
{


	return S_OK;
}


HRESULT CMonster09::Initialize(void* pArg)
{

	MONSTER09_DESC* pDesc = static_cast<MONSTER09_DESC*>(pArg);
	pDesc->iNumPartObject = PART_END;
	m_pMonster09s = pDesc->pMonster09s;
	m_pBossHPInfo = pDesc->pBossHPInfo;
	m_pDead = pDesc->pDead;
	m_iSplattingIndex = pDesc->iSplattingIndex;
	__super::m_HPInfo = { 300,300,300,300 };

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	//fInitPosition
	XMStoreFloat3(&m_vInitPosition, XMLoadFloat4(&(pDesc->vPosition)));


	
	//initial Dir
	_float fChance = m_pGameInstance->Compute_Random(0.f, 10.f);
	if (5.f >= fChance)
	{
		m_iNowDir = M9_DIRECTION_LEFT;
	}
	else if (10.f >= fChance)
	{
		m_iNowDir = M9_DIRECTION_RIGHT;
	}

	if (FAILED(Ready_Components()))
		return E_FAIL;


	if (FAILED(Mapping_Anim_States()))
		return E_FAIL;

	
	
	//m_pModelCom->SetUp_Animation(1, true);

	m_iState = M9_STATE::M9_STATE_SUBMERGE;
	_uint iNumChance = (_uint)(m_pGameInstance->Compute_Random(0.f, 2.f));
	switch (iNumChance)
	{
	case 0:
		Skill_Order_Queue.push(M9_STATE::M9_STATE_ATTACK_RUSH);
		break;
	
	case 1:
		Skill_Order_Queue.push(M9_STATE::M9_STATE_ATTACK_WAVE);
		break;
	}

	

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	if(FAILED(Ready_Attack_Waves()))
		return E_FAIL;
	if (FAILED(Ready_Attack_Rush()))
		return E_FAIL;


	if (FAILED(Ready_Particles()))
		return E_FAIL;

	return S_OK;
}

void CMonster09::Priority_Update(_float fTimeDelta)
{
	


	if (true == m_isCutSceneNow)
	{
		m_fAccCutScene_RoarStart += fTimeDelta;
		CutSceneMoving();
		return;
	}

	if (nullptr != pMonster09_Rush)
		pMonster09_Rush->Priority_Update(fTimeDelta);
	if (nullptr != m_pParticle_Moving)
	{
		m_pParticle_Moving->Priority_Update(fTimeDelta);
	}

	if (false == m_bActive)
		return ;
	if (false == m_isCutSceneNow)
	{
			if (m_fAccVisibleTime < m_fVisibleTime) {
				m_fAccVisibleTime += fTimeDelta;
				m_iState = CONDITION_SUBMERGE;
				m_fRandom_Submerge_Duration = 1.f;
				m_fAcc_Submerge_Time = 0.f;

			}
		
	}

	
	m_fAccParticleTiming += fTimeDelta;

	Pri_Update_LoopCall_State();
	Pri_Update_Non_LoopCall_State();
	
	if (nullptr != m_pEffect_Moving)
		m_pEffect_Moving->Priority_Update(fTimeDelta);

	/*m_fAccTime*/
	{
		/*쿨타임*/
		if (Skill_Order_Queue.size() <= 10.f)
		{
			if (!CONDITION_ATTACK_SHOT)
			{
				m_fAcc_Shot_Time +=fTimeDelta;
			}
			if (!CONDITION_ATTACK_RUSH)
			{
				m_fAcc_Rush_Time += fTimeDelta;
			}
			if (!CONDITION_ATTACK_WAVE)
			{
				m_fAcc_Wave_Time += fTimeDelta;
			}
			
			/*쿨타임이 다 찼다면 메시지 큐에 넣는다. */
			if (m_fAcc_Shot_Time > m_fShot_Cool_Time)
			{
				m_fAcc_Shot_Time = 0.f;
				Skill_Order_Queue.push(M9_STATE::M9_STATE_ATTACK_SHOT);
			}
			
			if (m_fAcc_Rush_Time > m_fRush_Cool_Time)
			{
				m_fAcc_Rush_Time = 0.f;
				Skill_Order_Queue.push(M9_STATE::M9_STATE_ATTACK_RUSH);
			}
			if (m_fAcc_Wave_Time > m_fWave_Cool_Time)
			{
				m_fAcc_Wave_Time = 0.f;
				Skill_Order_Queue.push(M9_STATE::M9_STATE_ATTACK_WAVE);
			}

		}
		

	}
	if (true == *m_pDead)
	{
		if (CONDITION_DEAD && m_isAnimEnd)
		{
			m_bActive = false;
			return;
		}
		
		if (CONDITION_SUBMERGE)
		{
			m_bActive = false;
			return;
		}
		else 
		{
			m_iState = M9_STATE::M9_STATE_DEAD;
		}
		
	}

	else if ((!m_isAnimEnd) && (CONDITION_NON_LOOP))
	{
		if (CONDITION_ATTACK_SHOT)
		{
			m_fAcc_Shot_Timing += fTimeDelta;
		}
		if (CONDITION_ATTACK_WAVE)
		{
			m_fAcc_Wave_Timing += fTimeDelta;
		}
		if (CONDITION_ATTACK_RUSH)
		{
			m_fAcc_Rush_Timing += fTimeDelta;
		}
		//지금 애님이 않끝났고  애님이 돌고 있으면 공격중인경우  거리,시간 에의한 상태변화를 막는다

		if (CONDITION_POPOUT)
		{
			m_fAccShoutTime_Sound += fTimeDelta;
		}
	}


	else if (m_isAnimEnd && CONDITION_NON_LOOP) {
		/* 아직 애니메이션이끝나지않았을때 조건을 더해줌*/

		/*pop_out*/
		if (CONDITION_POPOUT)
		{
			m_iState ^= M9_STATE::M9_STATE_POPOUT;
			/*m_iState = M9_STATE::M9_STATE_IDLE;
			m_fAcc_Idle_Time = 0.f;
			m_fRandom_Idle_Duration=m_pGameInstance->Compute_Random(0.2f, 0.2f);*/
			m_iState = M9_STATE::M9_STATE_LOOP;

			m_fAccShoutTime_Sound = 0.f;
			
		}

		/*Pattern1_ATTACK Rush*/
		else if (CONDITION_ATTACK_RUSH)
		{
			m_iState ^= M9_STATE::M9_STATE_ATTACK_RUSH;
			m_iState = M9_STATE::M9_STATE_DIVE_AFTER_ATTACK;
			m_fAcc_Rush_Timing = 0.f;
			m_fAccMovingTextureLifeTime = 0.f;
		}
		/*Pattern2_Attack _Shot*/
		else if (CONDITION_ATTACK_SHOT)
		{
			m_iState ^= M9_STATE::M9_STATE_ATTACK_SHOT;
			m_fAcc_Shot_Timing = 0.f;
			
			m_iState = M9_STATE::M9_STATE_DIVE_AFTER_WAVE_LEFT;
		}
		/*Dive After Pattern Attack*/
		else if (CONDITION_DIVE_AFTER_ATTACK)
		{
			m_iState ^= M9_STATE::M9_STATE_DIVE_AFTER_ATTACK;
			m_iState = M9_STATE::M9_STATE_SUBMERGE;
			m_fAcc_Submerge_Time = 0.f;
			m_fRandom_Submerge_Duration = m_pGameInstance->Compute_Random(1.f, 2.5f);

		}
		/* Attack Wave Left*/
		else if (CONDITION_ATTACK_WAVE_LEFT)
		{
			m_iState ^= M9_STATE::M9_STATE_ATTACK_WAVE_LEFT;
			m_iState = M9_STATE::M9_STATE_DIVE_AFTER_WAVE_LEFT;
			m_fAcc_Wave_Timing = 0.f;
			m_bWaveSign = true;
		}
		else if (CONDITION_DIVE_AFTER_WAVE_LEFT)
		{
			m_iState ^= M9_STATE::M9_STATE_DIVE_AFTER_WAVE_LEFT;
			m_iState = M9_STATE::M9_STATE_SUBMERGE;

			m_fAcc_Submerge_Time = 0.f;
			m_fRandom_Submerge_Duration = m_pGameInstance->Compute_Random(1.f, 2.5f);
		}
		/*Attack Wave Right*/
		else if (CONDITION_ATTACK_WAVE_RIGHT)
		{
			m_iState ^= M9_STATE::M9_STATE_ATTACK_WAVE_RIGHT;
			m_iState = M9_STATE::M9_STATE_DIVE_AFTER_WAVE_RIGHT;
			m_fAcc_Wave_Timing = 0.f;
			m_bWaveSign = true;
		}
		else if (CONDITION_DIVE_AFTER_WAVE_RIGHT)
		{
			m_iState ^= M9_STATE::M9_STATE_DIVE_AFTER_WAVE_RIGHT;
			m_iState = M9_STATE::M9_STATE_SUBMERGE;
			m_fAcc_Submerge_Time = 0.f;
			m_fRandom_Submerge_Duration = m_pGameInstance->Compute_Random(1.f, 2.5f);
		}

	}
	/*그다음  공간에 의한 상태변화*/
	else {

					
		/* loop이지만 사실상 시간이 정해져있는 것들 */

		if (m_fRandom_Idle_Duration >= m_fAcc_Idle_Time && CONDITION_IDLE) //고정으로 1초는 가져감
		{
			m_fAcc_Idle_Time += fTimeDelta;
		}
		else if (m_fRandom_Submerge_Duration >= m_fAcc_Submerge_Time && CONDITION_SUBMERGE)
		{
			m_fAcc_Submerge_Time += fTimeDelta;
		}


		else {

			/*상태 해제*/
			if (m_fRandom_Idle_Duration <= m_fAcc_Idle_Time && CONDITION_IDLE)
			{
				m_fAcc_Idle_Time = 0.f;
				m_iState ^= M9_STATE::M9_STATE_IDLE;
				m_iState = M9_STATE::M9_STATE_LOOP;
			}
			else if ( Can_PopOut() && !Skill_Order_Queue.empty() && m_fRandom_Submerge_Duration <= m_fAcc_Submerge_Time && CONDITION_SUBMERGE)
			{
				m_fAcc_Submerge_Time = 0.f;
				m_fRandom_Submerge_Duration = 0.f;
				m_iState ^= M9_STATE::M9_STATE_SUBMERGE;
				
				/*애니메이션 샷은 올라오는 모션도 합쳐져있음....*/
				if (M9_STATE::M9_STATE_ATTACK_SHOT == Skill_Order_Queue.front())
				{
					m_iState = M9_STATE::M9_STATE_ATTACK_SHOT;//attackshot 시작
					m_fAcc_Shot_Timing = 0.f;
					Skill_Order_Queue.pop();
				}
				else
				{
					m_iState = M9_STATE::M9_STATE_POPOUT;
				}
				

			}
			if (CONDITION_IDLE)
			{

			}
			else if (CONDITION_SUBMERGE)
			{

			}
			else if (CONDITION_POPOUT)
			{

			}
			else
			{


				{

					/*Attack*/
					
					if ( !CONDITION_ATTACK_SHOT && !Skill_Order_Queue.empty())
					{
						_uint iSkill = Skill_Order_Queue.front();
						Skill_Order_Queue.pop();
						if (iSkill & M9_STATE::M9_STATE_ATTACK_WAVE)
						{
							_float fChance = m_pGameInstance->Compute_Random(0.f, 10.f);
							if (5.f >= fChance)
							{
								iSkill = M9_STATE::M9_STATE_ATTACK_WAVE_RIGHT;
							}
							else if (10.f >= fChance)
							{
								iSkill = M9_STATE::M9_STATE_ATTACK_WAVE_LEFT;
							}
						}

						m_iState = iSkill;

					
						if (iSkill & M9_STATE::M9_STATE_ATTACK_RUSH)
						{
							m_fAcc_Rush_Timing = 0.f;
						}
						else if (iSkill & M9_STATE::M9_STATE_ATTACK_WAVE)
						{
							m_fAcc_Wave_Timing = 0.f;
						}
					}

				}



			}

		}

	}
	/*<------------------------ANIMSTATE--------------------------------->*/


	/*Pop Out*/
	if (CONDITION_POPOUT)
	{
		if (m_bCall_Popout)
		{
		
			
			_float fChance = m_pGameInstance->Compute_Random(0.f, 10.f);
			
			 if (5.f >= fChance)
			{
				m_pModelCom->SetUp_Animation(m_vecAnimStates[(_uint)M9_ANIM::M9_ANIM_POP_OUT1],false);
				m_bShoutStart_Sound = true;
			}
			
			else if (10.f >= fChance)
			{
				m_pModelCom->SetUp_Animation(m_vecAnimStates[(_uint)M9_ANIM::M9_ANIM_POP_OUT2],false);
			}
			


			m_bCall_Popout = false;
		}


	}
	/*Attack*/
	if (CONDITION_ATTACK_SHOT)
	{
		if (m_bCall_Attack_Shot)
		{

			
			m_pModelCom->Move_NextAnim(m_vecAnimStates[(_uint)M9_ANIM::M9_ANIM_ATTACK1_SHOT],0.2f,false);
			m_bCall_Attack_Shot = false;
		}
	}
	
	else if (CONDITION_ATTACK_RUSH)
	{
		if (m_bCall_Attack_Rush)
		{

			
			m_pModelCom->Move_NextAnim(m_vecAnimStates[(_uint)M9_ANIM::M9_ANIM_ATTACK2_RUSH2], 0.2f, false);
 			
			m_bCall_Attack_Rush = false;
		}
	}

	else if (CONDITION_ATTACK_WAVE_LEFT)
	{

		if (m_bCall_Attack_Wave_Left)
		{
			
			m_pGameInstance->PlayEffect(L"Monster09_Wave_Wind.mp3", 0.5f);
			m_pModelCom->Move_NextAnim(m_vecAnimStates[(_uint)M9_ANIM::M9_ANIM_ATTACK3_WAVE_LEFT],0.2f,false);
			m_bCall_Attack_Wave_Left = false;
		}
	}
	else if (CONDITION_ATTACK_WAVE_RIGHT)
	{
		if (m_bCall_Attack_Wave_Right)
		{
		
			m_pGameInstance->PlayEffect(L"Monster09_Wave_Wind.mp3", 0.5f);
			m_pModelCom->Move_NextAnim(m_vecAnimStates[(_uint)M9_ANIM::M9_ANIM_ATTACK3_WAVE_RIGHT],0.2f,false);
			m_bCall_Attack_Wave_Right = false;
		}
	}
	/*DIve After Attack*/

	else if (CONDITION_DIVE_AFTER_ATTACK)
	{
		if (m_bCall_Dive_After_Attack)
		{
			m_pModelCom->Move_NextAnim(m_vecAnimStates[(_uint)M9_ANIM::M9_ANIM_DIVE_AFTER_RUSH_ATTACK],0.3f,false);
			m_bCall_Dive_After_Attack = false;
		}
	}

	else if (CONDITION_DIVE_AFTER_WAVE_LEFT)
	{
		if (m_bCall_Dive_After_Wave_Left)
		{
			m_pModelCom->Move_NextAnim(m_vecAnimStates[(_uint)M9_ANIM::M9_ANIM_DIVE_AFTER_ATTACK_WAVE_LEFT],0.1f,false);
			m_bCall_Dive_After_Wave_Left = false;
		}
	}
	else if (CONDITION_DIVE_AFTER_WAVE_RIGHT)
	{
		if (m_bCall_Dive_After_Wave_Right)
		{
			m_pModelCom->Move_NextAnim(m_vecAnimStates[(_uint)M9_ANIM::M9_ANIM_DIVE_AFTER_ATTACK_WAVE_RIGHT],0.1f,false);
			m_bCall_Dive_After_Wave_Right = false;
		}
	}
	/*iDLE*/
	else if (CONDITION_IDLE)
	{

		if (m_bCall_Idle)
		{
			m_pModelCom->Move_NextAnim(m_vecAnimStates[(_uint)M9_ANIM::M9_ANIM_IDLE], 0.2f, true);
			m_bCall_Idle = false;
		}

	}
	/*Dead*/
	else if (CONDITION_DEAD)
	{
		if (m_bCall_Dead)
		{

			_float fChance = m_pGameInstance->Compute_Random(0.f, 10.f);
			if (3.3f >= fChance)
			{
				m_pModelCom->Move_NextAnim(m_vecAnimStates[(_uint)M9_ANIM::M9_ANIM_DEAD1], 0.2f, false);
			}
			else if (6.6f >= fChance)
			{
				m_pModelCom->Move_NextAnim(m_vecAnimStates[(_uint)M9_ANIM::M9_ANIM_DEAD2], 0.2f, false);
			}
			else if (10.f >= fChance)
			{
				m_pModelCom->Move_NextAnim(m_vecAnimStates[(_uint)M9_ANIM::M9_ANIM_DEAD3], 0.2f, false);
			}

			m_bCall_Dead = false;
		}
	}
	

/*<----------------------------------------------------------------------------------------->*/
	//PatternLogic

	if (CONDITION_SUBMERGE)
	{
 		_vector vNowPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		

		if (M9_DIRECTION::M9_DIRECTION_LEFT == m_iNowDir)
		{
			m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180.f));
			if (XMVectorGetX(vNowPosition) <= m_vInitPosition.x + m_fDomainMinX)
			{
				m_iNowDir = M9_DIRECTION::M9_DIRECTION_RIGHT;
				
				m_pTransformCom->Go_Left (fTimeDelta);
			}
			else
				m_pTransformCom->Go_Right(fTimeDelta);
		}
		else if (M9_DIRECTION::M9_DIRECTION_RIGHT == m_iNowDir)
		{
			m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180.f));
			if (XMVectorGetX(vNowPosition) >= m_vInitPosition.x + m_fDomainMaxX)
			{
				m_iNowDir = M9_DIRECTION::M9_DIRECTION_LEFT;
				m_pTransformCom->Go_Right(fTimeDelta);
			}
			else 
				m_pTransformCom->Go_Left(fTimeDelta);
		}
	
	}




	/*<-----------------------------------Animation-----------------------------------> */
	if (CONDITION_POPOUT)
		Util_TurnTo_Player(fTimeDelta);

	if(CONDITION_ATTACK_SHOT)
		Util_TurnTo_Player(fTimeDelta);
	if(CONDITION_ATTACK_WAVE)
		Util_TurnTo_Player(fTimeDelta);
	/*Attack_ Wave*/
	if (true == m_bWaveSign && m_fAcc_Wave_Timing >= 1.f)
	{

		_matrix matInitTrans;
		_vector vRight = { -1.f, 0.f, 0.f, 0.f };
		_vector vUp = { 0.f, 1.f, 0.f, 0.f };
		_vector vLook = {0.f, 0.f, -1.f, 0.f};
		_vector vPos =XMVectorSetW(XMLoadFloat3(&m_vInitPosition),1.f);
		
		_vector vOffsetLeft = { 10.f,0.f,0.f };
		_vector vOffsetRight = { -10.f,0.f,0.f };
		matInitTrans.r[0] = vRight;
		matInitTrans.r[1] = vUp;
		matInitTrans.r[2] = vLook;
		matInitTrans.r[3] = vPos;
		if (CONDITION_ATTACK_WAVE_LEFT)
		{
			

			m_vecAttackWaves[m_iNowWaveIndex]->Set_Active(true, CMonster09_Wave::WAVE_DIRECTION::WAVE_LEFT );
			vPos = XMVectorSetX(vPos, XMVectorGetX(vPos) + m_fDomainMaxX);
			vPos = XMVectorSetY(vPos, XMVectorGetY(vPos) + XMVectorGetX(XMVector3Length(vUp*3)) );
			vPos = XMVectorSetZ(vPos, XMVectorGetZ(vPos) - XMVectorGetX(XMVector3Length(vLook*40)));
			matInitTrans.r[3] = vPos+ vOffsetLeft;

			m_vecAttackWaves[m_iNowWaveIndex]->Reset_Bullets(matInitTrans);
		}
		else if (CONDITION_ATTACK_WAVE_RIGHT)
		{
			
			m_vecAttackWaves[m_iNowWaveIndex]->Set_Active(true, CMonster09_Wave::WAVE_DIRECTION::WAVE_RIGHT);
			vPos = XMVectorSetX(vPos, XMVectorGetX(vPos) + m_fDomainMinX);
			vPos = XMVectorSetY(vPos, XMVectorGetY(vPos) + XMVectorGetX(XMVector3Length(vUp * 3)));
			vPos = XMVectorSetZ(vPos, XMVectorGetZ(vPos) - XMVectorGetX(XMVector3Length(vLook *40)));
			matInitTrans.r[3] = vPos+ vOffsetRight;
			m_vecAttackWaves[m_iNowWaveIndex]->Reset_Bullets(matInitTrans);
		}
		m_bWaveSign = false;
		
		m_iNowWaveIndex = (m_iNowWaveIndex + 1)%3;
	}
	for (auto& Wave : m_vecAttackWaves)
		Wave->Priority_Update(fTimeDelta);
	

	/*Attack_Rush*/
	if (CONDITION_ATTACK_RUSH)
	{
		/* ATTACK SIGN*/
		m_fAccMovingTextureLifeTime += fTimeDelta * 0.5f;
	}
	if (CONDITION_ATTACK_RUSH)
	{
		if (m_fAcc_Rush_Timing > 0.1f&&false== m_bRushStart)
		{
			pMonster09_Rush->Set_Active(true);
			m_bRushStart = true;

			pMonster09_Rush->Reset_Bullets(Get_RushMatrix());
			
		}
		
		if (m_fAcc_Rush_Timing > 2.2f)
		{
			m_bRushStart = false;
			pMonster09_Rush->Set_Active(false);
		}
	}
	/*Logic depending on anim state*/
	

	__super::Priority_Update(fTimeDelta);
}

void CMonster09::Update(_float fTimeDelta)
{

	if (false == m_bActive )
		return;

	
	if (nullptr != m_pParticle_Moving)
	{
		if(M9_DIRECTION_LEFT ==m_iNowDir )
		m_pParticle_Moving->Set_Matrix(m_pTransformCom->Get_WorldMatrix(), 1);
		else
			m_pParticle_Moving->Set_Matrix(m_pTransformCom->Get_WorldMatrix(), 0);

		
		m_pParticle_Moving->Update(fTimeDelta);
		

	}
	if (nullptr != m_pEffect_Moving)
	{
	
		if (M9_DIRECTION_LEFT == m_iNowDir)
		{
			m_pEffect_Moving->Set_Matrix(m_pTransformCom->Get_WorldMatrix(), false);
		}
		else if(M9_DIRECTION_RIGHT == m_iNowDir)
		{
			m_pEffect_Moving->Set_Matrix(m_pTransformCom->Get_WorldMatrix(), true);
		}
		m_pEffect_Moving->Update(fTimeDelta);
	}

	m_isAnimEnd = m_pModelCom->Play_Animation(fTimeDelta, m_isChangingMotion);


	for (auto& Wave : m_vecAttackWaves)
		Wave->Update(fTimeDelta);
	
	if (nullptr != pMonster09_Rush)
		pMonster09_Rush->Update(fTimeDelta);
	///*ChangeState When Condition is All right*/
	//if(false==m_isCutSceneNow)
		__super::Update(fTimeDelta);


}

void CMonster09::Late_Update(_float fTimeDelta)
{
	

	if (false == m_bActive)
		return;
	

	if (nullptr != m_pParticle_Moving)
	{
		if(CONDITION_SUBMERGE)
		m_pParticle_Moving->Late_Update(fTimeDelta);
	}
	if (nullptr != m_pEffect_Moving&&CONDITION_SUBMERGE)
		m_pEffect_Moving->Late_Update(fTimeDelta);

	if (!CONDITION_SUBMERGE) 
	{
		m_pGameInstance->Add_RenderObject(CRenderer::RG_GLOW, this);
		m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
	}
	for (auto& Wave : m_vecAttackWaves)
		Wave->Late_Update(fTimeDelta);

	if (nullptr != pMonster09_Rush)
		pMonster09_Rush->Late_Update(fTimeDelta);

		__super::Late_Update(fTimeDelta);



		/*Sound*/
		if ((true == m_bShoutStart_Sound) && m_fAccShoutTime_Sound > 0.9f)
		{
			m_pGameInstance->PlayEffect(L"Monster09_Roar02.mp3", 0.3f);
			m_bShoutStart_Sound = false;
		}

}

HRESULT CMonster09::Render()
{
	if (!CONDITION_SUBMERGE)
	{

		if (true == m_bGlow)
		{
			if (FAILED(Render_Glow()))
				return E_FAIL;
		}
		else {
			if (!CONDITION_SUBMERGE)
				__super::Render();


			if (CONDITION_ATTACK_RUSH)
			{
				Render_RushSign();
			}
		}

	}

	return S_OK;
}

HRESULT CMonster09::Render_Glow()
{
	if (FAILED(Bind_ShaderResources_Glow()))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; i++)
	{
		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom_Monster09, "g_DiffuseTexture", i, aiTextureType_DIFFUSE, 0)))
			return E_FAIL;
		if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom_Monster09, "g_BoneMatrices", i)))
			return E_FAIL;

		m_pShaderCom_Monster09->Begin(0);

		m_pModelCom->Render(i);
	}
	return S_OK;
}

HRESULT CMonster09::Mapping_Anim_States()
{

	m_vecAnimStates.resize((_uint)M9_ANIM::M9_ANIM_END);
	/*ATTACK_SHOT*/
	m_vecAnimStates[(_uint)M9_ANIM::M9_ANIM_ATTACK1_SHOT] = 0;


	/*ATTACK_RUSH*/
	m_vecAnimStates[(_uint)M9_ANIM::M9_ANIM_ATTACK2_RUSH1] = 5;
	m_vecAnimStates[(_uint)M9_ANIM::M9_ANIM_ATTACK2_RUSH2] = 12;

	/*sUBMERGE AfterAttack*/
	m_vecAnimStates[(_uint)M9_ANIM::M9_ANIM_DIVE_AFTER_RUSH_ATTACK] = 13;

	/*Wave Attack*/
	m_vecAnimStates[(_uint)M9_ANIM::M9_ANIM_ATTACK3_WAVE_RIGHT] = 14;
	m_vecAnimStates[(_uint)M9_ANIM::M9_ANIM_ATTACK3_WAVE_LEFT] = 16;

	/*Submerge After Wave*/
	m_vecAnimStates[(_uint)M9_ANIM::M9_ANIM_DIVE_AFTER_ATTACK_WAVE_RIGHT] = 15;
	m_vecAnimStates[(_uint)M9_ANIM::M9_ANIM_DIVE_AFTER_ATTACK_WAVE_LEFT] = 17;
	
	m_vecAnimStates[(_uint)M9_ANIM::M9_ANIM_DEAD1] = 6;
	m_vecAnimStates[(_uint)M9_ANIM::M9_ANIM_DEAD2] = 7;
	m_vecAnimStates[(_uint)M9_ANIM::M9_ANIM_DEAD3] = 8;

	m_vecAnimStates[(_uint)M9_ANIM::M9_ANIM_POP_OUT1] = 0;
	m_vecAnimStates[(_uint)M9_ANIM::M9_ANIM_POP_OUT2] = 4;

	/*Idle*/
	m_vecAnimStates[(_uint)M9_ANIM::M9_ANIM_IDLE] = 1;


	return S_OK;
}


void CMonster09::Pri_Update_Cancle_LoopState()
{

	if (CONDITION_SUBMERGE)
		m_iState ^= M9_STATE::M9_STATE_IDLE;
	if (CONDITION_IDLE)
		m_iState ^= M9_STATE::M9_STATE_SUBMERGE;

	return;
}

void CMonster09::Pri_Update_LoopCall_State()
{


	if (!CONDITION_SUBMERGE && false == m_bCall_Submerge)
		m_bCall_Submerge = true;

	if (!CONDITION_IDLE && false == m_bCall_Idle)
		m_bCall_Idle = true;




}

void CMonster09::Pri_Update_Non_LoopCall_State()
{
	if (!CONDITION_POPOUT && false == m_bCall_Popout)
		m_bCall_Popout = true;

	if (!CONDITION_DEAD && false == m_bCall_Dead)
		m_bCall_Dead = true;

	if (!CONDITION_ATTACK_SHOT && false == m_bCall_Attack_Shot)
		m_bCall_Attack_Shot = true;

	if (!CONDITION_ATTACK_RUSH && false == m_bCall_Attack_Rush)
		m_bCall_Attack_Rush = true;
	if(!CONDITION_ATTACK_WAVE_LEFT && false == m_bCall_Attack_Wave_Left)
		m_bCall_Attack_Wave_Left = true;
	if (!CONDITION_ATTACK_WAVE_RIGHT && false == m_bCall_Attack_Wave_Right)
		m_bCall_Attack_Wave_Right = true;

	if (!CONDITION_DIVE_AFTER_WAVE_LEFT && false == m_bCall_Dive_After_Wave_Left)
		m_bCall_Dive_After_Wave_Left = true;

	if (!CONDITION_DIVE_AFTER_WAVE_RIGHT && false == m_bCall_Dive_After_Wave_Right)
		m_bCall_Dive_After_Wave_Right = true;

	if (!CONDITION_DIVE_AFTER_ATTACK && false == m_bCall_Dive_After_Attack)
		m_bCall_Dive_After_Attack = true;

}

_bool CMonster09::Can_PopOut()
{
	CTransform* pTargetTransform = 
		static_cast<CTransform*>
		(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY2, L"Layer_APlayer", L"Com_Transform", 0));
	_float fTargetX = XMVectorGetX((pTargetTransform->Get_WorldMatrix()).r[3]);
	_vector vMyPos;
	_float fMyX = XMVectorGetX(vMyPos = m_pTransformCom->Get_WorldMatrix().r[3]);

	_float fMin;
	_float fMax;
	/* min max 정하기  min max 사이에 대가리가 있고 submerge 가 아니면 나오지 못한다.  일정거리 이상 닿으면 나오지 못한다.*/
	if (fTargetX > fMyX)
	{
		fMin = fMyX;
		fMax = fTargetX;
	}
	else if (fTargetX < fMyX)
	{
		fMin = fTargetX;
		fMax = fMyX;
	}
	else
	{
		fMin = fMax = fMyX;
	}
	_bool bCanPopOut = true;

	/* ------나----O2(SUB_X)------Player-----O1---------- ->false */
	for (auto& Monster09 : *m_pMonster09s)
	{
		if (Monster09 == this)
			continue;
		
		_vector vOthersPos = (Monster09->m_pTransformCom->Get_WorldMatrix()).r[3];
		
		if (Monster09->Get_Rush_State())
			bCanPopOut = false;
		else if (true != (Monster09->Get_Submerge_State()))
		{
			/*1번째 조건 -> min max 사이에 submerge가 아닌 개체가 없어야함 */
			if (fMin <= XMVectorGetX(vOthersPos) && XMVectorGetX(vOthersPos) <= fMax)
			{
				bCanPopOut = false;
				break;
			}
			
			/* ->통과시 2번째 조건 : 내가 주위에 개체 와 닿지 않는 상태여야 한다.*/
			
			_vector vDistanceBTW_Other_Me = vOthersPos - vMyPos;
			_float fDistanceBTW_Other_Me =XMVectorGetX(XMVector3Length(vDistanceBTW_Other_Me));
		
			if (fDistanceBTW_Other_Me<=18.f)
			{
				bCanPopOut = false;
			}
		}
	}

		

	return bCanPopOut;
}

HRESULT CMonster09::Render_RushSign()
{
	_matrix SignMatrix =XMMatrixIdentity();
	_vector vSignPos = m_pTransformCom->Get_WorldMatrix().r[3] + XMVector3Normalize(m_pTransformCom->Get_WorldMatrix().r[2]) * 55 + XMVector3Normalize(m_pTransformCom->Get_WorldMatrix().r[0]) * -10.f;
	vSignPos=XMVectorSetY(vSignPos, 3.6f);
	
	_vector vRight = XMVector3Normalize(m_pTransformCom->Get_WorldMatrix().r[0]);
	_vector vLook = XMVector3Normalize( m_pTransformCom->Get_WorldMatrix().r[2]);
	_vector vUp =  XMVector3Normalize(m_pTransformCom->Get_WorldMatrix().r[1]);
	_matrix PrematRot = /*XMMatrixIdentity();*/ XMMatrixRotationAxis(XMVectorSet (1.f,0.f,0.f,0.f),XMConvertToRadians(90.f));
	
	SignMatrix.r[2] = vLook;

	SignMatrix =PrematRot*SignMatrix;
	SignMatrix.r[0] = SignMatrix.r[0] * 10;
	SignMatrix.r[1] = SignMatrix.r[1] * 30;
	SignMatrix.r[3] = vSignPos;

	_float4x4  SignFloat4x4;
	XMStoreFloat4x4(&SignFloat4x4,SignMatrix); 


	if (FAILED(m_pShaderCom_VtxPos->Bind_Matrix("g_WorldMatrix", &SignFloat4x4)))
		return E_FAIL;
	if (FAILED(m_pShaderCom_VtxPos->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom_VtxPos->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;


	if (FAILED(m_pTextureCom[SKILL_TYPE::SKILL_TYPE_RUSH][TEXTURE_TYPE::TEXTURE_TYPE_MONING_BRUSH]->Bind_ShaderResource(m_pShaderCom_VtxPos, "g_MovingTexture")))
		return E_FAIL;

	if (FAILED(m_pTextureCom[SKILL_TYPE::SKILL_TYPE_RUSH][TEXTURE_TYPE::TEXTURE_TYPE_BRUSH]->Bind_ShaderResource(m_pShaderCom_VtxPos, "g_RangeTexture")))
		return E_FAIL;

	if (FAILED(m_pShaderCom_VtxPos->Bind_RawValue("g_fAccTimeDelta", &m_fAccMovingTextureLifeTime, sizeof(_float))))
		return E_FAIL;


	m_pShaderCom_VtxPos->Begin(3);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();

	return S_OK;
}

void CMonster09::CutSceneMoving()
{
	
	 if (true == m_bStartCutScene ) {
		m_pModelCom->Set_AnimSpeed(m_vecAnimStates[(_uint)M9_ANIM::M9_ANIM_ATTACK1_SHOT], 0.5f);
		m_iState = M9_STATE_ATTACK_SHOT;
		m_pModelCom->SetUp_Animation(m_vecAnimStates[(_uint)M9_ANIM::M9_ANIM_ATTACK1_SHOT],false);
		m_isAnimEnd = false;

		m_bStartCutScene = false;
	}
	else if(CONDITION_ATTACK_SHOT &&true== m_isAnimEnd)
	{
		
		m_iState = M9_STATE_DIVE_AFTER_ATTACK;
		m_pModelCom->Move_NextAnim(m_vecAnimStates[(_uint)M9_ANIM::M9_ANIM_DIVE_AFTER_ATTACK_WAVE_RIGHT], 0.2f, false);
		m_isAnimEnd = false;
	}
	

	else if (CONDITION_DIVE_AFTER_ATTACK && true == m_isAnimEnd)
	{
		 m_pModelCom->Set_AnimSpeed(m_vecAnimStates[(_uint)M9_ANIM::M9_ANIM_ATTACK1_SHOT], 1.f);
		m_iState = M9_STATE_SUBMERGE;
		m_isCutSceneNow = false;
	}
	 if ( true ==m_bRoarStart_Sound &&3.5f<=m_fAccCutScene_RoarStart)
	 {
		 m_pGameInstance->PlayEffect(L"Monster09_Roar01.mp3", 0.1f);	
		 m_bRoarStart_Sound = false;
	 }


}

void CMonster09::CutSceneStart()
{
	m_bStartCutScene = true;
	m_isCutSceneNow = true; 

}

void CMonster09::Set_Active()
{
	 m_bActive = true;
	 m_bVisible = true;
	 m_iState = M9_STATE_SUBMERGE;
}

_matrix CMonster09::Get_RushMatrix()
{
	_matrix RushMatrix = XMMatrixIdentity();
	_vector vSignPos = m_pTransformCom->Get_WorldMatrix().r[3] ;
	
	_float YPos = m_vInitPosition.y;
	vSignPos = XMVectorSetY(vSignPos, 5.f);
	_vector vRight = XMVector3Normalize(m_pTransformCom->Get_WorldMatrix().r[0]);
	_vector vLook = XMVector3Normalize(m_pTransformCom->Get_WorldMatrix().r[2]);
	_vector vUp = XMVector3Normalize(m_pTransformCom->Get_WorldMatrix().r[1]);

	RushMatrix.r[2] = vLook;
	RushMatrix.r[0] = RushMatrix.r[0] * 10;
	RushMatrix.r[1] = RushMatrix.r[1] * 30;
	RushMatrix.r[3] = vSignPos;

	return RushMatrix;
}


HRESULT CMonster09::Ready_Components()
{

	/* Com_Navigation */
	CNavigation::NAVIGATION_DESC		NaviDesc{};
	NaviDesc.iStartIndex = m_iInitial_NaviIndex;
	if (FAILED(Add_Component(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_Navigation"),
		TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), &NaviDesc)))
		return E_FAIL;
	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_Shader_VtxAnim_Emissive_Monster09"),
		TEXT("Com_Shader2"), reinterpret_cast<CComponent**>(&m_pShaderCom_Monster09))))
		return E_FAIL;

	
	/* Com_Model */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_Model_Monster09"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;


	/*Com Texture*/
	if (FAILED(Add_Component(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_Texture_Monster09_SignBeforeRush_Moving"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom[SKILL_TYPE::SKILL_TYPE_RUSH][TEXTURE_TYPE::TEXTURE_TYPE_MONING_BRUSH]))))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_Texture_Monster09_SignBeforeRush_Range"),
		TEXT("Com_Texture2"), reinterpret_cast<CComponent**>(&m_pTextureCom[SKILL_TYPE::SKILL_TYPE_RUSH][TEXTURE_TYPE::TEXTURE_TYPE_BRUSH]))))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_Texture_Monster09_Glow_Emissive_Body"),
		TEXT("Com_Texture3"), reinterpret_cast<CComponent**>(&m_pTextureCom_Emissive_Body))))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_Texture_Monster09_Glow_Emissive_Eye"),
		TEXT("Com_Texture4"), reinterpret_cast<CComponent**>(&m_pTextureCom_Emissive_Eye))))
		return E_FAIL;



	/*Com_VIbuffer_Rect*/
	/* Com_VIBuffer */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader_RushSign"), reinterpret_cast<CComponent**>(&m_pShaderCom_VtxPos))))
		return E_FAIL;

	/*스플레팅 보류 */
	m_pTerrainShaderCom = static_cast<CShader*> (m_pGameInstance->Get_Component(LEVEL_GAMEPLAY2,TEXT("Layer_Map"),TEXT("Com_Shader"),1));
	Safe_AddRef(m_pTerrainShaderCom);
	return S_OK;


}

HRESULT CMonster09::Bind_ShaderResources()
{
	__super::Bind_ShaderResources();

	return S_OK;
}

HRESULT CMonster09::Bind_ShaderResources_Glow()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom_Monster09, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom_Monster09->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom_Monster09->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;


	if (FAILED(m_pTextureCom_Emissive_Body->Bind_ShaderResource(m_pShaderCom_Monster09, "g_Emissive_Body_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pTextureCom_Emissive_Eye->Bind_ShaderResource(m_pShaderCom_Monster09, "g_Emissive_Eye_Texture", 0)))
		return E_FAIL;


	_float3 vBodyThreshold = { 0.0f,0.3f,0.35f };
	if (FAILED(m_pShaderCom_Monster09->Bind_RawValue("g_vBodyThreshold", &vBodyThreshold, sizeof(_float3))))
		return E_FAIL;

	
	_float fIntensity = 0.8f;
	if (FAILED(m_pShaderCom_Monster09->Bind_RawValue("g_fIntensity", &fIntensity, sizeof(_float))))
		return E_FAIL;



	return S_OK;
}

HRESULT CMonster09::Ready_PartObjects()
{
	CMonster09_Body::MONSTER09_BODY_DESC BodyDesc = {};
	BodyDesc.fRotationPerSec = XMConvertToRadians(90.f);
	BodyDesc.fSpeedPerSec = 10.f;
	BodyDesc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4_Ptr();
	

	BodyDesc.pSocketMatrix[0] = Get_BoneMatrix("Bone008");
	BodyDesc.pSocketMatrix[1] = Get_BoneMatrix("Bone011");
	BodyDesc.pSocketMatrix[2] = Get_BoneMatrix("Bone014");
	BodyDesc.pSocketMatrix[3] = Get_BoneMatrix("Bone017");
	BodyDesc.pSocketMatrix[4] = Get_BoneMatrix("Bone019");
	BodyDesc.pSocketMatrix[5] = Get_BoneMatrix("Bone022");
	BodyDesc.pSocketMatrix[6] = Get_BoneMatrix("Bone025");
	BodyDesc.pSocketMatrix[7] = Get_BoneMatrix("Bone028");
	BodyDesc.pSocketMatrix[8] = Get_BoneMatrix("Bone031");
	BodyDesc.pSocketMatrix[9] = Get_BoneMatrix("Bone034");
	BodyDesc.pSocketMatrixFor_AngelRing = Get_BoneMatrix("Bone042");
	BodyDesc.pState = &m_iState;
	BodyDesc.pBossHPInfo = m_pBossHPInfo;
	BodyDesc.pHitTime = &m_fHitTime;
	BodyDesc.pAccHitTime = &m_fAccHitTime;
	m_PartObjects[PART_BODY] = static_cast<CPartObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_GAMEPLAY2, TEXT("Prototype_GameObject_Monster09_Body"), &BodyDesc));
	if (nullptr == m_PartObjects[PART_BODY])
		return E_FAIL;



	CMonster09_Head::MONSTER09_HEAD_DESC HeadDesc = {};
	HeadDesc.fRotationPerSec = XMConvertToRadians(90.f);
	HeadDesc.fSpeedPerSec = 10.f;
	HeadDesc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4_Ptr();
	HeadDesc.pSocketMatrix = Get_BoneMatrix("fire_01");
	HeadDesc.pSocketMatrix_Eyes[0] = Get_BoneMatrix("3921_eye_Bone067");
	HeadDesc.pSocketMatrix_Eyes[1] = Get_BoneMatrix("3921_eye_Bone067mi");
	HeadDesc.pSocketMatrix_Eyes[2] = Get_BoneMatrix("3921_eye_Bone068");
	HeadDesc.pState = &m_iState;
	HeadDesc.pNowShotTiming = &m_fAcc_Shot_Timing;
	m_PartObjects[PART_HEAD]= static_cast<CPartObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_GAMEPLAY2, TEXT("Prototype_GameObject_Monster09_Head"), &HeadDesc));
	if (nullptr == m_PartObjects[PART_HEAD])
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster09::Ready_Attack_Waves()
{

	//Prototype_GameObject_Monster09_Wave
	m_vecAttackWaves.resize(3);
	for (int i = 0; i < 3; ++i)
	{
		m_vecAttackWaves[i] = static_cast<CMonster09_Wave*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ,LEVEL_GAMEPLAY2,TEXT("Prototype_GameObject_Monster09_Wave")));
 	}


	return S_OK;
}

HRESULT CMonster09::Ready_Attack_Rush()
{
	pMonster09_Rush = static_cast<CMonster09_Rush*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_GAMEPLAY2, TEXT("Prototype_GameObject_Monster09_Rush")));

	return S_OK;
}

HRESULT CMonster09::Ready_Particles()
{

	
	m_pParticle_Moving = static_cast<CParticle_Monster09_Moving*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_GAMEPLAY2, TEXT("Prototype_GameObject_Particle_Monster09_Moving"), nullptr));
	m_pEffect_Moving =static_cast<CEffect_Monster09_Moving*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_GAMEPLAY2, TEXT("Prototype_GameObject_Effect_Monster09_Moving"), nullptr));
	
	

	return S_OK;
}

CMonster09* CMonster09::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMonster09* pInstance = new CMonster09(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Created CMonster09 Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CMonster09::Clone(void* pArg)
{
	CMonster09* pInstance = new CMonster09(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Clone CMonster09 Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CMonster09::Free()
{
	__super::Free();

	for(int i=0;i<SKILL_TYPE::SKILL_TYPE_END;++i)
		for (int j = 0; j < TEXTURE_TYPE::TEXTURE_TYPE_END; ++j)
			Safe_Release(m_pTextureCom[i][j]);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom_VtxPos);
	Safe_Release(m_pTerrainShaderCom);
	
	Safe_Release(m_pEffect_Moving);
	
	
	Safe_Release(m_pParticle_Moving);
	
	Safe_Release(m_pTextureCom_Emissive_Body);
	Safe_Release(m_pTextureCom_Emissive_Eye);
	Safe_Release(m_pShaderCom_Monster09);

	Safe_Release(pMonster09_Rush);
	for(auto Wave: m_vecAttackWaves)
		Safe_Release(Wave);
	m_vecAttackWaves.clear();
	
}