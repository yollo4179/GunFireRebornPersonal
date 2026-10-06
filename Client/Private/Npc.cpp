#include"stdafx.h"
#include"Npc.h"
#include"GameInstance.h"
#include"Player.h"
#include"UI_Talk.h"
CNpc::CNpc(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject{ pDevice, pContext }
{
}

CNpc::CNpc(const CNpc& Prototype)
	:CGameObject(Prototype)
{
}

HRESULT CNpc::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CNpc::Initialize(void* pArg)
{
	NPC_DESC* pDesc = static_cast<NPC_DESC*>(pArg);
	m_NpcNo = pDesc->NPCNo;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (nullptr == pDesc && nullptr != m_pTransformCom)
	{
		m_pTransformCom->Scaling(1, 1, 1);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, 0.f, 0.f, 1.f));
	}
	if (nullptr != pDesc)
	{


		_float3 InitScale = pDesc->vScale;
		_float4 InitPos = pDesc->vPosition;
		_float InitAngle = pDesc->fAngle;

		_float4 Axis = { 0.f,1.f,0.f,0.f };
		m_pTransformCom->Scaling(InitScale.x, InitScale.y, InitScale.z);
		m_pTransformCom->Rotation(XMLoadFloat4(&Axis), InitAngle);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&InitPos));



	}

	
	return S_OK;
}

void CNpc::Priority_Update(_float fTimeDelta)
{
	if (false == m_isActive)
		return;
	CPlayer* pPlayer = static_cast<CPlayer*> (m_pGameInstance->Get_GameObject(m_pGameInstance->Get_Current_Level(), TEXT("Layer_APlayer"), 0));
	_vector vPlayerPos = pPlayer->Get_WorldMatrix().r[3];
	_vector vMyPos = m_pTransformCom->Get_WorldMatrix().r[3];

	_vector vPlayerLook = pPlayer->Get_WorldMatrix().r[2];
	_vector vMyLook = m_pTransformCom->Get_WorldMatrix().r[2];


	_float fDistance = XMVectorGetX(XMVector3Length(vPlayerPos - vMyPos));
	_float fDot = XMVectorGetX(XMVector3Dot(vPlayerLook, vMyLook));

	CUI_Talk* pUITalk = nullptr;
	pUITalk = static_cast<CUI_Talk*> (m_pGameInstance->Get_UI_By_Tag(TEXT("Talk")));


	if (0 < fDot && 5.f > fDistance)
	{
		m_bActiveTalkBox = true;
	}
	else if (true == m_bPlayerOutFirst)
	{
		m_bActiveTalkBox = false;
		pUITalk->Set_InActive();
		m_bPlayerOutFirst = false;



	}


	if (m_bActiveTalkBox)
	{
		m_bPlayerOutFirst = true;

		_matrix UIMatrix = m_pTransformCom->Get_WorldMatrix();
		UIMatrix.r[3] = XMVectorSetY(UIMatrix.r[3], XMVectorGetY(UIMatrix.r[3]) + 1.f);
		pUITalk->Set_Active();
		pUITalk->Set_WorldMatrix(UIMatrix);
		pUITalk->Find_XY_ForText();
		if(KEY_UP(KEY::F))
		{
			Activate_Npc();
		}
		
	}

}

void CNpc::Update(_float fTimeDelta)
{
	if (false == m_isActive)
		return;
	m_pGameInstance->Add_Collision_Group(CCollider_Manager::COLLISION_GROUP::COLLISION_NPC, this, m_pColliderCom);
	m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());


	m_isAnimEnd =m_pModelCom->Play_Animation(fTimeDelta, m_isChangingMotion);
}

void CNpc::Late_Update(_float fTimeDelta)
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

HRESULT CNpc::Render()
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

void CNpc::Set_InActive()
{
	
		m_isActive = false;
		CUI_Talk* pUITalk = nullptr;
		
		pUITalk = static_cast<CUI_Talk*> (m_pGameInstance->Get_UI_By_Tag(TEXT("Talk")));
		if (nullptr == pUITalk)
			return;
		pUITalk->Set_InActive();
}

HRESULT CNpc::Ready_Components()
{
	return S_OK;
}

HRESULT CNpc::Bind_ShaderResources()
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

void CNpc::Free()
{
	__super::Free();
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pColliderCom);
}
