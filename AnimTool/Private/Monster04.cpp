#include"stdafx.h"
#include "Monster04.h"
#include "GameInstance.h"


CMonster04::CMonster04(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CMonster{ pDevice, pContext }
{
}

CMonster04::CMonster04(const CMonster04& Prototype)
	:CMonster(Prototype)
{
}

HRESULT CMonster04::Initialize_Prototype()
{


	return S_OK;
}


HRESULT CMonster04::Initialize(void* pArg)
{
	
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_iNumAnims = m_pModelCom->Get_AnimNums();
	m_pModelCom->SetUp_Animation(rand() % 5, true);
	return S_OK;
}

void CMonster04::Priority_Update(_float fTimeDelta)
{
}

void CMonster04::Update(_float fTimeDelta)
{
	//m_pModelCom->Play_Animation(fTimeDelta);
	__super::Update(fTimeDelta);
}

void CMonster04::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_PRIORITY, this);
}

HRESULT CMonster04::Render()
{
	if (m_isVisible)
		__super::Render();
	return S_OK;
}

HRESULT CMonster04::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;
	/* Com_Model */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Monster04"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;
	
	return S_OK;


}

HRESULT CMonster04::Bind_ShaderResources()
{
	__super::Bind_ShaderResources();

	return S_OK;
}

CMonster04* CMonster04::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMonster04* pInstance = new CMonster04(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Created Monster04_ Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CMonster04::Clone(void* pArg)
{
	CMonster04* pInstance = new CMonster04(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Clone Monster04 Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CMonster04::Free()
{
	__super::Free();

}