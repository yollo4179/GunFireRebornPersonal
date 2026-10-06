#include"stdafx.h"
#include"Monster09_Mini_Shot.h"
#include"GameInstance.h"
CMonster09_Mini_Shot::CMonster09_Mini_Shot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject{pDevice, pContext}
{
}

CMonster09_Mini_Shot::CMonster09_Mini_Shot(const CMonster09_Mini_Shot& Prototype)
	:CGameObject(Prototype)
{
}

HRESULT CMonster09_Mini_Shot::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonster09_Mini_Shot::Initialize(void* pArg)
{
	MONSTER09_MINI_SHOT_DESC* pDesc = static_cast<MONSTER09_MINI_SHOT_DESC*>(pArg);

	m_fSpeed = pDesc->fSpeed;


	if (FAILED(__super::Initialize(pArg)))
	{
		return E_FAIL;
	}

	_matrix matWorld = XMMatrixIdentity();
	Reset_Bullets(matWorld);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CMonster09_Mini_Shot::Priority_Update(_float fTimeDelta)
{

	if (m_isActive && m_fAccNowLifeTime < m_fLifeTimeLimit)
	{
		m_fAccNowLifeTime += fTimeDelta;
	}
	else
	{
		m_isActive = false;
	}

	if (m_isActive)
	{

		/*Jumping Action */
		_vector vPos =m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		m_fCurHeight = XMVectorGetY(vPos);

		m_fCurHeight  += m_v0 * fTimeDelta;
		m_v0 -= m_fGravity * fTimeDelta;
		if (3.8f >= m_fCurHeight)
		{
			m_isActive = false;
		}
		_vector vPosUpdated = XMVectorSetY(vPos,m_fCurHeight);
	
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosUpdated);

		m_pTransformCom->Go_Straight(1 * fTimeDelta * m_fSpeed);
		m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());
	}

}

void CMonster09_Mini_Shot::Update(_float fTimeDelta)
{
	if (m_isActive)
	{
		m_pGameInstance->Add_Collision_Group(CCollider_Manager::COLLISION_GROUP::COLLISION_MONSTER_ATTACK, this, m_pColliderCom);
	}
}

void CMonster09_Mini_Shot::Late_Update(_float fTimeDelta)
{
	if (m_isActive)
	{
		CGameObject* pTargetObject = nullptr;
		if (m_pGameInstance->Check_Collision(m_pColliderCom, CCollider_Manager::COLLISION_GROUP::COLLISION_PLAYER, &pTargetObject))
		{
		}

		m_pGameInstance->Add_RenderObject(CRenderer::RG_DISTORTION, this);
		m_pGameInstance->Add_RenderObject(CRenderer::RG_GLOW, this);
	}
#ifdef _DEBUG
	if(m_isActive)
	m_pGameInstance->Add_DebugComponent(m_pColliderCom);
#endif
}
HRESULT CMonster09_Mini_Shot::Render()
{
	if (true == m_bGlow)
	{
		if (FAILED(Bind_SHaderResources_Glow()))
			return E_FAIL;

		_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

		for (size_t i = 2; i <= 2; i++)//1번 인덱스 : 구,( diffuse )  2번 인덱스 ( noise )
		{
			//(메시마다의 diffuse텍스처 던지기)
			if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE, 0)))
				return E_FAIL;
			//셰이더 제출
			m_pShaderCom->Begin(2);

			//정점 그리기
			m_pModelCom->Render(i);
		}
	}
	else {

		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

		for (size_t i = 2; i <= 2; i++)//1번 인덱스 : 구,( diffuse )  2번 인덱스 ( noise )
		{
			//(메시마다의 diffuse텍스처 던지기)
			if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE, 0)))
				return E_FAIL;
			//셰이더 제출
			m_pShaderCom->Begin(1);

			//정점 그리기
			m_pModelCom->Render(i);
		}
	}
	return S_OK;
}

void CMonster09_Mini_Shot::Set_Active(_bool isActive)
{
	CTransform* m_pTargetTransform = static_cast<CTransform*>(m_pGameInstance->Get_Component(m_pGameInstance->Get_Current_Level(), TEXT("Layer_APlayer"), TEXT("Com_Transform"), 0));
	if (nullptr == m_pTargetTransform)
		return;

	_vector vTargetPos =m_pTargetTransform->Get_State(CTransform::STATE_POSITION);
	
	_vector vMyPos =m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float fVelocityByLength = XMVectorGetX(XMVector3Length(vTargetPos - vMyPos));

	m_v0 = fVelocityByLength*0.15;
	m_isActive = isActive; 
}

