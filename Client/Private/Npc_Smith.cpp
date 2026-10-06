#include"stdafx.h"
#include "Npc_Smith.h"
#include"GameInstance.h"
#include"UI_Dialogue.h"
#include"UI_Talk.h"
CNpc_Smith::CNpc_Smith(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CNpc{pDevice, pContext}
{
}

CNpc_Smith::CNpc_Smith(const CNpc_Smith& Prototype)
	:CNpc(Prototype)
{
}

HRESULT CNpc_Smith::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CNpc_Smith::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;
	m_pModelCom->SetUp_Animation(0, true);
	return S_OK;
}

void CNpc_Smith::Priority_Update(_float fTimeDelta)
{
	if (!m_isActive)
		return;

	CUI_Dialogue* pDialogue = static_cast<CUI_Dialogue*>(m_pGameInstance->Get_UI_By_Tag(TEXT("Dialogue")));
	if (nullptr == pDialogue)
		return;
	_uint iNPCnUM = {};
	
	if ( (m_NpcNo== pDialogue->Get_NpcNowNo()) && true == pDialogue->Get_TextEnd())
	{
		m_GotoTheNextStage = true;
		m_isActive = false;
		pDialogue->Reset_Dialogue();


		CUI_Talk* pUITalk = nullptr;
		pUITalk = static_cast<CUI_Talk*> (m_pGameInstance->Get_UI_By_Tag(TEXT("Talk")));
		pUITalk->Set_InActive();

	}
	iNPCnUM = iNPCnUM;
	__super::Priority_Update(fTimeDelta);
}

void CNpc_Smith::Update(_float fTimeDelta)
{
	if (!m_isActive)
		return;

	__super::Update(fTimeDelta);
}

void CNpc_Smith::Late_Update(_float fTimeDelta)
{
	if (!m_isActive ||true==m_GotoTheNextStage)
		return;

	__super::Late_Update(fTimeDelta);
}

HRESULT CNpc_Smith::Render()
{
	__super::Render();
	return S_OK;
}

void CNpc_Smith::Activate_Npc()
{
	CUI_Dialogue* pDialogue = static_cast<CUI_Dialogue*>(m_pGameInstance->Get_UI_By_Tag(TEXT("Dialogue")));
	pDialogue->Set_Active();
	pDialogue->Reset_Dialogue();
	pDialogue->Set_Dialogues(m_Dialogues,m_NpcNo);
	pDialogue->Set_NpcName(m_szNpcName);
	
}

HRESULT CNpc_Smith::Ready_Components()
{
	CBounding_Sphere::BOUND_SPHERE_DESC ColliderDesc{};
	ColliderDesc.vCenter = _float3(0.f, 0.f, 0.f);
	ColliderDesc.fRadius = 0.5f;

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Model *///(0번 애니메이션만 사용하자)
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_NPC_Smith"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;
}
HRESULT CNpc_Smith::Ready_Particle()
{
	return E_NOTIMPL;
}

CNpc_Smith* CNpc_Smith::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CNpc_Smith* pInstance = new CNpc_Smith(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to create: CNpc_Smith");
		return nullptr;
	}

	return pInstance;
}

CGameObject* CNpc_Smith::Clone(void* pArg)
{
	CNpc_Smith* pInstance = new CNpc_Smith(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone: CNpc_Smith");
		return nullptr;
	}

	return pInstance;
}

void CNpc_Smith::Free()
{
	__super::Free();
}
