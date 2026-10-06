#include"stdafx.h"
#include "Monster03.h"
#include "GameInstance.h"


CMonster03::CMonster03(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CMonster{ pDevice, pContext }
{
}

CMonster03::CMonster03(const CMonster03& Prototype)
	:CMonster(Prototype)
{
}

HRESULT CMonster03::Initialize_Prototype()
{


	return S_OK;
}


HRESULT CMonster03::Initialize(void* pArg)
{
	
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	

	if (FAILED(Ready_Components()))
		return E_FAIL;
	m_iNumAnims = m_pModelCom->Get_AnimNums();
	m_pModelCom->SetUp_Animation(rand() % 5, true);

	return S_OK;
}

void CMonster03::Priority_Update(_float fTimeDelta)
{
}

void CMonster03::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
	//m_pModelCom->Play_Animation(fTimeDelta);
}

void CMonster03::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_PRIORITY, this);
}

HRESULT CMonster03::Render()
{
	if (m_isVisible)
		__super::Render();
	return S_OK;

}

HRESULT CMonster03::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Monster03"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;
	
	return S_OK;


}

HRESULT CMonster03::Bind_ShaderResources()
{
	__super::Bind_ShaderResources();

	return S_OK;
}

CMonster03* CMonster03::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMonster03* pInstance = new CMonster03(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Created Monster03_ Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CMonster03::Clone(void* pArg)
{
	CMonster03* pInstance = new CMonster03(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Clone Monster03 Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CMonster03::Free()
{
	__super::Free();

}