void CMonster09_Mini_Shot::Reset_Bullets(_fmatrix matInitialShootingPos)
{
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, XMVector3Normalize(matInitialShootingPos.r[0]));
	m_pTransformCom->Set_State(CTransform::STATE_UP, XMVector3Normalize(matInitialShootingPos.r[1]));
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, XMVector3Normalize(matInitialShootingPos.r[2]));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, matInitialShootingPos.r[3]);
	m_pTransformCom->Scaling(1.f, 1.f, 1.f);
	m_pGameInstance->PlayEffect(L"Monster09_Mini_Attack.mp3", 0.1f);
	m_fAccNowLifeTime = 0.f;

}

HRESULT CMonster09_Mini_Shot::Ready_Components()
{
	CBounding_Sphere::BOUND_SPHERE_DESC ColliderDesc{};
	ColliderDesc.vCenter = _float3(0.f, 0.f, 0.f);
	ColliderDesc.fRadius = 0.5f;

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;
	/*Model*/
	if (FAILED(Add_Component(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_Model_Monster09_Shot"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;


	/*ComTexture*/
	if (FAILED(Add_Component(LEVEL_GAMEPLAY2, TEXT("Prototype_Component_Texture_Monster09_Shot_Noise"),
		TEXT("Com_Texture1"), reinterpret_cast<CComponent**>(&m_pTextureCom_Noise))))
		return E_FAIL;



	return S_OK;
}

HRESULT CMonster09_Mini_Shot::Bind_ShaderResources()
{
	_matrix ModelMatrix = m_pTransformCom->Get_WorldMatrix();

	ModelMatrix.r[0] *= 0.1f;
	ModelMatrix.r[1] *= 0.1f;
	ModelMatrix.r[2] *= 0.1f;
	_float4x4 ModelFloat4x4;
	XMStoreFloat4x4(&ModelFloat4x4, ModelMatrix);


	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pShaderCom, "g_ScreenTexture", TEXT("Target_Final"))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &ModelFloat4x4)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;





	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Time", &m_fAccNowLifeTime, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom_Noise->Bind_ShaderResource(m_pShaderCom, "g_NoiseTexture", 0)))
		return E_FAIL;



	_float2 vWinSize = { (_float)g_iWinSizeX,(_float)g_iWinSizeY };
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vWinSize", &vWinSize, sizeof(_float2))))




		return S_OK;
}

HRESULT CMonster09_Mini_Shot::Bind_SHaderResources_Glow()
{
	_matrix ModelMatrix = m_pTransformCom->Get_WorldMatrix();

	ModelMatrix.r[0] *= 0.1f;
	ModelMatrix.r[1] *= 0.1f;
	ModelMatrix.r[2] *= 0.1f;
	_float4x4 ModelFloat4x4;
	XMStoreFloat4x4(&ModelFloat4x4, ModelMatrix);


	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pShaderCom, "g_ScreenTexture", TEXT("Target_Final"))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &ModelFloat4x4)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Time", &m_fAccNowLifeTime, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom_Noise->Bind_ShaderResource(m_pShaderCom, "g_NoiseTexture", 0)))
		return E_FAIL;

	_float2 vWinSize = { (_float)g_iWinSizeX,(_float)g_iWinSizeY };
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vWinSize", &vWinSize, sizeof(_float2))))
		return E_FAIL;

	_float3 vThreshold = { 0.0f,0.0f,0.1f };
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fThreshold", &vThreshold, sizeof(_float3))))
		return E_FAIL;
	_float vIntensity = 2.f;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fIntensity", &vIntensity, sizeof(_float))))
		return E_FAIL;

	return S_OK;
}

CMonster09_Mini_Shot* CMonster09_Mini_Shot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMonster09_Mini_Shot* pInstance = new CMonster09_Mini_Shot(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to create: CMonster09_Mini_Shot");
		return nullptr;
	}

	return pInstance;
}

CGameObject* CMonster09_Mini_Shot::Clone(void* pArg)
{
	CMonster09_Mini_Shot* pInstance = new CMonster09_Mini_Shot(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone: CMonster09_Mini_Shot");
		return nullptr;
	}

	return pInstance;
}

void CMonster09_Mini_Shot::Free()
{
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTextureCom_Noise);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	__super::Free();
}
