#include"stdafx.h"
#include "Monster01.h"
#include "GameInstance.h"


CMonster01::CMonster01(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CMonster{ pDevice, pContext }
{
}

CMonster01::CMonster01(const CMonster01& Prototype)
	:CMonster(Prototype)
{
}

HRESULT CMonster01::Initialize_Prototype()
{


	return S_OK;
}


HRESULT CMonster01::Initialize(void* pArg)
{
	
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	if (FAILED(Ready_Components()))
		return E_FAIL;
 	m_iNumAnims= m_pModelCom->Get_AnimNums();

	m_pModelCom->SetUp_Animation(rand() % 5, true);
	return S_OK;
}

void CMonster01::Priority_Update(_float fTimeDelta)
{
}

void CMonster01::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
	//m_pModelCom->Play_Animation(fTimeDelta);
}

void CMonster01::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_PRIORITY, this);
}

HRESULT CMonster01::Render()
{
	if(m_isVisible)
	__super::Render();
	
	
	return S_OK;
}

HRESULT CMonster01::Ready_Components()
{
	

	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Model  for Anim*/
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Monster01"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;
		
	
	return S_OK;


}

HRESULT CMonster01::Bind_ShaderResources()
{
	__super::Bind_ShaderResources();

	return S_OK;
}

CMonster01* CMonster01::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMonster01* pInstance = new CMonster01(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Created Monster01_ Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CMonster01::Clone(void* pArg)
{
	CMonster01* pInstance = new CMonster01(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Clone Monster01 Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CMonster01::Free()
{
	__super::Free();

}