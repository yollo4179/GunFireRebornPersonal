#include"stdafx.h"
#include "Monster.h"
#include "GameInstance.h"
#include"UI_MonsterHP.h"
#include"Player.h"

#include"Item_Manager.h"

CMonster::CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CContainerObject{ pDevice, pContext }
{

}

CMonster::CMonster(const CMonster& Prototype)
	:CContainerObject(Prototype)
	,m_pItem_Manager(CItem_Manager::GetInstance())
{
	Safe_AddRef(m_pItem_Manager);
}

HRESULT CMonster::Initialize_Prototype()
{


	return S_OK;
}


HRESULT CMonster::Initialize(void* pArg)
{
	MONSTER_DESC* pDesc = static_cast<MONSTER_DESC*>(pArg);


	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (nullptr==pDesc && nullptr != m_pTransformCom)
	{
		m_pTransformCom->Scaling(1, 1, 1);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, 0.f, 0.f, 1.f));
	}
	if (nullptr != pDesc)
	{

		m_iInitial_NaviIndex = pDesc->iNaviIndex;

		_float3 InitScale = pDesc->vScale;
		_float4 InitPos = pDesc->vPosition;
		_float InitAngle = pDesc->fAngle;

		_float4 Axis = { 0.f,1.f,0.f,0.f };
		m_pTransformCom->Scaling(InitScale.x, InitScale.y, InitScale.z);
		m_pTransformCom->Rotation(XMLoadFloat4(&Axis), InitAngle);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4( &InitPos));

		

	}

	


	

	return S_OK;
}

void CMonster::Priority_Update(_float fTimeDelta)
{
	
	if (true == m_bHit_NormalMonster)
	{
		m_fAccShowHPBarTime_GotHit_NormalMonster += fTimeDelta;

		if (m_fAccShowHPBarTime_GotHit_NormalMonster >= m_fShowpBarTime_GotHit_NormalMonster)
		{
			m_fAccShowHPBarTime_GotHit_NormalMonster = 0.f;
			m_bHit_NormalMonster = false;
		}
	}



	if (true ==m_isRemoving)
	{
		m_fAccDesolveTime += fTimeDelta;
	}
	if (m_fAccDesolveTime >= m_fDesolveTime)
	{
		m_bActive = false;
	}


	/*빨피가 0이라면 죽는다*/
	if (m_HPInfo.z <= 0  )
	{
		m_bDead = true;
		if(m_pUIHpBar)//일반 몬스터일 경우에만 
		static_cast<CUI_MonsterHP*>(m_pUIHpBar)->Set_InActive();
	}
	if (m_bDead)
	{
		
		
		if(false == m_isRemoving && nullptr != m_pItem_Manager &&nullptr !=m_pNavigationCom)
		{
			_uint iNumGrenades =static_cast<_uint>(m_pGameInstance->Compute_Random(0.f, 1.5f));
			for (size_t i = 0; i < iNumGrenades;++i)
				m_pItem_Manager->Active_Item(CItem_Manager::DROPITEM_GRENADE, m_pTransformCom->Get_WorldMatrix(), m_pNavigationCom);

			_uint iNumBullet_Special = static_cast<_uint>(m_pGameInstance->Compute_Random(0.f, 1.5f));
			for (size_t i = 0; i < iNumBullet_Special;++i)
				m_pItem_Manager->Active_Item(CItem_Manager::DROPITEM_BULLET_SPECIAL, m_pTransformCom->Get_WorldMatrix(), m_pNavigationCom);

			_uint iNumBullet_Standard = static_cast<_uint>(m_pGameInstance->Compute_Random(1.f, 3.f));
			for (size_t i = 0; i < iNumBullet_Standard;++i)
				m_pItem_Manager->Active_Item(CItem_Manager::DROPITEM_BULLET_STANDARD, m_pTransformCom->Get_WorldMatrix(), m_pNavigationCom);

			_uint iNumCoins = static_cast<_uint>(m_pGameInstance->Compute_Random(3.f, 5.f));
			for (size_t i = 0; i < iNumCoins;++i)
				m_pItem_Manager->Active_Item(CItem_Manager::DROPITEM_COIN, m_pTransformCom->Get_WorldMatrix(), m_pNavigationCom);
		}

		m_isRemoving = true;

	}
	/*Hit Time*/
	{
		if(m_fAccHitTime<5.f)
		m_fAccHitTime += fTimeDelta;

	}

	/*UI_HPBar*/
	{
		_matrix matCamera = (m_pGameInstance->Get_TransformInversedMatrix(CPipeLine::D3DTS_VIEW));
		
		
		//원래 카메라 룩이 맞음
		_vector vPlayerLook = matCamera.r[2];
		_vector vPlayerPos = matCamera.r[3];
		_vector vMonsterPos = m_pTransformCom->Get_WorldMatrix().r[3];

		_vector vTargetDir =   vMonsterPos- vPlayerPos;
		_float fDot = XMVectorGetX( XMVector3Dot(vPlayerLook, vTargetDir));
		_bool isCounterDir = (fDot >= 0) ? true : false;

		if (m_pUIHpBar&&false ==m_bDead) {
			if (m_isMonsterPicked)
				m_fAccShowHPBarTime = 0.f;
			else
				m_fAccShowHPBarTime += fTimeDelta;

			
			if (m_pGameInstance->isIn_Frustum_InWorldSpace(m_pTransformCom->Get_WorldMatrix().r[3], 2.f)&&isCounterDir &&(m_bHit_NormalMonster))
				m_pUIHpBar->Set_Active();
			else
				m_pUIHpBar->Set_InActive();
			/*ForHPBarUI*/
			{
				Pri_Update_UI_HpBarMatrix();
			}
		}
	}
	__super::Priority_Update(fTimeDelta);
}

