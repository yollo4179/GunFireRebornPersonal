#include"stdafx.h"
#include"Npc_ItemBox.h"
#include"GameInstance.h"
#include"UI_Goblet.h"
#include"Camera_Player.h"
CNpc_ItemBox::CNpc_ItemBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CNpc{ pDevice, pContext }
{
}

CNpc_ItemBox::CNpc_ItemBox(const CNpc_ItemBox& Prototype)
	:CNpc(Prototype)
{
}

HRESULT CNpc_ItemBox::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CNpc_ItemBox::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;
	m_pModelCom->SetUp_Animation(0, true);
	return S_OK;
}

void CNpc_ItemBox::Priority_Update(_float fTimeDelta)
{
	if (!m_isActive)
		return;
	__super::Priority_Update(fTimeDelta);
}

void CNpc_ItemBox::Update(_float fTimeDelta)
{
	if (!m_isActive)
		return;
	if (false == m_isAnimDone &&true == m_isAnimEnd)
	{
		CUI_Goblet* pGoblet = { nullptr };
		pGoblet = static_cast<CUI_Goblet*>(m_pGameInstance->Get_UI_By_Tag(L"Goblet"));
		pGoblet->Set_Active();
		pGoblet->Reset_RandomGoblets();
		m_isAnimDone = true;
		m_pGameInstance->PlayEffect(L"ItemBox_Drop.ogg", 0.1f);
	}
	
	__super::Update(fTimeDelta);
}

void CNpc_ItemBox::Late_Update(_float fTimeDelta)
{
	if (!m_isActive)
		return;
	m_pGameInstance->Add_RenderObject(CRenderer::RG_GLOW, this);
	__super::Late_Update(fTimeDelta);
}

HRESULT CNpc_ItemBox::Render()
{
	if (m_bGlow)
	{
		if (FAILED(Bind_ShaderResources_Glow()))
			return E_FAIL;
		_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

		for (size_t i = 0; i < iNumMeshes; i++)
		{
			if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE, 0)))
				return E_FAIL;
			if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i)))
				return E_FAIL;

			m_pShaderCom->Begin(2);

			m_pModelCom->Render(i);
		}
	}
	else 
	{
		__super::Render();
	}
	return S_OK;
}

void CNpc_ItemBox::Activate_Npc()
{
	if (false == m_isAnimDone)
	{
		CCamera_Player* pTargetCamera = static_cast<CCamera_Player*>(m_pGameInstance->Get_GameObject(m_pGameInstance->Get_Current_Level(), TEXT("Layer_Camara"), 0));
		if (nullptr == pTargetCamera)
			return;
		pTargetCamera->Set_Camera_Still();
		m_pModelCom->Move_NextAnim(1, 0.2f, false, false);
		m_pGameInstance->PlayEffect(L"ItemBox_Opening.mp3", 0.1f);
	}
}

HRESULT CNpc_ItemBox::Ready_Components()
{
	CBounding_Sphere::BOUND_SPHERE_DESC ColliderDesc{};
	ColliderDesc.vCenter = _float3(0.f, 0.f, 0.f);
	ColliderDesc.fRadius = 0.5f;

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimMesh_Npc_ItemBox"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Model *///(0번 애니메이션만 사용하자)
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_NPC_ItemBox"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ItemBox_Glow"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom_Glow))))
		return E_FAIL;

	
	return S_OK;
}

HRESULT CNpc_ItemBox::Ready_Particle()
{

	

	return S_OK;
}

HRESULT CNpc_ItemBox::Bind_ShaderResources_Glow()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	_float3 vThreshold = { 0.9f,0.9f,0.9f };
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vThreshold", &vThreshold, sizeof(_float3))))
		return E_FAIL;
	_float vIntensity = 0.1f;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fIntensity", &vIntensity, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pTextureCom_Glow->Bind_ShaderResource(m_pShaderCom, "g_EmissiveTexture", 0)))
		return E_FAIL;
	return S_OK;
	
}

CNpc_ItemBox* CNpc_ItemBox::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CNpc_ItemBox* pInstance = new CNpc_ItemBox(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to create: CNpc_ItemBox");
		return nullptr;
	}

	return pInstance;
}

CGameObject* CNpc_ItemBox::Clone(void* pArg)
{
	CNpc_ItemBox* pInstance = new CNpc_ItemBox(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone: CNpc_ItemBox");
		return nullptr;
	}

	return pInstance;
}

void CNpc_ItemBox::Free()
{
	Safe_Release(m_pTextureCom_Glow);
	__super::Free();
}
