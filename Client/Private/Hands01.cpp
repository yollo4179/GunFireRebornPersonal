#include"stdafx.h"
#include "Hands01.h"
#include"Hands01_Sword.h"
#include"GameInstance.h"
#include"Player.h"
#include"UI_Arms.h"

CHands01::CHands01(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CHands(pDevice,pContext)
{
}

CHands01::CHands01(const CHands01& Prototype)
	:CHands(Prototype)
{
}

const _float4x4* CHands01::Get_BoneMatrix(const _char* pBoneName) const
{
	return m_pModelCom->Get_BoneMatrix(pBoneName);
}

HRESULT CHands01::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CHands01::Initialize(void* pArg)
{
	HANDS01_DESC* pDesc = static_cast<HANDS01_DESC*>(pArg);
	m_pState = pDesc->pState;
	m_pPartObjects = pDesc->pPartObjects;

	m_HandsInfo = g_HandsInfo[0];
	m_isInfinity = true;
	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pModelCom->SetUp_Animation(1, true);//일단 3번 애니메이션으로 세팅하자 

	/* For Contolling Animation*/
	m_vecStates.resize((_uint)HANDS01_STATE::STATE_END);
	if (FAILED(Init_Mapping_Index_To_State()))
		return E_FAIL;
	if (FAILED(Init_AnimSpeed()))
		return E_FAIL;
	m_iNumBullets_ICanFireNow =m_iNumBullets_YouHave = m_HandsInfo.fMaxLoadBullets;
	
	return S_OK;
}

HRESULT CHands01::Init_Mapping_Index_To_State()
{

	m_vecStates[(_uint)HANDS01_STATE::STATE_ATTACK01] = 1;
	m_vecStates[(_uint)HANDS01_STATE::STATE_ATTACK02] = 2;
	m_vecStates[(_uint)HANDS01_STATE::STATE_ATTACK03] = 7;
	m_vecStates[(_uint)HANDS01_STATE::STATE_IDLE] = 0;
	m_vecStates[(_uint)HANDS01_STATE::STATE_LOAD] = 6;
	m_vecStates[(_uint)HANDS01_STATE::STATE_POP_OUT] = 3;
	return S_OK;
}

HRESULT CHands01::Init_AnimSpeed()
{
	m_pModelCom->Set_AnimSpeed(m_vecStates[(_uint)HANDS01_STATE::STATE_ATTACK01], 1.2f);
	m_pModelCom->Set_AnimSpeed(m_vecStates[(_uint)HANDS01_STATE::STATE_ATTACK02], 1.2f);
	m_pModelCom->Set_AnimSpeed(m_vecStates[(_uint)HANDS01_STATE::STATE_ATTACK03], 1.2f);


	return S_OK;
}

void CHands01::Priority_Update(_float fTimeDelta)
{
	if (false == m_bActive)
		return;
	/* 컨테이너 오브젝트에서 키입력에따른 상태 변화를 이미 적용한 상태*/
}

void CHands01::Update(_float fTimeDelta)
{
	if (false == m_bActive)
		return;
	//만약에 공격이 일정 시간이 지나면 중공격이나 강공격을 못하게 막기

	/*For Atttack*/
	if (m_bAttackStart)
	{
		m_fAccAttackTime += fTimeDelta;
	}
	{
		
		if (m_fAccAttackTime >= 0.27f) {
			if (3 == m_iAttackCount && false == m_bAttackEnd) {
				m_fAccAttackTime = 0.f;
				m_bAttackChance = true;
				m_bAttackStart = false;

				m_HandsInfo.fCriticalDemage *= 1.4;
				m_HandsInfo.fCriticalRate = 100;
				m_HandsInfo.fDemage = (_uint)(1.5 * m_HandsInfo.fDemage);
				m_bAttackEnd = true;
			}
		}

		if (m_fAccAttackTime >= 0.3f)
		{
			m_fAccAttackTime = 0.f;
			m_bAttackChance = true;
			m_bAttackStart = false;

			if (2 == m_iAttackCount && false == m_bAttackEnd) {
				m_HandsInfo.fCriticalDemage *= 1.2;
				m_HandsInfo.fCriticalRate = 100;
				m_HandsInfo.fDemage = (_uint)(1.3 * m_HandsInfo.fDemage);
				m_bAttackEnd = true;
			}

		}
	}
	

	/*For UI*/
	m_pArms->Set_BulletsICanFire(m_iNumBullets_ICanFireNow);
	m_pArms->Set_BulletsIHaveNow(m_iNumBullets_YouHave);
	m_pArms->Set_IsInfinityBullets(m_isInfinity);
	if (true == m_bStartSetUp && m_isAnimEnd)
	{
		*m_pState |= CPlayer::STATE_IDLE;
		*m_pState ^= CPlayer::STATE_POP_OUT;
		m_bStartSetUp = false;
		
	}
	if (CONDITION_LOAD && m_isAnimEnd)
	{
		*m_pState |= CPlayer::STATE_IDLE;
		*m_pState ^= CPlayer::STATE_LOAD;
	}

	
	if (CONDITION_ATTACK|| CONDITION_ATTACK_UP)
	{
		if (true != m_isAnimEnd)
		{
		}
		else {
			if (CONDITION_ATTACK_PRESSING) {
				m_isAnimEnd = true;

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
	}

	if (3 == m_iAttackCount && m_isAnimEnd)
	{
		m_iAttackCount = 0;
	}

	if (!CONDITION_ATTACK)
	{
		if (m_fAttackDuration < 8.f)
			m_fAttackDuration += fTimeDelta;
		
	}
	if (!CONDITION_ATTACK_DOWN &&m_fAttackDuration > 5.f)
	{
		m_iAttackCount = 0;
		m_fAttackDuration = 0.f;
	}
	if (CONDITION_ATTACK)
	{
		m_fAttackDuration = 0.f;
	}


	/*to Pop_Up*/
	if ((CPlayer::STATE_POP_OUT & *m_pState) && false == m_bStartSetUp) {

		m_pModelCom->SetUp_Animation(m_vecStates[(_uint)HANDS01_STATE::STATE_POP_OUT], false);
		m_bStartSetUp = true;
		/*For UI*/
		m_pArms->Set_BulletsICanFire(m_iNumBullets_ICanFireNow);
		m_pArms->Set_BulletsIHaveNow(m_iNumBullets_YouHave);
		m_pGameInstance->PlayEffect(L"Player01_Reload.ogg", 0.1f);
	}
	
	
	/*To Attack1 */
	else if (CONDITION_ATTACK)
	{
		m_fAttackDuration += fTimeDelta;

		//마우스가 지금 눌렸는지 한번 더확인 -> 보간중에 보간할것인자,  긑나고 보간할 것인지 결정
		if (CONDITION_LOOP || CONDITION_LOAD || (/*m_isAnimEnd*/ m_fAttackDuration>0.2f)&& (MOUSE_DOWN(MOUSE::DIM_LB) || MOUSE_PRESSING(MOUSE::DIM_LB))) 
		{

			bool bSlerping = false;
			_float fDistance = 0.2f;
			if (m_isChangingMotion) //보간중에 보간
			{
				bSlerping = true;
				
			}
			/*For UI*/
			if (__super::Fire_A_Bullet())
			{
				m_pArms->Set_BulletsICanFire(m_iNumBullets_ICanFireNow);
			}

			if (0 == m_iAttackCount)
			{
				m_bAttackStart = true;
				m_bAttackEnd = false;
				m_pModelCom->Move_NextAnim(m_vecStates[(_uint)HANDS01_STATE::STATE_ATTACK01], fDistance, false, bSlerping);
				++m_iAttackCount;
				m_pGameInstance->PlayEffect(L"Player01_Sword.mp3", 0.1f);
			}
			else if (1 == m_iAttackCount)
			{
				m_bAttackStart = true;
				m_bAttackEnd = false;
				m_pModelCom->Move_NextAnim(m_vecStates[(_uint)HANDS01_STATE::STATE_ATTACK02], fDistance, false, bSlerping);
				++m_iAttackCount;
				m_pGameInstance->PlayEffect(L"Player01_Sword.mp3", 0.1f);
			}
			else if (2 == m_iAttackCount)
			{
				m_bAttackStart = true;
				m_bAttackEnd = false;
				m_pModelCom->Move_NextAnim(m_vecStates[(_uint)HANDS01_STATE::STATE_ATTACK03], fDistance, false, bSlerping);
				m_iAttackCount=3;
				m_pGameInstance->PlayEffect(L"Player01_Sword.mp3", 0.1f);
			}
			if (CONDITION_IDLE)//루프 제거
			{
				*m_pState ^= CPlayer::STATE_IDLE;
			}
			if (CONDITION_LOAD)//장전 캔슬
			{
				*m_pState ^= CPlayer::STATE_LOAD;
			}
		}
	}

	/* to Load*/

	else if (CONDITION_LOAD)
	{
		if (CONDITION_LOOP || m_isAnimEnd)
		{
			/*For UI*/
			if (__super::Load_Bullets())
			{
				m_pArms->Set_BulletsICanFire(m_HandsInfo.fMaxLoadBullets);
				m_pArms->Set_BulletsIHaveNow(m_iNumBullets_YouHave = m_iNumBullets_ICanFireNow =m_HandsInfo.fMaxLoadBullets);
			}
			m_pGameInstance->PlayEffect(L"Player01_Reload.ogg", 0.1f);
			m_pModelCom->Move_NextAnim(m_vecStates[(_uint)HANDS01_STATE::STATE_LOAD], 0.2f, false);
			if (CONDITION_IDLE)//루프 제거
			{
				*m_pState ^= CPlayer::STATE_IDLE;
			}
		}
	}
	
	

	/*  to Idle*/
	else if (CONDITION_IDLE && m_isAnimEnd)
	{
		/* 다른 애니메이션끝났을때만 한번 보간 후 호출*/


		m_pModelCom->Move_NextAnim(m_vecStates[(_uint)HANDS01_STATE::STATE_IDLE], 0.202f, true);
		m_DebugCount++;
	}
		/*---------------------------------------------------------------------------------------------------------------*/
		
		/*---------------------------------------------------------------------------------------------------------------*/
		
		/*---------------------------------------------------------------------------------------------------------------*/
		
		
		/*---------------------------------------------------------------------------------------------------------------*/
		
		/*---------------------------------------------------------------------------------------------------------------*/
		

		/*---------------------------------------------------------------------------------------------------------------*/
	


	/*For Debuging*/
		if(CONDITION_LOOP)
			int a = 0;
		if(CONDITION_LOAD)
			int a = 0;
		if (CONDITION_ATTACK) {
			if (m_iAttackCount == 0)
			{
				int a = 0;
			}
			if (m_iAttackCount == 1)
			{
				int a = 0;
			}
			if (m_iAttackCount == 2)
			{
				int a = 0;
			}
			if (m_iAttackCount == 3)
			{
				int a = 0;
			}
			int  b = 0;
		}
	/*상태에 따른 애니메이션 세팅을 해준 다음에 애니메이션을 돌린다. */


		m_isAnimEnd=m_pModelCom->Play_Animation(fTimeDelta, m_isChangingMotion);

	if (m_isAnimEnd == true)
		int a = 0;
//바뀐 플레이어(컨테이너의 월드행렬에 따라서 나의 최종 행렬을 갱신해준다. 
	DirectX::XMStoreFloat4x4(&m_WorldMatrix, m_pTransformCom->Get_WorldMatrix()*XMLoadFloat4x4(m_pParentMatrix));
	                                  //나의 월드(로컬(항등* 뼈에 배치)* (부모위치로 이동)
}

void CHands01::Late_Update(_float fTimeDelta)
{
	if (false == m_bActive)
		return;

	//상태에 따라 애니메이션을 바꿔준다.
	/*if (*m_pState & CPlayer::STATE_IDLE)
		m_pModelCom->SetUp_Animation(0, true);

	else if (*m_pState & CPlayer::STATE_WALK)
		m_pModelCom->Move_NextAnim(2 , 0.2f ,true);*/

	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
	m_pGameInstance->Add_RenderObject(CRenderer::RG_GLOW, this);
}

HRESULT CHands01::Render()
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

HRESULT CHands01::Render_Glow()
{
	if (FAILED(Bind_ShaderResources_Glow()))
		return E_FAIL;

	if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom_Glow, "g_DiffuseTexture", 2, aiTextureType_DIFFUSE, 0)))
		return E_FAIL;
	if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom_Glow, "g_BoneMatrices", 2)))
		return E_FAIL;

	m_pShaderCom_Glow->Begin(2);

	m_pModelCom->Render(2);
	return S_OK;
}