void CMonster::Update(_float fTimeDelta)
{
	/*For NormalHPBarUI*/
	{
		if (m_pUIHpBar)
			Update_Get_ViewZDistance();//transform 게산은 priupdate에서 처리
	}
	/*Decouping with other Monsters*/
	{
		if (m_pColliderCom_Range) {
			_matrix matPos = m_pTransformCom->Get_WorldMatrix();
			matPos.r[3]= XMVectorSetY(matPos.r[3], XMVectorGetY(matPos.r[3]) + 3.f);
			m_pColliderCom_Range->Update(matPos );

			m_pGameInstance->Add_Collision_Group(CCollider_Manager::COLLISION_GROUP::COLLISION_MONSTER_RANGE, this, m_pColliderCom_Range);
		}
	}
	__super::Update(fTimeDelta);
}

void CMonster::Late_Update(_float fTimeDelta)
{

	CGameObject* pTarget = { nullptr };
	
	if (m_pColliderCom_Range) {
		if (m_pGameInstance->Check_Collision(m_pColliderCom_Range, CCollider_Manager::COLLISION_GROUP::COLLISION_MONSTER_RANGE, &pTarget))
		{
			_vector vTargetPos = pTarget->Get_WorldMatrix().r[3];
			_vector vMyPos = m_pTransformCom->Get_WorldMatrix().r[3];
			_vector vPushingDir = vTargetPos - vMyPos;
			_float fPower = 0.15f;
			vPushingDir = XMVector3Normalize(vPushingDir);
			CTransform* pTargetTransform = pTarget->Get_Transform();
			CNavigation* pTargetNavigation = static_cast<CMonster*>(pTarget)->Get_Navigation();
			vPushingDir = XMVector3Normalize(vPushingDir);
			if ( XMVectorGetX(XMVector3Dot(vPushingDir, XMLoadFloat4(&m_vPrevPushedDir))) >= 0) {
				pTargetTransform->Go_Pushed(fTimeDelta * fPower, vPushingDir, pTargetNavigation);
				pTargetTransform->Go_Backward(fTimeDelta * fPower, pTargetNavigation);
			}
			
			XMStoreFloat4(&m_vPrevPushedDir, vPushingDir);
		}
		else
		{
			m_vPrevPushedDir = {};
		}
		if (m_pGameInstance->Check_Collision(m_pColliderCom_Range, CCollider_Manager::COLLISION_GROUP::COLLISION_PLAYER, &pTarget))
		{
			_vector vTargetPos = pTarget->Get_WorldMatrix().r[3];
			_vector vMyPos = m_pTransformCom->Get_WorldMatrix().r[3];
			_vector vPushingDir = vTargetPos - vMyPos;
			_float fPower = 2.f;
			vPushingDir = XMVector3Normalize(vPushingDir);
			CTransform* pTargetTransform = pTarget->Get_Transform();
			CNavigation* pTargetNavigation = static_cast<CPlayer*>(pTarget)->Get_Navigation();
			pTargetTransform->Go_Pushed(fTimeDelta * fPower, vPushingDir, pTargetNavigation);
			vPushingDir=XMVector3TransformCoord(vPushingDir, XMMatrixRotationY(90.f));
			pTargetTransform->Go_Pushed(fTimeDelta * fPower, vPushingDir, pTargetNavigation);
		}


	}



	__super::Late_Update(fTimeDelta);
#ifdef _DEBUG
	m_pGameInstance->Add_DebugComponent(m_pColliderCom_Range);
#endif
}

