#include"stdafx.h"
#include"DropItem.h"
#include"GameInstance.h"
CDropItem::CDropItem(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject{ pDevice, pContext }
{
}

CDropItem::CDropItem(const CDropItem& Prototype)
	:CGameObject(Prototype)
{
}

HRESULT CDropItem::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDropItem::Initialize(void* pArg)
{
	DROPITEM_DESC* pDesc = static_cast<DROPITEM_DESC*>(pArg);


	m_iNowMapStage = pDesc->iNowMapStage;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL; 
	return S_OK;
}

void CDropItem::Priority_Update(_float fTimeDelta)
{
	if (false == m_bOnFround) {
		m_fCurHeight += m_v0 * fTimeDelta;
		m_v0 -= m_fGravity * fTimeDelta * 5.f;
	}
	_vector vPos = m_pNavigationCom->SetUp_HeightOnNavigation(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	_float fBaseHeight = XMVectorGetY(vPos);

	if (fBaseHeight >= m_fCurHeight)
	{
		m_fCurHeight = fBaseHeight;
		m_bOnFround = true;
		/*습득시 0으로*/
	}

	if (true == m_bOnFround)
	{
		CTransform* pTargetTransform = static_cast<CTransform*>(m_pGameInstance->Get_Component(m_pGameInstance->Get_Current_Level(), TEXT("Layer_APlayer"), TEXT("Com_Transform"), 0));
		if (nullptr == pTargetTransform)
			return;

		_vector vTargetPos = pTargetTransform->Get_WorldMatrix().r[3];

		//vTargetPos += XMVectorSet(0.f, 0.1f, 0.f, 0.f);

		_vector vMyPos = m_pTransformCom->Get_WorldMatrix().r[3];

		_float fDistance = XMVectorGetX(XMVector3Length(vTargetPos - vMyPos));

		if (fDistance < 0.5f)
		{
			m_isActive = false;
			//각자의 함수를 호출한다 
			Activate_DropItem();
		}

		else if (fDistance < 8.f)
		{

			_vector vDir = XMVector3Normalize(vTargetPos - vMyPos) * fTimeDelta*15.f ;

			vMyPos += vDir;
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vMyPos);

		}
		else
		{
			m_bOnFround = true;
		}

	}


}

void CDropItem::Update(_float fTimeDelta)
{

	if(false == m_bOnFround)
	m_pTransformCom->Go_Straight(fTimeDelta, false, m_pNavigationCom, true, m_fCurHeight);
	
	m_pGameInstance->Add_Collision_Group(CCollider_Manager::COLLISION_GROUP::COLLISION_ITEM, this, m_pColliderCom);
	m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());


	m_pModelCom->Play_Animation(fTimeDelta, m_isChangingMotion);
}

void CDropItem::Late_Update(_float fTimeDelta)
{
	CGameObject* pTarget = { nullptr };
	/*Collision-Picking*/
	if (m_pGameInstance->Check_Collision(m_pColliderCom, CCollider_Manager::COLLISION_GROUP::COLLISION_PLAYER, &pTarget))
	{

	}



	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);

#ifdef _DEBUG
		m_pGameInstance->Add_DebugComponent(m_pColliderCom);
#endif
}

HRESULT CDropItem::Render()
{
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


	return S_OK;
}

void CDropItem::Reset_Item(_fmatrix matInitialItemPos, CNavigation* pMonsterNavigation)
{
	m_isActive = true;

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, 1.f * XMVector3Normalize(matInitialItemPos.r[0]));
	m_pTransformCom->Set_State(CTransform::STATE_UP, 1.f * XMVector3Normalize(matInitialItemPos.r[1]));
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, 1.f * XMVector3Normalize(matInitialItemPos.r[2]));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, 0.f, 0.f, 1.f));
	
	_float fRandomRadians = m_pGameInstance->Compute_Random(0.f, XMConvertToRadians(360.f));
	m_pTransformCom->Rotation(XMVectorSet(0.f, 2.f, 0.f, 0.f), fRandomRadians);

	_vector fMonsterHeight = { 0.f,1.f,0.f,0.f };

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, matInitialItemPos.r[3]+ fMonsterHeight);

	m_pModelCom->SetUp_Animation(0, true);
	m_pNavigationCom->Set_Index(pMonsterNavigation->Get_Index());

	m_fGravity = 9.8f;
	m_fCurHeight = XMVectorGetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	m_v0 = m_pGameInstance->Compute_Random(0.0000001f, 0.0000005f);
	m_bOnFround = false;
}

HRESULT CDropItem::Ready_Components()
{
	return S_OK;
}

HRESULT CDropItem::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;



	const LIGHT_DESC* pLightDesc = m_pGameInstance->Get_LightDesc(0);

	return S_OK;
}


void CDropItem::Free()
{
	__super::Free();
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pNavigationCom);
}