void CHands01::SetActive(_bool bActiveState)
{
	m_bActive = bActiveState;
	if (m_pPartObjects)
		static_cast<CHands01_Sword*>((*m_pPartObjects)[CPlayer::PART_HANDS01_SWORD])->Set_Active(bActiveState);
	
}

HRESULT CHands01::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Model  for Anim*/
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Player01"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
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

HRESULT CHands01::Bind_ShaderResources()
{
	/*if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;*/
	//이제 월드는 트랜스폼이 아니라 파트오브젝트가 들고있다 
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	const LIGHT_DESC* pLightDesc = m_pGameInstance->Get_LightDesc(0);

	return S_OK;
}

HRESULT CHands01::Bind_ShaderResources_Glow()
{
	if (FAILED(m_pShaderCom_Glow->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom_Glow->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom_Glow->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	_float3 vThreshold = { 0.f,0.5f,0.f };
	if (FAILED(m_pShaderCom_Glow->Bind_RawValue("g_vThreshold", &vThreshold, sizeof(_float3))))
		return E_FAIL;
	_float vIntensity = 1.f;
	if (FAILED(m_pShaderCom_Glow->Bind_RawValue("g_fIntensity", &vIntensity, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pTextureCom_Glow->Bind_ShaderResource(m_pShaderCom_Glow, "g_EmissiveTexture", 0)))
		return E_FAIL;
	

	return S_OK;
}

CHands01* CHands01::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CHands01* pInstance = new CHands01(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed Create CHands01");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CHands01::Clone(void* pArg)
{
	CGameObject* pInstance = new CHands01(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed CHands01 :Clone");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CHands01::Free()
{
	__super::Free();
	g_HandsInfo[0] = m_HandsInfo;

	/*ForGlow*/
	Safe_Release(m_pShaderCom_Glow);
	Safe_Release(m_pTextureCom_Glow);
}