HRESULT CMonster::Render()
{
	
	if (m_isRemoving)/*For NormalMonster*/ // -> 이거쓰고 싶으면 보스몬스터도 구조 바꿔야함 
	{

		if (m_bGlow)
		{
			if (FAILED(Bind_ShaderResources_Desolve_Glow()))
				return E_FAIL;
			_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

			for (size_t i = 0; i < iNumMeshes; i++)
			{
				if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE, 0)))
					return E_FAIL;
				if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i)))
					return E_FAIL;

				m_pShaderCom->Begin(4);

				m_pModelCom->Render(i);
			}
		}
		else {

			if (FAILED(Bind_ShaderResources_Desolve()))
				return E_FAIL;
			_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

			for (size_t i = 0; i < iNumMeshes; i++)
			{
				if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE, 0)))
					return E_FAIL;
				if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i)))
					return E_FAIL;

				m_pShaderCom->Begin(3);

				m_pModelCom->Render(i);
			}
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

const _float4x4* CMonster::Get_BoneMatrix(const _char* pBoneName) const
{
	return m_pModelCom->Get_BoneMatrix(pBoneName);
}

_bool CMonster::Util_isPlayer_InRange(_float fRange)
{
	if (nullptr == m_pPlayerFloat4x4)
	{
		MSG_BOX("SET PLAYERmATRIX First");
		return false;
	}

	_float PlayerX = m_pPlayerFloat4x4->_41;
	_float PlayerZ = m_pPlayerFloat4x4->_43;

	_vector MonsterPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float4 MonsterPos_Float4 = {};
	XMStoreFloat4(&MonsterPos_Float4,MonsterPos);

	_float fdistX = PlayerX - MonsterPos_Float4.x;
	_float fdistZ = PlayerZ - MonsterPos_Float4.z;
	


	return (fdistX*fdistX + fdistZ* fdistZ  < fRange* fRange );
}

