#include"stdafx.h"
#include "Monster02.h"
#include "GameInstance.h"


CMonster02::CMonster02(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CMonster{ pDevice, pContext }
{
}

CMonster02::CMonster02(const CMonster02& Prototype)
	:CMonster(Prototype)
{
}

HRESULT CMonster02::Initialize_Prototype()
{


	return S_OK;
}


HRESULT CMonster02::Initialize(void* pArg)
{
	
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_iNumAnims = m_pModelCom->Get_AnimNums();
	m_pModelCom->SetUp_Animation(rand() % 5, true);

	return S_OK;
}

void CMonster02::Priority_Update(_float fTimeDelta)
{
}

void CMonster02::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
	//m_pModelCom->Play_Animation(fTimeDelta);
}

void CMonster02::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_PRIORITY, this);
}

HRESULT CMonster02::Render()
{
	if (m_isVisible)
	__super::Render();
	return S_OK;
}

HRESULT CMonster02::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Monster02"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;
	
	return S_OK;


}

HRESULT CMonster02::Bind_ShaderResources()
{
	__super::Bind_ShaderResources();

	return S_OK;
}

CMonster02* CMonster02::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMonster02* pInstance = new CMonster02(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Created Monster02_ Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CMonster02::Clone(void* pArg)
{
	CMonster02* pInstance = new CMonster02(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Clone Monster02 Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CMonster02::Free()
{
	__super::Free();
}