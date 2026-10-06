#include"stdafx.h"
#include "Monster06.h"
#include "GameInstance.h"


CMonster06::CMonster06(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CMonster{ pDevice, pContext }
{
}

CMonster06::CMonster06(const CMonster06& Prototype)
	:CMonster(Prototype)
{
}

HRESULT CMonster06::Initialize_Prototype()
{


	return S_OK;
}


HRESULT CMonster06::Initialize(void* pArg)
{
	
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	if (FAILED(Ready_Components()))
		return E_FAIL;
	m_iNumAnims = m_pModelCom->Get_AnimNums();
	m_pModelCom->SetUp_Animation(0, true);
	/*m_pModelCom->Set_AnimSpeed(1, 1.f);
	m_pModelCom->Set_AnimSpeed(2, 0.7f);
	m_pModelCom->Set_AnimSpeed(7, 0.7f);*/
	
	return S_OK;
}

void CMonster06::Priority_Update(_float fTimeDelta)
{
}

void CMonster06::Update(_float fTimeDelta)
{
	//m_pModelCom->Play_Animation(fTimeDelta);
	__super::Update(fTimeDelta);
}

void CMonster06::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_PRIORITY, this);
}

HRESULT CMonster06::Render()
{
	if(m_isVisible)
	__super::Render();
	
	return S_OK;

}

HRESULT CMonster06::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Monster09"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;
	
	return S_OK;


}

HRESULT CMonster06::Bind_ShaderResources()
{
	__super::Bind_ShaderResources();

	return S_OK;
}

CMonster06* CMonster06::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMonster06* pInstance = new CMonster06(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Created Monster06_ Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CMonster06::Clone(void* pArg)
{
	CMonster06* pInstance = new CMonster06(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Clone Monster06 Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CMonster06::Free()
{
	__super::Free();

}