_int CMonster::Util_TurnTo_Player(_float fTimeDelta)
{
	
	CTransform* TargetTransform = static_cast<CTransform*>(m_pGameInstance->Get_Component(m_pGameInstance->Get_Current_Level(), TEXT("Layer_APlayer"), TEXT("Com_Transform")));
	
	_float PlayerX =TargetTransform->Get_WorldFloat4x4_Ptr()->_41;
	_float PlayerZ = TargetTransform->Get_WorldFloat4x4_Ptr()->_43;
	//_float PlayerX = m_pPlayerFloat4x4->_41;
	//_float PlayerZ = m_pPlayerFloat4x4->_43;

	_vector vPlayerPos = DirectX::XMVectorSet(PlayerX, 0, PlayerZ,1.f);

	
	_vector vMonsterPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	//vMonsterPos = DirectX::XMVectorSet(DirectX::XMVectorGetX(vMonsterPos), 0.f, DirectX::XMVectorGetZ(vMonsterPos), 1.f);
	vMonsterPos = XMVectorSetY(vMonsterPos, 0.F);;
	_vector vMonsterLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);


	_vector vMonsterToPlayerDir = DirectX::XMVectorSubtract( vPlayerPos , vMonsterPos);

	vMonsterToPlayerDir = DirectX::XMVector3Normalize(vMonsterToPlayerDir);
	vMonsterLook = DirectX::XMVector3Normalize(vMonsterLook);


	_vector Result_Cross = DirectX:: XMVector3Cross(vMonsterLook, vMonsterToPlayerDir);


	_float fCosTheta = DirectX::XMVectorGetX( DirectX::XMVector3Dot(DirectX::XMVector3Normalize(vMonsterToPlayerDir), DirectX::XMVector3Normalize(vMonsterLook)));
	//cos Theta
	
	_float fRadian = acos(fCosTheta);

	if (fCosTheta <= -1)
	{
		fRadian = XMConvertToRadians(360) - fRadian;
	}

	
	_float4 Result_Float4 = {};

 	XMStoreFloat4(&Result_Float4,Result_Cross);


	_float fLimitRadian = abs(XMConvertToRadians(5));
	if (fRadian > fLimitRadian && 0 < Result_Float4.y   )
		m_pTransformCom->Turn(DirectX::XMVectorSet(0.f,1.f,0.f,0.f),fTimeDelta);

	else if(fRadian >fLimitRadian && 0>Result_Float4.y  )
		m_pTransformCom->Turn(DirectX::XMVectorSet(0.f, 1.f, 0.f, 0.f), -1.f *fTimeDelta);

	

	return _int();
}



void CMonster::Util_Go_Right(_float fTimeDelta, _float fWalkSpeed)
{
	m_bCan_GoRight = m_pTransformCom->Go_Right(fTimeDelta * fWalkSpeed,m_pNavigationCom);
}

void CMonster::Util_Go_Left(_float fTimeDelta, _float fWalkSpeed)
{
	m_bCan_GoLeft = m_pTransformCom->Go_Left(fTimeDelta * fWalkSpeed,m_pNavigationCom);
}

void CMonster::Util_Go_Back(_float fTimeDelta, _float fWalkSpeed)
{
	 m_pTransformCom->Go_Backward(fTimeDelta * fWalkSpeed, m_pNavigationCom);
}

void CMonster::Util_Go_Straight(_float fTimeDelta, _float fWalkSpeed)
{
 
	m_bCan_GoStraight = m_pTransformCom->Go_Straight(fTimeDelta * fWalkSpeed,true, m_pNavigationCom);
}

void CMonster::Pri_Update_NaviMesh()
{
	{
		_vector vPos = m_pNavigationCom->SetUp_HeightOnNavigation(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
		vPos = XMVectorSetY(vPos, XMVectorGetY(vPos));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	}

}

void CMonster::Pri_Update_UI_HpBarMatrix()
{
	
		/*소켓 메트릭스는 머즐본의(위치)*/
		_matrix SocketMatrix = XMLoadFloat4x4(&m_HPBarSocketMatrix);
		for (size_t j = 0; j < 3; ++j)
		{
			SocketMatrix.r[j] = XMVector3Normalize(SocketMatrix.r[j]);
		}
		XMStoreFloat4x4(&m_HPBarMatrix, SocketMatrix * m_pTransformCom->Get_WorldMatrix());


}

void CMonster::Update_Get_ViewZDistance()
{
	_vector vCameraPos =XMLoadFloat4(m_pGameInstance->Get_CamPosition());
	_vector vMyPos = (m_pTransformCom->Get_WorldMatrix()).r[3];
	_vector vViewZ = vCameraPos - vMyPos;
	_float fViewZDistance =XMVectorGetX( XMVector3Length(vViewZ));
	m_fViewZ_Distance = fViewZDistance;
}


void CMonster::Update_Get_isFocused()
{
}



void CMonster::Set_PlayerFloat4x4(const _float4x4* pPlayerFloat4x4)
{
	m_pPlayerFloat4x4 = pPlayerFloat4x4;
}



HRESULT CMonster::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Monster1"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Monster_Desolve"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom_Desolve))))
		return E_FAIL;
	
	

	return S_OK;


}

HRESULT CMonster::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	const LIGHT_DESC* pLightDesc = m_pGameInstance->Get_LightDesc(0);


	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDir", &pLightDesc->vDirection, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightAmbient", &pLightDesc->vAmbient, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster::Bind_ShaderResources_Desolve()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	_float NoiseStrangth = 1.f;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_NoiseStrength",reinterpret_cast<void*>( & NoiseStrangth), sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_NoiseMaxTime", reinterpret_cast<void*>(&m_fDesolveTime), sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_NoiseCurTime", reinterpret_cast<void*>(&m_fAccDesolveTime), sizeof(_float))))
		return E_FAIL;

	_float GlowIntensity = 0.5f;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_GlowIntensity", reinterpret_cast<void*>(&GlowIntensity), sizeof(_float))))
		return E_FAIL;


	if (FAILED(m_pTextureCom_Desolve->Bind_ShaderResource(m_pShaderCom, "g_NoiseTexture", 0)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster::Bind_ShaderResources_Desolve_Glow()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	_float NoiseStrangth = 1.f;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_NoiseStrength", reinterpret_cast<void*>(&NoiseStrangth), sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_NoiseMaxTime", reinterpret_cast<void*>(&m_fDesolveTime), sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_NoiseCurTime", reinterpret_cast<void*>(&m_fAccDesolveTime), sizeof(_float))))
		return E_FAIL;

	_float GlowIntensity = 0.5f;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_GlowIntensity", reinterpret_cast<void*>(&GlowIntensity), sizeof(_float))))
		return E_FAIL;


	if (FAILED(m_pTextureCom_Desolve->Bind_ShaderResource(m_pShaderCom, "g_NoiseTexture", 0)))
		return E_FAIL;
	
	/*ForGlow*/
	_float fIntensity = 0.3f; 
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fIntensity", reinterpret_cast<void*>(&fIntensity), sizeof(_float))))
		return E_FAIL;



	
	return S_OK;
}

HRESULT CMonster::Ready_UI_HPBar_LEVEL_GAMEPLAY()
{

	CUI_MonsterHP::UI_MONSTER_HP_DESC UI_HPDesc = {};
	UI_HPDesc.fRotationPerSec = XMConvertToRadians(90.F);
	UI_HPDesc.fSizeX = 0.f;
	UI_HPDesc.fSizeY = 0.f;
	UI_HPDesc.eType = UI_TYPE::UI_MONSTER;
	UI_HPDesc.fSpeedPerSec = 10.f;
	UI_HPDesc.fX = 100.f;
	UI_HPDesc.fY = 100.f;
	UI_HPDesc.UIWorlFloat4x4 = &m_HPBarMatrix;
	UI_HPDesc.iData = 0.f;
	UI_HPDesc.pViewZDistance = &m_fViewZ_Distance;
	UI_HPDesc.pHPInfo = &m_HPInfo;
	m_pUIHpBar = { nullptr };
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_MonsterHP"),
		LEVEL_GAMEPLAY, L"Layer_MonsterHp", &UI_HPDesc, reinterpret_cast<CGameObject**>(&m_pUIHpBar))))
		return E_FAIL;
	m_pUIHpBar->Set_InActive();
	m_pUIHpBar->Set_SortDepth(8);

	return S_OK;
}

CMonster* CMonster::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMonster* pInstance = new CMonster(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Created Monster_ Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CMonster::Clone(void* pArg)
{
	CMonster* pInstance = new CMonster(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Clone Monster01 Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CMonster::Free()
{
	
	__super::Free();
	
	Safe_Release(m_pItem_Manager);
	Safe_Release(m_pColliderCom_Range);
	Safe_Release(m_pUIHpBar);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pTextureCom_Desolve);
	Safe_Release(m_pNavigationCom);
}

